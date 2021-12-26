/*
 * Description:
 * 		Fronpannel led show implementation, reference to
 * 		kernel driver.
 *
 * Author: Celestial 
 */
#include <common.h>
#include <command.h>

/*************************************************************
 FPC_INIT
 Description: Boot with a default sign on fpc led.
 	      For a better interaction.
*************************************************************/

/* Register Map */
#define ORION_FPC_BASE          0x10172000

#define FPC_INTS                (0x00  >> 1) /* Interrupt Status Register */
#define FPC_INTM                (0x04  >> 1) /* Interrupt Mask Register */
#define FPC_RC_CTL_L            (0x08  >> 1) /* Remote Control Register */
#define FPC_RC_CMD_L            (0x100 >> 1) /* RC5 Command Register */
#define FPC_NEC_CMD_L           (0x104 >> 1) /* NEC Command Register */
#define FPC_LED_DATA_L          (0x200 >> 1) /* Led Display Data Register */
#define FPC_LED_CNTL_L          (0x204 >> 1) /* Led Display Control Register */
#define FPC_KSCAN_DATA_L        (0x300 >> 1) /* Key Scan Data Register */
#define FPC_KSCAN_CNTL_L        (0x304 >> 1) /* Key Scan Control Register */
#define FPC_ARBCNTL_L           (0x308 >> 1) /* Led & Key Scan Arbiter Control Register */
#define FPC_RC_CTL_H            (0x0a  >> 1) /* Remote Control Register */
#define FPC_RC_CMD_H            (0x102 >> 1) /* RC5 Command Register */
#define FPC_NEC_CMD_H           (0x106 >> 1) /* NEC Command Register */
#define FPC_LED_DATA_H          (0x202 >> 1) /* Led Display Data Register */
#define FPC_LED_CNTL_H          (0x206 >> 1) /* Led Display Control Register */
#define FPC_KSCAN_DATA_H        (0x302 >> 1) /* Key Scan Data Register */
#define FPC_KSCAN_CNTL_H        (0x306 >> 1) /* Key Scan Control Register */
#define FPC_ARBCNTL_H           (0x30a >> 1) /* Led & Key Scan Arbiter Control Register */


#define fpc_writew(v,a)         do { *((volatile unsigned short *)ORION_FPC_BASE + (a)) = v; }while(0)
#define fpc_readw(a)            *((volatile unsigned short  *)ORION_FPC_BASE + (a))

static unsigned char num_tab[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xff */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x10 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x20 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff,
	0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, /* 0x30 */
	0x01, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x11, 0xff, 0x63, 0xff, 0x61, 0x71, 0xff, /* 0x40 */
	0x91, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff,
	0x31, 0xff, 0xff, 0xff, 0x73, 0x83, 0xff, 0xff, /* 0x50 */
	0xff, 0x89, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x60 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x70 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x80 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x90 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xa0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xb0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xc0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xd0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xe0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xf0 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


void fpc_led_enable(int cmd) 
{
	fpc_writew(0x8000, FPC_LED_CNTL_H);
	fpc_writew(0 != cmd ? 0x1000 : 0x0000, FPC_LED_CNTL_L);
}

void fpc_led_disp(int arg)
{
	fpc_writew((arg >> 16) & 0xffff, FPC_LED_DATA_H);
	fpc_writew(arg & 0xffff, FPC_LED_DATA_L);
}

void fpc_init_show(const char *string)
{
	int i, val;
	char  str[5] = { 0 };

	sprintf(str,"%-4s", string);

	for (val = 0, i = 0; i<4 ; i++)
		val |= num_tab[(int)str[i]] << (8 * i);

	fpc_led_disp(val);
	fpc_led_enable(1);
}

#if defined(CONFIG_ARCH_CSM1201) /* for csm1201 */

#define GPIO_PINMUX() do { \
	volatile unsigned long *pinmux_base;                                    \
	pinmux_base = 0x10260000;    						\
	pinmux_base[0x030 >> 2] &= 0xffff0000;                                  \
	udelay(20);                                                             \
} while(0)

#else /* for csm1100 & csm1200 */

#define GPIO_PINMUX() do {}while(0)

#endif

int do_fpc_led (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	 GPIO_PINMUX();
	 fpc_init_show(argv[1]);

	 return 0;
}

U_BOOT_CMD(
	fpcd,	CFG_MAXARGS/*5*/,	1,	do_fpc_led,
	"fpcd    - Display data on frontpanel led\n",
	" <data>\n"
	);

int do_fpcraw (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	u32 val;

	val = simple_strtoul(argv[1], NULL, 16);

	GPIO_PINMUX();

	fpc_led_disp(val);
	fpc_led_enable(1);

	return 0;
}

U_BOOT_CMD(
	fpcraw,	CFG_MAXARGS/*5*/,	1,	do_fpcraw,
	"fpcraw  - Display data on frontpanel led\n",
	" <data>\n"
	);
