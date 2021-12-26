
/*
 * Author: Jiasheng.chen@celestialsemi.com
 * Version0.1 Inital @2008.02.18
 */
#include "spi_func.h"
#include "../stdlib/c_stdlib.h"
/*#define SPI_FUNC_ENA*/

enum _SPI_CONST_
{
	SPI_CTRLR0 = 0x10173000,
	SPI_CTRLR1 = 0x10173004,
	SPI_SSIENR = 0x10173008,
	SPI_SER    = 0x10173010,
	SPI_BAUDR  = 0x10173014,
	SPI_SR     = 0x10173028,
	SPI_DR     = 0x10173060,
	SPI_BDINIT = 0x00000038,	//@for 55MHz spi internal clock frequency	
	SPI_CR0INI = 0x00000307,
	SPI_CR1INI = 0x000003ff,
	SPI_RX_CNT = 0x10173024,

    	SMC_BOOT   = 0x10100034,
};

#define SPI_REG_WRITE_HALF(addr, data) (*((volatile unsigned short *)(addr)) = (data))
#define SPI_REG_READ_BYTES(addr) (*((volatile unsigned char *)(addr)))

unsigned char addr_len = 1;

int SPIOpen(void)
{
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
#endif
	return 1;
}

int SPIRead_1K(unsigned int StartAddr, unsigned char *Buf)
{
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
#endif

	return 1024;
	
}

int SPIClose(void)
{
#ifdef SPI_FUNC_ENA
	SPI_REG_WRITE_HALF(SPI_SSIENR,0x0);
	SPI_REG_WRITE_HALF(SPI_SER  , 0x0);
#endif
	return 1;
}
