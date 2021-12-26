/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2003
 * Texas Instruments, <www.ti.com>
 * Kshitij Gupta <Kshitij@ti.com>
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
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
#include <command.h>
#include "irqs.h"
DECLARE_GLOBAL_DATA_PTR;


#if defined(CONFIG_CELESTIAL) && (defined(CONFIG_ARCH_CSM1200) || defined(CONFIG_ARCH_CSM1201) )
#define CE0_CFG_0 0x10100000
#define CE1_CFG_0 0x1010000C
#define CE2_CFG_0 0x10100018
#define CE3_CFG_0 0x10100024

#define CE0_CFG_1 0x10100004
#define CE1_CFG_1 0x10100010
#define CE2_CFG_1 0x1010001C
#define CE3_CFG_1 0x10100028

#define CE0_CFG_2 0x10100008
#define CE1_CFG_2 0x10100014
#define CE2_CFG_2 0x10100020
#define CE3_CFG_2 0x1010002C

#define CFG_32M_MODE 0x10100038

#define CS0_RWLENGTH 0x0C
#define CS0_CE_SETUP 0x01
#define CS0_CE_HOLD  0x0B

#define CS0_OE_SETUP 0x01
#define CS0_OE_HOLD  0x0B

#define CS0_WE_SETUP 0x02
#define CS0_WE_HOLD  0x0A

#define CS0_BE_SETUP 0x01
#define CS0_BE_HOLD  0x0B


#define CS1_RWLENGTH 0x08
#define CS1_CE_SETUP 0x00
#define CS1_CE_HOLD  0x08

#define CS1_OE_SETUP 0x01
#define CS1_OE_HOLD  0x06

#define CS1_WE_SETUP 0x03
#define CS1_WE_HOLD  0x06

#define CS1_BE_SETUP 0x02
#define CS1_BE_HOLD  0x06

#endif  

#if defined(CONFIG_ARCH_CSM1201)
#define GPIO_PIN_MUX1 0x10260030
#define GPIO_PIN_MUX2 0x10260044
#endif 

#if 0 // test interrupt handle
#define PCLK_PERIOD	1818    /* in 10 picosecond */
#define ORIONRC_BASE		0x10172000
#define ORIONRC_INT_STA 	(0x000)
#define ORIONRC_INT_MSK 	(0x004)
#define ORIONRC_CTL_LO		(0x008)
#define ORIONRC_CTL_HI		(0x00A)
#define ORIONRC_RC5_LO  	(0x100)
#define ORIONRC_RC5_HI  	(0x102)
#define ORIONRC_NEC_LO  	(0x104)
#define ORIONRC_NEC_HI		(0x106)
#define ORIONKEY_DATA_L 	(0x300) /* Key Scan Data Register */
#define ORIONKEY_CNTL_L		(0x304) /* Key Scan Control Register */
#define ORIONKEY_CNTL_H 	(0x306) /* Key Scan Control Register */
#endif

void flash__init (void);
void ether__init (void);
void peripheral_power_enable (void);
extern void spi__init(void);

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

#define COMP_MODE_ENABLE ((unsigned int)0x0000EAEF)
#define PA_ETHMAC		0x34080000



static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0" (loops));
}


#ifdef CONFIG_USE_IRQ 

irq_handle orion_irq_vector[NR_IRQS+1];
void * orion_irq_data[NR_IRQS +1];

// int panel_irq_handle(unsigned int irq, void * data)
// {
//     int status,keydata;
//     printf("in panel irq_handle \n");
//     status = *((int *)(ORIONRC_BASE+ORIONRC_INT_STA));
//     printf("status1 = 0x%x\n",status);
//     data=*((int *)(ORIONRC_BASE+ORIONKEY_DATA_L));
//     printf("key data = 0x%x\n",keydata);

//     return 0;
// }

void orion_irq_enable(unsigned int irq)
{
	volatile unsigned int *irq_inten_l=(unsigned int *)(PA_VIC_BASE+VIC_IRQ_ENABLE);
	*irq_inten_l|=(1<<irq);
}

void orion_irq_disable(unsigned int irq)
{
	volatile unsigned int *irq_inten_l=(unsigned int *)(PA_VIC_BASE+VIC_IRQ_ENABLE);
	*irq_inten_l&=~(1<<irq);
}
void orion_irq_ack(unsigned int irq)
{	
	volatile unsigned int *irq_inten_l=(unsigned int *)(PA_VIC_BASE+VIC_IRQ_ENABLE);
	*irq_inten_l&=~(1<<irq);
}

void orion_register_irq(int irq, irq_handle irq_r, void * irq_data)
{
    if (irq>=0 && irq<= NR_IRQS){
        orion_irq_vector[irq] = irq_r;    
        orion_irq_data[irq] = irq_data;
    }
}

void orion_unregister_irq(int irq)
{
    if (irq>=0 && irq<= NR_IRQS){
        orion_irq_vector[irq] = NULL;    
        orion_irq_data[irq] = NULL;
    }
}
void orion_init_irq(void)
{
    int i;
//    int panel_sel;
//    int rc_mode;
//    int bit_time_cnt;
    
    *((unsigned int *)(PA_VIC_BASE + VIC_IRQ_ENABLE))= 0;
    *((unsigned int *)(PA_VIC_BASE + VIC_IRQ_MASK)) = 0;
	for (i=0; i<= NR_IRQS; i++) {
        orion_irq_vector[i] = NULL;
        orion_irq_data[i] = NULL;
    }

    //*((int *)(0x10172004)) = 0;
    //*((int *)(0x10172008)) = 1;
	
    /* Test Interrupt handle */
// 	panel_sel    = 11;	
// 	rc_mode	     = 0x1;
// 	bit_time_cnt = 112000000 / PCLK_PERIOD;

//         *((int *)(ORIONRC_BASE+ORIONRC_CTL_LO))  = ((0xc000 | (bit_time_cnt >> 8)) << 16 )| (rc_mode | (panel_sel << 2) | ((bit_time_cnt & 0xff) << 8));

//     	*((int *)(ORIONRC_BASE+ORIONKEY_CNTL_L)) = 0x000f << 16 |0x0001;

//      *((int *)(ORIONRC_BASE+ORIONRC_INT_MSK)) &= ~0x3;                         /* enable IRQ */
//      orion_register_irq(8, panel_irq_handle, (void *) NULL);
//      orion_irq_enable(8);
}

int orion_do_interrupt(void)
{

    int vec,ret;
    unsigned int  isr;
   
    printf("handling interrupt....\n");
    isr = (*(unsigned int *)(PA_VIC_BASE+VIC_IRQ_STATUS));
    for (vec = 0; vec <= NR_IRQS; vec++) {
        if (isr & (1<<vec)) {
            if (NULL != orion_irq_vector[vec]){
                ret =(orion_irq_vector[vec])(vec,orion_irq_data[vec]);
                return ret;
            }
        }
    }
    return -1;
}


#endif

/*
 * Miscellaneous platform dependent initialisations
 */

int board_init (void)
{
	
    /* arch number of ORION Board */
	gd->bd->bi_arch_number = 719;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x00000100;

	gd->flags = 0;

#if defined(CONFIG_ARCH_CSM1201)
    *((unsigned int *)GPIO_PIN_MUX1) = 0x0;
    *((unsigned int *)GPIO_PIN_MUX2) = 0x0;
#endif

	icache_enable ();

	flash__init ();
	ether__init ();
	spi__init();

#ifdef CONFIG_USE_IRQ 
    orion_init_irq();
#endif



#ifdef CONFIG_CSM1200_OVERLOAD
    //unsigned int temp;
    //temp = *((unsigned int *)0x10171408);
    *((unsigned int *)0x10171408) = 0x416; /* changed AVS_PLL clock to overload the decoder*/
    
    *((unsigned int *)0x418000cc) = 0x1;  /* disable DF prepare for updating*/
    *((unsigned int *)0x4180011c) = 0xe0; /* 128M is 0xe0, 64M (single bank) 0xc0 */
    *((unsigned int *)0x418000cc) = 0x0;  /* enable DF to update */
#endif 
	return 0;
}


int misc_init_r (void)
{
	setenv("verify", "n");
	return (0);
}

/******************************
 Routine:
 Description:
******************************/
void flash__init (void)
{
	 unsigned int *ce_cfg;
	 unsigned int ce_cfg_val;
     unsigned char iobits;
 
#if defined(CONFIG_ARCH_CSM1200) && defined(PHYS_FLASH_SIZE)

  
     ce_cfg = (unsigned int *)CE0_CFG_0;
	 iobits = ((*ce_cfg) >> 24) & 0x1;
	 
	 ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS0_RWLENGTH);
     //    printf("0x%x\n",ce_cfg_val);
     *ce_cfg = ce_cfg_val;
     
     ce_cfg = (unsigned int *)CE0_CFG_1;
     *ce_cfg = (CS0_OE_HOLD << 24) | (CS0_OE_SETUP << 16) | (CS0_CE_HOLD << 8) | CS0_CE_SETUP;    /* configure OE and CE Timing */
     //   printf("CE0_CFG_1 0x%x\n",*ce_cfg);
     ce_cfg = (unsigned int *)CE0_CFG_2;
     *ce_cfg = (CS0_BE_HOLD << 24) | (CS0_BE_SETUP << 16) | (CS0_WE_HOLD << 8) | CS0_WE_SETUP;    /* configure BE and WE Timing */
     //   printf("CE0_CFG_2 0x%x\n",*ce_cfg);


     if (PHYS_FLASH_SIZE > 0x800000){ /* Flash more than 8 MB */
         ce_cfg = (unsigned int *)CE1_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS0_RWLENGTH);
         *ce_cfg = ce_cfg_val;
         
         ce_cfg = (unsigned int *)CE1_CFG_1;
         *ce_cfg = (CS0_OE_HOLD << 24) | (CS0_OE_SETUP << 16) | (CS0_CE_HOLD << 8) | CS0_CE_SETUP;    /* configure OE and CE Timing */
         
         ce_cfg = (unsigned int *)CE1_CFG_2;
         *ce_cfg = (CS0_BE_HOLD << 24) | (CS0_BE_SETUP << 16) | (CS0_WE_HOLD << 8) | CS0_WE_SETUP;    /* configure BE and WE Timing */
         
         ce_cfg = (unsigned int *)CE1_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(1 << 24))) | (iobits << 24);
         *ce_cfg = ce_cfg_val;
     }
     else { /* configuration for CS1 to operate NAND Flash */
         ce_cfg = (unsigned int *)CE1_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS1_RWLENGTH);
         *ce_cfg = ce_cfg_val;
         
         ce_cfg = (unsigned int *)CE1_CFG_1;
         *ce_cfg = (CS1_OE_HOLD << 24) | (CS1_OE_SETUP << 16) | (CS1_CE_HOLD << 8) | CS1_CE_SETUP;    /* configure OE and CE Timing */
         
         ce_cfg = (unsigned int *)CE1_CFG_2;
         *ce_cfg = (CS1_BE_HOLD << 24) | (CS1_BE_SETUP << 16) | (CS1_WE_HOLD << 8) | CS1_WE_SETUP;    /* configure BE and WE Timing */
         
         ce_cfg = (unsigned int *)CE1_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(1 << 24))) | (iobits << 24);
         *ce_cfg = ce_cfg_val;

     }
     if (PHYS_FLASH_SIZE > 0x1000000){ /* Flash more than 16 MB */
 
         ce_cfg = (unsigned int *)CE2_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS0_RWLENGTH);
         *ce_cfg = ce_cfg_val;
         
         ce_cfg = (unsigned int *)CE2_CFG_1;
         *ce_cfg = (CS0_OE_HOLD << 24) | (CS0_OE_SETUP << 16) | (CS0_CE_HOLD << 8) | CS0_CE_SETUP;    /* configure OE and CE Timing */
         
         ce_cfg = (unsigned int *)CE2_CFG_2;
         *ce_cfg = (CS0_BE_HOLD << 24) | (CS0_BE_SETUP << 16) | (CS0_WE_HOLD << 8) | CS0_WE_SETUP;    /* configure BE and WE Timing */
 
         ce_cfg = (unsigned int *)CE2_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(1 << 24))) | (iobits << 24);
         *ce_cfg = ce_cfg_val;
     }
     if (PHYS_FLASH_SIZE > 0x1800000){ /* Flash more than 24 MB */
 
         ce_cfg = (unsigned int *)CE3_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS0_RWLENGTH);
         *ce_cfg = ce_cfg_val;
     
         ce_cfg = (unsigned int *)CE3_CFG_1;
         *ce_cfg = (CS0_OE_HOLD << 24) | (CS0_OE_SETUP << 16) | (CS0_CE_HOLD << 8) | CS0_CE_SETUP;    /* configure OE and CE Timing */
         
         ce_cfg = (unsigned int *)CE3_CFG_2;
         *ce_cfg = (CS0_BE_HOLD << 24) | (CS0_BE_SETUP << 16) | (CS0_WE_HOLD << 8) | CS0_WE_SETUP;    /* configure BE and WE Timing */
 
         ce_cfg = (unsigned int *)CE3_CFG_0;
         ce_cfg_val = ((*ce_cfg) & (~(1 << 24))) | (iobits << 24);
         *ce_cfg = ce_cfg_val;
     }
#endif




#if defined(CONFIG_ARCH_CSM1201) //&& !defined(CONFIG_32M_FLASHMODE)

	ce_cfg = (unsigned int *)CE0_CFG_0;
	 ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS0_RWLENGTH);
     *ce_cfg = ce_cfg_val;
     
     ce_cfg = (unsigned int *)CE0_CFG_1;
     *ce_cfg = (CS0_OE_HOLD << 24) | (CS0_OE_SETUP << 16) | (CS0_CE_HOLD << 8) | CS0_CE_SETUP;    /* configure OE and CE Timing */

     ce_cfg = (unsigned int *)CE0_CFG_2;
     *ce_cfg = (CS0_BE_HOLD << 24) | (CS0_BE_SETUP << 16) | (CS0_WE_HOLD << 8) | CS0_WE_SETUP;    /* configure BE and WE Timing */
#endif


#if defined(CONFIG_32M_FLASHMODE) || defined(CONFIG_16M_FLASHMODE)
     unsigned int *config_32m_mode;
     config_32m_mode = (unsigned int *) CFG_32M_MODE;
	 *config_32m_mode = 1;  /* enable 32M mode */

	 ce_cfg_val = *((unsigned int *) CE0_CFG_0);
	 ce_cfg = (unsigned int *) CE1_CFG_0;
	 *ce_cfg = ce_cfg_val; 
	 ce_cfg = (unsigned int *) CE2_CFG_0;
	 *ce_cfg = ce_cfg_val;
	 ce_cfg = (unsigned int *) CE3_CFG_0;
	 *ce_cfg = ce_cfg_val;

	 ce_cfg_val = *((unsigned int *) CE0_CFG_1);
	 ce_cfg = (unsigned int *) CE1_CFG_1;
	 *ce_cfg = ce_cfg_val; 
	 ce_cfg = (unsigned int *) CE2_CFG_1;
	 *ce_cfg = ce_cfg_val;
	 ce_cfg = (unsigned int *) CE3_CFG_1;
	 *ce_cfg = ce_cfg_val;

	 ce_cfg_val = *((unsigned int *) CE0_CFG_2);
	 ce_cfg = (unsigned int *) CE1_CFG_2;
	 *ce_cfg = ce_cfg_val; 
 	 ce_cfg = (unsigned int *) CE2_CFG_2;
	 *ce_cfg = ce_cfg_val;
	 ce_cfg = (unsigned int *) CE3_CFG_2;
	 *ce_cfg = ce_cfg_val;

#else
     /* configuration for CS1 NAND flash */
     ce_cfg = (unsigned int *)CE1_CFG_0;
     ce_cfg_val = ((*ce_cfg) & (~(0xff))) | (CS1_RWLENGTH);
     *ce_cfg = ce_cfg_val;
     
     ce_cfg = (unsigned int *)CE1_CFG_1;
     *ce_cfg = (CS1_OE_HOLD << 24) | (CS1_OE_SETUP << 16) | (CS1_CE_HOLD << 8) | CS1_CE_SETUP;    /* configure OE and CE Timing */
     
     ce_cfg = (unsigned int *)CE1_CFG_2;
     *ce_cfg = (CS1_BE_HOLD << 24) | (CS1_BE_SETUP << 16) | (CS1_WE_HOLD << 8) | CS1_WE_SETUP;    /* configure BE and WE Timing */
         

#endif 

   
}
/*************************************************************
 Routine:ether__init
 Description: take the Ethernet controller out of reset and wait
	  		   for the EEPROM load to complete.
*************************************************************/
void ether__init (void)
{

}

/******************************
 Routine:
 Description:
******************************/
int dram_init (void)
{
    gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
    gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

    return 0;
}


