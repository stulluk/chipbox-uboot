
.text

#define LOADREGS(cond, base, reglist...)\
        ldm##cond       base,reglist

#define ENTER   \
                mov     ip,sp   ;\
                stmfd   sp!,{r0,r4-r9,fp,ip,lr,pc}      ;\
                sub     fp,ip,#4

#define EXIT    \
                LOADREGS(ea, fp, {r0, r4 - r9, fp, sp, pc})

#define EXITEQ  \
                LOADREGS(eqea, fp, {r0, r4 - r9, fp, sp, pc})


.globl asm_spi_read_1k
asm_spi_read_1k:

      ENTER
      mov r2, r0

_reset_spi:

      # configurate spi baud rate
      ldr   r9, SPI_SSIENR
      mov   r0,  #0x0
      str  r0,  [r9]
	
      #configurate spi contrl register 0
      ldr   r9, SPI_CTRLR0
      ldr   r0,  SPI_CR0INI
      str  r0, [r9]

      #configurate spi control register 1
      ldr   r9, SPI_CTRLR1
      ldr   r0,  SPI_CR1INI	@4K byte data
      str  r0,  [r9]

      ldr   r9, SPI_BAUDR
      ldr   r0,  SPI_BDINIT
      str  r0,  [r9]	     	
  
      # select one spi slave
      ldr   r9, SPI_SER
      mov   r0,  #0x1
      str  r0,  [r9]

      #enable spi
      ldr   r9, SPI_SSIENR
      mov   r0,  #0x1
      str  r0,  [r9]

#if 1
      # configurate contol byte and address
      ldr   r8, SPI_DR	
      mov   r0,  #0x3
      str  r0,  [r8]

#if 1
      # SH: send addr
      mov   r0, r2
      mov   r0,  r0,   lsl  #8
      mov   r0,  r0,   lsr  #24
      str  r0,  [r8]

      mov   r0, r2
      mov   r0,  r0,   lsl  #16
      mov   r0,  r0,   lsr  #24
      str  r0,  [r8]

      mov   r0, r2
      mov   r0,  r0,   lsl  #24
      mov   r0,  r0,   lsr  #24
      str  r0,  [r8]
#else
        mov   r0,  #0x0
	str r0, [r8]
	str r0, [r8]
	str r0, [r8]
#endif

      # set read number
      ldr   r6, SPI_CP_NUM 
      ldr   r7, SPI_SR
			   	      
_wait_spi:
      teq   r6,  #0x0
      beq   _end_spi
			   	      
1:    ldr   r0,  [r7]
      ands  r0,  r0,   #0x8
      bne   _copy_spi
      b     1b
 		
_copy_spi:
      sub   r6,  r6,   #0x1
      ldrb  r5,  [r8]	
      strb  r5,  [r1], #0x1
      b     _wait_spi		
     	
_end_spi:

      ldr   r9, SPI_SER
      mov   r0,  #0x0
      str  r0,  [r9]

      #disable spi
      ldr   r9, SPI_SSIENR
      mov   r0,  #0x0
      str  r0,  [r9]
      b     _copy_end

#endif

_copy_end:
      EXIT
      mov  pc, lr

/* delay about 30us */
_udelay30:
      mov   r6, #400 
1:
      sub   r6,  r6,   #0x1
      teq   r6,  #0x0
      bne   1b
      mov  pc, lr

SPI_CTRLR0:
	.word 0x10173000
SPI_CTRLR1:
	.word 0x10173004
SPI_SSIENR:
	.word 0x10173008
SPI_SER:    
	.word 0x10173010
SPI_BAUDR:
	.word 0x10173014
SPI_SR:     
	.word 0x10173028
SPI_DR:    
	.word 0x10173060
SPI_BDINIT: 
	.word 0x00000038
	#.word 0x000000c8
SPI_CR0INI: 
	.word 0x000003c7
SPI_CR1INI: 
	.word 0x000003ff
SPI_RX_CNT: 
	.word 0x10173024
SMC_BOOT:   
	.word 0x10100034
SPI_CP_NUM:
	.word 0x400
