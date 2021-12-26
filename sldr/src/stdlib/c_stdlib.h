#ifndef _C_STDLIB_H_
#define _C_STDLIB_H_

//#define USE_C_STD_LIB

#ifdef USE_C_STD_LIB
//#include <string.h>
//#include <stdio.h>
//#include <assert.h>
//#include <stdlib.h>

#else
#include "x_clib.h"

#define assert(_Expression) x_assert(_Expression, #_Expression, NULL, __LINE__)
#define memcpy(s1, s2, n) x_memcpy(s1, s2, n)
#define memcmp(s1, s2, n) x_memcmp(s1, s2, n)
#define memset(s ,  c, n) x_memset(s ,  c, n)
#define malloc(size) x_malloc(size)
#define free(ptr) x_free(ptr)

#endif

#endif
