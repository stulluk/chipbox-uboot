/*
 * (C) Copyright 2001
 * Kyle Harris, Nexus Technologies, Inc. kharris@nexus-tech.net
 *
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2003
 * Texas Instruments, <www.ti.com>
 * Kshitij Gupta <Kshitij@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <linux/byteorder/swab.h>

#if defined(CFG_ORION_NOR_FLASH) && !defined(CFG_FLASH_CFI)

#define PHYS_FLASH_SECT_SIZE	0x00010000	    /* 64KB sector (actually block) */
flash_info_t flash_info[CFG_MAX_FLASH_BANKS];	/* info for FLASH chips    */

static ulong is_sst_6401b;

/* Board support for 1 or 2 flash devices */
#undef  FLASH_PORT_WIDTH32
#define FLASH_PORT_WIDTH16

#ifdef FLASH_PORT_WIDTH16
#define FLASH_PORT_WIDTH		ushort
#define FLASH_PORT_WIDTHV		vu_short
#define SWAP(x)			__swab16(x)
#else
#define FLASH_PORT_WIDTH		ulong
#define FLASH_PORT_WIDTHV		vu_long
#define SWAP(x)			__swab32(x)
#endif

#define FPW	FLASH_PORT_WIDTH
#define FPWV	FLASH_PORT_WIDTHV

#define mb() __asm__ __volatile__ ("" : : : "memory")


/*-----------------------------------------------------------------------
 * Functions
 */
unsigned long flash_init (void);
static ulong flash_get_size (volatile FPW * addr, flash_info_t * info);
static int write_data (flash_info_t * info, ulong dest, FPW data);
static void flash_get_offsets (ulong base, flash_info_t * info);
void inline spin_wheel (void);
void flash_print_info (flash_info_t * info);
void flash_unprotect_sectors (FPWV * addr);
int flash_erase (flash_info_t * info, int s_first, int s_last);
int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt);

/*-----------------------------------------------------------------------
 */

static void flash_vpp(int on)
{
	unsigned int tmp;

	tmp = *(unsigned int *)(VERSATILE_FLASHCTRL);

	if (on)
	    tmp |= VERSATILE_FLASHPROG_FLVPPEN;
	else
    	    tmp &= ~VERSATILE_FLASHPROG_FLVPPEN;

	*(unsigned int *)(VERSATILE_FLASHCTRL) = tmp;
}

unsigned long flash_init (void)
{
	int i;
	ulong size = 0;

	puts ("flash_init : Orion");
	for (i = 0; i < CFG_MAX_FLASH_BANKS; i++) {
		switch (i) {
		case 0:
		        flash_vpp(1);
			flash_get_size ((FPW *) PHYS_FLASH_1, &flash_info[i]);
			flash_get_offsets (PHYS_FLASH_1, &flash_info[i]);
			flash_vpp(0);
			break;
		default:
			panic ("configured too many flash banks!\n");
			break;
		}
		size += flash_info[i].size;
	}

	/* Protect monitor and environment sectors
	 */
	flash_protect (FLAG_PROTECT_SET,
			CFG_FLASH_BASE,
			CFG_FLASH_BASE + monitor_flash_len - 1, &flash_info[0]);

	return size;
}

/*-----------------------------------------------------------------------
 */
static void flash_get_offsets (ulong base, flash_info_t * info)
{
	int i;

	if (info->flash_id == FLASH_UNKNOWN) {
		return;
	}

	if ((info->flash_id & FLASH_VENDMASK) == FLASH_MAN_SST) {
		for (i = 0; i < info->sector_count; i++) {
			info->start[i] = base + (i * PHYS_FLASH_SECT_SIZE);
			info->protect[i] = 0;
		}
	}
}

/*-----------------------------------------------------------------------
 */
void flash_print_info (flash_info_t * info)
{
	int i;

	if (info->flash_id == FLASH_UNKNOWN) {
		printf ("missing or unknown FLASH type\n");
		return;
	}

	switch (info->flash_id & FLASH_VENDMASK) {
	case FLASH_MAN_SST:
		printf ("SST ");
		break;
	default:
		printf ("Unknown Vendor ");
		break;
	}

	switch (info->flash_id & FLASH_TYPEMASK) {
	case FLASH_SST640:
		printf ("SST39LF/VF640 (64 Mbit, uniform sector size)\n");
		break;
	default:
		printf ("Unknown Chip Type\n");
		break;
	}

	printf ("  Size: %ld MB in %d Sectors\n",
			info->size >> 20, info->sector_count);

	printf ("  Sector Start Addresses:");
	for (i = 0; i < info->sector_count; ++i) {
		if ((i % 5) == 0)
			printf ("\n   ");
		printf (" %08lX%s",
			info->start[i], info->protect[i] ? " (RO)" : "     ");
	}
	printf ("\n");
	return;
}

/*
 * The following code cannot be run from FLASH!
 */
static ulong flash_get_size (volatile FPW * addr, flash_info_t * info)
{
	volatile FPW value;

	/* Write auto select command: read Manufacturer ID */
	addr[0x5555] = (FPW) 0x00AA00AA;
	addr[0x2AAA] = (FPW) 0x00550055;
	addr[0x5555] = (FPW) 0x00900090;

	mb ();

	value = addr[0];

	switch (value) {

 	case (FPW) SST_MANUFACT:
		info->flash_id = FLASH_MAN_SST;
		break; 

	default:
		info->flash_id = FLASH_UNKNOWN;
		info->sector_count = 0;
		info->size = 0;
		addr[0] = (FPW) 0x00F000F0;	/* restore read mode */
		return (0);		/* no or unknown flash  */
	}

	mb ();
	value = addr[1];	/* device ID        */
	// printf("Device ID : 0x%X\n", value);
	// puts ("Device ID : 0x");
	// printf("%X\n", value);
	switch (value & ((FPW)0xFFF0FFF0) ) {

	case (FPW) (SST_ID_xF6401 & 0xFFF0FFF0):
		info->flash_id += FLASH_SST640;
		info->sector_count = 128;
		info->size = 0x00800000;
        is_sst_6401b = (value == (FPW)0x236D236D);
		break;			

	default:
		info->flash_id = FLASH_UNKNOWN;
		break;
	}

	if (info->sector_count > CFG_MAX_FLASH_SECT) {
		printf ("** ERROR: sector count %d > max (%d) **\n",
				info->sector_count, CFG_MAX_FLASH_SECT);
		info->sector_count = CFG_MAX_FLASH_SECT;
	}

	addr[0] = (FPW) 0x00F000F0;	/* restore read mode */

	return (info->size);
}


/* unprotects a sector for write and erase
 * on some intel parts, this unprotects the entire chip, but it
 * wont hurt to call this additional times per sector...
 */
void flash_unprotect_sectors (FPWV * addr)
{
    /* No software protect supported */
    return;
}


/*-----------------------------------------------------------------------
 */

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int flag, prot, sect;
	ulong type, start, last;
	int rcode = 0;

	printf ("Erase Flash Orion!!! \n");
	if ((s_first < 0) || (s_first > s_last)) {
		if (info->flash_id == FLASH_UNKNOWN) {
			printf ("- missing\n");
		} else {
			printf ("- no sectors to erase\n");
		}
		return 1;
	}

	type = (info->flash_id & FLASH_VENDMASK);
	if ((type != FLASH_MAN_SST)) {
		printf ("Can't erase unknown flash type %08lx - aborted\n",
				info->flash_id);
		return 1;
	}

	prot = 0;
	for (sect = s_first; sect <= s_last; ++sect) {
		if (info->protect[sect]) {
			prot++;
		}
	}

	if (prot) {
		printf ("- Warning: %d protected sectors will not be erased!\n",
				prot);
	} else {
		printf ("\n");
	}

	flash_vpp(1);

	start = get_timer (0);
	last = start;

	/* Disable interrupts which might cause a timeout here */
	flag = disable_interrupts ();

	/* Start erase on unprotected sectors */
	for (sect = s_first; sect <= s_last; sect++) {
		if (info->protect[sect] == 0) {	/* not protected */
			FPWV *addr0 = (FPWV *) (info->start[0]);
			FPWV *addr  = (FPWV *) (info->start[sect]);
			FPW status;

			printf ("Erasing sector %2d ... ", sect);

			flash_unprotect_sectors (addr);

			/* arm simple, non interrupt dependent timer */
			reset_timer_masked ();

            addr0[0x5555] = (FPW) 0x00AA00AA;
            addr0[0x2AAA] = (FPW) 0x00550055;
            addr0[0x5555] = (FPW) 0x00800080;
            addr0[0x5555] = (FPW) 0x00AA00AA;
            addr0[0x2AAA] = (FPW) 0x00550055;
            if(is_sst_6401b)
                *addr = (FPW) 0x00300030;
            else
			    *addr = (FPW) 0x00500050;

			while (((status =
				*addr) & (FPW) 0x00800080) !=
				(FPW) 0x00800080) {
					if (get_timer_masked () >
					CFG_FLASH_ERASE_TOUT) {
					printf ("Timeout\n");
					/* suspend erase     */
					*addr = (FPW) 0x00B000B0;
					/* reset to read mode */
					*addr = (FPW) 0x00F000F0;
					rcode = 1;
					break;
				}
			}

			*addr = (FPW) 0x00F000F0;/* resest to read mode */
			printf (" done\n");
		}
	}

	flash_vpp(0);

	return rcode;
}

/*-----------------------------------------------------------------------
 * Copy memory to flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 * 4 - Flash not identified
 */

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	ulong cp, wp;
	FPW data;
	int count, i, l, rc, port_width;

	if (info->flash_id == FLASH_UNKNOWN) {
		return 4;
	}
/* get lower word aligned address */
#ifdef FLASH_PORT_WIDTH16
	wp = (addr & ~1);
	port_width = 2;
#else
	wp = (addr & ~3);
	port_width = 4;
#endif

	flash_vpp(1);

	/*
	 * handle unaligned start bytes
	 */
	if ((l = addr - wp) != 0) {
		data = 0;
		for (i = 0, cp = wp; i < l; ++i, ++cp) {
			data = (data << 8) | (*(uchar *) cp);
		}
		for (; i < port_width && cnt > 0; ++i) {
			data = (data << 8) | *src++;
			--cnt;
			++cp;
		}
		for (; cnt == 0 && i < port_width; ++i, ++cp) {
			data = (data << 8) | (*(uchar *) cp);
		}

		if ((rc = write_data (info, wp, SWAP (data))) != 0) {
			flash_vpp(0);
			return (rc);
		}
		wp += port_width;
	}

	/*
	 * handle word aligned part
	 */
	count = 0;
	while (cnt >= port_width) {
		data = 0;
		for (i = 0; i < port_width; ++i) {
			data = (data << 8) | *src++;
		}
		if ((rc = write_data (info, wp, SWAP (data))) != 0) {
			flash_vpp(0);
			return (rc);
		}
		wp += port_width;
		cnt -= port_width;
		if (count++ > 0x800) {
			spin_wheel ();
			count = 0;
		}
	}

	if (cnt == 0) {
		flash_vpp(0);
		return (0);
	}

	/*
	 * handle unaligned tail bytes
	 */
	data = 0;
	for (i = 0, cp = wp; i < port_width && cnt > 0; ++i, ++cp) {
		data = (data << 8) | *src++;
		--cnt;
	}
	for (; i < port_width; ++i, ++cp) {
		data = (data << 8) | (*(uchar *) cp);
	}

	rc = write_data (info, wp, SWAP (data));

	flash_vpp(0);

	return rc;
}

/*-----------------------------------------------------------------------
 * Write a word or halfword to Flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 */
static int write_data (flash_info_t * info, ulong dest, FPW data)
{
	FPWV *addr0 = (FPWV *) info->start[0];
	FPWV *addr  = (FPWV *) dest;
	ulong status;
	int flag;

	/* Check if Flash is (sufficiently) erased */
	if ((*addr & data) != data) {
		printf ("not erased at %08lx (%x)\n", (ulong) addr, *((volatile ushort *)addr));
		return (2);
	}

	flash_vpp(1);

	flash_unprotect_sectors (addr);
	/* Disable interrupts which might cause a timeout here */
	flag = disable_interrupts ();
    addr0[0x5555] = (FPW) 0x00AA00AA;
    addr0[0x2AAA] = (FPW) 0x00550055;
    addr0[0x5555] = (FPW) 0x00A000A0;
	*addr = data;

	/* arm simple, non interrupt dependent timer */
	reset_timer_masked ();

	/* wait while polling the status register */
	while (((status = *addr) & (FPW) 0x00800080) != (data & (FPW) 0x00800080)) {
		if (get_timer_masked () > CFG_FLASH_WRITE_TOUT) {
			*addr = (FPW) 0x00F000F0;	/* restore read mode */
			flash_vpp(0);
			return (1);
		}
	}
	*addr = (FPW) 0x00F000F0;	/* restore read mode */
	flash_vpp(0);

	return (0);
}

void inline spin_wheel (void)
{
	static int p = 0;
	static char w[] = "\\/-";

	printf ("\010%c", w[p]);
	(++p == 3) ? (p = 0) : 0;
}

#endif
