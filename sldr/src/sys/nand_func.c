/*

// Author: Jiasheng.chen@celestialsemi.com
// Version0.1 Inital @2008.02.18
*/


#include "nand_func.h"
#include "../stdlib/c_stdlib.h"
#define NAND_FUNC_ENA
enum _NAND_FLASH_CONST_
{
#if 0
	NAND_IO   = 0x34000000,
	NAND_CMD  = 0x34000002,
	NAND_ADDR = 0x34000004,
#else
	NAND_IO   = 0x34800000,
	NAND_CMD  = 0x34800002,
	NAND_ADDR = 0x34800004,
#endif
	BOOT_CFG = 0x10100034,
	NAND_STA  = 0x10100030,
};

static unsigned char large_block =   0;
static unsigned char io_width_16 =   0;
static unsigned char addr_cyc_2  =   0;

static void NANDWait (void)
{
#ifdef NAND_FUNC_ENA
	int i;
	volatile unsigned int *p;

	p = (volatile unsigned int *)NAND_STA;

	do {
		for(i = 0; i < 20; i++) ;
	} while((*p & 0x1) == 0);    
#endif
}

int NANDOpen (void)
{
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
	io_width_16 = 0;
	addr_cyc_2 = 0;
#endif

	if(io_width_16) {
		ps  = (volatile unsigned short *)NAND_CMD;
		*ps = 0xff;
	} else {
		pc  = (volatile unsigned char  *)NAND_CMD;
		*pc = 0xff;
	}

	NANDWait ();
#endif
	return 1;
}

int NANDRead_1K(unsigned int StartAddr, unsigned char *Buf)  /* Warning: assume 1K alignment for StartAddr */
{
	int i;
	int j;
	volatile unsigned short *ps;
	volatile unsigned char  *pc;
	unsigned char  *pBuf = Buf;
#ifdef NAND_FUNC_ENA
	assert((StartAddr & 0x3ff) == 0);

	if(large_block) {
		if(io_width_16) {
			ps  = (volatile unsigned short *)NAND_CMD;
			*ps = 0x00;

			ps  = (volatile unsigned short *)NAND_ADDR;
			*ps = 0x00;
			*ps = (StartAddr >>  9) & 0x03;
			*ps = (StartAddr >> 11) & 0xff;
			*ps = (StartAddr >> 19) & 0xff;
			if(~addr_cyc_2)
				*ps = (StartAddr >> 27) & 0xff;

			ps  = (volatile unsigned short *)NAND_CMD;
			*ps = 0x30;

			NANDWait ();

			ps  = (volatile unsigned short *)NAND_IO;
			for(j = 0; j < 512; j++) {
				*((unsigned short *)pBuf) = *ps;
				pBuf += 2;
			} 

		} else {
			pc  = (volatile unsigned char  *)NAND_CMD;
			*pc = 0x00;

			pc  = (volatile unsigned char  *)NAND_ADDR;
			*pc = 0x00;
			*pc = (StartAddr >>  8) & 0x07;
			*pc = (StartAddr >> 11) & 0xff;
			*pc = (StartAddr >> 19) & 0xff;
			if(~addr_cyc_2)
				*pc = (StartAddr >> 27) & 0xff;

			pc  = (volatile unsigned char  *)NAND_CMD;
			*pc = 0x30;

			NANDWait ();

			pc  = (volatile unsigned char  *)NAND_IO;
			for(j = 0; j < 1024; j++) {
				*pBuf = *pc;
				pBuf++;
			}

		}
	} else {
		if(io_width_16) {
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
				pc  = (volatile unsigned char  *)NAND_CMD;
				*pc = (i & 0x1);

				pc  = (volatile unsigned char  *)NAND_ADDR;
				*pc = 0x00;
				*pc = ((StartAddr >>  9) & 0xff) | (i >> 1);
				*pc = (StartAddr >> 17) & 0xff;
				if(~addr_cyc_2)
					*pc = (StartAddr >> 25) & 0xff;

				NANDWait ();

				pc  = (volatile unsigned char  *)NAND_IO;
				for(j = 0; j < 256; j++) {
					*((unsigned char  *)pBuf) = *pc;
					pBuf++;
				}
			}

		}
	}
#endif
	return 1024;
}

int NANDClose(void)
{
	return 1;
}
