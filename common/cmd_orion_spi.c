#include <common.h>
#include <command.h>

#include <orion_spi.h>

int do_sflashre(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned char *maddr;
	unsigned int caddr;
	unsigned int size;

	if (argc != 4) {
		DEBUG("Usage: %s\n", cmdtp->help);
		return -1;
	}

	maddr = (unsigned char *)simple_strtoul(argv[1], NULL, 16);
	caddr = simple_strtoul(argv[2], NULL, 16);
	size = simple_strtoul(argv[3], NULL, 16);

	seeprom_read(caddr, maddr, size);

	return 0;
}

U_BOOT_CMD(
	sfre,	CFG_MAXARGS/*5*/,	1,	do_sflashre,
	"sfre    - spi flash read byte.\n",
	NULL
	);

int do_sflashwr(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned char *maddr;
	unsigned int caddr;
	int size;

	if (argc != 4) {
		DEBUG("Usage: %s\n", cmdtp->help);
		return -1;
	}

	maddr = (unsigned char *)simple_strtoul(argv[1], NULL, 16);
	caddr = simple_strtoul(argv[2], NULL, 16);
	size = simple_strtoul(argv[3], NULL, 16);

	DEBUG("caddr: 0x%x, maddr: 0x%x, size: 0x%x\n", 
	      caddr, maddr, size);

	seeprom_write(caddr, maddr, size);

	return 0;
}

U_BOOT_CMD(
	sfwr,	CFG_MAXARGS/*5*/,	1,	do_sflashwr,
	"sfwr    - spi flash write byte\n",
	NULL
	);

int do_sflasher(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	volatile unsigned char *maddr;
	unsigned int caddr, addrs, addre;
	int size;

	if (argc < 2) {
		DEBUG("Usage: %s\n", cmdtp->help);
		return -1;
	}

	if (strcmp(argv[1], "all") == 0) {
		DEBUG ("Erase Flash whole... \n");
		seeprom_erase(ERASE_CHIP, 0, 0);
		return 0;
	}

	caddr = simple_strtoul(argv[1], NULL, 16);
	size = simple_strtoul(argv[2], NULL, 16);

	//seeprom_erase(ERASE_SEC, caddr, size);
	seeprom_erase(ERASE_BLK, caddr, size);

	return 0;
}

U_BOOT_CMD(
	sfer,	CFG_MAXARGS/*5*/,	1,	do_sflasher,
	"sfer    - spi flash erase\n",
	NULL
	);

int do_eeid(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	spi_eeprom_mode();
	spi_timing_mode3();
	spi_write(rDR, 0x9f);

	while (spi_read(rSR) & 0x1);
	while (spi_read(rTXFLR) & 0x1);
	while (! (spi_read(rSR) & 0x8));
	udelay(10000);	/* important */
	printf("receive data frame size: %d\n", spi_read(rRXFLR));

	printf("Manufacture ID: 0x%x\n", spi_read(rDR));
	printf("Device ID 1: 	0x%x\n", spi_read(rDR));
	printf("Device ID 2: 	0x%x\n", spi_read(rDR));
}

U_BOOT_CMD(
	eeid,	CFG_MAXARGS/*5*/,	1,	do_eeid,
	"eeid    - display flash id\n",
	NULL
	);

#if 0
void spi_rx_flash(void)
{
	/* write data */
	spi_write(rDR, 0x3);
	/* write addr */
	spi_write(rDR, 0x0);
	spi_write(rDR, 0x0);
	spi_write(rDR, 0x0);
	/* read 3 byte */
	spi_write(rDR, 0x0);	/* for receive only */
	spi_write(rDR, 0x0);	/* for reveive only */
	spi_write(rDR, 0x0);	/* for reveive only */

	/* polling for finish */
	while (spi_read(rSR) & 0x1);
	while (spi_read(rTXFLR) & 0x1);
	while (! (spi_read(rSR) & 0x8));
	udelay(10000);	/* important */

	/* read dummy data */
	spi_read(rDR);
	spi_read(rDR);
	spi_read(rDR);
	spi_read(rDR);
	
	printf("read data 1: 	0x%x\n", spi_read(rDR));
	printf("read data 2: 	0x%x\n", spi_read(rDR));
	printf("read data 3: 	0x%x\n", spi_read(rDR));
}

void spi_flash_erase(void)
{
	/* write data */
	spi_write(rDR, 0xd8);
	/* write addr */
	spi_write(rDR, 0x0);
	spi_write(rDR, 0x0);
	spi_write(rDR, 0x0);

	/* polling for finish */
	while (spi_read(rSR) & 0x1);
	while (spi_read(rTXFLR) & 0x1);
	while (! (spi_read(rSR) & 0x8));
	udelay(10000);	/* important */
}
void spi_tx_flash(void)
{
	//spi_flash_erase();

	/* write data */
	spi_write(rDR, 0x2);
	/* write addr */
	spi_write(rDR, 0x0);
	spi_write(rDR, 0x0);
	spi_write(rDR, 0x0);
	/* read 3 byte */
#if 0
	spi_write(rDR, 0x1);	/* for receive only */
	spi_write(rDR, 0x2);	/* for reveive only */
	spi_write(rDR, 0x3);	/* for reveive only */
#else
	spi_write(rDR, 0x3);	/* for receive only */
	spi_write(rDR, 0x2);	/* for reveive only */
	spi_write(rDR, 0x1);	/* for reveive only */
#endif

	/* polling for finish */
	while (spi_read(rSR) & 0x1);
	while (spi_read(rTXFLR) & 0x1);
	while (! (spi_read(rSR) & 0x8));
	udelay(10000);	/* important */
}

int do_sflashr(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	spi_rxtx_mode();
	spi_timing_mode3();

	spi_rx_flash();
}

U_BOOT_CMD(
	sfr,	CFG_MAXARGS/*5*/,	1,	do_sflashr,
	"sfr     - flash read test\n",
	NULL
	);

int do_sflashw(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{     
	spi_rxtx_mode();
	spi_timing_mode3();

	spi_timing_mode0();
	spi_write(rDR, 0x06);
	mdelay(1);
	spi_timing_mode3();

	spi_tx_flash();

	spi_timing_mode0();
	spi_write(rDR, 0x04);
	mdelay(1);
	spi_timing_mode3();

	return 0;
}

U_BOOT_CMD(
	sfw,	CFG_MAXARGS/*5*/,	1,	do_sflashw,
	"sfw     - flash write test\n",
	NULL
	);

int do_sflashe(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{     
	spi_rxtx_mode();
	spi_timing_mode0();
	spi_write(rDR, 0x06);
	mdelay(1);
	spi_timing_mode3();

	spi_flash_erase();

	spi_timing_mode0();
	spi_write(rDR, 0x04);
	mdelay(1);
	spi_timing_mode3();

	return 0;
}

U_BOOT_CMD(
	sfe,	CFG_MAXARGS/*5*/,	1,	do_sflashe,
	"sfe     - flash erase test\n",
	NULL
	);

/*===================================================================
 * Two interface to write data bus.
 */

int do_spiw(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	spi_write(rDR, simple_strtoul(argv[1], NULL, 16));

	return 0;
}

int do_spir(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 1) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	printf("%x\n", spi_read(rDR));

	return 0;
}

U_BOOT_CMD(
	spiw,	CFG_MAXARGS/*5*/,	1,	do_spiw,
	"spiw    - wirte to spi bus(32)\n",
	NULL
	);
U_BOOT_CMD(
	spir,	CFG_MAXARGS/*5*/,	1,	do_spir,
	"spir    - read to spi bus(32)\n",
	NULL
	);
#endif

/* Do test */
#if 0
int do_spi_read1K(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned saddr, maddr, size;
	int i;


	saddr = simple_strtoul(argv[1], NULL, 16);
	maddr = simple_strtoul(argv[2], NULL, 16);
	size = simple_strtoul(argv[3], NULL, 16);

	for (i = 0; i < size; i++) {
		asm_spi_read_1k(saddr, maddr);
		saddr += 1024;
		maddr += 1024;
	}

	return 0;
}
U_BOOT_CMD(
	spir1k,	CFG_MAXARGS/*5*/,	1,	do_spi_read1K,
	"spir1k    - read 1k use asmbly method\n",
	NULL
	);

int do_nand_read(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned caddr, maddr, size;
	int i;


	caddr = simple_strtoul(argv[1], NULL, 16);
	maddr = simple_strtoul(argv[2], NULL, 16);
	size = simple_strtoul(argv[3], NULL, 16);

	NANDOpen();
	NANDRead_1K(maddr,caddr);
	for (i = 0; i <= size/1024; i++) {
		NANDRead_1K(maddr,caddr);
		caddr += 1024;
		maddr += 1024;
	}
	NANDClose();

	return 0;
}
U_BOOT_CMD(
	nandr,	CFG_MAXARGS/*5*/,	1,	do_nand_read,
	"nandr    - read 1k from nand\n",
	NULL
	);
#endif
