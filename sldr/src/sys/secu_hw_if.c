/*
// Author: Jiasheng.chen@celestialsemi.com
// Modified by Sun He at  Mon May 11 18:03:32 CST 2009
// Version0.1 Inital @2008.02.18
*/

#include "sys_func.h"
#include "secu_hw_if.h"
#include "spi_func.h"
#include "nor_func.h"
#include "nand_func.h"
#include "../stdlib/c_stdlib.h"

enum _SEC_REG_DEF_ {
	SMC_STA  = 0x10100030,
	SMC_BOOT = 0x10100034,
	OTP_CFG  = 0xFFFFF828,
	OTP_SEC_BOOT_BIT  = 0x3,
	OTP_KEY_INDEX_BIT = 0x10,
	PUB_KEY_START     = (0xffff0000 + (48 - 2) * 1024),
};

enum _OTP_CFG_ {
	RSA_CHECK_ENA = 0,
};

#define REG_READ(addr) (*((volatile unsigned int *)(addr)))

/* Return SMC_STA Value */
unsigned int SecHWGetBootInfo(void) 
{
	return REG_READ(SMC_BOOT);
}

unsigned int SecHWGetBootMode(void)
{
	unsigned int BootInfo = 0;
	unsigned int BootMode = 0;
	BootInfo = REG_READ(SMC_BOOT);
	if (BootInfo & 0x1) {
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
	return (((OTPInfo >> OTP_SEC_BOOT_BIT) & 0x1) == RSA_CHECK_ENA);
}

unsigned char *SecHWGetKeyAddr(void)
{
	unsigned int OTPInfo = 0;
	OTPInfo = REG_READ(OTP_CFG);
	OTPInfo = (OTPInfo >> OTP_KEY_INDEX_BIT) & 0x7;

	return (unsigned char *)(PUB_KEY_START + 256 * OTPInfo);
} 

int SecHWImgOpen(void)
{
	int ret = 0;
	unsigned int BootMode = 0;

	BootMode = SecHWGetBootMode();

	if (BootMode == SPI_BOOT) {
		ret = SPIOpen();
	} else if (BootMode == NAND_BOOT) {
		ret = NANDOpen();
	} else {
		ret = NOROpen();
	}

	return ret;
}
int SecHWImgClose(void)
{
	int ret = 0;
	unsigned int BootMode = 0;

	BootMode = SecHWGetBootMode();
	if (BootMode == SPI_BOOT) {
		ret = SPIClose();
	} else if (BootMode == NAND_BOOT) {
		ret = NANDClose();
	}
	else {
		ret = NORClose();
	}

	return ret;
}

int SecHWImgRead(unsigned int StartAddr, int Size, unsigned char *Buf)
{
	int ret = 0;
	unsigned int BootMode = 0;
	unsigned int  direct_copy;
	unsigned char buffer[1024];

	if(Size <= 0)
		return Size;

	/* cross 1K boundary not allowed! */
	assert((StartAddr & (~0x3ff)) == ((StartAddr + Size - 1) & ~0x3ff));    

	direct_copy = ((StartAddr & 0x3ff) == 0) && (Size == 1024);

	BootMode = SecHWGetBootMode();

	if (BootMode == SPI_BOOT) {
		if(direct_copy)
			ret = SPIRead_1K(StartAddr, Buf);
		else
			ret = SPIRead_1K(StartAddr & (~0x3ff), &buffer[0]);
	} else if (BootMode == NAND_BOOT) {
		if(direct_copy)
			ret = NANDRead_1K(StartAddr, Buf);
		else
			ret = NANDRead_1K(StartAddr & (~0x3ff), &buffer[0]);
	} else {
		if(direct_copy)
			ret = NORRead(StartAddr, 1024, Buf);
		else
			ret = NORRead(StartAddr & (~0x3ff), 1024, &buffer[0]);
	}

	if(!direct_copy)
		memcpy(Buf, &buffer[StartAddr & 0x3ff], Size);

	return Size;
}

