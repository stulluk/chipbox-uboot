#ifndef _NOR_FUNC_H_
#define _NOR_FUNC_H_

int NANDOpen (void);
int NANDRead_1K(unsigned int StartAddr, unsigned char *Buf);
int NANDClose(void);

#endif
