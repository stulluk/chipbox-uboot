
secu_loader.elf:     file format elf32-littlearm

Disassembly of section .text:

00000000 <_reset>:
#       (7) bootrom jump to 0x0 at last
#        
#=================================================
_reset:
        b               _start                           @ Reset Vector 
   0:	ea00000c 	b	38 <_start>
1:      b               1b
   4:	eafffffe 	b	4 <text_start+0x4>
1:      b               1b
   8:	eafffffe 	b	8 <text_start+0x8>
1:      b               1b 
   c:	eafffffe 	b	c <text_start+0xc>
1:      b               1b 
  10:	eafffffe 	b	10 <USR_MODE>
1:      b               1b
  14:	eafffffe 	b	14 <SVC_MODE+0x1>
1:      b               1b
  18:	eafffffe 	b	18 <ABT_MODE+0x1>
1:      b               1b
  1c:	eafffffe 	b	1c <UND_MODE+0x1>

00000020 <DATA_IMG_START>:
  20:	00000ea4 	andeq	r0, r0, r4, lsr #29

00000024 <DATA_SEC_START>:
  24:	00000ea4 	andeq	r0, r0, r4, lsr #29

00000028 <DATA_SEC_END>:
  28:	00000ea8 	andeq	r0, r0, r8, lsr #29

0000002c <BSS_SEC_START>:
  2c:	00000ea8 	andeq	r0, r0, r8, lsr #29

00000030 <BSS_SEC_END>:
  30:	00000eb4 	streqh	r0, [r0], -r4

00000034 <STACK_BASE>:
  34:	00000eb4 	streqh	r0, [r0], -r4

00000038 <_start>:
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
  38:	ee110f10 	mrc	15, 0, r0, cr1, cr0, {0}
      orr r0, r0, #0x00001000
  3c:	e3800a01 	orr	r0, r0, #4096	; 0x1000
      # Enable DCache
      #orr r0, r0, #0x00000100
      mcr p15, 0, r0, c1, c0, 0
  40:	ee010f10 	mcr	15, 0, r0, cr1, cr0, {0}

      #============================================
      #Initial DATA Section
      #============================================
      #0.0 Initial
      ldr   r8,  DATA_IMG_START
  44:	e51f802c 	ldr	r8, [pc, #-44]	; 20 <DATA_IMG_START>
      ldr   r9,  DATA_SEC_START
  48:	e51f902c 	ldr	r9, [pc, #-44]	; 24 <DATA_SEC_START>
      ldr   r10, DATA_SEC_END
  4c:	e51fa02c 	ldr	sl, [pc, #-44]	; 28 <DATA_SEC_END>
      ldr   r11, BSS_SEC_START
  50:	e51fb02c 	ldr	fp, [pc, #-44]	; 2c <BSS_SEC_START>
      ldr   r12, BSS_SEC_END
  54:	e51fc02c 	ldr	ip, [pc, #-44]	; 30 <BSS_SEC_END>
            
      #1.0 BSS Clear
1:
			mov r0, #0
  58:	e3a00000 	mov	r0, #0	; 0x0
			str r0, [r11], #4
  5c:	e48b0004 	str	r0, [fp], #4
      cmp r11, r12
  60:	e15b000c 	cmp	fp, ip
      bcc 1b
  64:	3afffffb 	bcc	58 <FIQ_MASK+0x18>
            
      # ***************************************************
      # Initialize the system stack pointers for C Main        
      # ***************************************************
      MRS r1, CPSR                  @ pick up current PSR
  68:	e10f1000 	mrs	r1, CPSR
      BIC r1, r1, #MODE_MASK        @ clear the mode bits
  6c:	e3c1101f 	bic	r1, r1, #31	; 0x1f
      ORR r1, r1, #SVC_MODE         @ set the SVC mode bits
  70:	e3811013 	orr	r1, r1, #19	; 0x13
      MSR CPSR, r1                  @ setup the new CPSR
  74:	e129f001 	msr	CPSR_fc, r1
         
      ldr    r1, STACK_BASE
  78:	e51f104c 	ldr	r1, [pc, #-76]	; 34 <STACK_BASE>
      MOV    r2, #SYSTEM_STACK_SIZE
  7c:	e3a02a02 	mov	r2, #8192	; 0x2000
      SUB    r2, r2, #4             @subtract one word for first addr
  80:	e2422004 	sub	r2, r2, #4	; 0x4
      ADD    r3, r1, r2             @build start of system stack area
  84:	e0813002 	add	r3, r1, r2
      BIC    r3, r3, #3             @ensure 4-byte aligned
  88:	e3c33003 	bic	r3, r3, #3	; 0x3
      MOV    sp, r3
  8c:	e1a0d003 	mov	sp, r3
    
      mov    r0, #0x00000000        @clear r0-r12, lr
  90:	e3a00000 	mov	r0, #0	; 0x0
      orr    r1, r0, r0             @for clean simulation
  94:	e1801000 	orr	r1, r0, r0
      stmfd  sp!, {r0-r1}
  98:	e92d0003 	stmdb	sp!, {r0, r1}
      ldmfd  sp!, {r2-r3}
  9c:	e8bd000c 	ldmia	sp!, {r2, r3}
      stmfd  sp!, {r0-r3}
  a0:	e92d000f 	stmdb	sp!, {r0, r1, r2, r3}
      ldmfd  sp!, {r4-r7}
  a4:	e8bd00f0 	ldmia	sp!, {r4, r5, r6, r7}
      stmfd  sp!, {r0-r7}
  a8:	e92d00ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
      ldmfd  sp!, {r6-r12, lr}      
  ac:	e8bd5fc0 	ldmia	sp!, {r6, r7, r8, r9, sl, fp, ip, lr}
      
      #============================================
      # Jump into the C Main Function
      # (1) if Do not Need to do Secu Check or
      #     Secu Check Passed,
      #     C main will do the first 4K Sys Boot Code 
      #     Copy
      # (2) if Secu Check Failed main will never return
      #============================================
      bl  main
  b0:	eb000001 	bl	bc <main>

000000b4 <_copy_end>:
_copy_end:
    
	  #================================================
      # Start the Sys U-Boot Image from DDR Region at 0x02000000
      #================================================
      mov r0, #0x02000000
  b4:	e3a00402 	mov	r0, #33554432	; 0x2000000
      # blx r0
      mov   pc, r0
  b8:	e1a0f000 	mov	pc, r0

000000bc <main>:



int main( int argc, char *argv[] )
{
  bc:	e52de004 	str	lr, [sp, #-4]!
	int rt = 0;

/* 1. Initial DDR*/
	InitDDR();
  c0:	eb0000b8 	bl	3a8 <InitDDR>
#if 0
	TestDDR();
/* 2. Initial Display and Set Back Ground 	*/
/* 3. Display the Context of DDR at 0x02000000 to Gfx Layer*/
 	{
 		unsigned int ImgStartAddr = 0;
		unsigned int ImgWidth     = 0;
		unsigned int ImgHeight    = 0;
		unsigned int ImgFormat    = 0;
 		GfxDispImg
		(
			ImgStartAddr = 0x02000000,
			ImgWidth  = 512, 
			ImgHeight = 320,
			ImgFormat = GFX_CF_RGB565
		);
 	}
#endif
/* 4. Copy the U-BOOT Region*/
 	SysImgCopyToDDR();
  c4:	eb0000f9 	bl	4b0 <SysImgCopyToDDR>
/* 5. Set BackGround to Blue*/
/*         DispSetBackGround(0x0, 0xFF, 0x0); 	*/
 	return 1;
}
  c8:	e3a00001 	mov	r0, #1	; 0x1
  cc:	e49df004 	ldr	pc, [sp], #4

000000d0 <HWReadInt>:
#endif

int HWReadInt   (unsigned int addr, unsigned int   *value)
{
	*value = HW_REG_READ(addr);
  d0:	e5903000 	ldr	r3, [r0]
  d4:	e5813000 	str	r3, [r1]
	return 1;
}
  d8:	e3a00001 	mov	r0, #1	; 0x1
  dc:	e1a0f00e 	mov	pc, lr

000000e0 <HWWriteInt>:
int HWWriteInt  (unsigned int addr, unsigned int    value)
{
	#ifdef TRACE_REG_LOG
	if (fpReg == NULL) 
	{
		fpReg = fopen("reg.log", "w+");
	}
	if (fpReg != NULL)
	{
		fprintf(fpReg, "REG_WRITE: ADDR(%08x) , Data (%08x)\n", addr, value);
		fflush(fpReg);
	}
	#endif
	HW_REG_WRITE(addr, value);
  e0:	e5801000 	str	r1, [r0]
	return 1;
}
  e4:	e3a00001 	mov	r0, #1	; 0x1
  e8:	e1a0f00e 	mov	pc, lr

000000ec <HWWriteShort>:

int HWWriteShort(unsigned int addr, unsigned short  value)
{
  ec:	e1a01801 	mov	r1, r1, lsl #16
  f0:	e1a01821 	mov	r1, r1, lsr #16
	*(volatile unsigned short*)(addr) = (value);
  f4:	e1c010b0 	strh	r1, [r0]
	return 1;
}
  f8:	e3a00001 	mov	r0, #1	; 0x1
  fc:	e1a0f00e 	mov	pc, lr

00000100 <HWReadShort>:
int HWReadShort (unsigned int addr, unsigned short *value)
{
	(*value) = *(volatile unsigned short*)(addr);
 100:	e1d030b0 	ldrh	r3, [r0]
 104:	e1c130b0 	strh	r3, [r1]
	return 1;
}
 108:	e3a00001 	mov	r0, #1	; 0x1
 10c:	e1a0f00e 	mov	pc, lr

00000110 <HWWriteByte>:

int HWWriteByte (unsigned int addr, unsigned char   value)
{
 110:	e20110ff 	and	r1, r1, #255	; 0xff
	#ifdef TRACE_REG_LOG
	if (fpReg == NULL) 
	{
		fpReg = fopen("reg.log", "w+");
	}
	if (fpReg != NULL)
	{
		fprintf(fpReg, "REG_WRITE: ADDR(%08x) , Data (%08x)\n", addr, value);
		fflush(fpReg);
	}
	#endif
	*(volatile unsigned char*)(addr) = (value);
 114:	e5c01000 	strb	r1, [r0]
	return 1;
}
 118:	e3a00001 	mov	r0, #1	; 0x1
 11c:	e1a0f00e 	mov	pc, lr

00000120 <HWReadByte>:
int HWReadByte  (unsigned int addr, unsigned char  *value)
{
	(*value) = *(volatile unsigned char*)(addr);
 120:	e5d03000 	ldrb	r3, [r0]
 124:	e5c13000 	strb	r3, [r1]
	return 1;
}
 128:	e3a00001 	mov	r0, #1	; 0x1
 12c:	e1a0f00e 	mov	pc, lr

00000130 <RegRead>:
//Register and System Memory Interface
unsigned int RegRead (unsigned int RegAddr)
{
 130:	e52de004 	str	lr, [sp, #-4]!
 134:	e24dd004 	sub	sp, sp, #4	; 0x4
	unsigned int ReadData = 0;	
 138:	e28d1004 	add	r1, sp, #4	; 0x4
 13c:	e3a03000 	mov	r3, #0	; 0x0
 140:	e5213004 	str	r3, [r1, #-4]!
	int rt = 0;
	rt = HWReadInt(RegAddr,&ReadData);
 144:	ebffffe1 	bl	d0 <HWReadInt>
	return ReadData; 	
}
 148:	e59d0000 	ldr	r0, [sp]
 14c:	e28dd004 	add	sp, sp, #4	; 0x4
 150:	e8bd8000 	ldmia	sp!, {pc}

00000154 <RegWrite>:
void RegWrite (unsigned int RegAddr, unsigned int RegValue)
{
 154:	e52de004 	str	lr, [sp, #-4]!
	int rt = 0;
	rt = HWWriteInt(RegAddr,RegValue);
 158:	ebffffe0 	bl	e0 <HWWriteInt>
}
 15c:	e49df004 	ldr	pc, [sp], #4

00000160 <RegWriteCheck>:

int RegWriteCheck(unsigned int RegAddr, unsigned int RegValue)
{
 160:	e92d4070 	stmdb	sp!, {r4, r5, r6, lr}
 164:	e24dd004 	sub	sp, sp, #4	; 0x4
 168:	e1a04000 	mov	r4, r0
 16c:	e1a05001 	mov	r5, r1
	int rt = 0;
 170:	e3a02000 	mov	r2, #0	; 0x0
	unsigned int ReadData = 0;
 174:	e1a06002 	mov	r6, r2
 178:	e58d2000 	str	r2, [sp]
	rt = HWWriteInt(RegAddr,RegValue);
 17c:	ebffffd7 	bl	e0 <HWWriteInt>
	rt = HWReadInt(RegAddr,&ReadData);
 180:	e1a00004 	mov	r0, r4
 184:	e1a0100d 	mov	r1, sp
 188:	ebffffd0 	bl	d0 <HWReadInt>
	if ((ReadData) != (RegValue))
 18c:	e59d3000 	ldr	r3, [sp]
 190:	e1530005 	cmp	r3, r5
	{	
		assert(0);
		//printf("##ERROR, RegWriteCheck: RegAddr(%08x), WriteValue(%08x), ReadValue(%08x)\n",
		//	       RegAddr, RegValue, ReadData);
		return 0;
	}
	return rt;
 194:	01a00000 	moveq	r0, r0
 198:	0a000005 	beq	1b4 <RegWriteCheck+0x54>
 19c:	e1a00006 	mov	r0, r6
 1a0:	e59f1014 	ldr	r1, [pc, #20]	; 1bc <.text+0x1bc>
 1a4:	e1a02006 	mov	r2, r6
 1a8:	e3a0306c 	mov	r3, #108	; 0x6c
 1ac:	eb0002fd 	bl	da8 <x_assert>
 1b0:	e1a00006 	mov	r0, r6
}
 1b4:	e28dd004 	add	sp, sp, #4	; 0x4
 1b8:	e8bd8070 	ldmia	sp!, {r4, r5, r6, pc}
 1bc:	00000ddc 	ldreqd	r0, [r0], -ip

000001c0 <HWDelay>:

void HWDelay(int cnt)
{
	int i = 0;
 1c0:	e3a03000 	mov	r3, #0	; 0x0
	int sum = 0;
	for (i = 0;i < cnt; i++ )
 1c4:	e1530000 	cmp	r3, r0
 1c8:	a1a0f00e 	movge	pc, lr
 1cc:	e2833001 	add	r3, r3, #1	; 0x1
 1d0:	e1530000 	cmp	r3, r0
 1d4:	bafffffc 	blt	1cc <HWDelay+0xc>
 1d8:	e1a0f00e 	mov	pc, lr

000001dc <SetTVEncoderClk>:
#endif


int SetTVEncoderClk(int IsHD, int IsProgressive)
{
 1dc:	e92d4030 	stmdb	sp!, {r4, r5, lr}
 1e0:	e24dd004 	sub	sp, sp, #4	; 0x4
 1e4:	e1a04000 	mov	r4, r0
 1e8:	e1a05001 	mov	r5, r1
	U16 FreqCtlValue = 0;
 1ec:	e28d1004 	add	r1, sp, #4	; 0x4
 1f0:	e3a03000 	mov	r3, #0	; 0x0
 1f4:	e16130b2 	strh	r3, [r1, #-2]!
	U16 TVEPLLValue  = 0;
	HWReadShort(REG_CLK_FREQ_CTRL, &FreqCtlValue);
 1f8:	e59f00d4 	ldr	r0, [pc, #212]	; 2d4 <.text+0x2d4>
 1fc:	ebffffbf 	bl	100 <HWReadShort>
	FreqCtlValue  &= (~(1 << 3)); //Disable the TVE Clock
 200:	e1dd30b2 	ldrh	r3, [sp, #2]
 204:	e3c33008 	bic	r3, r3, #8	; 0x8
 208:	e1cd30b2 	strh	r3, [sp, #2]
	HWWriteShort(REG_CLK_FREQ_CTRL, FreqCtlValue);
 20c:	e59f00c0 	ldr	r0, [pc, #192]	; 2d4 <.text+0x2d4>
 210:	e1dd10b2 	ldrh	r1, [sp, #2]
 214:	ebffffb4 	bl	ec <HWWriteShort>
	//Delay for A while
	HWDelay(1024);
 218:	e3a00b01 	mov	r0, #1024	; 0x400
 21c:	ebffffe7 	bl	1c0 <HWDelay>
	//Set TVE clk pll value
	if (IsHD)
	{
		TVEPLLValue = 0x4621;
	}
	else
	{
		TVEPLLValue = 0x8618;
	}
	HWWriteShort(IDS_PCTL_LO, TVEPLLValue);
 220:	e59f00b0 	ldr	r0, [pc, #176]	; 2d8 <.text+0x2d8>
 224:	e59f30b0 	ldr	r3, [pc, #176]	; 2dc <.text+0x2dc>
 228:	e59f10b0 	ldr	r1, [pc, #176]	; 2e0 <.text+0x2e0>
 22c:	e3540000 	cmp	r4, #0	; 0x0
 230:	01a01003 	moveq	r1, r3
 234:	ebffffac 	bl	ec <HWWriteShort>
	//Delay for A while
	HWDelay(1024);
 238:	e3a00b01 	mov	r0, #1024	; 0x400
 23c:	ebffffdf 	bl	1c0 <HWDelay>
	//Enable the TVE Clk
	FreqCtlValue |= (1 << 3);
 240:	e1dd30b2 	ldrh	r3, [sp, #2]
 244:	e3833008 	orr	r3, r3, #8	; 0x8
 248:	e1cd30b2 	strh	r3, [sp, #2]
	//Set TVE_CLK6 switch:
	FreqCtlValue &= (~( 0x3 << 4));
 24c:	e1dd30b2 	ldrh	r3, [sp, #2]
 250:	e3c33030 	bic	r3, r3, #48	; 0x30
 254:	e1cd30b2 	strh	r3, [sp, #2]
	if (!IsHD && IsProgressive)
 258:	e2743001 	rsbs	r3, r4, #1	; 0x1
 25c:	33a03000 	movcc	r3, #0	; 0x0
 260:	e3550000 	cmp	r5, #0	; 0x0
 264:	03a03000 	moveq	r3, #0	; 0x0
 268:	e3530000 	cmp	r3, #0	; 0x0
	{
		FreqCtlValue |= (0x2 << 4);
 26c:	11dd30b2 	ldrneh	r3, [sp, #2]
 270:	13833020 	orrne	r3, r3, #32	; 0x20
 274:	11cd30b2 	strneh	r3, [sp, #2]
	}
	else
	{
		FreqCtlValue |= (0x1 << 4);
 278:	01dd30b2 	ldreqh	r3, [sp, #2]
 27c:	03833010 	orreq	r3, r3, #16	; 0x10
 280:	01cd30b2 	streqh	r3, [sp, #2]
	}
	HWWriteShort(REG_CLK_FREQ_CTRL, FreqCtlValue);
 284:	e59f0048 	ldr	r0, [pc, #72]	; 2d4 <.text+0x2d4>
 288:	e1dd10b2 	ldrh	r1, [sp, #2]
 28c:	ebffff96 	bl	ec <HWWriteShort>
	//Read Back to Check
	{
		U16 ReadValue = 0;
 290:	e3a04000 	mov	r4, #0	; 0x0
 294:	e1cd40b0 	strh	r4, [sp]
		HWReadShort( REG_CLK_FREQ_CTRL,&ReadValue);
 298:	e59f0034 	ldr	r0, [pc, #52]	; 2d4 <.text+0x2d4>
 29c:	e1a0100d 	mov	r1, sp
 2a0:	ebffff96 	bl	100 <HWReadShort>
		assert(FreqCtlValue == ReadValue);
 2a4:	e1dd30b2 	ldrh	r3, [sp, #2]
 2a8:	e1dd00b0 	ldrh	r0, [sp]
 2ac:	e1530000 	cmp	r3, r0
 2b0:	13a00000 	movne	r0, #0	; 0x0
 2b4:	03a00001 	moveq	r0, #1	; 0x1
 2b8:	e59f1024 	ldr	r1, [pc, #36]	; 2e4 <.text+0x2e4>
 2bc:	e1a02004 	mov	r2, r4
 2c0:	e3a0307c 	mov	r3, #124	; 0x7c
 2c4:	eb0002b7 	bl	da8 <x_assert>
	}

	//For Test
	//Enable CCIR 656 Clk

	return 1;
}
 2c8:	e3a00001 	mov	r0, #1	; 0x1
 2cc:	e28dd004 	add	sp, sp, #4	; 0x4
 2d0:	e8bd8030 	ldmia	sp!, {r4, r5, pc}
 2d4:	10171108 	andnes	r1, r7, r8, lsl #2
 2d8:	1017140c 	andnes	r1, r7, ip, lsl #8
 2dc:	00008618 	andeq	r8, r0, r8, lsl r6
 2e0:	00004621 	andeq	r4, r0, r1, lsr #12
 2e4:	00000de0 	andeq	r0, r0, r0, ror #27

000002e8 <SetVideoDACMux>:

int SetVideoDACMux(int IsYPbPr)
{
 2e8:	e92d4030 	stmdb	sp!, {r4, r5, lr}
 2ec:	e24dd004 	sub	sp, sp, #4	; 0x4
 2f0:	e1a05000 	mov	r5, r0
	U16 RegValue = 0;
 2f4:	e28d1004 	add	r1, sp, #4	; 0x4
 2f8:	e3a03000 	mov	r3, #0	; 0x0
 2fc:	e16130b2 	strh	r3, [r1, #-2]!
	HWReadShort(REG_CLK_OPROC_CTRL, &RegValue);
 300:	e59f0094 	ldr	r0, [pc, #148]	; 39c <.text+0x39c>
 304:	ebffff7d 	bl	100 <HWReadShort>
	if (IsYPbPr)         //Bit0	Dac0	Dac1	Dac2
 308:	e3550000 	cmp	r5, #0	; 0x0
	{
		RegValue |= 0x1; //1	Y	    Pb	    Pr
 30c:	11dd30b2 	ldrneh	r3, [sp, #2]
 310:	13833001 	orrne	r3, r3, #1	; 0x1
 314:	11cd30b2 	strneh	r3, [sp, #2]
	}
	else
	{
		RegValue |= 0x0; //0	CVBS	S-Y     S-C
	}
	HWWriteShort(REG_CLK_OPROC_CTRL, RegValue);
 318:	e28d4004 	add	r4, sp, #4	; 0x4
 31c:	e59f0078 	ldr	r0, [pc, #120]	; 39c <.text+0x39c>
 320:	e17410b2 	ldrh	r1, [r4, #-2]!
 324:	ebffff70 	bl	ec <HWWriteShort>
	//Check Register Write
	HWReadShort(REG_CLK_OPROC_CTRL, &RegValue);
 328:	e59f006c 	ldr	r0, [pc, #108]	; 39c <.text+0x39c>
 32c:	e1a01004 	mov	r1, r4
 330:	ebffff72 	bl	100 <HWReadShort>
	if (IsYPbPr)         //Bit0	Dac0	Dac1	Dac2
 334:	e3550000 	cmp	r5, #0	; 0x0
 338:	0a000009 	beq	364 <SetVideoDACMux+0x7c>
	{
		if ((RegValue & 0x1) != 1) //1	Y	    Pb	    Pr
 33c:	e1dd00b2 	ldrh	r0, [sp, #2]
 340:	e3100001 	tst	r0, #1	; 0x1
 344:	1a000011 	bne	390 <SetVideoDACMux+0xa8>
		{
			assert(((RegValue & 0x1) == 1));
 348:	e2000001 	and	r0, r0, #1	; 0x1
 34c:	e59f104c 	ldr	r1, [pc, #76]	; 3a0 <.text+0x3a0>
 350:	e3a02000 	mov	r2, #0	; 0x0
 354:	e3a03098 	mov	r3, #152	; 0x98
 358:	eb000292 	bl	da8 <x_assert>
			return 0;
 35c:	e3a00000 	mov	r0, #0	; 0x0
 360:	ea00000b 	b	394 <SetVideoDACMux+0xac>
		}		
	}
	else
	{
		if ((RegValue & 0x1) != 0) //1	Y	    Pb	    Pr
 364:	e1dd00b2 	ldrh	r0, [sp, #2]
 368:	e3100001 	tst	r0, #1	; 0x1
 36c:	0a000007 	beq	390 <SetVideoDACMux+0xa8>
		{
			assert(((RegValue & 0x1) == 0));
 370:	e2200001 	eor	r0, r0, #1	; 0x1
 374:	e2000001 	and	r0, r0, #1	; 0x1
 378:	e59f1024 	ldr	r1, [pc, #36]	; 3a4 <.text+0x3a4>
 37c:	e3a02000 	mov	r2, #0	; 0x0
 380:	e3a030a0 	mov	r3, #160	; 0xa0
 384:	eb000287 	bl	da8 <x_assert>
			return 0;
 388:	e3a00000 	mov	r0, #0	; 0x0
 38c:	ea000000 	b	394 <SetVideoDACMux+0xac>
		}
	}
	return 1;	
 390:	e3a00001 	mov	r0, #1	; 0x1
}
 394:	e28dd004 	add	sp, sp, #4	; 0x4
 398:	e8bd8030 	ldmia	sp!, {r4, r5, pc}
 39c:	10171400 	andnes	r1, r7, r0, lsl #8
 3a0:	00000dfc 	streqd	r0, [r0], -ip
 3a4:	00000e14 	andeq	r0, r0, r4, lsl lr

000003a8 <InitDDR>:
          
int InitDDR()
{

	HW_REG_WRITE(0x10171100,DDR_PLL); 
 3a8:	e59f2040 	ldr	r2, [pc, #64]	; 3f0 <.text+0x3f0>
 3ac:	e59f3040 	ldr	r3, [pc, #64]	; 3f4 <.text+0x3f4>
 3b0:	e5832000 	str	r2, [r3]
	HW_REG_WRITE(0x10110000,DDR_TIMING);
 3b4:	e59f203c 	ldr	r2, [pc, #60]	; 3f8 <.text+0x3f8>
 3b8:	e59f303c 	ldr	r3, [pc, #60]	; 3fc <.text+0x3fc>
 3bc:	e5832000 	str	r2, [r3]
	HW_REG_WRITE(0x10110004,DDR_MOD);
 3c0:	e59f2038 	ldr	r2, [pc, #56]	; 400 <.text+0x400>
 3c4:	e2833004 	add	r3, r3, #4	; 0x4
 3c8:	e5832000 	str	r2, [r3]
	HW_REG_WRITE(0x10110008,DDR_DELAY_0);
 3cc:	e59f2030 	ldr	r2, [pc, #48]	; 404 <.text+0x404>
 3d0:	e2833004 	add	r3, r3, #4	; 0x4
 3d4:	e4832004 	str	r2, [r3], #4
	HW_REG_WRITE(0x1011000c,DDR_DELAY_1);
 3d8:	e5832000 	str	r2, [r3]
	HW_REG_WRITE(0x10110010,DDR_MISC);
 3dc:	e59f2024 	ldr	r2, [pc, #36]	; 408 <.text+0x408>
 3e0:	e2833004 	add	r3, r3, #4	; 0x4
 3e4:	e5832000 	str	r2, [r3]

    //configuration DDR queue prots to enhance DDR performance to support deinterlace
//	HW_REG_WRITE(0x1011004c,0x20);
//	HW_REG_WRITE(0x10110050,0x1);
//	HW_REG_WRITE(0x10110054,0x0);


	return 1;
}
 3e8:	e3a00001 	mov	r0, #1	; 0x1
 3ec:	e1a0f00e 	mov	pc, lr
 3f0:	0000084f 	andeq	r0, r0, pc, asr #16
 3f4:	10171100 	andnes	r1, r7, r0, lsl #2
 3f8:	77369890 	undefined
 3fc:	10110000 	andnes	r0, r1, r0
 400:	93410225 	cmpls	r1, #1342177282	; 0x50000002
 404:	08080808 	stmeqda	r8, {r3, fp}
 408:	30100465 	andccs	r0, r0, r5, ror #8

0000040c <TestDDR>:
int TestDDR()
{
 40c:	e92d4010 	stmdb	sp!, {r4, lr}
 410:	e24dd018 	sub	sp, sp, #24	; 0x18
	unsigned int TestData[] = 
 414:	e59f308c 	ldr	r3, [pc, #140]	; 4a8 <.text+0x4a8>
 418:	e1a0c00d 	mov	ip, sp
 41c:	e1a0e003 	mov	lr, r3
 420:	e8be000f 	ldmia	lr!, {r0, r1, r2, r3}
 424:	e8ac000f 	stmia	ip!, {r0, r1, r2, r3}
 428:	e89e0003 	ldmia	lr, {r0, r1}
 42c:	e88c0003 	stmia	ip, {r0, r1}
	{
		0x12345678,
		0x9ABCDEF0,
		0xAAAAAAAA,
		0x55555555,
		0x0,
		0xFFFFFFFF,
	};
	unsigned int StartAddr = 0x02000000;
 430:	e3a00402 	mov	r0, #33554432	; 0x2000000
	int i = 0;
	int rt = 0;
	//Write
	for (i = 0; i < sizeof(TestData)/sizeof(unsigned int); i++)
 434:	e3a01000 	mov	r1, #0	; 0x0
	{
		HW_REG_WRITE(StartAddr + i*4, TestData[i]);
 438:	e28d2018 	add	r2, sp, #24	; 0x18
 43c:	e0823101 	add	r3, r2, r1, lsl #2
 440:	e5133018 	ldr	r3, [r3, #-24]
 444:	e7803101 	str	r3, [r0, r1, lsl #2]
 448:	e2811001 	add	r1, r1, #1	; 0x1
 44c:	e3510005 	cmp	r1, #5	; 0x5
 450:	9afffff8 	bls	438 <TestDDR+0x2c>
	}
	//Check
	rt = 0;
 454:	e3a04000 	mov	r4, #0	; 0x0
	for (i = 0; i < sizeof(TestData)/sizeof(unsigned int); i++)
 458:	e1a01004 	mov	r1, r4
	{
		unsigned int ReadData = 0;
		ReadData = ~TestData[i];
 45c:	e28d2018 	add	r2, sp, #24	; 0x18
 460:	e0823101 	add	r3, r2, r1, lsl #2
 464:	e5132018 	ldr	r2, [r3, #-24]
		ReadData = HW_REG_READ(StartAddr + i*4);
 468:	e7903101 	ldr	r3, [r0, r1, lsl #2]
		if(ReadData != TestData[i])
 46c:	e1520003 	cmp	r2, r3
			rt++;
 470:	12844001 	addne	r4, r4, #1	; 0x1
 474:	e2811001 	add	r1, r1, #1	; 0x1
 478:	e3510005 	cmp	r1, #5	; 0x5
 47c:	9afffff6 	bls	45c <TestDDR+0x50>
	}
	assert(rt == 0);
 480:	e2740001 	rsbs	r0, r4, #1	; 0x1
 484:	33a00000 	movcc	r0, #0	; 0x0
 488:	e59f101c 	ldr	r1, [pc, #28]	; 4ac <.text+0x4ac>
 48c:	e3a02000 	mov	r2, #0	; 0x0
 490:	e3a030b5 	mov	r3, #181	; 0xb5
 494:	eb000243 	bl	da8 <x_assert>
	return !rt;
}
 498:	e2740001 	rsbs	r0, r4, #1	; 0x1
 49c:	33a00000 	movcc	r0, #0	; 0x0
 4a0:	e28dd018 	add	sp, sp, #24	; 0x18
 4a4:	e8bd8010 	ldmia	sp!, {r4, pc}
 4a8:	00000dc4 	andeq	r0, r0, r4, asr #27
 4ac:	00000e2c 	andeq	r0, r0, ip, lsr #28

000004b0 <SysImgCopyToDDR>:
#include "secu_hw_if.h"
#include "../stdlib/c_stdlib.h"

int SysImgCopyToDDR(void)
{
 4b0:	e92d40f0 	stmdb	sp!, {r4, r5, r6, r7, lr}
	int i;
	int ret = 0;
	unsigned char *DDRBuf = NULL;

	SecHWImgOpen();
 4b4:	eb000036 	bl	594 <SecHWImgOpen>
	DDRBuf = (unsigned char *)APP_IMG_COPY_DDR_ADDR;
 4b8:	e3a07402 	mov	r7, #33554432	; 0x2000000

	for(i = 0; i < APP_IMG_COPY_SIZE/1024; i++) { 
 4bc:	e3a04000 	mov	r4, #0	; 0x0
 4c0:	e59f6050 	ldr	r6, [pc, #80]	; 518 <.text+0x518>
		ret = SecHWImgRead(APP_IMG_START + (i << 10), 1024 , DDRBuf + (i << 10));
 4c4:	e1a02504 	mov	r2, r4, lsl #10
 4c8:	e2820b05 	add	r0, r2, #5120	; 0x1400
 4cc:	e3a01b01 	mov	r1, #1024	; 0x400
 4d0:	e0822007 	add	r2, r2, r7
 4d4:	eb000046 	bl	5f4 <SecHWImgRead>
 4d8:	e1a05000 	mov	r5, r0
		assert(ret == 1024);
 4dc:	e3500b01 	cmp	r0, #1024	; 0x400
 4e0:	13a00000 	movne	r0, #0	; 0x0
 4e4:	03a00001 	moveq	r0, #1	; 0x1
 4e8:	e59f102c 	ldr	r1, [pc, #44]	; 51c <.text+0x51c>
 4ec:	e3a02000 	mov	r2, #0	; 0x0
 4f0:	e3a03016 	mov	r3, #22	; 0x16
 4f4:	eb00022b 	bl	da8 <x_assert>
 4f8:	e2844001 	add	r4, r4, #1	; 0x1
 4fc:	e1540006 	cmp	r4, r6
 500:	daffffef 	ble	4c4 <SysImgCopyToDDR+0x14>
	}

	SecHWImgClose();
 504:	eb00002e 	bl	5c4 <SecHWImgClose>

	return ret > 0;
}
 508:	e3550000 	cmp	r5, #0	; 0x0
 50c:	d3a00000 	movle	r0, #0	; 0x0
 510:	c3a00001 	movgt	r0, #1	; 0x1
 514:	e8bd80f0 	ldmia	sp!, {r4, r5, r6, r7, pc}
 518:	0000012b 	andeq	r0, r0, fp, lsr #2
 51c:	00000e34 	andeq	r0, r0, r4, lsr lr

00000520 <SecHWGetBootInfo>:

/* Return SMC_STA Value */
unsigned int SecHWGetBootInfo(void) 
{
	return REG_READ(SMC_BOOT);
 520:	e59f3004 	ldr	r3, [pc, #4]	; 52c <.text+0x52c>
 524:	e5930000 	ldr	r0, [r3]
}
 528:	e1a0f00e 	mov	pc, lr
 52c:	10100034 	andnes	r0, r0, r4, lsr r0

00000530 <SecHWGetBootMode>:

unsigned int SecHWGetBootMode(void)
{
	unsigned int BootInfo = 0;
	unsigned int BootMode = 0;
	BootInfo = REG_READ(SMC_BOOT);
 530:	e59f301c 	ldr	r3, [pc, #28]	; 554 <.text+0x554>
 534:	e5933000 	ldr	r3, [r3]
	if (BootInfo & 0x1) {
 538:	e3130001 	tst	r3, #1	; 0x1
		if (BootInfo & 0x10) {
			BootMode = SPI_BOOT;
#ifdef SPI_NAND_BOOT
			BootMode = NAND_BOOT;
#endif
		} else {
			BootMode = NAND_BOOT;
		}
	} else {
		BootMode = NOR_BOOT;
 53c:	03a00000 	moveq	r0, #0	; 0x0
 540:	01a0f00e 	moveq	pc, lr
 544:	e3130010 	tst	r3, #16	; 0x10
 548:	03a00001 	moveq	r0, #1	; 0x1
 54c:	13a00002 	movne	r0, #2	; 0x2
 550:	e1a0f00e 	mov	pc, lr
 554:	10100034 	andnes	r0, r0, r4, lsr r0

00000558 <SecHWIsRSACheckEna>:
	}

#ifdef MANUAL_BOOT_MODE
	BootMode = MANUAL_BOOT_MODE;
#endif

	return BootMode;	
}

/*Read Secure Config */
int SecHWIsRSACheckEna(void) 
{
	unsigned int OTPInfo = 0;
	OTPInfo = REG_READ(OTP_CFG);
 558:	e59f3010 	ldr	r3, [pc, #16]	; 570 <.text+0x570>
 55c:	e5930000 	ldr	r0, [r3]
	return (((OTPInfo >> OTP_SEC_BOOT_BIT) & 0x1) == RSA_CHECK_ENA);
 560:	e1a001a0 	mov	r0, r0, lsr #3
 564:	e2200001 	eor	r0, r0, #1	; 0x1
}
 568:	e2000001 	and	r0, r0, #1	; 0x1
 56c:	e1a0f00e 	mov	pc, lr
 570:	fffff828 	swinv	0x00fff828

00000574 <SecHWGetKeyAddr>:

unsigned char *SecHWGetKeyAddr(void)
{
	unsigned int OTPInfo = 0;
	OTPInfo = REG_READ(OTP_CFG);
 574:	e59f3014 	ldr	r3, [pc, #20]	; 590 <.text+0x590>
 578:	e5930000 	ldr	r0, [r3]
	OTPInfo = (OTPInfo >> OTP_KEY_INDEX_BIT) & 0x7;
 57c:	e1a00820 	mov	r0, r0, lsr #16
 580:	e2000007 	and	r0, r0, #7	; 0x7

	return (unsigned char *)(PUB_KEY_START + 256 * OTPInfo);
 584:	e1a00400 	mov	r0, r0, lsl #8
} 
 588:	e2400b12 	sub	r0, r0, #18432	; 0x4800
 58c:	e1a0f00e 	mov	pc, lr
 590:	fffff828 	swinv	0x00fff828

00000594 <SecHWImgOpen>:

int SecHWImgOpen(void)
{
 594:	e52de004 	str	lr, [sp, #-4]!
	int ret = 0;
	unsigned int BootMode = 0;

	BootMode = SecHWGetBootMode();
 598:	ebffffe4 	bl	530 <SecHWGetBootMode>

	if (BootMode == SPI_BOOT) {
 59c:	e3500002 	cmp	r0, #2	; 0x2
 5a0:	1a000001 	bne	5ac <SecHWImgOpen+0x18>
		ret = SPIOpen();
 5a4:	eb000150 	bl	aec <SPIOpen>
 5a8:	e49df004 	ldr	pc, [sp], #4
	} else if (BootMode == NAND_BOOT) {
 5ac:	e3500001 	cmp	r0, #1	; 0x1
 5b0:	1a000001 	bne	5bc <SecHWImgOpen+0x28>
		ret = NANDOpen();
 5b4:	eb0000a0 	bl	83c <NANDOpen>
 5b8:	e49df004 	ldr	pc, [sp], #4
	} else {
		ret = NOROpen();
 5bc:	eb000059 	bl	728 <NOROpen>
	}

	return ret;
}
 5c0:	e49df004 	ldr	pc, [sp], #4

000005c4 <SecHWImgClose>:
int SecHWImgClose(void)
{
 5c4:	e52de004 	str	lr, [sp, #-4]!
	int ret = 0;
	unsigned int BootMode = 0;

	BootMode = SecHWGetBootMode();
 5c8:	ebffffd8 	bl	530 <SecHWGetBootMode>
	if (BootMode == SPI_BOOT) {
 5cc:	e3500002 	cmp	r0, #2	; 0x2
 5d0:	1a000001 	bne	5dc <SecHWImgClose+0x18>
		ret = SPIClose();
 5d4:	eb00014c 	bl	b0c <SPIClose>
 5d8:	e49df004 	ldr	pc, [sp], #4
	} else if (BootMode == NAND_BOOT) {
 5dc:	e3500001 	cmp	r0, #1	; 0x1
 5e0:	1a000001 	bne	5ec <SecHWImgClose+0x28>
		ret = NANDClose();
 5e4:	eb00013e 	bl	ae4 <NANDClose>
 5e8:	e49df004 	ldr	pc, [sp], #4
	}
	else {
		ret = NORClose();
 5ec:	eb000086 	bl	80c <NORClose>
	}

	return ret;
}
 5f0:	e49df004 	ldr	pc, [sp], #4

000005f4 <SecHWImgRead>:

int SecHWImgRead(unsigned int StartAddr, int Size, unsigned char *Buf)
{
 5f4:	e92d40f0 	stmdb	sp!, {r4, r5, r6, r7, lr}
 5f8:	e24ddb01 	sub	sp, sp, #1024	; 0x400
 5fc:	e1a05000 	mov	r5, r0
 600:	e1a07002 	mov	r7, r2
	int ret = 0;
	unsigned int BootMode = 0;
	unsigned int  direct_copy;
	unsigned char buffer[1024];

	if(Size <= 0)
 604:	e2516000 	subs	r6, r1, #0	; 0x0
		return Size;
 608:	d1a00006 	movle	r0, r6
 60c:	da000042 	ble	71c <SecHWImgRead+0x128>

	/* cross 1K boundary not allowed! */
	assert((StartAddr & (~0x3ff)) == ((StartAddr + Size - 1) & ~0x3ff));    
 610:	e3c53fff 	bic	r3, r5, #1020	; 0x3fc
 614:	e3c33003 	bic	r3, r3, #3	; 0x3
 618:	e0850006 	add	r0, r5, r6
 61c:	e2400001 	sub	r0, r0, #1	; 0x1
 620:	e3c00fff 	bic	r0, r0, #1020	; 0x3fc
 624:	e3c00003 	bic	r0, r0, #3	; 0x3
 628:	e1530000 	cmp	r3, r0
 62c:	13a00000 	movne	r0, #0	; 0x0
 630:	03a00001 	moveq	r0, #1	; 0x1
 634:	e59f10e8 	ldr	r1, [pc, #232]	; 724 <.text+0x724>
 638:	e3a02000 	mov	r2, #0	; 0x0
 63c:	e3a0307b 	mov	r3, #123	; 0x7b
 640:	eb0001d8 	bl	da8 <x_assert>

	direct_copy = ((StartAddr & 0x3ff) == 0) && (Size == 1024);
 644:	e1a04b05 	mov	r4, r5, lsl #22
 648:	e1a04b24 	mov	r4, r4, lsr #22
 64c:	e3540000 	cmp	r4, #0	; 0x0
 650:	03560b01 	cmpeq	r6, #1024	; 0x400
 654:	13a04000 	movne	r4, #0	; 0x0
 658:	03a04001 	moveq	r4, #1	; 0x1

	BootMode = SecHWGetBootMode();
 65c:	ebffffb3 	bl	530 <SecHWGetBootMode>

	if (BootMode == SPI_BOOT) {
 660:	e3500002 	cmp	r0, #2	; 0x2
 664:	1a00000a 	bne	694 <SecHWImgRead+0xa0>
		if(direct_copy)
 668:	e3540000 	cmp	r4, #0	; 0x0
 66c:	0a000003 	beq	680 <SecHWImgRead+0x8c>
			ret = SPIRead_1K(StartAddr, Buf);
 670:	e1a00005 	mov	r0, r5
 674:	e1a01007 	mov	r1, r7
 678:	eb00011f 	bl	afc <SPIRead_1K>
 67c:	ea00001d 	b	6f8 <SecHWImgRead+0x104>
		else
			ret = SPIRead_1K(StartAddr & (~0x3ff), &buffer[0]);
 680:	e3c50fff 	bic	r0, r5, #1020	; 0x3fc
 684:	e3c00003 	bic	r0, r0, #3	; 0x3
 688:	e1a0100d 	mov	r1, sp
 68c:	eb00011a 	bl	afc <SPIRead_1K>
 690:	ea000018 	b	6f8 <SecHWImgRead+0x104>
	} else if (BootMode == NAND_BOOT) {
 694:	e3500001 	cmp	r0, #1	; 0x1
 698:	1a00000a 	bne	6c8 <SecHWImgRead+0xd4>
		if(direct_copy)
 69c:	e3540000 	cmp	r4, #0	; 0x0
 6a0:	0a000003 	beq	6b4 <SecHWImgRead+0xc0>
			ret = NANDRead_1K(StartAddr, Buf);
 6a4:	e1a00005 	mov	r0, r5
 6a8:	e1a01007 	mov	r1, r7
 6ac:	eb000075 	bl	888 <NANDRead_1K>
 6b0:	ea000010 	b	6f8 <SecHWImgRead+0x104>
		else
			ret = NANDRead_1K(StartAddr & (~0x3ff), &buffer[0]);
 6b4:	e3c50fff 	bic	r0, r5, #1020	; 0x3fc
 6b8:	e3c00003 	bic	r0, r0, #3	; 0x3
 6bc:	e1a0100d 	mov	r1, sp
 6c0:	eb000070 	bl	888 <NANDRead_1K>
 6c4:	ea00000b 	b	6f8 <SecHWImgRead+0x104>
	} else {
		if(direct_copy)
 6c8:	e3540000 	cmp	r4, #0	; 0x0
 6cc:	0a000004 	beq	6e4 <SecHWImgRead+0xf0>
			ret = NORRead(StartAddr, 1024, Buf);
 6d0:	e1a00005 	mov	r0, r5
 6d4:	e3a01b01 	mov	r1, #1024	; 0x400
 6d8:	e1a02007 	mov	r2, r7
 6dc:	eb000013 	bl	730 <NORRead>
 6e0:	ea000004 	b	6f8 <SecHWImgRead+0x104>
		else
			ret = NORRead(StartAddr & (~0x3ff), 1024, &buffer[0]);
 6e4:	e3c50fff 	bic	r0, r5, #1020	; 0x3fc
 6e8:	e3c00003 	bic	r0, r0, #3	; 0x3
 6ec:	e3a01b01 	mov	r1, #1024	; 0x400
 6f0:	e1a0200d 	mov	r2, sp
 6f4:	eb00000d 	bl	730 <NORRead>
	}

	if(!direct_copy)
 6f8:	e3540000 	cmp	r4, #0	; 0x0
 6fc:	1a000005 	bne	718 <SecHWImgRead+0x124>
		memcpy(Buf, &buffer[StartAddr & 0x3ff], Size);
 700:	e1a01b05 	mov	r1, r5, lsl #22
 704:	e1a01b21 	mov	r1, r1, lsr #22
 708:	e1a00007 	mov	r0, r7
 70c:	e08d1001 	add	r1, sp, r1
 710:	e1a02006 	mov	r2, r6
 714:	eb000167 	bl	cb8 <x_memcpy>

	return Size;
 718:	e1a00006 	mov	r0, r6
}
 71c:	e28ddb01 	add	sp, sp, #1024	; 0x400
 720:	e8bd80f0 	ldmia	sp!, {r4, r5, r6, r7, pc}
 724:	00000e40 	andeq	r0, r0, r0, asr #28

00000728 <NOROpen>:

int NOROpen (void)
{
	return 1;
}
 728:	e3a00001 	mov	r0, #1	; 0x1
 72c:	e1a0f00e 	mov	pc, lr

00000730 <NORRead>:

int NORRead(unsigned int StartAddr, int Size, unsigned char *Buf)
{
 730:	e92d4010 	stmdb	sp!, {r4, lr}
	int i = 0;
 734:	e3a04000 	mov	r4, #0	; 0x0
	unsigned char *Src = 0;
	Src += (NOR_FLASH_BASE + StartAddr);
 738:	e280c30d 	add	ip, r0, #872415232	; 0x34000000
	if (( (StartAddr &0x3) == (((unsigned int)Buf) & 0x3)) 
 73c:	e2000003 	and	r0, r0, #3	; 0x3
 740:	e1a0e000 	mov	lr, r0
 744:	e2023003 	and	r3, r2, #3	; 0x3
 748:	e1500003 	cmp	r0, r3
 74c:	1a00001c 	bne	7c4 <NORRead+0x94>
 750:	e2803004 	add	r3, r0, #4	; 0x4
 754:	e1510003 	cmp	r1, r3
 758:	3a000019 	bcc	7c4 <NORRead+0x94>
	    && (Size >= ((StartAddr &0x3 ) + 4) )) {
		unsigned int *s = 0;
		unsigned int *d = 0;

		for (i = 0; i < (StartAddr &0x3); i++)
 75c:	e1540000 	cmp	r4, r0
 760:	2a000004 	bcs	778 <NORRead+0x48>
			*Buf++ = *Src++;
 764:	e4dc3001 	ldrb	r3, [ip], #1
 768:	e4c23001 	strb	r3, [r2], #1
 76c:	e2844001 	add	r4, r4, #1	; 0x1
 770:	e154000e 	cmp	r4, lr
 774:	3afffffa 	bcc	764 <NORRead+0x34>

		s = (unsigned int*)Src;
 778:	e1a0000c 	mov	r0, ip
		d = (unsigned int*)Buf;
		for (; i < Size; i += 4)
 77c:	e1540001 	cmp	r4, r1
 780:	aa000004 	bge	798 <NORRead+0x68>
			*d++ = *s++;
 784:	e4903004 	ldr	r3, [r0], #4
 788:	e4823004 	str	r3, [r2], #4
 78c:	e2844004 	add	r4, r4, #4	; 0x4
 790:	e1540001 	cmp	r4, r1
 794:	bafffffa 	blt	784 <NORRead+0x54>

		Src = (unsigned char*)(s - 1) + 1;
 798:	e240c003 	sub	ip, r0, #3	; 0x3
		Buf = (unsigned char*)(d - 1) + 1;		
 79c:	e2422003 	sub	r2, r2, #3	; 0x3
		i = (i - 4 + 1);
 7a0:	e2444003 	sub	r4, r4, #3	; 0x3
		for (; i < Size; i++)
 7a4:	e1540001 	cmp	r4, r1
 7a8:	aa00000d 	bge	7e4 <NORRead+0xb4>
			*Buf++ = *Src++;
 7ac:	e4dc3001 	ldrb	r3, [ip], #1
 7b0:	e4c23001 	strb	r3, [r2], #1
 7b4:	e2844001 	add	r4, r4, #1	; 0x1
 7b8:	e1540001 	cmp	r4, r1
 7bc:	bafffffa 	blt	7ac <NORRead+0x7c>
 7c0:	ea000007 	b	7e4 <NORRead+0xb4>
	} else {
		for (i = 0; i < Size; i++)
 7c4:	e3a04000 	mov	r4, #0	; 0x0
 7c8:	e1540001 	cmp	r4, r1
 7cc:	aa000004 	bge	7e4 <NORRead+0xb4>
			*Buf++ = *Src++;
 7d0:	e4dc3001 	ldrb	r3, [ip], #1
 7d4:	e4c23001 	strb	r3, [r2], #1
 7d8:	e2844001 	add	r4, r4, #1	; 0x1
 7dc:	e1540001 	cmp	r4, r1
 7e0:	bafffffa 	blt	7d0 <NORRead+0xa0>
	}

	assert(i == Size);
 7e4:	e1540001 	cmp	r4, r1
 7e8:	13a00000 	movne	r0, #0	; 0x0
 7ec:	03a00001 	moveq	r0, #1	; 0x1
 7f0:	e59f1010 	ldr	r1, [pc, #16]	; 808 <.text+0x808>
 7f4:	e3a02000 	mov	r2, #0	; 0x0
 7f8:	e3a0302f 	mov	r3, #47	; 0x2f
 7fc:	eb000169 	bl	da8 <x_assert>
	return i;
	
}
 800:	e1a00004 	mov	r0, r4
 804:	e8bd8010 	ldmia	sp!, {r4, pc}
 808:	00000e7c 	andeq	r0, r0, ip, ror lr

0000080c <NORClose>:
int NORClose(void)
{
	return 1;
}
 80c:	e3a00001 	mov	r0, #1	; 0x1
 810:	e1a0f00e 	mov	pc, lr

00000814 <NANDWait>:
#ifdef NAND_FUNC_ENA
	int i;
	volatile unsigned int *p;

	p = (volatile unsigned int *)NAND_STA;
 814:	e59f201c 	ldr	r2, [pc, #28]	; 838 <.text+0x838>

	do {
		for(i = 0; i < 20; i++) ;
 818:	e3a03000 	mov	r3, #0	; 0x0
 81c:	e2833001 	add	r3, r3, #1	; 0x1
 820:	e3530013 	cmp	r3, #19	; 0x13
 824:	dafffffc 	ble	81c <NANDWait+0x8>
	} while((*p & 0x1) == 0);    
 828:	e5923000 	ldr	r3, [r2]
 82c:	e3130001 	tst	r3, #1	; 0x1
 830:	0afffff8 	beq	818 <NANDWait+0x4>
#endif
}
 834:	e1a0f00e 	mov	pc, lr
 838:	10100030 	andnes	r0, r0, r0, lsr r0

0000083c <NANDOpen>:

int NANDOpen (void)
{
 83c:	e52de004 	str	lr, [sp, #-4]!
#ifdef NAND_FUNC_ENA
	unsigned int boot_cfg;
	volatile unsigned int    *p;
	volatile unsigned short  *ps;
	volatile unsigned char   *pc;

#if 0
	p = (volatile unsigned int *)BOOT_CFG;
	boot_cfg = *p;

	large_block =  (unsigned char)((boot_cfg >> 1) & 0x1); 
	io_width_16 =  (unsigned char)((boot_cfg >> 2) & 0x1); 
	addr_cyc_2  =  (unsigned char)((boot_cfg >> 3) & 0x1); 

#else
	/* TODO SUNEH: temparary exist for test */
	large_block = 1;
 840:	e3a02001 	mov	r2, #1	; 0x1
 844:	e59f302c 	ldr	r3, [pc, #44]	; 878 <.text+0x878>
 848:	e5c32000 	strb	r2, [r3]
	io_width_16 = 0;
 84c:	e2422001 	sub	r2, r2, #1	; 0x1
 850:	e59f3024 	ldr	r3, [pc, #36]	; 87c <.text+0x87c>
 854:	e5c32000 	strb	r2, [r3]
	addr_cyc_2 = 0;
 858:	e59f3020 	ldr	r3, [pc, #32]	; 880 <.text+0x880>
 85c:	e5c32000 	strb	r2, [r3]
#endif

	if(io_width_16) {
		ps  = (volatile unsigned short *)NAND_CMD;
		*ps = 0xff;
	} else {
		pc  = (volatile unsigned char  *)NAND_CMD;
		*pc = 0xff;
 860:	e2422001 	sub	r2, r2, #1	; 0x1
 864:	e59f3018 	ldr	r3, [pc, #24]	; 884 <.text+0x884>
 868:	e5c32000 	strb	r2, [r3]
	}

	NANDWait ();
 86c:	ebffffe8 	bl	814 <NANDWait>
#endif
	return 1;
}
 870:	e3a00001 	mov	r0, #1	; 0x1
 874:	e49df004 	ldr	pc, [sp], #4
 878:	00000ea8 	andeq	r0, r0, r8, lsr #29
 87c:	00000ea9 	andeq	r0, r0, r9, lsr #29
 880:	00000eaa 	andeq	r0, r0, sl, lsr #29
 884:	34800002 	strcc	r0, [r0], #2

00000888 <NANDRead_1K>:

int NANDRead_1K(unsigned int StartAddr, unsigned char *Buf)  /* Warning: assume 1K alignment for StartAddr */
{
 888:	e92d41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 88c:	e1a05000 	mov	r5, r0
 890:	e1a04001 	mov	r4, r1
	int i;
	int j;
	volatile unsigned short *ps;
	volatile unsigned char  *pc;
	unsigned char  *pBuf = Buf;
#ifdef NAND_FUNC_ENA
	assert((StartAddr & 0x3ff) == 0);
 894:	e1a00b00 	mov	r0, r0, lsl #22
 898:	e1a00b20 	mov	r0, r0, lsr #22
 89c:	e2700001 	rsbs	r0, r0, #1	; 0x1
 8a0:	33a00000 	movcc	r0, #0	; 0x0
 8a4:	e59f121c 	ldr	r1, [pc, #540]	; ac8 <.text+0xac8>
 8a8:	e3a02000 	mov	r2, #0	; 0x0
 8ac:	e3a03058 	mov	r3, #88	; 0x58
 8b0:	eb00013c 	bl	da8 <x_assert>

	if(large_block) {
 8b4:	e59f3210 	ldr	r3, [pc, #528]	; acc <.text+0xacc>
 8b8:	e5d33000 	ldrb	r3, [r3]
 8bc:	e3530000 	cmp	r3, #0	; 0x0
 8c0:	0a000040 	beq	9c8 <NANDRead_1K+0x140>
		if(io_width_16) {
 8c4:	e59f3204 	ldr	r3, [pc, #516]	; ad0 <.text+0xad0>
 8c8:	e5d33000 	ldrb	r3, [r3]
 8cc:	e3530000 	cmp	r3, #0	; 0x0
 8d0:	0a00001f 	beq	954 <NANDRead_1K+0xcc>
			ps  = (volatile unsigned short *)NAND_CMD;
 8d4:	e59fc1f8 	ldr	ip, [pc, #504]	; ad4 <.text+0xad4>
			*ps = 0x00;
 8d8:	e3a03000 	mov	r3, #0	; 0x0
 8dc:	e1cc30b0 	strh	r3, [ip]

			ps  = (volatile unsigned short *)NAND_ADDR;
 8e0:	e28cc002 	add	ip, ip, #2	; 0x2
			*ps = 0x00;
 8e4:	e1cc30b0 	strh	r3, [ip]
			*ps = (StartAddr >>  9) & 0x03;
 8e8:	e1a034a5 	mov	r3, r5, lsr #9
 8ec:	e2033003 	and	r3, r3, #3	; 0x3
 8f0:	e1cc30b0 	strh	r3, [ip]
			*ps = (StartAddr >> 11) & 0xff;
 8f4:	e1a035a5 	mov	r3, r5, lsr #11
 8f8:	e20330ff 	and	r3, r3, #255	; 0xff
 8fc:	e1cc30b0 	strh	r3, [ip]
			*ps = (StartAddr >> 19) & 0xff;
 900:	e1a039a5 	mov	r3, r5, lsr #19
 904:	e20330ff 	and	r3, r3, #255	; 0xff
 908:	e1cc30b0 	strh	r3, [ip]
			if(~addr_cyc_2)
 90c:	e59f31c4 	ldr	r3, [pc, #452]	; ad8 <.text+0xad8>
 910:	e5d33000 	ldrb	r3, [r3]
 914:	e3730001 	cmn	r3, #1	; 0x1
				*ps = (StartAddr >> 27) & 0xff;
 918:	11a03da5 	movne	r3, r5, lsr #27
 91c:	11cc30b0 	strneh	r3, [ip]

			ps  = (volatile unsigned short *)NAND_CMD;
 920:	e59fc1ac 	ldr	ip, [pc, #428]	; ad4 <.text+0xad4>
			*ps = 0x30;
 924:	e3a03030 	mov	r3, #48	; 0x30
 928:	e1cc30b0 	strh	r3, [ip]

			NANDWait ();
 92c:	ebffffb8 	bl	814 <NANDWait>

			ps  = (volatile unsigned short *)NAND_IO;
 930:	e3a0c5d2 	mov	ip, #880803840	; 0x34800000
			for(j = 0; j < 512; j++) {
 934:	e3a02000 	mov	r2, #0	; 0x0
 938:	e59f119c 	ldr	r1, [pc, #412]	; adc <.text+0xadc>
				*((unsigned short *)pBuf) = *ps;
 93c:	e1dc30b0 	ldrh	r3, [ip]
 940:	e0c430b2 	strh	r3, [r4], #2
 944:	e2822001 	add	r2, r2, #1	; 0x1
 948:	e1520001 	cmp	r2, r1
 94c:	dafffffa 	ble	93c <NANDRead_1K+0xb4>
 950:	ea00005a 	b	ac0 <NANDRead_1K+0x238>
				pBuf += 2;
			} 

		} else {
			pc  = (volatile unsigned char  *)NAND_CMD;
 954:	e59fc178 	ldr	ip, [pc, #376]	; ad4 <.text+0xad4>
			*pc = 0x00;
 958:	e3a03000 	mov	r3, #0	; 0x0
 95c:	e4cc3002 	strb	r3, [ip], #2

			pc  = (volatile unsigned char  *)NAND_ADDR;
			*pc = 0x00;
 960:	e5cc3000 	strb	r3, [ip]
			*pc = (StartAddr >>  8) & 0x07;
 964:	e1a03425 	mov	r3, r5, lsr #8
 968:	e2033007 	and	r3, r3, #7	; 0x7
 96c:	e5cc3000 	strb	r3, [ip]
			*pc = (StartAddr >> 11) & 0xff;
 970:	e1a035a5 	mov	r3, r5, lsr #11
 974:	e5cc3000 	strb	r3, [ip]
			*pc = (StartAddr >> 19) & 0xff;
 978:	e1a039a5 	mov	r3, r5, lsr #19
 97c:	e5cc3000 	strb	r3, [ip]
			if(~addr_cyc_2)
 980:	e59f3150 	ldr	r3, [pc, #336]	; ad8 <.text+0xad8>
 984:	e5d33000 	ldrb	r3, [r3]
 988:	e3730001 	cmn	r3, #1	; 0x1
				*pc = (StartAddr >> 27) & 0xff;
 98c:	11a03da5 	movne	r3, r5, lsr #27
 990:	15cc3000 	strneb	r3, [ip]

			pc  = (volatile unsigned char  *)NAND_CMD;
 994:	e59fc138 	ldr	ip, [pc, #312]	; ad4 <.text+0xad4>
			*pc = 0x30;
 998:	e3a03030 	mov	r3, #48	; 0x30
 99c:	e5cc3000 	strb	r3, [ip]

			NANDWait ();
 9a0:	ebffff9b 	bl	814 <NANDWait>

			pc  = (volatile unsigned char  *)NAND_IO;
 9a4:	e3a0c5d2 	mov	ip, #880803840	; 0x34800000
			for(j = 0; j < 1024; j++) {
 9a8:	e3a02000 	mov	r2, #0	; 0x0
 9ac:	e59f112c 	ldr	r1, [pc, #300]	; ae0 <.text+0xae0>
				*pBuf = *pc;
 9b0:	e5dc3000 	ldrb	r3, [ip]
 9b4:	e4c43001 	strb	r3, [r4], #1
 9b8:	e2822001 	add	r2, r2, #1	; 0x1
 9bc:	e1520001 	cmp	r2, r1
 9c0:	dafffffa 	ble	9b0 <NANDRead_1K+0x128>
 9c4:	ea00003d 	b	ac0 <NANDRead_1K+0x238>
				pBuf++;
			}

		}
	} else {
		if(io_width_16) {
 9c8:	e59f3100 	ldr	r3, [pc, #256]	; ad0 <.text+0xad0>
 9cc:	e5d33000 	ldrb	r3, [r3]
 9d0:	e3530000 	cmp	r3, #0	; 0x0
			for(i = 0; i < 4; i++) {
				ps  = (volatile unsigned short *)NAND_CMD;
				*ps = (i & 0x1);

				ps  = (volatile unsigned short *)NAND_ADDR;
				*ps = 0x00;
				*ps = ((StartAddr >>  9) & 0xff) | (i >> 1);
				*ps = (StartAddr >> 17) & 0xff;
				if(~addr_cyc_2)
					*ps = (StartAddr >> 25) & 0xff;

				NANDWait ();

				ps  = (volatile unsigned short *)NAND_IO;
				for(j = 0; j < 128; j++) {
					*((unsigned short *)pBuf) = *ps;
					pBuf += 2;
				}
			}

		} else {
			for(i = 0; i < 4; i++) {
 9d4:	03a06000 	moveq	r6, #0	; 0x0
 9d8:	0a00001e 	beq	a58 <NANDRead_1K+0x1d0>
 9dc:	e3a06000 	mov	r6, #0	; 0x0
 9e0:	e1a034a5 	mov	r3, r5, lsr #9
 9e4:	e20370ff 	and	r7, r3, #255	; 0xff
 9e8:	e1a038a5 	mov	r3, r5, lsr #17
 9ec:	e20380ff 	and	r8, r3, #255	; 0xff
 9f0:	e1a05ca5 	mov	r5, r5, lsr #25
 9f4:	e59fc0d8 	ldr	ip, [pc, #216]	; ad4 <.text+0xad4>
 9f8:	e2063001 	and	r3, r6, #1	; 0x1
 9fc:	e1cc30b0 	strh	r3, [ip]
 a00:	e28cc002 	add	ip, ip, #2	; 0x2
 a04:	e3a03000 	mov	r3, #0	; 0x0
 a08:	e1cc30b0 	strh	r3, [ip]
 a0c:	e18730c6 	orr	r3, r7, r6, asr #1
 a10:	e1cc30b0 	strh	r3, [ip]
 a14:	e1cc80b0 	strh	r8, [ip]
 a18:	e59f30b8 	ldr	r3, [pc, #184]	; ad8 <.text+0xad8>
 a1c:	e5d33000 	ldrb	r3, [r3]
 a20:	e3730001 	cmn	r3, #1	; 0x1
 a24:	11cc50b0 	strneh	r5, [ip]
 a28:	ebffff79 	bl	814 <NANDWait>
 a2c:	e3a0c5d2 	mov	ip, #880803840	; 0x34800000
 a30:	e3a02000 	mov	r2, #0	; 0x0
 a34:	e1dc30b0 	ldrh	r3, [ip]
 a38:	e0c430b2 	strh	r3, [r4], #2
 a3c:	e2822001 	add	r2, r2, #1	; 0x1
 a40:	e352007f 	cmp	r2, #127	; 0x7f
 a44:	dafffffa 	ble	a34 <NANDRead_1K+0x1ac>
 a48:	e2866001 	add	r6, r6, #1	; 0x1
 a4c:	e3560003 	cmp	r6, #3	; 0x3
 a50:	daffffe7 	ble	9f4 <NANDRead_1K+0x16c>
 a54:	ea000019 	b	ac0 <NANDRead_1K+0x238>
				pc  = (volatile unsigned char  *)NAND_CMD;
 a58:	e59fc074 	ldr	ip, [pc, #116]	; ad4 <.text+0xad4>
				*pc = (i & 0x1);
 a5c:	e2063001 	and	r3, r6, #1	; 0x1
 a60:	e4cc3002 	strb	r3, [ip], #2

				pc  = (volatile unsigned char  *)NAND_ADDR;
				*pc = 0x00;
 a64:	e3a03000 	mov	r3, #0	; 0x0
 a68:	e5cc3000 	strb	r3, [ip]
				*pc = ((StartAddr >>  9) & 0xff) | (i >> 1);
 a6c:	e1a030c6 	mov	r3, r6, asr #1
 a70:	e18334a5 	orr	r3, r3, r5, lsr #9
 a74:	e5cc3000 	strb	r3, [ip]
				*pc = (StartAddr >> 17) & 0xff;
 a78:	e1a038a5 	mov	r3, r5, lsr #17
 a7c:	e5cc3000 	strb	r3, [ip]
				if(~addr_cyc_2)
 a80:	e59f3050 	ldr	r3, [pc, #80]	; ad8 <.text+0xad8>
 a84:	e5d33000 	ldrb	r3, [r3]
 a88:	e3730001 	cmn	r3, #1	; 0x1
					*pc = (StartAddr >> 25) & 0xff;
 a8c:	11a03ca5 	movne	r3, r5, lsr #25
 a90:	15cc3000 	strneb	r3, [ip]

				NANDWait ();
 a94:	ebffff5e 	bl	814 <NANDWait>

				pc  = (volatile unsigned char  *)NAND_IO;
 a98:	e3a0c5d2 	mov	ip, #880803840	; 0x34800000
				for(j = 0; j < 256; j++) {
 a9c:	e3a02000 	mov	r2, #0	; 0x0
					*((unsigned char  *)pBuf) = *pc;
 aa0:	e5dc3000 	ldrb	r3, [ip]
 aa4:	e4c43001 	strb	r3, [r4], #1
 aa8:	e2822001 	add	r2, r2, #1	; 0x1
 aac:	e35200ff 	cmp	r2, #255	; 0xff
 ab0:	dafffffa 	ble	aa0 <NANDRead_1K+0x218>
 ab4:	e2866001 	add	r6, r6, #1	; 0x1
 ab8:	e3560003 	cmp	r6, #3	; 0x3
 abc:	daffffe5 	ble	a58 <NANDRead_1K+0x1d0>
					pBuf++;
				}
			}

		}
	}
#endif
	return 1024;
}
 ac0:	e3a00b01 	mov	r0, #1024	; 0x400
 ac4:	e8bd81f0 	ldmia	sp!, {r4, r5, r6, r7, r8, pc}
 ac8:	00000e88 	andeq	r0, r0, r8, lsl #29
 acc:	00000ea8 	andeq	r0, r0, r8, lsr #29
 ad0:	00000ea9 	andeq	r0, r0, r9, lsr #29
 ad4:	34800002 	strcc	r0, [r0], #2
 ad8:	00000eaa 	andeq	r0, r0, sl, lsr #29
 adc:	000001ff 	streqd	r0, [r0], -pc
 ae0:	000003ff 	streqd	r0, [r0], -pc

00000ae4 <NANDClose>:

int NANDClose(void)
{
	return 1;
}
 ae4:	e3a00001 	mov	r0, #1	; 0x1
 ae8:	e1a0f00e 	mov	pc, lr

00000aec <SPIOpen>:

unsigned char addr_len = 1;

int SPIOpen(void)
{
 aec:	e52de004 	str	lr, [sp, #-4]!
#ifdef SPI_FUNC_ENA
	SPI_REG_WRITE_HALF(SPI_SSIENR,0x0);
#if 0
    if(SPI_REG_READ_BYTES(SMC_BOOT) & 0x2)
	    SPI_REG_WRITE_HALF(SPI_BAUDR, 0x37);	/* 0x4 0xc8(256K)*/
    else
	    SPI_REG_WRITE_HALF(SPI_BAUDR, 0x37);	/* SPI_BDINIT */
#endif
    	SPI_REG_WRITE_HALF(SPI_BAUDR, 0xc8);	
	
	SPI_REG_WRITE_HALF(SPI_SER  , 0x1);

	SPI_REG_WRITE_HALF(SPI_CTRLR0, SPI_CR0INI|(((SPI_REG_READ_BYTES(SMC_BOOT)) & 0xc)<<4));
	addr_len=SPI_REG_READ_BYTES(SMC_BOOT) >> 5;
	
#else
	asm_reset_spi();
 af0:	eb000007 	bl	b14 <asm_reset_spi>
#endif
	return 1;
}
 af4:	e3a00001 	mov	r0, #1	; 0x1
 af8:	e49df004 	ldr	pc, [sp], #4

00000afc <SPIRead_1K>:

int SPIRead_1K(unsigned int StartAddr, unsigned char *Buf)
{
 afc:	e52de004 	str	lr, [sp, #-4]!
#ifdef SPI_FUNC_ENA
	int i  = 0;
	int j  = 0;
    	assert((StartAddr & 0x3ff) == 0);
    	SPI_REG_WRITE_HALF(SPI_SSIENR,0x0);
    	SPI_REG_WRITE_HALF(SPI_CTRLR1, SPI_CR1INI);
	SPI_REG_WRITE_HALF(SPI_SSIENR,0x1);
	SPI_REG_WRITE_HALF(SPI_DR,0x3);

	if(addr_len>=2)
		SPI_REG_WRITE_HALF(SPI_DR,((StartAddr>>16) & 0xff));
	if(addr_len>=1)
		SPI_REG_WRITE_HALF(SPI_DR,((StartAddr>>8) & 0xff));
	SPI_REG_WRITE_HALF(SPI_DR,(StartAddr & 0xff));

	while(i<=SPI_CR1INI) {
		while(!(j=SPI_REG_READ_BYTES(SPI_RX_CNT)));
		while(j>0) {
			Buf[i]=SPI_REG_READ_BYTES(SPI_DR);
			++i;
			--j;
		}
	}
	SPI_REG_WRITE_HALF(SPI_SSIENR,0x0);
#else
	asm_spi_read_1k(StartAddr, Buf);
 b00:	eb00000f 	bl	b44 <asm_spi_read_1k>
#endif

	return 1024;
	
}
 b04:	e3a00b01 	mov	r0, #1024	; 0x400
 b08:	e49df004 	ldr	pc, [sp], #4

00000b0c <SPIClose>:

int SPIClose(void)
{
#ifdef SPI_FUNC_ENA
	SPI_REG_WRITE_HALF(SPI_SSIENR,0x0);
	SPI_REG_WRITE_HALF(SPI_SER  , 0x0);
#endif
	return 1;
}
 b0c:	e3a00001 	mov	r0, #1	; 0x1
 b10:	e1a0f00e 	mov	pc, lr

00000b14 <asm_reset_spi>:
                LOADREGS(eqea, fp, {r0, r4 - r9, fp, sp, pc})

.globl asm_reset_spi
asm_reset_spi:
      ENTER
 b14:	e1a0c00d 	mov	ip, sp
 b18:	e92ddbf1 	stmdb	sp!, {r0, r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
 b1c:	e24cb004 	sub	fp, ip, #4	; 0x4
      ldr   r9, SPI_SSIENR
 b20:	e59f9120 	ldr	r9, [pc, #288]	; c48 <SPI_SSIENR>
      mov   r0,  #0x0
 b24:	e3a00000 	mov	r0, #0	; 0x0
      str  r0,  [r9]
 b28:	e5890000 	str	r0, [r9]
      ldr   r9, SPI_SSIENR
 b2c:	e59f9114 	ldr	r9, [pc, #276]	; c48 <SPI_SSIENR>
      mov   r0,  #0x1
 b30:	e3a00001 	mov	r0, #1	; 0x1
      str  r0,  [r9]
 b34:	e5890000 	str	r0, [r9]
      bl _delay
 b38:	eb00003b 	bl	c2c <_delay>
      EXIT
 b3c:	e91babf1 	ldmdb	fp, {r0, r4, r5, r6, r7, r8, r9, fp, sp, pc}
      mov pc, lr
 b40:	e1a0f00e 	mov	pc, lr

00000b44 <asm_spi_read_1k>:

.globl asm_spi_read_1k
asm_spi_read_1k:

      ENTER
 b44:	e1a0c00d 	mov	ip, sp
 b48:	e92ddbf1 	stmdb	sp!, {r0, r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
 b4c:	e24cb004 	sub	fp, ip, #4	; 0x4
      mov r2, r0
 b50:	e1a02000 	mov	r2, r0

00000b54 <_reset_spi>:

_reset_spi:

      # configurate spi baud rate
      ldr   r9, SPI_SSIENR
 b54:	e59f90ec 	ldr	r9, [pc, #236]	; c48 <SPI_SSIENR>
      mov   r0,  #0x0
 b58:	e3a00000 	mov	r0, #0	; 0x0
      str  r0,  [r9]
 b5c:	e5890000 	str	r0, [r9]
	
      #configurate spi contrl register 0
      ldr   r9, SPI_CTRLR0
 b60:	e59f90d8 	ldr	r9, [pc, #216]	; c40 <SPI_CTRLR0>
      ldr   r0,  SPI_CR0INI
 b64:	e59f00f4 	ldr	r0, [pc, #244]	; c60 <SPI_CR0INI>
      str  r0, [r9]
 b68:	e5890000 	str	r0, [r9]

      #configurate spi control register 1
      ldr   r9, SPI_CTRLR1
 b6c:	e59f90d0 	ldr	r9, [pc, #208]	; c44 <SPI_CTRLR1>
      ldr   r0,  SPI_CR1INI	@4K byte data
 b70:	e59f00ec 	ldr	r0, [pc, #236]	; c64 <SPI_CR1INI>
      str  r0,  [r9]
 b74:	e5890000 	str	r0, [r9]

      ldr   r9, SPI_BAUDR
 b78:	e59f90d0 	ldr	r9, [pc, #208]	; c50 <SPI_BAUDR>
      ldr   r0,  SPI_BDINIT
 b7c:	e59f00d8 	ldr	r0, [pc, #216]	; c5c <SPI_BDINIT>
      str  r0,  [r9]	     	
 b80:	e5890000 	str	r0, [r9]
  
      # select one spi slave
      ldr   r9, SPI_SER
 b84:	e59f90c0 	ldr	r9, [pc, #192]	; c4c <SPI_SER>
      mov   r0,  #0x1
 b88:	e3a00001 	mov	r0, #1	; 0x1
      str  r0,  [r9]
 b8c:	e5890000 	str	r0, [r9]

      #enable spi
      ldr   r9, SPI_SSIENR
 b90:	e59f90b0 	ldr	r9, [pc, #176]	; c48 <SPI_SSIENR>
      mov   r0,  #0x1
 b94:	e3a00001 	mov	r0, #1	; 0x1
      str  r0,  [r9]
 b98:	e5890000 	str	r0, [r9]

#bl _delay

      # configurate contol byte and address
      ldr   r8, SPI_DR	
 b9c:	e59f80b4 	ldr	r8, [pc, #180]	; c58 <SPI_DR>
      mov   r0,  #0x3
 ba0:	e3a00003 	mov	r0, #3	; 0x3
      str  r0,  [r8]
 ba4:	e5880000 	str	r0, [r8]

      # set addr
      mov   r0, r2
 ba8:	e1a00002 	mov	r0, r2
      mov   r0,  r0,   lsl  #8
 bac:	e1a00400 	mov	r0, r0, lsl #8
      mov   r0,  r0,   lsr  #24
 bb0:	e1a00c20 	mov	r0, r0, lsr #24
      str  r0,  [r8]
 bb4:	e5880000 	str	r0, [r8]

      mov   r0, r2
 bb8:	e1a00002 	mov	r0, r2
      mov   r0,  r0,   lsl  #16
 bbc:	e1a00800 	mov	r0, r0, lsl #16
      mov   r0,  r0,   lsr  #24
 bc0:	e1a00c20 	mov	r0, r0, lsr #24
      str  r0,  [r8]
 bc4:	e5880000 	str	r0, [r8]

      mov   r0, r2
 bc8:	e1a00002 	mov	r0, r2
      mov   r0,  r0,   lsl  #24
 bcc:	e1a00c00 	mov	r0, r0, lsl #24
      mov   r0,  r0,   lsr  #24
 bd0:	e1a00c20 	mov	r0, r0, lsr #24
      str  r0,  [r8]
 bd4:	e5880000 	str	r0, [r8]

      # set read number
      ldr   r6, SPI_CP_NUM 
 bd8:	e59f6090 	ldr	r6, [pc, #144]	; c70 <SPI_CP_NUM>
      ldr   r7, SPI_SR
 bdc:	e59f7070 	ldr	r7, [pc, #112]	; c54 <SPI_SR>

00000be0 <_wait_spi>:
			   	      
_wait_spi:
      teq   r6,  #0x0
 be0:	e3360000 	teq	r6, #0	; 0x0
      beq   _end_spi
 be4:	0a000007 	beq	c08 <_end_spi>
			   	      
1:    ldr   r0,  [r7]
 be8:	e5970000 	ldr	r0, [r7]
      ands  r0,  r0,   #0x8
 bec:	e2100008 	ands	r0, r0, #8	; 0x8
      bne   _copy_spi
 bf0:	1a000000 	bne	bf8 <_copy_spi>
      b     1b
 bf4:	eafffffb 	b	be8 <_wait_spi+0x8>

00000bf8 <_copy_spi>:
 		
_copy_spi:
      sub   r6,  r6,   #0x1
 bf8:	e2466001 	sub	r6, r6, #1	; 0x1
      ldrb  r5,  [r8]	
 bfc:	e5d85000 	ldrb	r5, [r8]
      strb  r5,  [r1], #0x1
 c00:	e4c15001 	strb	r5, [r1], #1
      b     _wait_spi		
 c04:	eafffff5 	b	be0 <_wait_spi>

00000c08 <_end_spi>:
     	
_end_spi:

      ldr   r9, SPI_SER
 c08:	e59f903c 	ldr	r9, [pc, #60]	; c4c <SPI_SER>
      mov   r0,  #0x0
 c0c:	e3a00000 	mov	r0, #0	; 0x0
      str  r0,  [r9]
 c10:	e5890000 	str	r0, [r9]

      #disable spi
      ldr   r9, SPI_SSIENR
 c14:	e59f902c 	ldr	r9, [pc, #44]	; c48 <SPI_SSIENR>
      mov   r0,  #0x0
 c18:	e3a00000 	mov	r0, #0	; 0x0
      str  r0,  [r9]
 c1c:	e5890000 	str	r0, [r9]
      b     _copy_end
 c20:	eaffffff 	b	c24 <_copy_end>

00000c24 <_copy_end>:

_copy_end:
      EXIT
 c24:	e91babf1 	ldmdb	fp, {r0, r4, r5, r6, r7, r8, r9, fp, sp, pc}
      mov  pc, lr
 c28:	e1a0f00e 	mov	pc, lr

00000c2c <_delay>:

/* delay */
_delay:
      ldr   r6, =15000
 c2c:	e59f6040 	ldr	r6, [pc, #64]	; c74 <SPI_CP_NUM+0x4>
1:
      sub   r6,  r6,   #0x1
 c30:	e2466001 	sub	r6, r6, #1	; 0x1
      teq   r6,  #0x0
 c34:	e3360000 	teq	r6, #0	; 0x0
      bne   1b
 c38:	1afffffc 	bne	c30 <_delay+0x4>
      mov  pc, lr
 c3c:	e1a0f00e 	mov	pc, lr

00000c40 <SPI_CTRLR0>:
 c40:	10173000 	andnes	r3, r7, r0

00000c44 <SPI_CTRLR1>:
 c44:	10173004 	andnes	r3, r7, r4

00000c48 <SPI_SSIENR>:
 c48:	10173008 	andnes	r3, r7, r8

00000c4c <SPI_SER>:
 c4c:	10173010 	andnes	r3, r7, r0, lsl r0

00000c50 <SPI_BAUDR>:
 c50:	10173014 	andnes	r3, r7, r4, lsl r0

00000c54 <SPI_SR>:
 c54:	10173028 	andnes	r3, r7, r8, lsr #32

00000c58 <SPI_DR>:
 c58:	10173060 	andnes	r3, r7, r0, rrx

00000c5c <SPI_BDINIT>:
 c5c:	00000038 	andeq	r0, r0, r8, lsr r0

00000c60 <SPI_CR0INI>:
 c60:	000003c7 	andeq	r0, r0, r7, asr #7

00000c64 <SPI_CR1INI>:
 c64:	000003ff 	streqd	r0, [r0], -pc

00000c68 <SPI_RX_CNT>:
 c68:	10173024 	andnes	r3, r7, r4, lsr #32

00000c6c <SMC_BOOT>:
 c6c:	10100034 	andnes	r0, r0, r4, lsr r0

00000c70 <SPI_CP_NUM>:
 c70:	00000400 	andeq	r0, r0, r0, lsl #8
 c74:	00003a98 	muleq	r0, r8, sl

00000c78 <x_memset>:
void *x_memset(
   void* dest,
   int c, 
   int count )
{
 c78:	e52de004 	str	lr, [sp, #-4]!
	int i = 0;
 c7c:	e3a03000 	mov	r3, #0	; 0x0
	if (dest == NULL || count <=0)
 c80:	e1a0e000 	mov	lr, r0
 c84:	e1a0c002 	mov	ip, r2
 c88:	e1500003 	cmp	r0, r3
 c8c:	11520003 	cmpne	r2, r3
	{
		return NULL;
 c90:	d1a00003 	movle	r0, r3
 c94:	d49df004 	ldrle	pc, [sp], #4
	}
	while (i < count)
	{
		*((char *)dest + i) = (char)c;
		i++;
 c98:	e1530002 	cmp	r3, r2
 c9c:	aa000003 	bge	cb0 <x_memset+0x38>
 ca0:	e7ce1003 	strb	r1, [lr, r3]
 ca4:	e2833001 	add	r3, r3, #1	; 0x1
 ca8:	e153000c 	cmp	r3, ip
 cac:	bafffffb 	blt	ca0 <x_memset+0x28>
	}
	return dest;
 cb0:	e1a0000e 	mov	r0, lr
}
 cb4:	e49df004 	ldr	pc, [sp], #4

00000cb8 <x_memcpy>:

void *x_memcpy(
   void *dest,
   const void *src,
   int count 
)
{
 cb8:	e1a0c000 	mov	ip, r0
	int i = 0;
 cbc:	e3a00000 	mov	r0, #0	; 0x0
	if (count <=0)
 cc0:	e3520000 	cmp	r2, #0	; 0x0
 cc4:	d1a0f00e 	movle	pc, lr
	{
		return NULL;
	}
	for (i = 0; i < count ; i++)
 cc8:	e3a00000 	mov	r0, #0	; 0x0
 ccc:	e1500002 	cmp	r0, r2
 cd0:	aa000004 	bge	ce8 <x_memcpy+0x30>
	{
		*((char *)dest + i) = *((char *)src + i); 
 cd4:	e7d13000 	ldrb	r3, [r1, r0]
 cd8:	e7cc3000 	strb	r3, [ip, r0]
 cdc:	e2800001 	add	r0, r0, #1	; 0x1
 ce0:	e1500002 	cmp	r0, r2
 ce4:	bafffffa 	blt	cd4 <x_memcpy+0x1c>
	}
	return dest;
 ce8:	e1a0000c 	mov	r0, ip
}
 cec:	e1a0f00e 	mov	pc, lr

00000cf0 <x_memcmp>:

int  x_memcmp(const void* str1, const void* str2, int n)
{
 cf0:	e92d4010 	stmdb	sp!, {r4, lr}
  register unsigned char *s1 = (unsigned char*)str1;
  register unsigned char *s2 = (unsigned char*)str2;
	if (s1 == NULL || s2 == NULL)
 cf4:	e1a0e000 	mov	lr, r0
 cf8:	e1a04001 	mov	r4, r1
 cfc:	e3510000 	cmp	r1, #0	; 0x0
 d00:	13500000 	cmpne	r0, #0	; 0x0
		return 0;
 d04:	03a00000 	moveq	r0, #0	; 0x0
 d08:	08bd8010 	ldmeqia	sp!, {r4, pc}
  while (n-- > 0) {
    if (*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
 d0c:	e242c001 	sub	ip, r2, #1	; 0x1
 d10:	e3520000 	cmp	r2, #0	; 0x0
 d14:	da00000d 	ble	d50 <x_memcmp+0x60>
 d18:	e4de2001 	ldrb	r2, [lr], #1
 d1c:	e4d43001 	ldrb	r3, [r4], #1
 d20:	e1520003 	cmp	r2, r3
 d24:	0a000005 	beq	d40 <x_memcmp+0x50>
 d28:	e55e2001 	ldrb	r2, [lr, #-1]
 d2c:	e5543001 	ldrb	r3, [r4, #-1]
 d30:	e1520003 	cmp	r2, r3
 d34:	23a00001 	movcs	r0, #1	; 0x1
 d38:	33e00000 	mvncc	r0, #0	; 0x0
 d3c:	e8bd8010 	ldmia	sp!, {r4, pc}
 d40:	e1a0300c 	mov	r3, ip
 d44:	e24cc001 	sub	ip, ip, #1	; 0x1
 d48:	e3530000 	cmp	r3, #0	; 0x0
 d4c:	cafffff1 	bgt	d18 <x_memcmp+0x28>
  }
  return 0;
 d50:	e3a00000 	mov	r0, #0	; 0x0
}
 d54:	e8bd8010 	ldmia	sp!, {r4, pc}

00000d58 <__udivsi3>:

unsigned int __udivsi3(unsigned int num, unsigned int den)
{
  unsigned int quot = 0, qbit = 1;
 d58:	e3a02000 	mov	r2, #0	; 0x0
 d5c:	e3a03001 	mov	r3, #1	; 0x1

  if ( den == 0 ) {
 d60:	e3510000 	cmp	r1, #0	; 0x0
    ///__divide_error();
    return 0;			/* If trap returns... */
 d64:	01a00002 	moveq	r0, r2
 d68:	01a0f00e 	moveq	pc, lr
  }

  /* Left-justify denominator and count shift */
  while ( (int)den >= 0 ) {
    den <<= 1;
    qbit <<= 1;
 d6c:	e3510000 	cmp	r1, #0	; 0x0
 d70:	ba000002 	blt	d80 <__udivsi3+0x28>
 d74:	e1a03083 	mov	r3, r3, lsl #1
 d78:	e1b01081 	movs	r1, r1, lsl #1
 d7c:	5afffffc 	bpl	d74 <__udivsi3+0x1c>
  }

  while ( qbit ) {
    if ( den <= num ) {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
 d80:	e3530000 	cmp	r3, #0	; 0x0
 d84:	0a000005 	beq	da0 <__udivsi3+0x48>
 d88:	e1510000 	cmp	r1, r0
 d8c:	90610000 	rsbls	r0, r1, r0
 d90:	90822003 	addls	r2, r2, r3
 d94:	e1a010a1 	mov	r1, r1, lsr #1
 d98:	e1b030a3 	movs	r3, r3, lsr #1
 d9c:	1afffff9 	bne	d88 <__udivsi3+0x30>
  }
/*
  if ( rem_p )
    *rem_p = num;
*/
  return quot;
 da0:	e1a00002 	mov	r0, r2
}
 da4:	e1a0f00e 	mov	pc, lr

00000da8 <x_assert>:
int x_assert(int exp, char *exp_str, char *file, int line)
{    
    volatile char *p;

	if (!exp)
 da8:	e3500000 	cmp	r0, #0	; 0x0
 dac:	11a0f00e 	movne	pc, lr
 db0:	e59f2008 	ldr	r2, [pc, #8]	; dc0 <.text+0xdc0>
 db4:	e3e03000 	mvn	r3, #0	; 0x0
	{
		#ifdef ENA_PRINT
			printf("Assert: FILE%s Line %d:\n", file, line);
			printf("      : %s:\n", exp_str);
			getchar();
		#else
		while(1) //Never Stop
		{
            /* Stop Simulation */                    
            p   = (volatile char *)0x34800001;
            *p  = 0xff;
 db8:	e5c23000 	strb	r3, [r2]

			exp = line * line;
 dbc:	eafffffd 	b	db8 <x_assert+0x10>
 dc0:	34800001 	strcc	r0, [r0], #1
 dc4:	12345678 	eornes	r5, r4, #125829120	; 0x7800000
 dc8:	9abcdef0 	bls	fef38990 <SYSTEM_STACK_SIZE+0xfef36990>
 dcc:	aaaaaaaa 	bge	feaab87c <SYSTEM_STACK_SIZE+0xfeaa987c>
 dd0:	55555555 	ldrplb	r5, [r5, #-1365]
 dd4:	00000000 	andeq	r0, r0, r0
 dd8:	ffffffff 	swinv	0x00ffffff
 ddc:	00000030 	andeq	r0, r0, r0, lsr r0
 de0:	71657246 	cmnvc	r5, r6, asr #4
 de4:	566c7443 	strplbt	r7, [ip], -r3, asr #8
 de8:	65756c61 	ldrvsb	r6, [r5, #-3169]!
 dec:	203d3d20 	eorcss	r3, sp, r0, lsr #26
 df0:	64616552 	strvsbt	r6, [r1], #-1362
 df4:	756c6156 	strvcb	r6, [ip, #-342]!
 df8:	00000065 	andeq	r0, r0, r5, rrx
 dfc:	65522828 	ldrvsb	r2, [r2, #-2088]
 e00:	6c615667 	stcvsl	6, cr5, [r1], #-412
 e04:	26206575 	undefined
 e08:	31783020 	cmncc	r8, r0, lsr #32
 e0c:	3d3d2029 	ldccc	0, cr2, [sp, #-164]!
 e10:	00293120 	eoreq	r3, r9, r0, lsr #2
 e14:	65522828 	ldrvsb	r2, [r2, #-2088]
 e18:	6c615667 	stcvsl	6, cr5, [r1], #-412
 e1c:	26206575 	undefined
 e20:	31783020 	cmncc	r8, r0, lsr #32
 e24:	3d3d2029 	ldccc	0, cr2, [sp, #-164]!
 e28:	00293020 	eoreq	r3, r9, r0, lsr #32
 e2c:	3d207472 	cfstrscc	mvf7, [r0, #-456]!
 e30:	0030203d 	eoreqs	r2, r0, sp, lsr r0
 e34:	20746572 	rsbcss	r6, r4, r2, ror r5
 e38:	31203d3d 	teqcc	r0, sp, lsr sp
 e3c:	00343230 	eoreqs	r3, r4, r0, lsr r2
 e40:	61745328 	cmnvs	r4, r8, lsr #6
 e44:	64417472 	strvsb	r7, [r1], #-1138
 e48:	26207264 	strcst	r7, [r0], -r4, ror #4
 e4c:	307e2820 	rsbccs	r2, lr, r0, lsr #16
 e50:	66663378 	undefined
 e54:	3d202929 	stccc	9, cr2, [r0, #-164]!
 e58:	2828203d 	stmcsda	r8!, {r0, r2, r3, r4, r5, sp}
 e5c:	72617453 	rsbvc	r7, r1, #1392508928	; 0x53000000
 e60:	64644174 	strvsbt	r4, [r4], #-372
 e64:	202b2072 	eorcs	r2, fp, r2, ror r0
 e68:	657a6953 	ldrvsb	r6, [sl, #-2387]!
 e6c:	31202d20 	teqcc	r0, r0, lsr #26
 e70:	20262029 	eorcs	r2, r6, r9, lsr #32
 e74:	3378307e 	cmncc	r8, #126	; 0x7e
 e78:	00296666 	eoreq	r6, r9, r6, ror #12
 e7c:	3d3d2069 	ldccc	0, cr2, [sp, #-420]!
 e80:	7a695320 	bvc	1a55b08 <SYSTEM_STACK_SIZE+0x1a53b08>
 e84:	00000065 	andeq	r0, r0, r5, rrx
 e88:	61745328 	cmnvs	r4, r8, lsr #6
 e8c:	64417472 	strvsb	r7, [r1], #-1138
 e90:	26207264 	strcst	r7, [r0], -r4, ror #4
 e94:	33783020 	cmncc	r8, #32	; 0x20
 e98:	20296666 	eorcs	r6, r9, r6, ror #12
 e9c:	30203d3d 	eorcc	r3, r0, sp, lsr sp
 ea0:	00000000 	andeq	r0, r0, r0
