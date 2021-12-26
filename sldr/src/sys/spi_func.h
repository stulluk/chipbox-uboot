#ifndef _SPI_FUNC_H_
#define _SPI_FUNC_H_

int SPIOpen(void);
int SPIRead_1K(unsigned int StartAddr, unsigned char *Buf);
int SPIClose(void);

#endif
