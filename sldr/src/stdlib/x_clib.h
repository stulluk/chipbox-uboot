#ifndef _X_CLIB_H_
#define _X_CLIB_H_

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#define min(a, b) ((a) < (b) ? (a):(b))
#define max(a, b) ((a) > (b) ? (a):(b))
int x_assert(int exp, char *exp_str, char *file, int line);
void* x_memcpy(void *s1, const void *s2, int n);
 int  x_memcmp(const void* s1, const void* s2, int n);
void* x_memset(void *s, int c, int n);
void* x_malloc(int size);
void  x_free(void *ptr);

unsigned int __udivsi3(unsigned int num, unsigned int den);

#endif
