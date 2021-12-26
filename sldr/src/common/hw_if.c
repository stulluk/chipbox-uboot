/*// ---------------------------------------------------------------------------
// FILE NAME        : hw_if.c
// MODULE NAME      : Base Hardware Interface
// AUTHOR           : Jiasheng Chen
// AUTHOR'S EMAIL   : jiasheng.chen@celestialsemi.com
// ---------------------------------------------------------------------------
// [RELEASE HISTORY]                           Last Modified : 07-05-23
// VERSION  DATE       AUTHOR                  DESCRIPTION
// 0.1      07-05-23   jiasheng Chen           Original
// ---------------------------------------------------------------------------
// [DESCRIPTION]
// Base Hardware Interface
// ---------------------------------------------------------------------------
// $Id: 
///////////////////////////////////////////////////////////////////////*/
#ifdef WIN32_PCI_TEST  
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "../PCI/pci_if.h"
#else 
#include "../stdlib/c_stdlib.h" 
#endif
#include "hw_if.h"

//#define TRACE_REG_LOG
#ifdef TRACE_REG_LOG
FILE *fpReg = NULL;
#endif

int HWReadInt   (unsigned int addr, unsigned int   *value)
{
	*value = HW_REG_READ(addr);
	return 1;
}
int HWWriteInt  (unsigned int addr, unsigned int    value)
{
	#ifdef TRACE_REG_LOG
	if (fpReg == NULL) 
	{
		fpReg = fopen("reg.log", "w+");
	}
	if (fpReg != NULL)
	{
		fprintf(fpReg, "REG_WRITE: ADDR(%08x) , Data (%08x)\n", addr, value);
		fflush(fpReg);
	}
	#endif
	HW_REG_WRITE(addr, value);
	return 1;
}

int HWWriteShort(unsigned int addr, unsigned short  value)
{
	*(volatile unsigned short*)(addr) = (value);
	return 1;
}
int HWReadShort (unsigned int addr, unsigned short *value)
{
	(*value) = *(volatile unsigned short*)(addr);
	return 1;
}

int HWWriteByte (unsigned int addr, unsigned char   value)
{
	#ifdef TRACE_REG_LOG
	if (fpReg == NULL) 
	{
		fpReg = fopen("reg.log", "w+");
	}
	if (fpReg != NULL)
	{
		fprintf(fpReg, "REG_WRITE: ADDR(%08x) , Data (%08x)\n", addr, value);
		fflush(fpReg);
	}
	#endif
	*(volatile unsigned char*)(addr) = (value);
	return 1;
}
int HWReadByte  (unsigned int addr, unsigned char  *value)
{
	(*value) = *(volatile unsigned char*)(addr);
	return 1;
}
//Register and System Memory Interface
unsigned int RegRead (unsigned int RegAddr)
{
	unsigned int ReadData = 0;	
	int rt = 0;
	rt = HWReadInt(RegAddr,&ReadData);
	return ReadData; 	
}
void RegWrite (unsigned int RegAddr, unsigned int RegValue)
{
	int rt = 0;
	rt = HWWriteInt(RegAddr,RegValue);
}

int RegWriteCheck(unsigned int RegAddr, unsigned int RegValue)
{
	int rt = 0;
	unsigned int ReadData = 0;
	rt = HWWriteInt(RegAddr,RegValue);
	rt = HWReadInt(RegAddr,&ReadData);
	if ((ReadData) != (RegValue))
	{	
		assert(0);
		//printf("##ERROR, RegWriteCheck: RegAddr(%08x), WriteValue(%08x), ReadValue(%08x)\n",
		//	       RegAddr, RegValue, ReadData);
		return 0;
	}
	return rt;
}

void HWDelay(int cnt)
{
	int i = 0;
	int sum = 0;
	for (i = 0;i < cnt; i++ )
	{
		sum += cnt*cnt / (i + 1);
	}
	return;
}
