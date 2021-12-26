/*
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
#include <command.h>
#include <malloc.h>
#include <image.h>
#include <asm/byteorder.h>
#include <usb.h>

#ifdef CFG_HUSH_PARSER
#include <hush.h>
#endif


#ifdef CONFIG_AUTO_UPDATE

#ifndef CONFIG_USB_OHCI
#error "must define CONFIG_USB_OHCI"
#endif

#ifndef CONFIG_USB_STORAGE
#error "must define CONFIG_USB_STORAGE"
#endif

#ifndef CFG_HUSH_PARSER
#error "must define CFG_HUSH_PARSER"
#endif

#if !(CONFIG_COMMANDS & CFG_CMD_FAT)
#error "must define CFG_CMD_FAT"
#endif

#undef AU_DEBUG

#undef debug
#ifdef	AU_DEBUG
#define debug(fmt,args...)	printf (fmt ,##args)
#else
#define debug(fmt,args...)
#endif	/* AU_DEBUG */

#if defined(CONFIG_ARCH_CSM1100)
extern void usb_release_bus(void);
#endif

/* possible names of files on the USB stick. */
#define AU_FIRMWARE	"u-boot.img"
#define AU_KERNEL	"uimage"
#define AU_RAMDISK	"ramdisk.img"


struct flash_layout {
	long start;
	long end;
};

char bootargs_len = 0;
char bootcmd_len = 0;

/* layout of the FLASH. ST = start address, ND = end address. */
#define AU_FL_FIRMWARE_ST	0x34000000
#define AU_FL_FIRMWARE_ND	0x3403FFFF
#define AU_FL_KERNEL_ST		0x34100000
#define AU_FL_KERNEL_ND		0x342FFFFF
#define AU_FL_RAMDISK_ST	0x34300000
#define AU_FL_RAMDISK_ND	0x345FFFFF

static int au_usb_stor_curr_dev; /* current device */

/* index of each file in the following arrays */
#define IDX_FIRMWARE	0
#define IDX_KERNEL	1
#define IDX_RAMDISK	2

/* max. number of files which could interest us */
#if defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201)
#define AU_MAXFILES 3
#else
#define AU_MAXFILES 1
#endif

#define BOOT_START	0x34400000

/* pointers to file names */
char *aufile[AU_MAXFILES];

/* sizes of flash areas for each file */
long ausize[AU_MAXFILES];

/* the index table of the file type */
int autype[AU_MAXFILES] = { \
	IH_TYPE_FIRMWARE, IH_TYPE_KERNEL, IH_TYPE_RAMDISK};

#if defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201)
/* array of flash areas start and end addresses */
struct flash_layout aufl_layout[AU_MAXFILES] = { \
	{AU_FL_FIRMWARE_ST, AU_FL_FIRMWARE_ND,}, \
	{AU_FL_KERNEL_ST, AU_FL_KERNEL_ND,}, \
	{AU_FL_RAMDISK_ST, AU_FL_RAMDISK_ND,}, \
};
#endif

/* where to load files into memory */
#define LOAD_ADDR 	((unsigned char *)0x00200000)
#define VALID_ADDR 	(LOAD_ADDR + 2 + bootargs_len + bootcmd_len + 48)

/* the app is the largest image */
#define MAX_LOADSZ ausize[IDX_RAMDISK]

/* externals */
extern int fat_register_device(block_dev_desc_t *, int);
extern int file_fat_detectfs(void);
extern long file_fat_read(const char *, void *, unsigned long);
//extern int i2c_read (unsigned char, unsigned int, int , unsigned char* , int);
extern int flash_sect_erase(ulong, ulong);
extern int flash_sect_protect (int, ulong, ulong);
extern int flash_write (char *, ulong, ulong);
/* change char* to void* to shutup the compiler */
extern block_dev_desc_t *get_dev (char*, int);
extern int u_boot_hush_start(void);

int au_check_cksum_valid(long nbytes)
{
	image_header_t *hdr;
	unsigned long checksum;

	hdr = (image_header_t *)(VALID_ADDR);

	/* check the data CRC */
	checksum = ntohl(hdr->ih_dcrc);

	if (crc32 (0, (uchar *)(VALID_ADDR + sizeof(*hdr)), ntohl(hdr->ih_size)) != checksum) {
		printf ("misc.img bad data checksum\n");
		return -1;
	}
	return 0;
}

int au_check_header_valid(int idx, long nbytes)
{
	image_header_t *hdr;
	unsigned long checksum;

	hdr = (image_header_t *)VALID_ADDR;
	/* check the easy ones first */
#undef CHECK_VALID_DEBUG
#ifdef CHECK_VALID_DEBUG
	printf("magic %#x %#x ", ntohl(hdr->ih_magic), IH_MAGIC);
	printf("arch %#x %#x ", hdr->ih_arch, IH_CPU_ARM);
	printf("size %#x %#lx ", ntohl(hdr->ih_size), nbytes);
	printf("type %#x %#x ", hdr->ih_type, IH_TYPE_KERNEL);
#endif
	if (nbytes < sizeof(*hdr)) {
		printf ("Image %s bad header SIZE\n", aufile[idx]);
		return -1;
	}
	if (ntohl(hdr->ih_magic) != IH_MAGIC || hdr->ih_arch != IH_CPU_ARM) {
		printf ("Image %s bad MAGIC or ARCH\n", aufile[idx]);
		return -1;
	}
	/* check the hdr CRC */
	checksum = ntohl(hdr->ih_hcrc);
	hdr->ih_hcrc = 0;

	if (crc32 (0, (uchar *)hdr, sizeof(*hdr)) != checksum) {
		printf ("Image %s bad header checksum\n", aufile[idx]);
		return -1;
	}
	hdr->ih_hcrc = htonl(checksum);
#if defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201)
	/* check the type - could do this all in one gigantic if() */
	if ((idx == IDX_FIRMWARE) && (hdr->ih_type != IH_TYPE_FIRMWARE)) {
		printf ("Image %s wrong type\n", aufile[idx]);
		return -1;
	}
	if ((idx == IDX_KERNEL) && (hdr->ih_type != IH_TYPE_KERNEL)) {
		printf ("Image %s wrong type\n", aufile[idx]);
		return -1;
	}
#endif
	/* recycle checksum */
	checksum = ntohl(hdr->ih_size);
	/* for kernel and app the image header must also fit into flash */
	if (idx != IDX_FIRMWARE)
		checksum += sizeof(*hdr);
	/* check the size does not exceed space in flash. HUSH scripts */
	/* all have ausize[] set to 0 */
	if ((ausize[idx] != 0) && (ausize[idx] < checksum)) {
		printf ("Image %s is bigger than FLASH\n", aufile[idx]);
		return -1;
	}
	return 0;
}

void decrypt(char * addr, int nbytes)
{
	int i;

	for (i = 0; i < nbytes; i++) {
		*addr++ ^= 0xE3;
	}
}		

int au_do_update(int idx, char * address)
{
	image_header_t *hdr;
	long start, end;
	char *addr;
	int off, rc;
	uint nbytes;

	hdr = (image_header_t *)address;

#if defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201)
	start = aufl_layout[idx].start;
	end = aufl_layout[idx].end;
#endif

	/* unprotect the address range */
	/* this assumes that ONLY the firmware is protected! */
	if (idx == IDX_FIRMWARE) {
		flash_sect_protect(0, start, end);
	}

	/*
	 * erase the address range.
	 */
	debug ("flash_sect_erase(%lx, %lx);\n", start, end);
	if( hdr->ih_type == IH_TYPE_RAMDISK )
	{
		flash_sect_erase(start, 0x34800000);
		printf ("flash_sect_erase(%lx, %lx);\n", start, 0x34800000);
	}
	else
	{
		flash_sect_erase(start, end);
		printf ("flash_sect_erase(%lx, %lx);\n", start, end);
	}
	wait_ms(100);
	/* strip the header for the U-boot */
	if (hdr->ih_type != IH_TYPE_KERNEL) {
		addr = (char *)((char *)hdr + sizeof(*hdr));
		off = 0;
		nbytes = ntohl(hdr->ih_size);
	} else {
		addr = (char *)hdr;
		off = sizeof(*hdr);
		nbytes = sizeof(*hdr) + ntohl(hdr->ih_size);
	}

	/* copy the data from RAM to FLASH */
	printf("flash_write(%p, %lx %x)\n", addr, start, nbytes);
	debug ("flash_write(%p, %lx %x)\n", addr, start, nbytes);
	rc = flash_write(addr, start, nbytes);
	printf("flash write finished!\n");
	if (rc != 0) {
		printf("Flashing failed due to error %d\n", rc);
		return -1;
	}

	/* check the dcrc of the copy */
	if (crc32 (0, (uchar *)(start + off), ntohl(hdr->ih_size)) != ntohl(hdr->ih_dcrc)) {
		printf ("Image %s Bad Data Checksum After COPY\n", aufile[idx]);
		return -1;
	}

	/* protect the address range */
	/* this assumes that ONLY the firmware is protected! */
	if (idx == IDX_FIRMWARE)
		flash_sect_protect(1, start, end);
	return 0;
}

/*
 * this is called from board_init() after the hardware has been set up
 * and is usable. That seems like a good time to do this.
 * Right now the return value is ignored.
 */
int do_auto_update(void)
{
	block_dev_desc_t *stor_dev;
	long sz;
	int i, j, res, bitmap_first, old_ctrlc;
	char *env;
	long start, end;
	long actual_size;
	image_header_t *hdr;
	char * addr;
	char st[9], len[9];
	char *ptr = LOAD_ADDR;
	char *ep;
	char *bootargs = NULL;
	char *bootcmd = NULL;

	au_usb_stor_curr_dev = -1;
	/* start USB */
	if (usb_stop() < 0) {
		debug ("usb_stop failed\n");
		return -1;
	}
	if (usb_init() < 0) {
		debug ("usb_init failed\n");
#if defined(CONFIG_ARCH_CSM1100)
		usb_release_bus();
#endif
		return -1;
	}
	/*
	 * check whether a storage device is attached (assume that it's
	 * a USB memory stick, since nothing else should be attached).
	 */
	au_usb_stor_curr_dev = usb_stor_scan(0);
	if (au_usb_stor_curr_dev == -1) {
		debug ("No device found. Not initialized?\n");
#if defined(CONFIG_ARCH_CSM1100)
		usb_release_bus();
#endif
		return -1;
	}
	/* check whether it has a partition table */
	stor_dev = get_dev("usb", 0);
	if (stor_dev == NULL) {
		debug ("uknown device type\n");
#if defined(CONFIG_ARCH_CSM1100)
		usb_release_bus();
#endif
		return -1;
	}
	if (fat_register_device(stor_dev, 1) != 0) {
		debug ("Unable to use USB %d:%d for fatls\n",
			au_usb_stor_curr_dev, 1);
#if defined(CONFIG_ARCH_CSM1100)
		usb_release_bus();
#endif
		return -1;
	}
	if (file_fat_detectfs() != 0) {
		debug ("file_fat_detectfs failed\n");
	}

#if defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201)
	/* initialize the array of file names */
	memset(aufile, 0, sizeof(aufile));
	aufile[IDX_FIRMWARE] = AU_FIRMWARE;
	aufile[IDX_KERNEL] = AU_KERNEL;
	aufile[IDX_RAMDISK] = AU_RAMDISK;
	/* initialize the array of flash sizes */
	memset(ausize, 0, sizeof(ausize));
	ausize[IDX_FIRMWARE] = (AU_FL_FIRMWARE_ND + 1) - AU_FL_FIRMWARE_ST;
	ausize[IDX_KERNEL] = (AU_FL_KERNEL_ND + 1) - AU_FL_KERNEL_ST;
	ausize[IDX_RAMDISK] = (AU_FL_RAMDISK_ND + 1) - AU_FL_RAMDISK_ST;
	/*
	 * now check whether start and end are defined using environment
	 * variables.
	 */
	/*start = -1;
	end = 0;
	env = getenv("firmware_st");
	if (env != NULL)
		start = simple_strtoul(env, NULL, 16);
	env = getenv("firmware_nd");
	if (env != NULL)
		end = simple_strtoul(env, NULL, 16);
	if (start >= 0 && end && end > start) {
		ausize[IDX_FIRMWARE] = (end + 1) - start;
		aufl_layout[0].start = start;
		aufl_layout[0].end = end;
	}
	start = -1;
	end = 0;
	env = getenv("kernel_st");
	if (env != NULL)
		start = simple_strtoul(env, NULL, 16);
	env = getenv("kernel_nd");
	if (env != NULL)
		end = simple_strtoul(env, NULL, 16);
	if (start >= 0 && end && end > start) {
		ausize[IDX_KERNEL] = (end + 1) - start;
		aufl_layout[1].start = start;
		aufl_layout[1].end = end;
	}
	start = -1;
	end = 0;
	env = getenv("ramdisk_st");
	if (env != NULL)
		start = simple_strtoul(env, NULL, 16);
	env = getenv("ramdisk_nd");
	if (env != NULL)
		end = simple_strtoul(env, NULL, 16);
	if (start >= 0 && end && end > start) {
		ausize[IDX_KERNEL] = (end + 1) - start;
		aufl_layout[2].start = start;
		aufl_layout[2].end = end;
	}*/
	/* make certain that HUSH is runnable */
	u_boot_hush_start();
	/* make sure that we see CTRL-C and save the old state */
	old_ctrlc = disable_ctrlc(0);

	bitmap_first = 0;
	// the first byte is the string length of bootargs
	sz = file_fat_read("misc.img", LOAD_ADDR, 2);
	if (sz <= 0) {
		debug ("file misc.img not found\n");
		return -1;
	}
	decrypt(LOAD_ADDR, 2);
	bootargs_len = *ptr++;
	bootcmd_len = *ptr;
	
	sz = file_fat_read("misc.img", LOAD_ADDR, 
				2 + bootargs_len + bootcmd_len + 48 + sizeof(image_header_t));
	if (sz <= 0 || sz < 2 + bootargs_len + bootcmd_len + 48 + sizeof(image_header_t)) {
		debug ("file misc.img not found\n");
		return -1;
	}
	decrypt(LOAD_ADDR, 2 + bootargs_len + bootcmd_len + 48 + sizeof(image_header_t));
	bootargs = malloc(bootargs_len + 1);
	memcpy(bootargs, LOAD_ADDR + 2, bootargs_len);
	bootargs[bootargs_len] = '\0';
	printf("[bootargs]: %s\n", bootargs);
	bootcmd = malloc(bootcmd_len + 1);
	memcpy(bootcmd, LOAD_ADDR + 2 + bootargs_len, bootcmd_len);
	bootcmd[bootcmd_len] = '\0';
	printf("[bootcmd]: %s\n", bootcmd);

	ptr = LOAD_ADDR + 2 + bootargs_len + bootcmd_len;
	for (i = 0; i < 3; i++) {
		for(j = 0; j < 8; j++) {
			st[j] = *ptr++;
		}
		st[j] = '\0';
		for(j = 0; j < 8; j++) {
			len[j] = *ptr++;
		}
		len[j] = '\0';

		start = simple_strtoul(st, &ep, 16);
		if (ep == st || *ep != '\0') {
			printf("simple_strtoul return error!\n");
			return -1;
		}
		if (start == 0)
			continue;
		if (do_addr_spec(st, len, &start, &end) < 0) {
			printf ("Bad address format\n");
			return -1;
		}
		if (start >= end) {
			printf("Start address beyond end address\n");
			return -1;
		}
		ausize[i] = (end + 1) - start;
		aufl_layout[i].start = start;
		aufl_layout[i].end = end;
		printf("%s: 0x%x--0x%x\n", aufile[i], start, end);
	}	
	hdr = (image_header_t *)(VALID_ADDR);
	actual_size = ntohl(hdr->ih_size);
	sz = file_fat_read("misc.img", LOAD_ADDR, 
		2 + bootargs_len + bootcmd_len + 48 + sizeof(image_header_t) + actual_size);
	decrypt(LOAD_ADDR, sz);
	if (au_check_cksum_valid(sz) < 0) {
		debug ("misc.img checksum not valid\n");
		return -1;
	}

	addr = VALID_ADDR + sizeof(image_header_t);
	/* just loop thru all the possible files */
	for (i = 0; i < AU_MAXFILES; i++) {
		hdr = (image_header_t *)addr;
		if (hdr->ih_type == autype[i]) {
			res = au_do_update(i, addr);
			addr += (sizeof(image_header_t) + ntohl(hdr->ih_size));
		}	
	}

	if (bootargs_len != 0)
		setenv("bootargs", bootargs);
	if (bootcmd_len != 0)
		setenv("bootcmd", bootcmd);
	if ((bootargs_len != 0) || (bootcmd_len != 0))
		saveenv();
	
	free(bootargs);
	free(bootcmd);
	
	usb_stop();
	/* restore the old state */
	disable_ctrlc(old_ctrlc);
#else
	int rc;
	sz = file_fat_read("sdk.bin", LOAD_ADDR, 0x200000);
	usb_stop();
	usb_release_bus();

	flash_sect_erase(BOOT_START, BOOT_START + 0x1fffff);
	wait_ms(100);
	rc = flash_write(LOAD_ADDR, BOOT_START, sz);
	if (rc != 0) {
		printf("Flashing failed due to error %d\n", rc);
		return -1;
	}
	printf("auto update finished successfully!\n");
#endif
	return 0;
}
#endif /* CONFIG_AUTO_UPDATE */

