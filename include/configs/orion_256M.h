/*
 * (C) Copyright 2003
 * Texas Instruments.
 * Kshitij Gupta <kshitij@ti.com>
 * Configuation settings for the TI OMAP Innovator board.
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 * Configuration for Versatile PB.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM926EJS	1	/* This is an arm926ejs CPU core  */
#define CONFIG_ORION	    1	/* in ORION Platform Board	*/
#define CONFIG_ARCH_ORION   1	/* Specifically, a ORION	*/

#define CFG_PCLK_FREQ		55000000

#define CFG_MEMTEST_START       0x100000
#define CFG_MEMTEST_END         0x10000000
#define CFG_HZ                  CFG_PCLK_FREQ
#define CFG_TIMERBASE           0x101E2000	/* Timer 0 and 1 base */

#define CFG_TIMER_INTERVAL	10000
#define CFG_TIMER_RELOAD	(CFG_TIMER_INTERVAL >> 4)	/* Divide by 16 */
#define CFG_TIMER_CTRL          0x84				/* Enable, Clock / 16 */

/*
 * System controller bit assignment
 */
#define VERSATILE_REFCLK	0
#define VERSATILE_TIMCLK	1

#define VERSATILE_TIMER1_EnSel	15
#define VERSATILE_TIMER2_EnSel	17
#define VERSATILE_TIMER3_EnSel	19
#define VERSATILE_TIMER4_EnSel	21

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs  */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_MISC_INIT_R		1	/* call misc_init_r during start up */
/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN	(CFG_ENV_SIZE + 128*1024)
#define CFG_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

/*
 * NS16550 Configuration
 */
#define CFG_NS16550
#define CFG_NS16550_SERIAL
#define CFG_NS16550_REG_SIZE       (-4)
#define CFG_NS16550_CLK        CFG_PCLK_FREQ     
#define CFG_NS16550_COM1       0x101F1000     
#define CFG_NS16550_COM2       0x101F2000     

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1         1         

#define CONFIG_CONS_INDEX      1
#define CONFIG_BAUDRATE        115200
#define CFG_BAUDRATE_TABLE     { 9600, 19200, 38400, 57600, 115200, 230400 }

#ifdef  CONFIG_ARCH_ORION_13
#define CONFIG_DRIVER_DM9000   0
#define CONFIG_DM9000_BASE     0x35000000
#define DM9000_IO              CONFIG_DM9000_BASE
#define DM9000_DATA           (CONFIG_DM9000_BASE+4)
#define CONFIG_DM9000_USE_16BIT
#endif /* CONFIG_ARCH_ORION_13 */

#ifdef CONFIG_ARCH_ORION_14
#define CONFIG_DRIVER_CN100
#define CONFIG_ENV_OVERWRITE
#endif

#if defined(CONFIG_ARCH_ORION_14)
#define CONFIG_COMMANDS	(CFG_CMD_LOADS      | CFG_CMD_LOADB     | \
                         CFG_CMD_DHCP       | CFG_CMD_IMI       | \
                         CFG_CMD_NET        | CFG_CMD_PING      | \
                         CFG_CMD_BDI        | CFG_CMD_MEMORY    | \
                         CFG_CMD_FLASH      | CFG_CMD_ENV       | \
                         CFG_CMD_MISC       | CFG_CMD_NET       | \
                         CFG_CMD_DHCP       | CFG_CMD_I2C       | \
                         CFG_CMD_NAND       | CFG_CMD_EEPROM  )
#endif

#if defined(CONFIG_ARCH_ORION_13)
#define CONFIG_COMMANDS	(CFG_CMD_LOADS      | CFG_CMD_LOADB     | \
                         CFG_CMD_DHCP       | CFG_CMD_IMI       | \
                         CFG_CMD_NET        | CFG_CMD_PING      | \
                         CFG_CMD_BDI        | CFG_CMD_MEMORY    | \
                         CFG_CMD_FLASH      | CFG_CMD_ENV       | \
                         CFG_CMD_MISC       | CFG_CMD_NET       | \
                         CFG_CMD_DHCP       | CFG_CMD_I2C )
#endif

#define CONFIG_BOOTP_MASK	CONFIG_BOOTP_DEFAULT
/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

/*Configuration for the board */
#if defined(CONFIG_ARCH_ORION_14) && defined(CONFIG_ARCH_ORION_14_NAND)

#if (CONFIG_COMMANDS & CFG_CMD_NAND) 
#define CFG_ORION_NAND_BOOT 1
#endif 
#if (CONFIG_COMMANDS & CFG_CMD_FLASH)
#define CFG_ORION_NOR_FLASH_SUPPORT 1
#endif

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
#define CFG_ORION_ETHADDR_FROM_EEPROM 1
#else
#define CFG_ORION_ETHADDR_FROM_NOR_FLASH 1
#endif
#endif 

#if defined(CONFIG_ARCH_ORION_14) && !defined(CONFIG_ARCH_ORION_14_NAND)
#if (CONFIG_COMMANDS & CFG_CMD_FLASH)
#define CFG_ORION_NOR_FLASH_SUPPORT 1
#endif

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
#define CFG_ORION_ETHADDR_FROM_EEPROM 1
#else
#define CFG_ORION_ETHADDR_FROM_NOR_FLASH 1
#endif

#endif 

#if defined(CONFIG_ARCH_ORION_13)
#if (CONFIG_COMMANDS & CFG_CMD_FLASH)
#define CFG_ORION_NOR_FLASH_SUPPORT 1
#define CFG_ORION_ETHADDR_FROM_NOR_FLASH 1
#endif
#endif 





#define CONFIG_BOOTDELAY	4
#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_BOOTARGS "root=/dev/rd/0 mem=64M console=ttyS0,115200"
/*#define CONFIG_BOOTCOMMAND "bootp ; bootm" */

/*
 * Static configuration when assigning fixed address
 */
/*#define CONFIG_NETMASK	255.255.255.0	/--* talk on MY local net */
/*#define CONFIG_IPADDR		xx.xx.xx.xx	/--* static IP I currently own */
/*#define CONFIG_SERVERIP	xx.xx.xx.xx	/--* current IP of my dev pc */
#define CONFIG_BOOTFILE	    "/tftpboot/uImage" /* file to load */


/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP	/* undef to save memory     */
#define CFG_PROMPT	"ORION # "	/* Monitor Command Prompt   */
#define CFG_CBSIZE	256		/* Console I/O Buffer Size  */
/* Print Buffer Size */
#define CFG_PBSIZE	(CFG_CBSIZE+sizeof(CFG_PROMPT)+16)
#define CFG_MAXARGS	16		/* max number of command args   */
#define CFG_BARGSIZE	CFG_CBSIZE	/* Boot Argument Buffer Size    */

#undef	CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */
#define CFG_LOAD_ADDR	0x7fc0	/* default load address */

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 *----------------------------------------------------------------------*/
#define CONFIG_NR_DRAM_BANKS    1	/* we have 1 bank of DRAM */
#define PHYS_SDRAM_1            0x00000000	/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE       0x10000000	/* 128 MB */



/*-----------------------------------------------------------------------
 * I2C
 *----------------------------------------------------------------------*/
#define CONFIG_HARD_I2C     1       /* I2C with hardware support    */
#define CFG_I2C_SPEED       100000  /* I2C speed and slave address  */
#define CFG_I2C_SLAVE       0x7f
#define CFG_I2C_BASE	    0x10170000
#define CFG_I2C_CLK	    CFG_PCLK_FREQ
#define CFG_EEPROM_PAGE_WRITE_DELAY_MS 5

/*-----------------------------------------------------------------------
 * EEPROM Support
 *----------------------------------------------------------------------*/
#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
#define CFG_I2C_EEPROM_ADDR 0x50
#define CFG_I2C_EEPROM_ADDR_LEN 2
#define CFG_ORION_ETH_EEPROM_OFFSET 0x114
  /* The EEPROM can do 32byte ( 1 << 5 ) page writes. */
#define CFG_EEPROM_PAGE_WRITE_BITS 5
//#define CFG_I2C_MULTI_EEPROMS 1
#endif



/*-----------------------------------------------------------------------
 * NAND FLASH Support
 *----------------------------------------------------------------------*/

#if (CONFIG_COMMANDS & CFG_CMD_NAND)

#if defined(CFG_ORION_NAND_BOOT)
#define CFG_NAND0_BASE		        0x34000000 /* */
#define ORION_NAND_CLE_BASE 		0x34000002
#define ORION_NAND_ALE_BASE         0x34000004
#define NAND_PAGE_SIZE              2048

#define STACK_BASE                  0x20000
#define STACK_SIZE                  0x8000
#define UBOOT_RAM_BASE              0x40000
#define UBOOT_RAM_LEN               0x20000
#else
#define CFG_NAND0_BASE	        	0x34800000 /* */
#define ORION_NAND_CLE_BASE 		0x34800002
#define ORION_NAND_ALE_BASE         0x34800004

#define NAND_PAGE_SIZE              2048

#define STACK_BASE                  0x20000
#define STACK_SIZE                  0x8000
#define UBOOT_RAM_BASE              0x40000
#define UBOOT_RAM_LEN               0x20000
#endif /*  defined(CFG_ORION_NAND_BOOT) */

#define CFG_NAND_BASE_LIST	{ CFG_NAND0_BASE }
#define CFG_MAX_NAND_DEVICE	1	/* Max number of NAND devices */
#define NAND_MAX_CHIPS 1
#endif  /*(CONFIG_COMMANDS & CFG_CMD_NAND) */



/* -------------------------------------------------
*NOR Flash Memory Map  and CFI Envirment variable                   
*--------------------------------------------------*/
#if defined(CFG_ORION_NOR_FLASH_SUPPORT)
#if defined(CFG_ORION_NAND_BOOT) 
#define CFG_FLASH_BASE          0x34800000
//#define CFG_ORION_NOR_FLASH 1    /* For supporting non CFI NOR Flash such as SST */
#define CFG_FLASH_CFI           1       /* Flash is CFI conformant              */
#define CFG_FLASH_CFI_DRIVER    1       /* Use the common driver                */
#define CFG_FLASH_USE_BUFFER_WRITE  1   /* flash write speed-up                 */
#else
#define CFG_FLASH_BASE          0x34000000
//#define CFG_ORION_NOR_FLASH 1    /* For supporting non CFI NOR Flash such as SST */
#define CFG_FLASH_CFI           1       /* Flash is CFI conformant              */
#define CFG_FLASH_CFI_DRIVER    1       /* Use the common driver                */
#define CFG_FLASH_USE_BUFFER_WRITE  1   /* flash write speed-up                 */
#endif
#endif
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define VERSATILE_SYS_BASE                    0x10100000
#define VERSATILE_SYS_FLASH_OFFSET            0xA4
#define VERSATILE_FLASHCTRL		      (VERSATILE_SYS_BASE + VERSATILE_SYS_FLASH_OFFSET)
#define VERSATILE_FLASHPROG_FLVPPEN	      (1 << 1)	/* Enable writing to flash */

#define CFG_MAX_FLASH_BANKS	1		/* max number of memory banks */
#define PHYS_FLASH_SIZE         0x800000	/* 8MB */
/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20*CFG_HZ)	/* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(20*CFG_HZ)	/* Timeout for Flash Write */
#define CFG_MAX_FLASH_SECT	(256)

#define PHYS_FLASH_1		(CFG_FLASH_BASE)


/*-----------------------------------------------------------------------
* Envirment variable in NOR/NAND  Flash
*-----------------------------------------------------------------------*/
#define CFG_ENV_SIZE            0x00010000      /* Total Size of Environment Sector */
#define CFG_ENV_OFFSET          0x00050000      /* environment starts here  */

#if (CONFIG_COMMANDS & CFG_CMD_NAND) && defined (CFG_ORION_NAND_BOOT)
#define CFG_ENV_IS_IN_NAND 1
#else
#define CFG_ENV_IS_IN_FLASH     1               /* env in flash instead of CFG_ENV_IS_NOWHERE *//*in NOR Flash*/
#define CFG_ENV_SECT_SIZE       0x00010000      /* 64 KB sectors */
#define CFG_ENV_ADDR            (CFG_FLASH_BASE + CFG_ENV_OFFSET)
#endif


/*----------------------------------------------------------------
* JFFS2 and MTD Partations SUPPORT 
*-----------------------------------------------------------------*/
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2)
#define CONFIG_JFFS2_CMDLINE
#define CONFIG_JFFS2_NAND
//#define MTDIDS_DEFAULT		"nand0=catcenter"
//#define MTDPARTS_DEFAULT	"mtdparts=catcenter:2m(nand)"
#endif


/*------------------------gfx related -------------------------*/
#define CFG_GFX
#define CFG_CMD_CSLOGO
#define CFG_SKETCH	/* draw pixel, line, rectangle and ASCII font */

#endif							/* __CONFIG_H */

