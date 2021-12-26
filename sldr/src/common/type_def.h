#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

typedef const Const;
typedef void Void;
typedef char Char;
typedef const char C_Char;
typedef unsigned char Byte;/*  8 bits */
typedef unsigned char UChar;
typedef short int Short;
typedef unsigned short UShort;
typedef int  Int;
typedef long int LInt;
typedef unsigned int UInt;
typedef unsigned long int ULInt;
typedef float  Float;
typedef double Double;
typedef unsigned char Boolean ;
typedef Byte  BOOLEAN;

typedef unsigned int        UINT32;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
//typedef unsigned int        DWORD;
typedef int                 BOOL;

typedef unsigned char           U8;
typedef signed char             I8;
typedef unsigned short         U16;
typedef signed short           I16;
typedef unsigned int           U32;
typedef signed int             I32;

#ifdef WIN32_PCI_TEST
typedef unsigned __int64       U64;
typedef   signed __int64       I64;
#else //GCC
typedef unsigned long long int U64;
typedef long long int          I64;
#endif



#endif
