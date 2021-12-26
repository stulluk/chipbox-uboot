#ifndef _SECU_HW_IF_H_
#define _SECU_HW_IF_H_

enum _BOOT_MODE_ {
	NOR_BOOT  = 0,
	NAND_BOOT = 1,
	SPI_BOOT  = 2 
};

/* Return SMC_STA Value */
unsigned int SecHWGetBootInfo(void); 
unsigned int SecHWGetBootMode(void);  
/* Need to Read OTP */
int SecHWIsRSACheckEna(void); 
unsigned char *SecHWGetKeyAddr(void);

int SecHWImgOpen(void);
int SecHWImgClose(void);
int SecHWImgRead(unsigned int StartAddr, int Size, unsigned char *Buf);

#endif
