
start.o:     file format elf32-littlearm

Disassembly of section .text:

00000000 <_start>:
   0:	ea000012 	b	50 <reset>
   4:	e59ff014 	ldr	pc, [pc, #20]	; 20 <_undefined_instruction>
   8:	e59ff014 	ldr	pc, [pc, #20]	; 24 <_software_interrupt>
   c:	e59ff014 	ldr	pc, [pc, #20]	; 28 <_prefetch_abort>
  10:	e59ff014 	ldr	pc, [pc, #20]	; 2c <_data_abort>
  14:	e59ff014 	ldr	pc, [pc, #20]	; 30 <_not_used>
  18:	e59ff014 	ldr	pc, [pc, #20]	; 34 <_irq>
  1c:	e59ff014 	ldr	pc, [pc, #20]	; 38 <_fiq>

00000020 <_undefined_instruction>:
  20:	00000380 	andeq	r0, r0, r0, lsl #7

00000024 <_software_interrupt>:
  24:	000003e0 	andeq	r0, r0, r0, ror #7

00000028 <_prefetch_abort>:
  28:	00000440 	andeq	r0, r0, r0, asr #8

0000002c <_data_abort>:
  2c:	000004a0 	andeq	r0, r0, r0, lsr #9

00000030 <_not_used>:
  30:	00000500 	andeq	r0, r0, r0, lsl #10

00000034 <_irq>:
  34:	00000560 	andeq	r0, r0, r0, ror #10

00000038 <_fiq>:
  38:	000005c0 	andeq	r0, r0, r0, asr #11
  3c:	deadbeef 	cdple	14, 10, cr11, cr13, cr15, {7}

00000040 <_TEXT_BASE>:
  40:	02000000 	andeq	r0, r0, #0	; 0x0

00000044 <_armboot_start>:
  44:	00000000 	andeq	r0, r0, r0

00000048 <_bss_start>:
  48:	00000000 	andeq	r0, r0, r0

0000004c <_bss_end>:
  4c:	00000000 	andeq	r0, r0, r0

00000050 <reset>:
  50:	e10f0000 	mrs	r0, CPSR
  54:	e3c0001f 	bic	r0, r0, #31	; 0x1f
  58:	e38000d3 	orr	r0, r0, #211	; 0xd3
  5c:	e129f000 	msr	CPSR_fc, r0
  60:	eb0000b5 	bl	33c <cpu_init_crit>
  64:	eb000022 	bl	f4 <copy_myself>
  68:	e59f15b4 	ldr	r1, [pc, #1460]	; 624 <SMC_BOOT+0x4>
  6c:	e281f000 	add	pc, r1, #0	; 0x0
  70:	e1a00000 	nop			(mov r0,r0)
  74:	e1a00000 	nop			(mov r0,r0)
  78:	eafffffe 	b	78 <reset+0x28>

0000007c <on_the_ram>:
  7c:	e24f0084 	sub	r0, pc, #132	; 0x84
  80:	e51f1048 	ldr	r1, [pc, #-72]	; 40 <_TEXT_BASE>
  84:	e1500001 	cmp	r0, r1
  88:	0a000007 	beq	ac <stack_setup>
  8c:	e51f2050 	ldr	r2, [pc, #-80]	; 44 <_armboot_start>
  90:	e51f3050 	ldr	r3, [pc, #-80]	; 48 <_bss_start>
  94:	e0432002 	sub	r2, r3, r2
  98:	e0802002 	add	r2, r0, r2

0000009c <copy_loop>:
  9c:	e8b007f8 	ldmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
  a0:	e8a107f8 	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
  a4:	e1500002 	cmp	r0, r2
  a8:	dafffffb 	ble	9c <copy_loop>

000000ac <stack_setup>:
  ac:	e51f0074 	ldr	r0, [pc, #-116]	; 40 <_TEXT_BASE>
  b0:	e2400701 	sub	r0, r0, #262144	; 0x40000
  b4:	e2400080 	sub	r0, r0, #128	; 0x80
  b8:	e240d00c 	sub	sp, r0, #12	; 0xc

000000bc <clear_bss>:
  bc:	e51f007c 	ldr	r0, [pc, #-124]	; 48 <_bss_start>
  c0:	e51f107c 	ldr	r1, [pc, #-124]	; 4c <_bss_end>
  c4:	e3a02000 	mov	r2, #0	; 0x0

000000c8 <clbss_l>:
  c8:	e5802000 	str	r2, [r0]
  cc:	e2800004 	add	r0, r0, #4	; 0x4
  d0:	e1500001 	cmp	r0, r1
  d4:	dafffffb 	ble	c8 <clbss_l>
  d8:	e59ff00c 	ldr	pc, [pc, #12]	; ec <_relocated_position>

000000dc <relocated_position>:
  dc:	e3a01001 	mov	r1, #1	; 0x1
  e0:	e59f2008 	ldr	r2, [pc, #8]	; f0 <_remap_ctrl>
  e4:	e1c210b0 	strh	r1, [r2]
  e8:	eafffffe 	b	0 <start_armboot>

000000ec <_relocated_position>:
  ec:	000000dc 	ldreqd	r0, [r0], -ip

000000f0 <_remap_ctrl>:
  f0:	101e0008 	andnes	r0, lr, r8

000000f4 <copy_myself>:
  f4:	e1a0200e 	mov	r2, lr
  f8:	e59f0520 	ldr	r0, [pc, #1312]	; 620 <SMC_BOOT>
  fc:	e5907000 	ldr	r7, [r0]
 100:	e59f8520 	ldr	r8, [pc, #1312]	; 628 <SMC_BOOT+0x8>
 104:	e51f30c4 	ldr	r3, [pc, #-196]	; 48 <_bss_start>
 108:	e0439008 	sub	r9, r3, r8
 10c:	e3a0a30d 	mov	sl, #872415232	; 0x34000000
 110:	e3a0b38d 	mov	fp, #872415234	; 0x34000002
 114:	e59fc510 	ldr	ip, [pc, #1296]	; 62c <SMC_BOOT+0xc>
 118:	e2171004 	ands	r1, r7, #4	; 0x4
 11c:	1a000003 	bne	130 <_reset_16>

00000120 <_reset_8>:
 120:	e3a000ff 	mov	r0, #255	; 0xff
 124:	e5cb0000 	strb	r0, [fp]
 128:	eb000078 	bl	310 <_wait_ready>
 12c:	ea000003 	b	140 <_copy_begin>

00000130 <_reset_16>:
 130:	e3a000ff 	mov	r0, #255	; 0xff
 134:	e1cb00b0 	strh	r0, [fp]
 138:	eb000074 	bl	310 <_wait_ready>
 13c:	eaffffff 	b	140 <_copy_begin>

00000140 <_copy_begin>:
 140:	e2171002 	ands	r1, r7, #2	; 0x2
 144:	1a000045 	bne	260 <_copy_large_block>

00000148 <_copy_small_block>:
 148:	e2171004 	ands	r1, r7, #4	; 0x4
 14c:	1a000021 	bne	1d8 <_copy_small_block_16>

00000150 <_copy_small_block_8>:
 150:	e3a00000 	mov	r0, #0	; 0x0
 154:	e5cb0000 	strb	r0, [fp]
 158:	e5cc0000 	strb	r0, [ip]
 15c:	e5cc9000 	strb	r9, [ip]
 160:	e5cc0000 	strb	r0, [ip]
 164:	e2171008 	ands	r1, r7, #8	; 0x8
 168:	1a000000 	bne	170 <_copy_small_block_8+0x20>
 16c:	e5cc0000 	strb	r0, [ip]
 170:	eb000066 	bl	310 <_wait_ready>
 174:	e3a00000 	mov	r0, #0	; 0x0
 178:	e5da1000 	ldrb	r1, [sl]
 17c:	e4c81001 	strb	r1, [r8], #1
 180:	e2800001 	add	r0, r0, #1	; 0x1
 184:	e3300c01 	teq	r0, #256	; 0x100
 188:	1afffffa 	bne	178 <_copy_small_block_8+0x28>
 18c:	e3a00001 	mov	r0, #1	; 0x1
 190:	e5cb0000 	strb	r0, [fp]
 194:	e3a00000 	mov	r0, #0	; 0x0
 198:	e5cc0000 	strb	r0, [ip]
 19c:	e5cc9000 	strb	r9, [ip]
 1a0:	e5cc0000 	strb	r0, [ip]
 1a4:	e2171008 	ands	r1, r7, #8	; 0x8
 1a8:	1a000000 	bne	1b0 <_copy_small_block_8+0x60>
 1ac:	e5cc0000 	strb	r0, [ip]
 1b0:	eb000056 	bl	310 <_wait_ready>
 1b4:	e3a00000 	mov	r0, #0	; 0x0
 1b8:	e5da1000 	ldrb	r1, [sl]
 1bc:	e4c81001 	strb	r1, [r8], #1
 1c0:	e2800001 	add	r0, r0, #1	; 0x1
 1c4:	e3300c01 	teq	r0, #256	; 0x100
 1c8:	1afffffa 	bne	1b8 <_copy_small_block_8+0x68>
 1cc:	e2499c01 	sub	r9, r9, #256	; 0x100
 1d0:	4affffde 	bmi	150 <_copy_small_block_8>
 1d4:	ea000057 	b	338 <_copy_end>

000001d8 <_copy_small_block_16>:
 1d8:	e3a00000 	mov	r0, #0	; 0x0
 1dc:	e1cb00b0 	strh	r0, [fp]
 1e0:	e1cc00b0 	strh	r0, [ip]
 1e4:	e1cc90b0 	strh	r9, [ip]
 1e8:	e1cc00b0 	strh	r0, [ip]
 1ec:	e2171008 	ands	r1, r7, #8	; 0x8
 1f0:	1a000000 	bne	1f8 <_copy_small_block_16+0x20>
 1f4:	e1cc00b0 	strh	r0, [ip]
 1f8:	eb000044 	bl	310 <_wait_ready>
 1fc:	e3a00000 	mov	r0, #0	; 0x0
 200:	e1da10b0 	ldrh	r1, [sl]
 204:	e0c810b2 	strh	r1, [r8], #2
 208:	e2800001 	add	r0, r0, #1	; 0x1
 20c:	e3300080 	teq	r0, #128	; 0x80
 210:	1afffffa 	bne	200 <_copy_small_block_16+0x28>
 214:	e3a00001 	mov	r0, #1	; 0x1
 218:	e1cb00b0 	strh	r0, [fp]
 21c:	e3a00000 	mov	r0, #0	; 0x0
 220:	e1cc00b0 	strh	r0, [ip]
 224:	e1cc90b0 	strh	r9, [ip]
 228:	e1cc00b0 	strh	r0, [ip]
 22c:	e2171008 	ands	r1, r7, #8	; 0x8
 230:	1a000000 	bne	238 <_copy_small_block_16+0x60>
 234:	e1cc00b0 	strh	r0, [ip]
 238:	eb000034 	bl	310 <_wait_ready>
 23c:	e3a00000 	mov	r0, #0	; 0x0
 240:	e1da10b0 	ldrh	r1, [sl]
 244:	e0c810b2 	strh	r1, [r8], #2
 248:	e2800001 	add	r0, r0, #1	; 0x1
 24c:	e3300080 	teq	r0, #128	; 0x80
 250:	1afffffa 	bne	240 <_copy_small_block_16+0x68>
 254:	e2499080 	sub	r9, r9, #128	; 0x80
 258:	4affffde 	bmi	1d8 <_copy_small_block_16>
 25c:	ea000035 	b	338 <_copy_end>

00000260 <_copy_large_block>:
 260:	e2171004 	ands	r1, r7, #4	; 0x4
 264:	1a000014 	bne	2bc <_copy_large_block_16>

00000268 <_copy_large_block_8>:
 268:	e3a00000 	mov	r0, #0	; 0x0
 26c:	e5cb0000 	strb	r0, [fp]
 270:	e5cc0000 	strb	r0, [ip]
 274:	e5cc0000 	strb	r0, [ip]
 278:	e5cc9000 	strb	r9, [ip]
 27c:	e5cc0000 	strb	r0, [ip]
 280:	e2171008 	ands	r1, r7, #8	; 0x8
 284:	1a000000 	bne	28c <_copy_large_block_8+0x24>
 288:	e5cc0000 	strb	r0, [ip]
 28c:	e3a00030 	mov	r0, #48	; 0x30
 290:	e5cb0000 	strb	r0, [fp]
 294:	eb00001d 	bl	310 <_wait_ready>
 298:	e3a00000 	mov	r0, #0	; 0x0
 29c:	e5da1000 	ldrb	r1, [sl]
 2a0:	e4c81001 	strb	r1, [r8], #1
 2a4:	e2800001 	add	r0, r0, #1	; 0x1
 2a8:	e3300b02 	teq	r0, #2048	; 0x800
 2ac:	1afffffa 	bne	29c <_copy_large_block_8+0x34>
 2b0:	e2499b02 	sub	r9, r9, #2048	; 0x800
 2b4:	4affffeb 	bmi	268 <_copy_large_block_8>
 2b8:	ea00001e 	b	338 <_copy_end>

000002bc <_copy_large_block_16>:
 2bc:	e3a00000 	mov	r0, #0	; 0x0
 2c0:	e1cb00b0 	strh	r0, [fp]
 2c4:	e1cc00b0 	strh	r0, [ip]
 2c8:	e1cc00b0 	strh	r0, [ip]
 2cc:	e1cc90b0 	strh	r9, [ip]
 2d0:	e1cc00b0 	strh	r0, [ip]
 2d4:	e2171008 	ands	r1, r7, #8	; 0x8
 2d8:	1a000000 	bne	2e0 <_copy_large_block_16+0x24>
 2dc:	e1cc00b0 	strh	r0, [ip]
 2e0:	e3a00030 	mov	r0, #48	; 0x30
 2e4:	e1cb00b0 	strh	r0, [fp]
 2e8:	eb000008 	bl	310 <_wait_ready>
 2ec:	e3a00000 	mov	r0, #0	; 0x0
 2f0:	e1da10b0 	ldrh	r1, [sl]
 2f4:	e0c810b2 	strh	r1, [r8], #2
 2f8:	e2800001 	add	r0, r0, #1	; 0x1
 2fc:	e3300b01 	teq	r0, #1024	; 0x400
 300:	1afffffa 	bne	2f0 <_copy_large_block_16+0x34>
 304:	e2499b01 	sub	r9, r9, #1024	; 0x400
 308:	4affffeb 	bmi	2bc <_copy_large_block_16>
 30c:	ea000009 	b	338 <_copy_end>

00000310 <_wait_ready>:
 310:	e59f5304 	ldr	r5, [pc, #772]	; 61c <SMC_STA>
 314:	e3a04000 	mov	r4, #0	; 0x0
 318:	e5956000 	ldr	r6, [r5]
 31c:	e2844001 	add	r4, r4, #1	; 0x1
 320:	e3340014 	teq	r4, #20	; 0x14
 324:	1afffffb 	bne	318 <_wait_ready+0x8>
 328:	e5956000 	ldr	r6, [r5]
 32c:	e2166001 	ands	r6, r6, #1	; 0x1
 330:	0afffffc 	beq	328 <_wait_ready+0x18>
 334:	e1a0f00e 	mov	pc, lr

00000338 <_copy_end>:
 338:	e1a0f002 	mov	pc, r2

0000033c <cpu_init_crit>:
 33c:	e3a00000 	mov	r0, #0	; 0x0
 340:	ee070f17 	mcr	15, 0, r0, cr7, cr7, {0}
 344:	ee080f17 	mcr	15, 0, r0, cr8, cr7, {0}
 348:	ee110f10 	mrc	15, 0, r0, cr1, cr0, {0}
 34c:	e3c00c23 	bic	r0, r0, #8960	; 0x2300
 350:	e3c00087 	bic	r0, r0, #135	; 0x87
 354:	e3800002 	orr	r0, r0, #2	; 0x2
 358:	e3800a01 	orr	r0, r0, #4096	; 0x1000
 35c:	ee010f10 	mcr	15, 0, r0, cr1, cr0, {0}
 360:	e1a0c00e 	mov	ip, lr
 364:	ebfffffe 	bl	0 <lowlevel_init>
 368:	e1a0e00c 	mov	lr, ip
 36c:	e1a0f00e 	mov	pc, lr
	...

00000380 <undefined_instruction>:
 380:	e51fd344 	ldr	sp, [pc, #-836]	; 44 <_armboot_start>
 384:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 388:	e24dd088 	sub	sp, sp, #136	; 0x88
 38c:	e58de000 	str	lr, [sp]
 390:	e14fe000 	mrs	lr, SPSR
 394:	e58de004 	str	lr, [sp, #4]
 398:	e3a0d013 	mov	sp, #19	; 0x13
 39c:	e169f00d 	msr	SPSR_fc, sp
 3a0:	e1a0e00f 	mov	lr, pc
 3a4:	e1b0f00e 	movs	pc, lr
 3a8:	e24dd048 	sub	sp, sp, #72	; 0x48
 3ac:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 3b0:	e51f2374 	ldr	r2, [pc, #-884]	; 44 <_armboot_start>
 3b4:	e2422806 	sub	r2, r2, #393216	; 0x60000
 3b8:	e2422088 	sub	r2, r2, #136	; 0x88
 3bc:	e892000c 	ldmia	r2, {r2, r3}
 3c0:	e28d0048 	add	r0, sp, #72	; 0x48
 3c4:	e28d5034 	add	r5, sp, #52	; 0x34
 3c8:	e1a0100e 	mov	r1, lr
 3cc:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 3d0:	e1a0000d 	mov	r0, sp
 3d4:	ebfffffe 	bl	0 <do_undefined_instruction>
	...

000003e0 <software_interrupt>:
 3e0:	e51fd3a4 	ldr	sp, [pc, #-932]	; 44 <_armboot_start>
 3e4:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 3e8:	e24dd088 	sub	sp, sp, #136	; 0x88
 3ec:	e58de000 	str	lr, [sp]
 3f0:	e14fe000 	mrs	lr, SPSR
 3f4:	e58de004 	str	lr, [sp, #4]
 3f8:	e3a0d013 	mov	sp, #19	; 0x13
 3fc:	e169f00d 	msr	SPSR_fc, sp
 400:	e1a0e00f 	mov	lr, pc
 404:	e1b0f00e 	movs	pc, lr
 408:	e24dd048 	sub	sp, sp, #72	; 0x48
 40c:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 410:	e51f23d4 	ldr	r2, [pc, #-980]	; 44 <_armboot_start>
 414:	e2422806 	sub	r2, r2, #393216	; 0x60000
 418:	e2422088 	sub	r2, r2, #136	; 0x88
 41c:	e892000c 	ldmia	r2, {r2, r3}
 420:	e28d0048 	add	r0, sp, #72	; 0x48
 424:	e28d5034 	add	r5, sp, #52	; 0x34
 428:	e1a0100e 	mov	r1, lr
 42c:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 430:	e1a0000d 	mov	r0, sp
 434:	ebfffffe 	bl	0 <do_software_interrupt>
	...

00000440 <prefetch_abort>:
 440:	e51fd404 	ldr	sp, [pc, #-1028]	; 44 <_armboot_start>
 444:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 448:	e24dd088 	sub	sp, sp, #136	; 0x88
 44c:	e58de000 	str	lr, [sp]
 450:	e14fe000 	mrs	lr, SPSR
 454:	e58de004 	str	lr, [sp, #4]
 458:	e3a0d013 	mov	sp, #19	; 0x13
 45c:	e169f00d 	msr	SPSR_fc, sp
 460:	e1a0e00f 	mov	lr, pc
 464:	e1b0f00e 	movs	pc, lr
 468:	e24dd048 	sub	sp, sp, #72	; 0x48
 46c:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 470:	e51f2434 	ldr	r2, [pc, #-1076]	; 44 <_armboot_start>
 474:	e2422806 	sub	r2, r2, #393216	; 0x60000
 478:	e2422088 	sub	r2, r2, #136	; 0x88
 47c:	e892000c 	ldmia	r2, {r2, r3}
 480:	e28d0048 	add	r0, sp, #72	; 0x48
 484:	e28d5034 	add	r5, sp, #52	; 0x34
 488:	e1a0100e 	mov	r1, lr
 48c:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 490:	e1a0000d 	mov	r0, sp
 494:	ebfffffe 	bl	0 <do_prefetch_abort>
	...

000004a0 <data_abort>:
 4a0:	e51fd464 	ldr	sp, [pc, #-1124]	; 44 <_armboot_start>
 4a4:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 4a8:	e24dd088 	sub	sp, sp, #136	; 0x88
 4ac:	e58de000 	str	lr, [sp]
 4b0:	e14fe000 	mrs	lr, SPSR
 4b4:	e58de004 	str	lr, [sp, #4]
 4b8:	e3a0d013 	mov	sp, #19	; 0x13
 4bc:	e169f00d 	msr	SPSR_fc, sp
 4c0:	e1a0e00f 	mov	lr, pc
 4c4:	e1b0f00e 	movs	pc, lr
 4c8:	e24dd048 	sub	sp, sp, #72	; 0x48
 4cc:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 4d0:	e51f2494 	ldr	r2, [pc, #-1172]	; 44 <_armboot_start>
 4d4:	e2422806 	sub	r2, r2, #393216	; 0x60000
 4d8:	e2422088 	sub	r2, r2, #136	; 0x88
 4dc:	e892000c 	ldmia	r2, {r2, r3}
 4e0:	e28d0048 	add	r0, sp, #72	; 0x48
 4e4:	e28d5034 	add	r5, sp, #52	; 0x34
 4e8:	e1a0100e 	mov	r1, lr
 4ec:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 4f0:	e1a0000d 	mov	r0, sp
 4f4:	ebfffffe 	bl	0 <do_data_abort>
	...

00000500 <not_used>:
 500:	e51fd4c4 	ldr	sp, [pc, #-1220]	; 44 <_armboot_start>
 504:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 508:	e24dd088 	sub	sp, sp, #136	; 0x88
 50c:	e58de000 	str	lr, [sp]
 510:	e14fe000 	mrs	lr, SPSR
 514:	e58de004 	str	lr, [sp, #4]
 518:	e3a0d013 	mov	sp, #19	; 0x13
 51c:	e169f00d 	msr	SPSR_fc, sp
 520:	e1a0e00f 	mov	lr, pc
 524:	e1b0f00e 	movs	pc, lr
 528:	e24dd048 	sub	sp, sp, #72	; 0x48
 52c:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 530:	e51f24f4 	ldr	r2, [pc, #-1268]	; 44 <_armboot_start>
 534:	e2422806 	sub	r2, r2, #393216	; 0x60000
 538:	e2422088 	sub	r2, r2, #136	; 0x88
 53c:	e892000c 	ldmia	r2, {r2, r3}
 540:	e28d0048 	add	r0, sp, #72	; 0x48
 544:	e28d5034 	add	r5, sp, #52	; 0x34
 548:	e1a0100e 	mov	r1, lr
 54c:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 550:	e1a0000d 	mov	r0, sp
 554:	ebfffffe 	bl	0 <do_not_used>
	...

00000560 <irq>:
 560:	e51fd524 	ldr	sp, [pc, #-1316]	; 44 <_armboot_start>
 564:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 568:	e24dd088 	sub	sp, sp, #136	; 0x88
 56c:	e58de000 	str	lr, [sp]
 570:	e14fe000 	mrs	lr, SPSR
 574:	e58de004 	str	lr, [sp, #4]
 578:	e3a0d013 	mov	sp, #19	; 0x13
 57c:	e169f00d 	msr	SPSR_fc, sp
 580:	e1a0e00f 	mov	lr, pc
 584:	e1b0f00e 	movs	pc, lr
 588:	e24dd048 	sub	sp, sp, #72	; 0x48
 58c:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 590:	e51f2554 	ldr	r2, [pc, #-1364]	; 44 <_armboot_start>
 594:	e2422806 	sub	r2, r2, #393216	; 0x60000
 598:	e2422088 	sub	r2, r2, #136	; 0x88
 59c:	e892000c 	ldmia	r2, {r2, r3}
 5a0:	e28d0048 	add	r0, sp, #72	; 0x48
 5a4:	e28d5034 	add	r5, sp, #52	; 0x34
 5a8:	e1a0100e 	mov	r1, lr
 5ac:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 5b0:	e1a0000d 	mov	r0, sp
 5b4:	ebfffffe 	bl	0 <do_irq>
	...

000005c0 <fiq>:
 5c0:	e51fd584 	ldr	sp, [pc, #-1412]	; 44 <_armboot_start>
 5c4:	e24dd806 	sub	sp, sp, #393216	; 0x60000
 5c8:	e24dd088 	sub	sp, sp, #136	; 0x88
 5cc:	e58de000 	str	lr, [sp]
 5d0:	e14fe000 	mrs	lr, SPSR
 5d4:	e58de004 	str	lr, [sp, #4]
 5d8:	e3a0d013 	mov	sp, #19	; 0x13
 5dc:	e169f00d 	msr	SPSR_fc, sp
 5e0:	e1a0e00f 	mov	lr, pc
 5e4:	e1b0f00e 	movs	pc, lr
 5e8:	e24dd048 	sub	sp, sp, #72	; 0x48
 5ec:	e88d1fff 	stmia	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 5f0:	e51f25b4 	ldr	r2, [pc, #-1460]	; 44 <_armboot_start>
 5f4:	e2422806 	sub	r2, r2, #393216	; 0x60000
 5f8:	e2422088 	sub	r2, r2, #136	; 0x88
 5fc:	e892000c 	ldmia	r2, {r2, r3}
 600:	e28d0048 	add	r0, sp, #72	; 0x48
 604:	e28d5034 	add	r5, sp, #52	; 0x34
 608:	e1a0100e 	mov	r1, lr
 60c:	e885000f 	stmia	r5, {r0, r1, r2, r3}
 610:	e1a0000d 	mov	r0, sp
 614:	ebfffffe 	bl	0 <do_fiq>

00000618 <DW_STACK_START>:
 618:	00027ffc 	streqd	r7, [r2], -ip

0000061c <SMC_STA>:
 61c:	10100030 	andnes	r0, r0, r0, lsr r0

00000620 <SMC_BOOT>:
 620:	10100034 	andnes	r0, r0, r4, lsr r0
 624:	0000007c 	andeq	r0, r0, ip, ror r0
 628:	00000040 	andeq	r0, r0, r0, asr #32
 62c:	34000004 	strcc	r0, [r0], #-4
 630:	e1a00000 	nop			(mov r0,r0)
 634:	e1a00000 	nop			(mov r0,r0)
 638:	e1a00000 	nop			(mov r0,r0)
 63c:	e1a00000 	nop			(mov r0,r0)
Disassembly of section .debug_line:

00000000 <.debug_line>:
   0:	00000130 	andeq	r0, r0, r0, lsr r1
   4:	001e0002 	andeqs	r0, lr, r2
   8:	01020000 	tsteq	r2, r0
   c:	000d0efb 	streqd	r0, [sp], -fp
  10:	01010101 	tsteq	r1, r1, lsl #2
  14:	01000000 	tsteq	r0, r0
  18:	00010000 	andeq	r0, r1, r0
  1c:	72617473 	rsbvc	r7, r1, #1929379840	; 0x73000000
  20:	00532e74 	subeqs	r2, r3, r4, ror lr
  24:	00000000 	andeq	r0, r0, r0
  28:	00020500 	andeq	r0, r2, r0, lsl #10
  2c:	03000000 	movweq	r0, #0	; 0x0
  30:	2f2f0135 	svccs	0x002f0135
  34:	2f2f2f2f 	svccs	0x002f2f2f
  38:	00c8032f 	sbceq	r0, r8, pc, lsr #6
  3c:	2f2f9008 	svccs	0x002f9008
  40:	3133352f 	teqcc	r3, pc, lsr #10
  44:	2f2f2f2f 	svccs	0x002f2f2f
  48:	2f2f2f36 	svccs	0x002f2f36
  4c:	2f2f2f30 	svccs	0x002f2f30
  50:	2f2f2f31 	svccs	0x002f2f31
  54:	322f2f33 	eorcc	r2, pc, #204	; 0xcc
  58:	302f2f31 	eorcc	r2, pc, r1, lsr pc
  5c:	322f2f2f 	eorcc	r2, pc, #188	; 0xbc
  60:	302f2f32 	eorcc	r2, pc, r2, lsr pc
  64:	32661203 	rsbcc	r1, r6, #805306368	; 0x30000000
  68:	2f2f332f 	svccs	0x002f332f
  6c:	302f2f31 	eorcc	r2, pc, r1, lsr pc
  70:	2f2f312f 	svccs	0x002f312f
  74:	2f2f312f 	svccs	0x002f312f
  78:	322f322f 	eorcc	r3, pc, #-268435454	; 0xf0000002
  7c:	2f2f322f 	svccs	0x002f322f
  80:	2f2f2f2f 	svccs	0x002f2f2f
  84:	2f2f2f2f 	svccs	0x002f2f2f
  88:	2f2f2f2f 	svccs	0x002f2f2f
  8c:	2f2f2f30 	svccs	0x002f2f30
  90:	2f2f2f2f 	svccs	0x002f2f2f
  94:	2f2f2f2f 	svccs	0x002f2f2f
  98:	2f2f2f2f 	svccs	0x002f2f2f
  9c:	32302f30 	eorccs	r2, r0, #192	; 0xc0
  a0:	2f2f2f2f 	svccs	0x002f2f2f
  a4:	2f2f2f2f 	svccs	0x002f2f2f
  a8:	2f2f2f2f 	svccs	0x002f2f2f
  ac:	2f302f2f 	svccs	0x00302f2f
  b0:	2f2f2f2f 	svccs	0x002f2f2f
  b4:	2f2f2f2f 	svccs	0x002f2f2f
  b8:	2f2f2f2f 	svccs	0x002f2f2f
  bc:	2f302f2f 	svccs	0x00302f2f
  c0:	322f3231 	eorcc	r3, pc, #268435459	; 0x10000003
  c4:	2f2f2f2f 	svccs	0x002f2f2f
  c8:	2f2f2f2f 	svccs	0x002f2f2f
  cc:	2f2f2f2f 	svccs	0x002f2f2f
  d0:	2f2f2f2f 	svccs	0x002f2f2f
  d4:	312f302f 	teqcc	pc, pc, lsr #32
  d8:	2f2f2f32 	svccs	0x002f2f32
  dc:	2f2f2f2f 	svccs	0x002f2f2f
  e0:	2f2f2f2f 	svccs	0x002f2f2f
  e4:	2f2f2f2f 	svccs	0x002f2f2f
  e8:	2f302f2f 	svccs	0x00302f2f
  ec:	2f303131 	svccs	0x00303131
  f0:	302f2f2f 	eorcc	r2, pc, pc, lsr #30
  f4:	312f2f2f 	teqcc	pc, pc, lsr #30
  f8:	2f2e1503 	svccs	0x002e1503
  fc:	2f2f332f 	svccs	0x002f332f
 100:	332f2f2f 	teqcc	pc, #188	; 0xbc
 104:	032f2f2f 	teqeq	pc, #188	; 0xbc
 108:	089e00ec 	ldmeqia	lr, {r2, r3, r5, r6, r7}
 10c:	6a59083d 	bvs	1642208 <SMC_BOOT+0x1641be8>
 110:	59083d08 	stmpldb	r8, {r3, r8, sl, fp, ip, sp}
 114:	083d086a 	ldmeqda	sp!, {r1, r3, r5, r6, fp}
 118:	3d086a59 	fstscc	s12, [r8, #-356]
 11c:	086a5908 	stmeqda	sl!, {r3, r8, fp, ip, lr}^
 120:	0359083d 	cmpeq	r9, #3997696	; 0x3d0000
 124:	3d086617 	stccc	6, cr6, [r8, #-92]
 128:	086a5908 	stmeqda	sl!, {r3, r8, fp, ip, lr}^
 12c:	0259083d 	subeqs	r0, r9, #3997696	; 0x3d0000
 130:	0101000e 	tsteq	r1, lr
Disassembly of section .debug_info:

00000000 <.debug_info>:
   0:	0000004f 	andeq	r0, r0, pc, asr #32
   4:	00000002 	andeq	r0, r0, r2
   8:	01040000 	tsteq	r4, r0
	...
  14:	00000630 	andeq	r0, r0, r0, lsr r6
  18:	72617473 	rsbvc	r7, r1, #1929379840	; 0x73000000
  1c:	00532e74 	subeqs	r2, r3, r4, ror lr
  20:	6d6f682f 	stcvsl	8, cr6, [pc, #-188]!
  24:	78662f65 	stmvcda	r6!, {r0, r2, r5, r6, r8, r9, sl, fp, sp}^
  28:	2d752f64 	ldccsl	15, cr2, [r5, #-400]!
  2c:	746f6f62 	strvcbt	r6, [pc], #3938	; 34 <.debug_info+0x34>
  30:	322e312d 	eorcc	r3, lr, #1073741835	; 0x4000000b
  34:	632f302e 	teqvs	pc, #46	; 0x2e
  38:	612f7570 	teqvs	pc, r0, ror r5
  3c:	32396d72 	eorccs	r6, r9, #7296	; 0x1c80
  40:	736a6536 	cmnvc	sl, #226492416	; 0xd800000
  44:	554e4700 	strplb	r4, [lr, #-1792]
  48:	20534120 	subcss	r4, r3, r0, lsr #2
  4c:	37312e32 	undefined
  50:	Address 0x00000050 is out of bounds.

Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
   0:	10001101 	andne	r1, r0, r1, lsl #2
   4:	12011106 	andne	r1, r1, #-2147483647	; 0x80000001
   8:	1b080301 	blne	200c14 <SMC_BOOT+0x2005f4>
   c:	13082508 	movwne	r2, #34056	; 0x8508
  10:	00000005 	andeq	r0, r0, r5
Disassembly of section .debug_aranges:

00000000 <.debug_aranges>:
   0:	0000001c 	andeq	r0, r0, ip, lsl r0
   4:	00000002 	andeq	r0, r0, r2
   8:	00040000 	andeq	r0, r4, r0
	...
  14:	00000630 	andeq	r0, r0, r0, lsr r6
	...
