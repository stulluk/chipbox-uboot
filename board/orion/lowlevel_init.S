/*
 * Board specific setup info
 *
 * (C) Copyright 2003, ARM Ltd.
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

#include <config.h>
#include <version.h>

/* Set up the platform, once the cpu has been initialized */
.globl lowlevel_init
lowlevel_init:

    ldr  r2, DRAM_MISC
    ldr  r2, [r2]
    ands r2, r2, #0x10000000	/* Already Initialized */
    bne  DRAM_CONFIG_END

    /* Setup DRAM Frequency */
    ldr  r2, SW_RST_Addr
    ldrh r1, [r2]
    bic  r1, r1, #0x2
    strh r1, [r2]

    ldr  r2, DDR_PLL_Addr
    ldr  r1, DDR_PLL_Value
    strh r1, [r2]

    /* PLL need ~100us to be stable */
    mov  r2, #0x4000                 
1:  sub  r2, r2,#1
    cmp  r2, #0
    bne  1b    

    ldr  r2, SW_RST_Addr
    ldrh r1, [r2]
    orr  r1, r1, #0x2
    strh r1, [r2] 

    /* Init DRAM Controller */
    adr  r1, DRAM_Value
    adr  r2, DRAM_Addr
    mov  r3, r2
1:  ldr  r4, [r1], #4
    ldr  r5, [r2], #4
    str  r4, [r5]
    cmp  r1, r3
    bcc  1b

#ifdef CONFIG_ARCH_CSM1200
#ifndef CONFIG_SKIP_DELAY_CALIBRATION
    /*
     * Registers:
     *  r0      : LDM/STM address
     *  r1-r4   : Reference data
     *  r5-r8   : Actual data, temp use
     *  r9-r10  : Delay settings
     */
    adr         r0, TEST_PATTERN
    ldmia       r0, {r1-r4}
    mov         r0, #0x02000000
    stmia       r0, {r1-r4}

    mov         r9,  #0x0
    mov         r10, #0x0

test_delay_setting:
    mov         r6, #0x10000000          /* set new delay settings */
    orr		r6, r6, #0x110000
    strb        r9, [r6, #0x8] 
    strb        r9, [r6, #0x9] 
    strb        r9, [r6, #0xa] 
    strb        r9, [r6, #0xb]
    ldr		r6, [r6, #0x8] 

    ldmia       r0, {r5-r8}              /* read data and compare them */
    eor         r5, r5, r1
    eor         r6, r6, r2
    eor         r7, r7, r3
    eor         r8, r8, r4
    orr         r5, r5, r6
    orr         r5, r5, r7
    orrs        r5, r5, r8

    mov         r5, #1
    orreq       r10, r10, r5, lsl r9     /* record correct delay settings */

    add         r9, r9, #1
    cmp         r9, #16
    bne         test_delay_setting

    mov         r8, #0x10000000          /* set the final delay settings */
    orr         r8, r8, #0x110000
    str         r10, [r8, #0xc]          /* debug purpose */

    clz		r5, r10			 
    mvn		r10, r10, lsl r5
    clz		r6, r10
    add		r5, r6, r5, lsl #1
    rsb		r5, r5, #64		 /* tap = ceil((min+max)/2)  */
    mov		r5, r5, lsr #1           /* r5 = final delay setting */

    strb	r5, [r8, #0x8] 
    strb	r5, [r8, #0x9] 
    strb	r5, [r8, #0xa] 
    strb	r5, [r8, #0xb] 
    ldr		r8, [r8, #0x8]
#endif
#endif

DRAM_CONFIG_END:
    mov  pc, lr
		
.globl tcm_config_relocate
tcm_config_relocate:
    /*==================================================
      The following code is set the TCM
      wing code is set the TCM
    ==================================================*/                        
    /* config TCM register:
     1. set BASE=
     2. enable I/DTCM
     */

#define ITCM_BASE  0x20000000        // 32k
#define ITCM_SIZE  0x8000            //32k        

#define Region_32k 0x18
#define Region_Enable 0x1

      MOV r3, #ITCM_BASE                    //set I-TCM base address
      ORR r5, r3, #Region_Enable            //enable I-TCM
      ORR r5, r5, #Region_32k               //set I-TCM size
      MCR p15, 0, r5, c9, c1, 1             //write ITCM register

      LDR r3, DTCM_BASE
      ORR r5, r3, #Region_Enable            //enable D-TCM
      ORR r5, r5, #Region_32k               //set D-TCM size
      MCR p15, 0, r5, c9, c1, 0            //write DTCM register

      mov  pc, lr

DTCM_BASE:
    .word 	   0x20008000
SW_RST_Addr:
    .word          0x10171200

DDR_PLL_Addr:
    .word          0x10171100

#ifdef CONFIG_ARCH_CSM1100
DDR_PLL_Value:
    .word          0x00000625

DRAM_Value:
    .word          0x3a1b89ab
    .word          0x213b218a
    .word          0x05050505
    .word          0x05050505
    .word          0x11002020
DRAM_Addr:
    .word          0x10110000
    .word          0x10110004
    .word          0x10110008
    .word          0x1011000C
DRAM_MISC:
    .word          0x10110010
#endif

#if defined(CONFIG_ARCH_CSM1200) && !defined(CONFIG_CSM1200_OVERLOAD) 

DDR_PLL_Value:
//	.word          0x000016cc  //500M
//	.word          0x000012c8  //600M
//  .word          0x000012dc  //660M
//  .word          0x000014f7  //666.9M
//  .word          0x000014cc  //550M
    .word          0x0000084f  //533M
//	.word          0x000036c8  //200M
//	.word		   0x00000c59  //400M
DRAM_Value:
    
#ifdef CONFIG_256M_DDR
//  .word          0x72e4aa40
//  .word          0x45dc2293

    .word          0x8eedaea3
    .word          0x43412293
#else
//	.word          0x8badaec0
//	.word          0x1be82293
	.word          0x8eedaea3   //0x76adaea3
    .word          0x23412293
#endif
    .word          0x06060606
    .word          0x06060606
#ifdef CONFIG_16BIT_DDR
	.word          0x51100000
#else
    .word          0x11100000  
#endif 
DRAM_Addr:
    .word          0x10110000
    .word          0x10110004
    .word          0x10110008
    .word          0x1011000C
DRAM_MISC:
    .word          0x10110010 

TEST_PATTERN:
    .word          0x55aa55aa
    .word          0xaa55aa55
    .word          0xffffffff
    .word          0x00000000
#endif


#if defined(CONFIG_ARCH_CSM1200) && defined(CONFIG_CSM1200_OVERLOAD) 
DDR_PLL_Value:

	.word          0x000012c8    //600M
DRAM_Value:
    
	.word          0x83db6ea3   //0x76adaea3
    .word          0x25dc2213

    .word          0x05050505
    .word          0x05050505
    .word          0x11100000   
DRAM_Addr:
    .word          0x10110000
    .word          0x10110004
    .word          0x10110008
    .word          0x1011000C
DRAM_MISC:
    .word          0x10110010 

#endif

        
#ifdef CONFIG_ARCH_CSM1201
DDR_PLL_Value:
    .word          0x0000084f  // 533MHz

DRAM_Value:
    
#ifdef CONFIG_256M_DDR
    .word          0x97671892   // for 256M
    .word          0x937d0225   // for 256M
    .word          0x08080808  // only for PLL 533MHz of 256MB
    .word          0x08080808
    .word          0x30000465
#else // for 128M
    .word          0x77669890   
    .word          0x437d01c5
    .word          0x08080808  // only for PLL 533MHz
    .word          0x08080808
    .word          0x3000045c
#endif // 256MB/128MB

DRAM_Addr:
    .word          0x10110000
    .word          0x10110004
    .word          0x10110008
    .word          0x1011000C
DRAM_MISC:
    .word          0x10110010 

TEST_PATTERN:
    .word          0x55aa55aa
    .word          0xaa55aa55
    .word          0xffffffff
    .word          0x00000000
#endif
.global __aeabi_unwind_cpp_pr0
__aeabi_unwind_cpp_pr0:

