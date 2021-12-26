/*
 *  armboot - Startup Code for ARM926EJS CPU-core
 *
 *  Copyright (c) 2003  Texas Instruments
 *
 *  ----- Adapted for OMAP1610 OMAP730 from ARM925t code ------
 *
 *  Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 *  Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 *  Copyright (c) 2002	Gary Jennejohn <gj@denx.de>
 *  Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 *  Copyright (c) 2003	Kshitij <kshitij@ti.com>
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

#if defined(CONFIG_OMAP1610)
#include <./configs/omap1510.h>
#elif defined(CONFIG_OMAP730)
#include <./configs/omap730.h>
#endif

/*
 *************************************************************************
 *
 * Jump vector table as in table 3.1 in [1]
 *
 *************************************************************************
 */


.globl _start
_start:
	b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word software_interrupt
_prefetch_abort:
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word irq
_fiq:
	.word fiq

	.balignl 16,0xdeadbeef


/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */

_TEXT_BASE:
	.word	TEXT_BASE

.globl _armboot_start
_armboot_start:
	.word _start

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

#ifdef CONFIG_USE_IRQ
/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word 0x0badc0de
#endif


/*
 * the actual reset code
 */

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

	/*
	 * we do sys-critical inits only at reboot,
	 * not when booting from ram!
	 */
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	bl	cpu_init_crit
#endif

#if (CONFIG_COMMANDS & CFG_CMD_NAND) && defined(CFG_ORION_NAND_BOOT)

	  adr	  r0, _start		/* r0 <- current position of code   */
	  mov r1,#0x0		      /* test if we run from Nand flash or RAM */
	  cmp     r0, r1                  /* don't copy code from nand during debug         */
	  bne     tcm_relocate
	  bl  copy_myself
	 
	 /*jump to ram */
	  ldr   r1, =on_the_ram
	  add  pc, r1, #0
	  nop
	  nop
	  1:    b     1b          /* infinite loop*/
 
on_the_ram:
#endif
tcm_relocate:
  bl tcm_config_relocate

#ifndef CONFIG_SKIP_RELOCATE_UBOOT
relocate:				/* relocate U-Boot to RAM	    */
	adr	r0, _start		/* r0 <- current position of code   */
	ldr	r1, _TEXT_BASE		/* test if we run from flash or RAM */
	cmp     r0, r1                  /* don't reloc during debug         */
	beq     stack_setup

	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2		/* r2 <- size of armboot            */
	add	r2, r0, r2		/* r2 <- source end address         */

copy_loop:
	ldmia	r0!, {r3-r10}		/* copy from source address [r0]    */
	stmia	r1!, {r3-r10}		/* copy to   target address [r1]    */
	cmp	r0, r2			/* until source end addreee [r2]    */
	ble	copy_loop
#endif	/* CONFIG_SKIP_RELOCATE_UBOOT */

	/* Set up the stack						    */
stack_setup:
	ldr	r0, _TEXT_BASE		/* upper 128 KiB: relocated uboot   */
	sub	r0, r0, #CFG_MALLOC_LEN	/* malloc area                      */
	sub	r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo                        */
#ifdef CONFIG_USE_IRQ
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12		/* leave 3 words for abort-stack    */

clear_bss:
	ldr	r0, _bss_start		/* find start of bss segment        */
	ldr	r1, _bss_end		/* stop here                        */
	mov 	r2, #0x00000000		/* clear                            */

clbss_l:str	r2, [r0]		/* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l

#ifdef CONFIG_CELESTIAL       

    ldr pc, _relocated_position
relocated_position:

    /* Remap to NORMAL mode */
    mov  r1, #1
    ldr  r2, _remap_ctrl
    strh r1, [r2]
    /* Disable bootcfg, enable 656 output */
    ldr  r1, _enable_display656
    ldrh r2, [r1]
    orr  r3, r2, #0x1000
    strh r3, [r1]

    adr     r0, _start
    mov     r1, #0x0

    ldr     r2, _copy_len
    add     r2, r2, r0
copy_loop_vec:
   	ldmia	r0!, {r3-r10}		/* copy from source address [r0]    */
	stmia	r1!, {r3-r10}		/* copy to   target address [r1]    */
	cmp	r0, r2			/* until source end addreee [r2]    */
	ble	copy_loop_vec

    b    start_armboot

_relocated_position:
	.word relocated_position
_remap_ctrl:
    .word 0x101e0008
_enable_display656:
    .word 0x10171404
_copy_len:
    .word 0x3c
                
#else

    ldr pc, _start_armboot

_start_armboot:
    .word start_armboot

#endif

#if (CONFIG_COMMANDS & CFG_CMD_NAND) && defined(CFG_ORION_NAND_BOOT)
copy_myself:
			mov r2, lr
      #============================================
      # Get boot settings
      #============================================
      ldr   r0,  SMC_BOOT
      ldr   r7,  [r0]            @boot settings

      #============================================
      # Copy NAND Flash
      #============================================
      ldr   r8,  _TEXT_BASE      @destination address
      ldr 	r3, _bss_start
	    sub	  r9, r3, r8		        /* r9 <- size of uboot text section            */
      mov   r3,  #0               /*Page Counter */
			mov   r13, #0

      ldr   r10, =CFG_NAND0_BASE
      ldr   r11, =ORION_NAND_CLE_BASE
      ldr   r12, =ORION_NAND_ALE_BASE

      ands  r1, r7, #0x4
      bne   _reset_16

_reset_8:
      mov   r0, #0xff
      strb  r0, [r11]
      bl    _wait_ready
      b    _copy_begin

_reset_16:
      mov   r0, #0xff
      strh  r0, [r11]
      bl    _wait_ready
      b    _copy_begin

_copy_begin:

      ands  r1, r7, #0x2
      bne   _copy_large_block

_copy_small_block:

      ands  r1, r7, #0x4
      bne   _copy_small_block_16

      #small block at 8bit mode      
_copy_small_block_8:
      mov   r0,  #0
      strb  r0,  [r11]
      strb  r0,  [r12]
      strb  r3,  [r12]
      strb  r13, [r12]
      ands  r1, r7, #0x8
      bne   1f
      strb  r0,  [r12] 
1:    bl    _wait_ready
      mov   r0,  #0
1:    ldrb  r1,  [r10]
      strb  r1,  [r8],  #1
      add   r0,  r0,  #1
      teq   r0,  #256
      bne   1b

      mov   r0,  #1
      strb  r0,  [r11]
      mov   r0,  #0
      strb  r0,  [r12]
      strb  r3,  [r12]
      strb  r13, [r12]
      ands  r1, r7, #0x8
      bne   1f
      strb  r0,  [r12] 
1:    bl    _wait_ready 
      mov   r0,  #0
1:    ldrb  r1,  [r10]
      strb  r1,  [r8],  #1
      add   r0,  r0,  #1
      teq   r0,  #256
      bne   1b 
			
			add   r3, r3, #1
			teq   r3,  #256
			bne   1f
			mov   r3, #0
			add   r13, r13, #1
			
1:    subs   r9,  r9, #256
      bmi   _copy_end

      b     _copy_small_block_8

      #small block at 16bit mode
_copy_small_block_16:
      mov   r0,  #0
      strh  r0,  [r11]
      strh  r0,  [r12]
      strh  r3,  [r12]
      strh  r13,  [r12]
      ands  r1, r7, #0x8
      bne   1f
      strh  r0,  [r12] 
1:    bl    _wait_ready
      mov   r0,  #0
1:    ldrh  r1,  [r10]
      strh  r1,  [r8],  #2
      add   r0,  r0,  #1
      teq   r0,  #128
      bne   1b

      mov   r0,  #1
      strh  r0,  [r11]
      mov   r0,  #0
      strh  r0,  [r12]
      strh  r3,  [r12]
      strh  r13, [r12]
      ands  r1, r7, #0x8
      bne   1f
      strh  r0,  [r12] 
1:    bl    _wait_ready 
      mov   r0,  #0
1:    ldrh  r1,  [r10]
      strh  r1,  [r8],  #2
      add   r0,  r0,  #1
      teq   r0,  #128
      bne   1b 

			add   r3, r3, #1
			teq   r3,  #256
			bne   1f
			mov   r3, #0
			add   r13, r13, #1

1:    subs   r9,  r9, #128
      bmi   _copy_end


      b    _copy_small_block_16      

_copy_large_block:

      ands  r1, r7, #0x4
      bne   _copy_large_block_16 

      #large block at 8bit mode
_copy_large_block_8:
      mov   r0,  #0
      strb  r0,  [r11]
      strb  r0,  [r12]
      strb  r0,  [r12]
      strb  r3,  [r12]
      strb  r13,  [r12]
      ands  r1, r7, #0x8
      bne   1f
      strb  r0,  [r12]
1:    mov   r0,  #0x30 
      strb  r0,  [r11]
      bl    _wait_ready
      mov   r0,  #0
1:    ldrb  r1,  [r10]
      strb  r1,  [r8],  #1
      add   r0,  r0,  #1
      teq   r0,  #2048
      bne   1b

			add   r3, r3, #1
			teq   r3,  #256
			bne   1f
			mov   r3, #0
			add   r13, r13, #1
			
1:    subs   r9,  r9, #2048
      bmi   _copy_end


      b     _copy_large_block_8 

      #large block at 16bit mode
_copy_large_block_16:
      mov   r0,  #0
      strh  r0,  [r11]
      strh  r0,  [r12]
      strh  r0,  [r12]
      strh  r3,  [r12]
      strh  r13,  [r12]
      ands  r1, r7, #0x8
      bne   1f
      strh  r0,  [r12]
1:    mov   r0,  #0x30  
      strh  r0,  [r11]
      bl    _wait_ready
      mov   r0,  #0
1:    ldrh  r1,  [r10]
      strh  r1,  [r8],  #2
      add   r0,  r0,  #1
      teq   r0,  #1024
      bne   1b

			add   r3, r3, #1
			teq   r3,  #256
			bne   1f
			mov   r3, #0
			add   r13, r13, #1

1:		subs   r9,  r9, #1024
      bmi   _copy_end  

			b    _copy_large_block_16          

_wait_ready:
      ldr   r5, SMC_STA

      mov   r4, #0
1:    ldr   r6, [r5]
      add   r4, r4, #1
      teq   r4, #20
      bne   1b

1:    ldr   r6, [r5]
      ands  r6, r6, #1
      beq   1b
      mov   pc, r14
        
_copy_end: 
			mov pc, r2			
#endif   /* CFG_ORION_NAND_BOOT */



/*
 *************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************
 */


cpu_init_crit:
	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	/* clear bits 13, 9:8 (--V- --RS) */
	bic	r0, r0, #0x00000087	/* clear bits 7, 2:0 (B--- -CAM) */
	orr	r0, r0, #0x00000002	/* set bit 2 (A) Align */
	orr	r0, r0, #0x00001000	/* set bit 12 (I) I-Cache */
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Go setup Memory and board specific bits prior to relocation.
	 */
	mov	ip, lr		/* perserve link reg across call */
	bl	lowlevel_init	/* go setup pll,mux,memory */
	mov	lr, ip		/* restore link */
	mov	pc, lr		/* back to my caller */
/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */

@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */

	.macro	bad_save_user_regs
	@ carve out a frame on current user stack
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}	@ Save user registers (now in svc mode) r0-r12

	ldr	r2, _armboot_start
	sub	r2, r2, #(CONFIG_STACKSIZE+CFG_MALLOC_LEN)
	sub	r2, r2, #(CFG_GBL_DATA_SIZE+8)  @ set base 2 words into abort stack
	@ get values for "aborted" pc and cpsr (into parm regs)
	ldmia	r2, {r2 - r3}
	add	r0, sp, #S_FRAME_SIZE		@ grab pointer to old stack
	add	r5, sp, #S_SP
	mov	r1, lr
	stmia	r5, {r0 - r3}	@ save sp_SVC, lr_SVC, pc, cpsr
	mov	r0, sp		@ save current stack into r0 (param register)
	.endm

	.macro	irq_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	@ !!!! R8 NEEDS to be saved !!!! a reserved stack spot would be good.
	add	r8, sp, #S_PC
	stmdb	r8, {sp, lr}^		@ Calling SP, LR
	str	lr, [r8, #0]		@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]		@ Save CPSR
	str	r0, [r8, #8]		@ Save OLD_R0
	mov	r0, sp
	.endm

	.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #S_PC]			@ Get PC
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4		@ return & move spsr_svc into cpsr
	.endm

	.macro get_bad_stack
	ldr	r13, _armboot_start		@ setup our mode stack
	sub	r13, r13, #(CONFIG_STACKSIZE+CFG_MALLOC_LEN)
	sub	r13, r13, #(CFG_GBL_DATA_SIZE+8) @ reserved a couple spots in abort stack

	str	lr, [r13]	@ save caller lr in position 0 of saved stack
	mrs	lr, spsr	@ get the spsr
	str	lr, [r13, #4]	@ save spsr in position 1 of saved stack
	mov	r13, #MODE_SVC	@ prepare SVC-Mode
	@ msr	spsr_c, r13
	msr	spsr, r13	@ switch modes, make sure moves will execute
	mov	lr, pc		@ capture return pc
	movs	pc, lr		@ jump to next instruction & switch modes.
	.endm

	.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, IRQ_STACK_START
	.endm

	.macro get_fiq_stack			@ setup FIQ stack
	ldr	sp, FIQ_STACK_START
	.endm

/*
 * exception handlers
 */
	.align  5
undefined_instruction:
	get_bad_stack
	bad_save_user_regs
	bl	do_undefined_instruction

	.align	5
software_interrupt:
	get_bad_stack
	bad_save_user_regs
	bl	do_software_interrupt

	.align	5
prefetch_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_prefetch_abort

	.align	5
data_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_data_abort

	.align	5
not_used:
	get_bad_stack
	bad_save_user_regs
	bl	do_not_used

#ifdef CONFIG_USE_IRQ

	.align	5
irq:
	get_irq_stack
	irq_save_user_regs
	bl 	do_irq
	irq_restore_user_regs

	.align	5
fiq:
	get_fiq_stack
	/* someone ought to write a more effiction fiq_save_user_regs */
	irq_save_user_regs
	bl 	do_fiq
	irq_restore_user_regs

#else

	.align	5
irq:
	get_bad_stack
	bad_save_user_regs
	bl	do_irq

	.align	5
fiq:
	get_bad_stack
	bad_save_user_regs
	bl	do_fiq

#endif

#if (CONFIG_COMMANDS & CFG_CMD_NAND) && defined(CFG_ORION_NAND_BOOT)
   .align     2
DW_STACK_START:
 .word   STACK_BASE+STACK_SIZE-4
SMC_STA:    
 .word   0x10100030
SMC_BOOT:   
 .word   0x10100034
#endif
