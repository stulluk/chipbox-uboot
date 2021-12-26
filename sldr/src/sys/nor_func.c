/*
// Author: Jiasheng.chen@celestialsemi.com
// Version0.1 Inital @2008.02.18
*/


#include "nor_func.h"
#include "../stdlib/c_stdlib.h"

enum _NOR_FLASH_CONST_ {
	NOR_FLASH_BASE = 0x34000000,
};

int NOROpen (void)
{
	return 1;
}

int NORRead(unsigned int StartAddr, int Size, unsigned char *Buf)
{
	int i = 0;
	unsigned char *Src = 0;
	Src += (NOR_FLASH_BASE + StartAddr);
	if (( (StartAddr &0x3) == (((unsigned int)Buf) & 0x3)) 
	    && (Size >= ((StartAddr &0x3 ) + 4) )) {
		unsigned int *s = 0;
		unsigned int *d = 0;

		for (i = 0; i < (StartAddr &0x3); i++)
			*Buf++ = *Src++;

		s = (unsigned int*)Src;
		d = (unsigned int*)Buf;
		for (; i < Size; i += 4)
			*d++ = *s++;

		Src = (unsigned char*)(s - 1) + 1;
		Buf = (unsigned char*)(d - 1) + 1;		
		i = (i - 4 + 1);
		for (; i < Size; i++)
			*Buf++ = *Src++;
	} else {
		for (i = 0; i < Size; i++)
			*Buf++ = *Src++;
	}

	assert(i == Size);
	return i;
	
}
int NORClose(void)
{
	return 1;
}
