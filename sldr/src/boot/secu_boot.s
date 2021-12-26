#==================================================
#  Celestial Semiconductior Inc.
#        
#  Description:  
#       bootrom to support NAND Flash booting
#                        
#  Author:  Jiasheng Chen (jiasheng.chen@celestialsemi.com) 
#  Rev 0.1 @ 2008.02.18 First Edition
#
#  Details:
#       (1) INITRAM = 1 and VINITHI = 1
#       (2) Execute from 0xffff0000
#       (3) Copy the Boot Room Data Section to 4KBytes(ITCM)
#       (4) Jump the SecuBootMain()
#       (5) Sig Check the Boot Image(NOR_FLASH, NAND_FLASH, SPI FLASH)
#           if Failed never return;
#       (6) bootrom copy NAND Flash to 0x0(ITCM) with Fixed size 4KBytes
#       (7) bootrom jump to 0x0 at last
#        
#=================================================
_reset:
        b               _start                           @ Reset Vector 
1:      b               1b
1:      b               1b
1:      b               1b 
1:      b               1b 
1:      b               1b
1:      b               1b
1:      b               1b
#const from Link Scrips

DATA_IMG_START:
  .word data_img_start
DATA_SEC_START:
  .word data_start
DATA_SEC_END  :
  .word data_end
BSS_SEC_START :
  .word bss_start
BSS_SEC_END   :
  .word bss_end

#sys const
STACK_BASE :
  .word bss_end
SYSTEM_STACK_SIZE       =             (1024 * 8)
IRQ_STACK_SIZE          =             4096
FIQ_STACK_SIZE          =             4096               
                
MODE_MASK               =             0x1f
USR_MODE                =             0x10
FIQ_MODE                =             0x11                
IRQ_MODE                =             0x12
SVC_MODE                =             0x13
ABT_MODE                =             0x17
UND_MODE                =             0x1b
SYS_MODE                =             0x1f

IRQ_MASK                =             0x80
FIQ_MASK                =             0x40

.global _start
_start:
      #=============================================================
      # Enable ICache
      #=============================================================
      mrc p15, 0, r0, c1, c0, 0
      orr r0, r0, #0x00001000
      # Enable DCache
      #orr r0, r0, #0x00000100
      mcr p15, 0, r0, c1, c0, 0

      #============================================
      #Initial DATA Section
      #============================================
      #0.0 Initial
      ldr   r8,  DATA_IMG_START
      ldr   r9,  DATA_SEC_START
      ldr   r10, DATA_SEC_END
      ldr   r11, BSS_SEC_START
      ldr   r12, BSS_SEC_END
            
      #1.0 BSS Clear
1:
			mov r0, #0
			str r0, [r11], #4
      cmp r11, r12
      bcc 1b
            
      # ***************************************************
      # Initialize the system stack pointers for C Main        
      # ***************************************************
      MRS r1, CPSR                  @ pick up current PSR
      BIC r1, r1, #MODE_MASK        @ clear the mode bits
      ORR r1, r1, #SVC_MODE         @ set the SVC mode bits
      MSR CPSR, r1                  @ setup the new CPSR
         
      ldr    r1, STACK_BASE
      MOV    r2, #SYSTEM_STACK_SIZE
      SUB    r2, r2, #4             @subtract one word for first addr
      ADD    r3, r1, r2             @build start of system stack area
      BIC    r3, r3, #3             @ensure 4-byte aligned
      MOV    sp, r3
    
      mov    r0, #0x00000000        @clear r0-r12, lr
      orr    r1, r0, r0             @for clean simulation
      stmfd  sp!, {r0-r1}
      ldmfd  sp!, {r2-r3}
      stmfd  sp!, {r0-r3}
      ldmfd  sp!, {r4-r7}
      stmfd  sp!, {r0-r7}
      ldmfd  sp!, {r6-r12, lr}      
      
      #============================================
      # Jump into the C Main Function
      # (1) if Do not Need to do Secu Check or
      #     Secu Check Passed,
      #     C main will do the first 4K Sys Boot Code 
      #     Copy
      # (2) if Secu Check Failed main will never return
      #============================================
      bl  main
_copy_end:
    
	  #================================================
      # Start the Sys U-Boot Image from DDR Region at 0x02000000
      #================================================
      mov r0, #0x02000000
      # blx r0
      mov   pc, r0
 
