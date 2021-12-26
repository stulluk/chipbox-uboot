
/*#include <linux/config.h>*/
/*#include <linux/types.h>*/
/*#include <linux/module.h>*/
/*#include <linux/kernel.h>*/
/*#include <linux/ioport.h>*/
/*#include <linux/hdreg.h>*/
/*#include <linux/ide.h>*/
/*#include <linux/delay.h>*/
/*#include <linux/init.h>*/

#include <common.h>
#include <asm/io.h>
/*#include <asm/hardware/clock.h>*/
/*#include <asm/mach-types.h>*/

#include "palm_bk3710.h"
/*#include "ide-timing.h"*/

#if 0
MODULE_AUTHOR("Zhongkai Du, <zhongkai.du@celestialsemi.com>");
MODULE_DESCRIPTION("Celestial Semiconductor IDE Controller driver");
MODULE_LICENSE("GPL");
#endif

/*
 * Standard (generic) timings for Taskfile modes, from ATA2 specification.
 * Some drives may specify a mode, while also specifying a different
 * value for cycle_time (from drive identification data).
 */
const palm_bk3710_piotiming palm_bk3710_tasktimings[] = {
	{290, 600},	/* PIO Mode 0 */
	{290, 383},	/* PIO Mode 1 */
	{290, 240},	/* PIO Mode 2 */
	{290, 240},	/* PIO Mode 2 */
	{290, 240}	/* PIO Mode 2 */
};

/*
 * Standard (generic) timings for PIO modes, from ATA2 specification.
 * Some drives may specify a mode, while also specifying a different
 * value for cycle_time (from drive identification data).
 */
const palm_bk3710_piotiming palm_bk3710_piotimings[] = {
	{165, 600},	/* PIO Mode 0 */
	{125, 383},	/* PIO Mode 1 */
	{100, 240},	/* PIO Mode 2 */
	{80, 180},	/* PIO Mode 3 with IORDY */
	{70, 120}	/* PIO Mode 4 with IORDY */
};

/*
 * Standard (generic) timings for DMA modes, from ATA2 specification.
 * Some drives may specify a mode, while also specifying a different
 * value for cycle_time (from drive identification data).
 */
const palm_bk3710_dmatiming palm_bk3710_dmatimings[] = {
	{215, 215, 480},/* DMA Mode 0 */
	{80,  50,  150},/* DMA Mode 1 */
	{70,  25,  120} /* DMA Mode 2 */
};

/*
 * Standard (generic) timings for UDMA modes, from ATA2 specification.
 * Some drives may specify a mode, while also specifying a different
 * value for cycle_time (from drive identification data).
 */
const palm_bk3710_udmatiming palm_bk3710_udmatimings[] = {
	{45, 160, 114},	/* UDMA Mode 0 */
	{45, 125, 75}, 	/* UDMA Mode 1 */
	{45, 100, 55},	/* UDMA Mode 2 */
	{20, 100, 39}, 	/* UDMA Mode 3 */
	{20, 85,  25}, 	/* UDMA Mode 4 */
	{20, 85,  17}	/* UDMA Mode 5 */
};

volatile palm_bk3710_ideregs *palm_bk3710_base = (volatile palm_bk3710_ideregs*)0x10210000;

// DELME static spinlock_t palm_bk3710_spinlock = SPIN_LOCK_UNLOCKED;

#if defined(PALM_BK3710_DEBUG)
#define DBG(fmt, arg...)	printk(fmt, ##arg)
#else
#define DBG(fmt, arg...) 
#endif

static int palm_bk3710_setpiomode (palm_bk3710_ideregs *, unsigned int, unsigned int);

/*
 * Set the device PIO mode on Palm Chip 3710.
 *
 * In:
 *   handle, IDE Controller info
 *   dev, drive to tune
 *   level, desired level
 *
 * Out:
 *   level in PIO mode
 */
static int palm_bk3710_setpiomode (palm_bk3710_ideregs *handle, unsigned int dev, unsigned int mode)
{
	int is_slave = (dev == 1) ? 1 : 0;

	if (!is_slave) {
		/* setup master device parameters */
		palm_bk3710_base->config.idetimp |= 0xb301; 

		/* Disable UDMA for Device 0 */
		palm_bk3710_base->config.udmactl &= 0xFF02;

	} else {
		/* setup slave device parameters */
		palm_bk3710_base->config.idetimp |= 0xb310;
 
		/* Disable UDMA for Device 1 */
		palm_bk3710_base->config.udmactl &= 0xFF01;
	}

	DBG("%s: idetimp=0x%x udmactl=0x%x \n",
		__FUNCTION__, 
		palm_bk3710_base->config.idetimp,
		palm_bk3710_base->config.udmactl);

	return mode;
}

/*
 * Configures the Palm Chip Controller in the desired default operating mode
 */
int palm_bk3710_chipinit (void)
{
	int atacount = 0;

	/* enable the reset_en of ATA controller so that when ata signals are brought
	 * out , by writing into device config. at that time por_n signal should not be
	 * 'Z' and have a stable value.
	 */
	palm_bk3710_base->config.miscctl = 0x0300;

	/* wait for some time and deassert the reset of ATA Device. */
	for(atacount = 0; atacount < 5; atacount++);

	/* Deassert the Reset */
	palm_bk3710_base->config.miscctl = 0x0200;

	/* 
	 * Program the IDETIMP Register Value based on the following assumptions
	 *
	 * (ATA_IDETIMP_IDEEN      , ENABLE ) |
	 * (ATA_IDETIMP_SLVTIMEN   , ENABLE) |
	 * (ATA_IDETIMP_RDYSMPL    , 70NS)    |
	 * (ATA_IDETIMP_RDYRCVRY   , 50NS)    |
	 * (ATA_IDETIMP_DMAFTIM1   , PIOCOMP) |
	 * (ATA_IDETIMP_PREPOST1   , ENABLE) |
	 * (ATA_IDETIMP_RDYSEN1    , ENABLE) |
	 * (ATA_IDETIMP_PIOFTIM1   , ENABLE) |
	 * (ATA_IDETIMP_DMAFTIM0   , PIOCOMP) |
	 * (ATA_IDETIMP_PREPOST0   , ENABLE) |
	 * (ATA_IDETIMP_RDYSEN0    , ENABLE) |
	 * (ATA_IDETIMP_PIOFTIM0   , ENABLE)
	 */
	palm_bk3710_base->config.idetimp = 0xb3ff;

	/* 
	 * Configure  SIDETIM  Register
	 * (ATA_SIDETIM_RDYSMPS1   ,120NS ) |
	 * (ATA_SIDETIM_RDYRCYS1   ,120NS )
	 */
	palm_bk3710_base->config.sidetim = 0;

	/* 
	 * UDMACTL Ultra-ATA DMA Control
	 * (ATA_UDMACTL_UDMAP1     ,0) |
	 * (ATA_UDMACTL_UDMAP0     ,0)
	 */
	palm_bk3710_base->config.udmactl = 0;

	/* 
	 * MISCCTL Miscellaneous Conrol Register
	 * (ATA_MISCCTL_RSTMODEP   ,1) |
	 * (ATA_MISCCTL_RESETP     ,0) |
	 * (ATA_MISCCTL_TIMORIDE   ,1)
	 */
	palm_bk3710_base->config.miscctl = 0x201;

	/* 
	 * IORDYTMP IORDY Timer for Primary Register
	 * (ATA_IORDYTMP_IORDYTMP  ,0xffff)
	 */
	palm_bk3710_base->config.iordytmp = 0xffff;

	/*
	 * Configure BMISP Register
	 * (ATA_BMISP_DMAEN1     ,DISABLE) |
	 * (ATA_BMISP_DMAEN0	 ,DISABLE) |
	 * (ATA_BMISP_IORDYINT	 ,CLEAR) |
	 * (ATA_BMISP_INTRSTAT	 ,CLEAR) |
	 * (ATA_BMISP_DMAERROR	 ,CLEAR)
	 */
	palm_bk3710_base->dmaengine.bmisp = 0;

	palm_bk3710_setpiomode (NULL, 0, 0);
	palm_bk3710_setpiomode (NULL, 1, 0);

	return 1;
}

