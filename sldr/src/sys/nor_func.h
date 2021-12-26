#ifndef _NOR_FUNC_H_
#define _NOR_FUNC_H_

int NOROpen (void);
int NORRead(unsigned int StartAddr, int Size, unsigned char *Buf);
int NORClose(void);

#endif
