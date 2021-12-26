#include <common.h>
#include <command.h>

DECLARE_GLOBAL_DATA_PTR;

unsigned char alen = 3;

static void inline spin_wheel (void)
{
	static int p = 0;
	static char w[] = "\\/-";

	printf ("\010%c", w[p]);
	(++p == 3) ? (p = 0) : 0;
}

void spi__init(void)
{
	unsigned int ctrl0 = 0;

	/* Disable the ssi by write 0 to SSIEN,tx and rx buffer was cleared */
	spi_write(rSSIENR, 0x0);

	/* Config the CTRLR0 register */
	ctrl0 = 0x0 << 12  |	/* CFS (Control Frame Size). 4bit length */
		0x0 << 11  |	/* SRL 0:nomormod;1:test mode */
		0x0 << 10  |	/* SLV_OE */
		0x0 << 8   |	/* TMOD master  TODO 0*/
		0x1 << 7   |	/* SCPOL clock inactive state */
		0x1 << 6   |	/* SCPH */
		0x0 << 4   |	/* frame format select  SPI */
		0x7 << 0;	/* DFS data frame size */
	
	/*  rCTRLR0=0x2B00; receive and transmit enable */

	spi_write(rCTRLR0, ctrl0);

	/* ctrl1 */
	spi_write(rCTRLR1, (MAX_RCV_SZ-1));	

	/* SSI_CLK baud out */
/*        spi_write(rBAUDR, 0xC8);	|+ PP_SZK +|*/
/*        spi_write(rBAUDR, 0x6e);	|+ 512K +|*/
/*        spi_write(rBAUDR, 0x37);	|+ 1M   +|*/
/*        spi_write(rBAUDR, 0x1b);	|+ 2M   +|*/
/*        spi_write(rBAUDR, 0x12);	|+ 3M   +|*/
/*        spi_write(rBAUDR, 0x0e);	|+ 4M   +|*/
/*        spi_write(rBAUDR, 0x0b);	|+ 5M   +|*/
	spi_write(rBAUDR, 0x05);	/* 10M   */

	/* rSER, select the device */
	spi_write(rSER, 0x1);

	/* rSSIENR, enable the ssi bus */
	spi_write(rSSIENR, 0x1);

	alen = spi_read(BOOTCFG)>>5 & 0x3;
}

void spi_baud(unsigned short baud)
{
	spi_write(rSSIENR, 0x0);
	spi_write(rBAUDR, baud);
	spi_write(rSSIENR, 0x1);
}

static void master_set_mode(TMOD_E tmod, TIMING_E tm)
{
	unsigned short ctrl0 = spi_read(rCTRLR0);

	ctrl0 &= ~(0xf<<6);
	ctrl0 |= (tmod<<8 | tm<<6);

	spi_write(rSSIENR, 0x0);
	spi_write(rCTRLR0, ctrl0);
	spi_write(rSSIENR, 0x1);
}
static unsigned char master_check_mode(TMOD_E tmod, TIMING_E tm)
{
	unsigned short ctrl0 = spi_read(rCTRLR0);

	if (ctrl0 & (tmod<<8 | tm<<6)) 
		return 1;

	return 0;
}

/*===================================================================
 * 	flash oepration
 */
static inline void read_out_data(volatile unsigned char *buf, int size)
{
	int i = 0, j = 0;

	while (i < size) {
		while (!(j = spi_read(rRXFLR)));
		while (j > 0) {
			if (buf) buf[i] = spi_read(rDR);
			else     spi_read(rDR);

			i++; j--;
		}
	}
}

static inline unsigned char flash_status(void)
{
	unsigned char buf[2];

	/* If check fail, return 0xff, also satisfy while's condition */
	if (! master_check_mode(TMOD_TXRX, TIMING3))
		return -1;

	spi_write(rDR, CMD_RDSR); spi_write(rDR, 0x0); 
	read_out_data(buf, 2);
	//printf("status: %x\n", buf[1]);
	return buf[1];
}

static void wait_for_finish(void)
{
	int tmp = 0;

	while (flash_status() & 1) {
		udelay(1000);
		if (!(tmp % 100)) 
			spin_wheel();		
		tmp++;
	}
}

/* Only Used For Erase Read Write */
static void flash_send_cmd(unsigned char cmd, unsigned int caddr)
{
	int i;
	unsigned char *byte_addr = (unsigned char *) &caddr;

	if (/*CMD_WRITE == cmd || */ CMD_ER_SEC == cmd || 
	    CMD_ER_BLK == cmd || CMD_ER_CHIP == cmd) {

		master_set_mode(TMOD_TXRX, TIMING3);
		if (CMD_ER_CHIP == cmd) {
			spi_write(rDR, cmd); 
			read_out_data(NULL, 1);
		} else {
			spi_write(rDR, cmd); 
			for (i = alen - 1; i >= 0; i--)
				spi_write(rDR, byte_addr[i]);
			
			/*if (CMD_WRITE != cmd)*/
				read_out_data(NULL, alen+1);
		}
		
	} else if (CMD_WREN == cmd) {

/*                master_set_mode(TMOD_TXRX, TIMING3);*/
		master_set_mode(TMOD_TXRX, TIMING0);
		spi_write(rDR, cmd); 
		read_out_data(NULL, 1);

	} else if (CMD_WRITE == cmd) {

		master_set_mode(TMOD_TX, TIMING3);
		spi_write(rDR, cmd); 
		for (i = alen - 1; i >= 0; i--)
			spi_write(rDR, byte_addr[i]);

	} else if (CMD_READ == cmd || CMD_RDID == cmd) {

		master_set_mode(TMOD_EE, TIMING3);
		if (CMD_RDID == cmd) {
			spi_write(rDR, cmd); 
		} else {
			spi_write(rDR, cmd); 
			for (i = alen - 1; i >= 0; i--)
				spi_write(rDR, byte_addr[i]);
		}

	} else {
		SDEBUG("flash_send_cmd: Not Support this command!\n");
	}

	return;
}

volatile unsigned char *tcm_buf = (volatile unsigned char *)0x20000000;
static int _flash_read(unsigned int caddr, volatile unsigned char *buf, int size)
{
	if (size > MAX_RCV_SZ || size <= 0) {
		SDEBUG("_flash_read: Error! size no valid!\n");
		return -1;
	}

	flash_send_cmd(CMD_READ, caddr);

/*        read_out_data(buf, size);*/
	int i = 0, j = 0;
	while (i < size) {
		while (!(j = spi_read(rRXFLR)));
		while (j > 0) {
			buf[i] = spi_read(rDR);
			i++; j--;
		}
	}
/*        memcpy(buf,tcm_buf, size);*/

	return 0;
}

/* max size is PP_SZ */
static int _flash_write(unsigned int caddr, volatile unsigned char *buf, int size)
{
	int i = 0;

	if (size > PP_SZ || size <=0) {
		SDEBUG("_flash_write: Error! size no valid!\n");
		return -1;
	}

	flash_send_cmd(CMD_WREN, 0);
	flash_send_cmd(CMD_WRITE, caddr);

	while (i < size) {
		if (spi_read(rSR) & 0x2)
			spi_write(rDR, buf[i++]);
		else
			while (spi_read(rTXFLR));
	}

	spin_wheel();
	udelay(PP_TIME_US);

	return 0;
}

static int _flash_erase(unsigned int caddr, flasherase_t cmd)
{
	flash_send_cmd(CMD_WREN, 0);

	switch (cmd) {
		case ERASE_SEC:	/* sector */
			flash_send_cmd(CMD_ER_SEC, caddr); 
			wait_for_finish(); 
			break;
		case ERASE_BLK: /* block */
			flash_send_cmd(CMD_ER_BLK, caddr); 
			wait_for_finish(); 
			break;
		case ERASE_CHIP: /* chip */
			flash_send_cmd(CMD_ER_CHIP, 0); 
			wait_for_finish(); 
			break;
		default:
			SDEBUG("Error! No such chioce.\n");
	}

	return 0;
}

int seeprom_read(unsigned caddr, uchar *maddr, unsigned size)
{
	unsigned cnt;

/*        SDEBUG("SFlash Read...\n");*/

	while (size > 0) {
		cnt = (size >= RD_SZ) ? RD_SZ : size;
		_flash_read(caddr, maddr, cnt);
		caddr += cnt;
		maddr += cnt;
		size  -= cnt;
	}

	return 0;
}

int seeprom_write(unsigned caddr, uchar *maddr, unsigned size)
{
	unsigned cnt;

	SDEBUG("SFlash chip Write...\n");

	spi_baud(WR_BAUD);

	while (size > 0) {
		cnt = (size >= PP_SZ) ? PP_SZ : size;
		_flash_write(caddr, maddr, cnt);
		caddr += cnt;
		maddr += cnt;
		size  -= cnt;
	}

	/* Are we free? */
	while (spi_read(rSR) & 0x1);

	spi_baud(DFT_BAUD);

	return 0;
}

int seeprom_erase(unsigned cmd, unsigned saddr, unsigned size)
{
	unsigned eaddr;
	unsigned sector_size = gd->sf_inf->sector_size;

	eaddr = saddr + size;
	if (eaddr > gd->sf_size)  	
		eaddr = gd->sf_size;

	switch (cmd) {
		case ERASE_CHIP: /* chip */
			SDEBUG("SFlash chip Erase...\n");
			_flash_erase(saddr, cmd);
			return 0;
		case ERASE_BLK: /* block */
			SDEBUG("SFlash Erase from 0x%x to 0x%x \n", saddr, eaddr);
#if 0 && defined(CONFIG_EN25B64)
			if (saddr < 0x10000) {
				_flash_erase(0x0000, cmd);
				_flash_erase(0x1000, cmd);
				_flash_erase(0x2000, cmd);
				_flash_erase(0x4000, cmd);
				_flash_erase(0x8000, cmd);

				if (eaddr >= 0x10000)
					saddr = 0x10000;
			}
#endif
			if (saddr < eaddr) {
				_flash_erase(saddr, cmd);
				saddr = (saddr&~0xffff)+sector_size;
			}
			for (saddr; saddr < eaddr; saddr+=sector_size) {
				_flash_erase(saddr, cmd);
			}
			break;
		case ERASE_SEC:	/* sector */
			SDEBUG("SFlash Erase from 0x%x to 0x%x \n", saddr, eaddr);
			for (saddr; saddr < eaddr; saddr=(saddr&~0xfff)+0x1000)
				_flash_erase(saddr, cmd);
			break;
		default:
			SDEBUG("Error! No such chioce.\n");
			return -1;
	}

	return 0;
}

static sfinf_t sflash_data [] = {

	/* Atmel -- some are (confusingly) marketed as "DataFlash" */
	{ "at25fs010",  0x1f6601, 0, 32 * 1024, 4, SECT_4K, },
	{ "at25fs040",  0x1f6604, 0, 64 * 1024, 8, SECT_4K, },

	{ "at25df041a", 0x1f4401, 0, 64 * 1024, 8, SECT_4K, },
	{ "at25df641",  0x1f4800, 0, 64 * 1024, 128, SECT_4K, },

	{ "at26f004",   0x1f0400, 0, 64 * 1024, 8, SECT_4K, },
	{ "at26df081a", 0x1f4501, 0, 64 * 1024, 16, SECT_4K, },
	{ "at26df161a", 0x1f4601, 0, 64 * 1024, 32, SECT_4K, },
	{ "at26df321",  0x1f4701, 0, 64 * 1024, 64, SECT_4K, },

	/* Macronix */
	{ "mx25l12805d", 0xc22018, 0, 64 * 1024, 256, },

	/* Spansion -- single (large) sector size only, at least
	 * for the chips listed here (without boot sectors).
	 */
	{ "s25sl004a", 0x010212, 0, 64 * 1024, 8, },
	{ "s25sl008a", 0x010213, 0, 64 * 1024, 16, },
	{ "s25sl016a", 0x010214, 0, 64 * 1024, 32, },
	{ "s25sl032a", 0x010215, 0, 64 * 1024, 64, },
	{ "s25sl064a", 0x010216, 0, 64 * 1024, 128, },
        { "s25sl12800", 0x012018, 0x0300, 256 * 1024, 64, },
	{ "s25sl12801", 0x012018, 0x0301, 64 * 1024, 256, },

	/* SST -- large erase sizes are "overlays", "sectors" are 4K */
	{ "sst25vf040b", 0xbf258d, 0, 64 * 1024, 8, SECT_4K, },
	{ "sst25vf080b", 0xbf258e, 0, 64 * 1024, 16, SECT_4K, },
	{ "sst25vf016b", 0xbf2541, 0, 64 * 1024, 32, SECT_4K, },
	{ "sst25vf032b", 0xbf254a, 0, 64 * 1024, 64, SECT_4K, },

	/* ST Microelectronics -- newer production may have feature updates */
	{ "m25p05",  0x202010,  0, 32 * 1024, 2, },
	{ "m25p10",  0x202011,  0, 32 * 1024, 4, },
	{ "m25p20",  0x202012,  0, 64 * 1024, 4, },
	{ "m25p40",  0x202013,  0, 64 * 1024, 8, },
	{ "m25p80",         0,  0, 64 * 1024, 16, },
	{ "m25p16",  0x202015,  0, 64 * 1024, 32, },
	{ "m25p32",  0x202016,  0, 64 * 1024, 64, },
	{ "m25p64",  0x202017,  0, 64 * 1024, 128, },
	{ "m25p128", 0x202018, 0, 256 * 1024, 64, },

	{ "m45pe10", 0x204011,  0, 64 * 1024, 2, },
	{ "m45pe80", 0x204014,  0, 64 * 1024, 16, },
	{ "m45pe16", 0x204015,  0, 64 * 1024, 32, },

	{ "m25pe80", 0x208014,  0, 64 * 1024, 16, },
	{ "m25pe16", 0x208015,  0, 64 * 1024, 32, SECT_4K, },

	/* Winbond -- w25x "blocks" are 64K, "sectors" are 4KiB */
	{ "w25x10", 0xef3011, 0, 64 * 1024, 2, SECT_4K, },
	{ "w25x20", 0xef3012, 0, 64 * 1024, 4, SECT_4K, },
	{ "w25x40", 0xef3013, 0, 64 * 1024, 8, SECT_4K, },
	{ "w25x80", 0xef3014, 0, 64 * 1024, 16, SECT_4K, },
	{ "w25x16", 0xef3015, 0, 64 * 1024, 32, SECT_4K, },
	{ "w25x32", 0xef3016, 0, 64 * 1024, 64, SECT_4K, },
	{ "w25x64", 0xef3017, 0, 64 * 1024, 128, SECT_4K, },

	{ "s25fl008a", 0x1c2017, 0, 64 * 1024, 128, },

	{ "mx25l6445e", 0xc22017, 0, 64 * 1024, 128, },
};

static void get_manufacture_id(unsigned char *buf, int size)
{
	flash_send_cmd(CMD_RDID, 0);
	read_out_data(buf, size);
}

int seeprom_probe(void)
{
	int			tmp;
	u8			id[5];
	u32			jedec;
	u16                     ext_jedec;
	struct sflash_info	*info;

	get_manufacture_id(id, 5);
	jedec = id[0];
	jedec = jedec << 8;
	jedec |= id[1];
	jedec = jedec << 8;
	jedec |= id[2];

	ext_jedec = id[3] << 8 | id[4];

	for (tmp = 0, info = sflash_data;
			tmp < ARRAY_SIZE(sflash_data);
			tmp++, info++) {
		if (info->jedec_id == jedec) {
			if (info->ext_id != 0 && info->ext_id != ext_jedec)
				continue;
			goto found;
		}
	}

	SDEBUG("Probe Fail! No Such Devices!\n");

	return 0;

found:
	gd->sf_inf = info;
	gd->sf_alen = alen;
	if (spi_read(BOOTCFG) & 0x10)	
		gd->sf_boot = 1;
	else 	gd->sf_boot = 0;

	gd->sf_rspd = CFG_PCLK_FREQ/1000000/RD_BAUD;
	gd->sf_wspd = CFG_PCLK_FREQ/1000000/WR_BAUD;

	gd->sf_size  = info->sector_size * info->n_sectors;

	return 1;
}
