/*
// Author: Jiasheng.chen@celestialsemi.com
// Version0.1 Inital @2008.02.15
*/

#include "x_clib.h"

//#define ENA_PRINT

#ifdef ENA_PRINT
#include <stdio.h>
#endif

int x_assert(int exp, char *exp_str, char *file, int line)
{    
    volatile char *p;

	if (!exp)
	{
		#ifdef ENA_PRINT
			printf("Assert: FILE%s Line %d:\n", file, line);
			printf("      : %s:\n", exp_str);
			getchar();
		#else
		while(1) //Never Stop
		{
            /* Stop Simulation */                    
            p   = (volatile char *)0x34800001;
            *p  = 0xff;

			exp = line * line;
		}
		#endif
	}
	return exp;
}
