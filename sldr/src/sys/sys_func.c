/*
// Author: Jiasheng.chen@celestialsemi.com
// Version0.1 Inital @2008.02.18
*/


#include "sys_func.h"
#include "secu_hw_if.h"
#include "../stdlib/c_stdlib.h"

int SysImgCopyToDDR(void)
{
	int i;
	int ret = 0;
	unsigned char *DDRBuf = NULL;

	SecHWImgOpen();
	DDRBuf = (unsigned char *)APP_IMG_COPY_DDR_ADDR;

	for(i = 0; i < APP_IMG_COPY_SIZE/1024; i++) { 
		ret = SecHWImgRead(APP_IMG_START + (i << 10), 1024 , DDRBuf + (i << 10));
		assert(ret == 1024);
	}

	SecHWImgClose();

	return ret > 0;
}
