/*
 * (C) Copyright 2006 Detlev Zundel, dzu@denx.de
 * (C) Copyright 2006 DENX Software Engineering
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


#if (CONFIG_COMMANDS & CFG_CMD_NAND)
#if !defined(CFG_NAND_LEGACY)

#include <nand.h>

extern void udelay (unsigned long usec);
/*
 *	hardware specific access to control-lines
 */

static int orion_device_ready(struct mtd_info *mtd)
{
    if ((*((int *)(0x10100030))) & 0x1){
        return 1;
    }
    else {
        return 0;
    }
	
}
 

static void orion_hwcontrol(struct mtd_info *mtd, int cmd)
{
	struct nand_chip *this = mtd->priv;
	unsigned int value;
	
	switch(cmd) {
	case NAND_CTL_SETCLE:        /* Select the command latch by setting CLE to high */
		 this->IO_ADDR_W = ORION_NAND_CLE_BASE; //	*(((volatile __u8 *) this->IO_ADDR_W) + 0xa) = 0;
#ifdef NAND_DEBUG
	printf("SET_CLE: Set Write Address 0x%x\n",this->IO_ADDR_W);
#endif 
		break;
	case NAND_CTL_CLRCLE:        /* Deselect the command latch by setting CLE to low */
		this->IO_ADDR_W = CFG_NAND0_BASE;  // *(((volatile __u8 *) this->IO_ADDR_W) + 0x8) = 0;
#ifdef NAND_DEBUG
	printf("CLR_CLE: Set Write Address 0x%x\n",this->IO_ADDR_W);
#endif 
		break;
	case NAND_CTL_SETALE:        /* Select the address latch by setting ALE to high */
		this->IO_ADDR_W = ORION_NAND_ALE_BASE; // *(((volatile __u8 *) this->IO_ADDR_W) + 0x9) = 0;
#ifdef NAND_DEBUG
	printf("SET_ALE: Set Write Address 0x%x\n",this->IO_ADDR_W);
#endif 
		break;
	case NAND_CTL_CLRALE:        /* Deselect the address latch by setting ALE to low */
		this->IO_ADDR_W = CFG_NAND0_BASE ; // *(((volatile __u8 *) this->IO_ADDR_W) + 0x8) = 0;
#ifdef NAND_DEBUG
		printf("CLR_ALE: Set Write Address 0x%x\n",this->IO_ADDR_W);
#endif 
		break;
	case NAND_CTL_SETNCE:        /* Select the chip by setting nCE to low *///		*(((volatile __u8 *) this->IO_ADDR_W) + 0x8) = 0;
		value = *((unsigned int *)ORION_NAND_CFG_BASE);
		*((unsigned int *)(0x1010000c)) = value | 0x00000200;
		break;
	case NAND_CTL_CLRNCE:        /* Deselect the chip by setting nCE to high *///		*(((volatile __u8 *) this->IO_ADDR_W) + 0xc) = 0;
		value = *((unsigned int *)ORION_NAND_CFG_BASE);
		*((unsigned int *)(0x1010000c)) = value & (~0x00000200);

		break;
	}
}

/*
 * Board-specific NAND initialization. The following members of the
 * argument are board-specific (per include/linux/mtd/nand.h):
 * - IO_ADDR_R?: address to read the 8 I/O lines of the flash device
 * - IO_ADDR_W?: address to write the 8 I/O lines of the flash device
 * - hwcontrol: hardwarespecific function for accesing control-lines
 * - dev_ready: hardwarespecific function for  accesing device ready/busy line
 * - enable_hwecc?: function to enable (reset)  hardware ecc generator. Must
 *   only be provided if a hardware ECC is available
 * - eccmode: mode of ecc, see defines
 * - chip_delay: chip dependent delay for transfering data from array to
 *   read regs (tR)
 * - options: various chip options. They can partly be set to inform
 *   nand_scan about special functionality. See the defines for further
 *   explanation
 * Members with a "?" were not set in the merged testing-NAND branch,
 * so they are not set here either.
 */
int board_nand_init(struct nand_chip *nand)
{

	nand->hwcontrol = orion_hwcontrol;
	nand->eccmode = NAND_ECC_SOFT;
#if defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201)
    nand->dev_ready = orion_device_ready;
#endif 
	nand->chip_delay = 30;
	nand->options = NAND_USE_FLASH_BBT | NAND_BBT_LASTBLOCK | NAND_BBT_CREATE|NAND_BBT_VERSION | NAND_BBT_WRITE;
	return 0;
}

#else  /* defined(CFG_NAND_LEGACY)*/
#error "nand_legacy is not implemented !"
#endif /* !defined(CFG_NAND_LEGACY) */

#endif /* (CONFIG_COMMANDS & CFG_CMD_NAND) */
