#ifndef _HW_IF_H_
#define _HW_IF_H_

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif

int  InitHWPlatForm ();
void ResetHWPlatForm();
int  DeInitHWPlatForm();

int HWReadInt   (unsigned int addr, unsigned int   *value);
int HWWriteInt  (unsigned int addr, unsigned int    value);

int HWWriteShort(unsigned int addr, unsigned short  value);
int HWReadShort (unsigned int addr, unsigned short *value);

int HWWriteByte (unsigned int addr, unsigned char   value);
int HWReadByte  (unsigned int addr, unsigned char  *value);

//Burst Read/Write
int HWWriteIntData(unsigned int addr,unsigned int *data, unsigned int len);
int HWReadIntData(unsigned int addr,unsigned int *data, unsigned int len);

//Register and System Memory Interface
unsigned int RegRead (unsigned int RegAddr);
void RegWrite (unsigned int RegAddr, unsigned int RegValue);

int RegWriteCheck(unsigned int RegAddr, unsigned int RegValue);
int RegWriteCheckMask(unsigned int RegAddr, unsigned int RegValue, unsigned int Mask);

UINT32 ReadReg(UINT32 Addr, UINT32 *Value);
UINT32 WriteReg(UINT32 Addr, UINT32 Value);
UInt CheckReg(UINT32 nADDR, UINT32 nValueExp);


#ifndef HW_REG_WRITE
#define HW_REG_WRITE( byte_addr, Data) *(volatile unsigned int*)(byte_addr) = (Data)
#endif
#ifndef HW_REG_READ
#define HW_REG_READ( ByteAddr )      (*(volatile unsigned int*)(ByteAddr))
#endif



void HWDelay(int cnt);


#ifdef __cplusplus
}
#endif

#endif
