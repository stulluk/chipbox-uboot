

#ifdef WIN32_PCI_TEST  
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#else 
#include "../stdlib/c_stdlib.h" 
#endif


#include "../common/hw_if.h"
#include "../common/clock.h"

#include "cve5.h"
//#define DEBUG_CVE5

enum _DF_VIDEO_FORMAT_
{
	NOT_STANDARD_VIDEO_MODE = -1,
		DISP_IS_PAL 	        =  3, 
		DISP_IS_NTSC		    =  0,
		DISP_IS_1080I		    =  2, 
		DISP_IS_1080I_25FPS     =  4, 
		DISP_IS_720P	 	    =  1, 
		DISP_IS_720P_50FPS	    =  5, 
		DISP_IS_USR_DEF         =  7,
	    DISP_IS_480P            =  ((1<< 4) | 7), 
	    DISP_IS_576P            =  ((2<< 4) | 7), 
}DF_VIDEO_FORMAT;



#define  CVE5_REG_BASE_ADDR (0x10160000)//???
#define  REG_NUM     (49)

#define  REG_ADDR    (0) 
#define  TYPE_NTSC   (1)
#define  TYPE_PAL    (2)
#define  TYPE_PAL_M  (3)
#define  TYPE_PAL_N  (4)
#define  TYPE_PAL_CN (5)
#define  TYPE_SECAM  (6)
#define  TYPE_480p   (7)
#define  TYPE_720p   (8)
#define  TYPE_1080i  (9)
#define  TYPE_TOTAL  (3)




#define CVE5_REG_ACCESS_BYTE //For Orion1.3.0/1.3.1

//dont change this table
static U8 CVE5_REG[REG_NUM][TYPE_TOTAL] =
{
/*	   address		      ntsc		pal		
/*0*/	{ 0x0,        	0x00,		0x00,  },
/*1*/ 	{ 0x1,        	0x50,		0x50,},
        //Why by pass the Y output cl},
/*2*/ 	{ 0x2,        	0x11,		0x11,},
/*3*/ 	{ 0x4,        	0x09,		0x09,},
/*4*/ 	{ 0x6,        	0x01,		0x01,},
/*5*/ 	{ 0x7,        	0x00,		0x34,},
/*6*/ 	{ 0x8,         	0x02,		0x02,},
/*7*/ 	{ 0x9,         	0x0D,		0x71,},
/*8*/ 	{ 0xa,         	21,		    23,},
/*9*/ 	{ 0xb,         	126,		 126,},
/*10*/ 	{ 0xd,         	118,		 138,},
/*11*/ 	{ 0xe,         	0x00,		0x00,},
/*12*/ 	{ 0xf,         	  32,		  24,},
/*13*/ 	{ 0x10,        	5 ,		     5,},
/*14*/ 	{ 0x11,        	160,		 160,},
/*15*/ 	{ 0x12,        	68,	    	64,},
/*16*/ 	{ 0x13,        	22,		    26,},
/*17*/ 	{ 0x16,        	0x10,		0x15,},
/*18*/ 	{ 0x17,        	0xf8,		0x04,},
/*19*/ 	{ 0x18,        	0x3e,		0xc5,},
/*20*/ 	{ 0x19,        	0x0f,		0x66,},
/*21*/	{ 0x1a,		      0x00,		0x00,},
/*22*/	{ 0x1b,		      0x00,		0x00,},
/*23*/	{ 0x1c,		      0x00,		0x00,},
/*24*/	{ 0x1d,		      0x00,		0x00,},
/*25*/ 	{ 0x1e,        	0x00,		0x00,},
/*26*/ 	{ 0x1f,        	0x00,		0x00,},
/*27*/ 	{ 0x20,        	32,		     4,},
/*28*/ 	{ 0x21,        	60,		    44,},
/*29*/ 	{ 0x22,        	0,		    31,},
/*30*/ 	{ 0x23,        	139,		 165,},
/*31*/ 	{ 0x24,        	139,		 165,},
/*32*/ 	{ 0x27,        	3,		     3,},
/*33*/ 	{ 0x28,        	32,		    32,},
/*34*/ 	{ 0x29,        	1,		     0,},
/*35*/ 	{ 0x2a,        	26,		   208,},
/*36*/ 	{ 0x2b,        	0,		     0,},
/*37*/ 	{ 0x2c,        	240,		 251,},
/*38*/ 	{ 0x2d,        	0,		     0,},
/*39*/ 	{ 0x2e,        	240,		 251,},
/*40*/ 	{ 0x2f,        	0,	    	 0,},
/*41*/ 	{ 0x30,        	0,		     0,},
/*42*/ 	{ 0x31,        	16,		    16,},
/*43*/ 	{ 0x33,        	1,		     1,},
/*44*/ 	{ 0x34,        	230,		 230,},
/*45*/ 	{ 0x37,        	8,	       8,},
/*46*/ 	{ 0x38,        	0,		     0,},
/*47*/ 	{ 0x39,        	5,		     5,},
/*48*/ 	{ 0x3a,        	10,		    10,},
};

static U8 cve5_reg_read(U8 addr_base)
{
	U8 data_rd;
	U32 addr ;
	addr = CVE5_REG_BASE_ADDR +(addr_base);
	HWReadByte(addr,&data_rd);
    return data_rd;
}

static void cve5_reg_write(U8 addr_base, U8 data)
{
	U8 data_rd;
	U32 addr ;
	addr = CVE5_REG_BASE_ADDR +(addr_base);

    HWWriteByte(addr,data);
	HWReadByte(addr,&data_rd);

    if(data_rd != data)
	{
	  assert(data_rd == data);
  	  //printf("CVE5: Reg Write ERROR: addr_base = %02x, wr_data =%08x,rd_data = %08x\n",
  	  //                               addr_base,data,data_rd);
	}
}


int InitCVE5( int DispFormat)
{
	int rt = 1;
	static int PreDispFormat = ~0;
	if (PreDispFormat != DispFormat)
	{
		rt  = InitCVE5Raw(DispFormat , 0);
	}
	PreDispFormat = DispFormat;
	return rt;
}	
int InitCVE5Raw( int DispFormat , int EnableColorBar)
{
    U8 data = 0;
	int i = 0;
	int VIDEO_TYPE  = TYPE_PAL;
	int IsHD = 0;
	int IsProgressive = 0;
	int rt = 1;

	//Map the Display Format Value to CVE5 Format Value
	switch (DispFormat)
	{
		case DISP_IS_PAL:
			VIDEO_TYPE = TYPE_PAL;
			IsHD = 0;
			IsProgressive = 0;
			break;
		case DISP_IS_NTSC:
			VIDEO_TYPE = TYPE_NTSC;
			IsHD = 0;
			IsProgressive = 0;
			break;
		default:
			VIDEO_TYPE = TYPE_PAL;
			IsHD = 0;
			IsProgressive = 0;
			break;
	}
	//Set Clk input frequecy
	rt &= SetTVEncoderClk(IsHD, IsProgressive);

	//Hold CVE5 Timing
	cve5_reg_write(CVE5_REG[0][REG_ADDR],0x1);
	data = cve5_reg_read(CVE5_REG[1][REG_ADDR]);
    for (i=2; i<REG_NUM; i++)
    {
        cve5_reg_write(CVE5_REG[i][REG_ADDR],CVE5_REG[i][VIDEO_TYPE]);
    }
	if (EnableColorBar)
	{
		U8 RegValue = 0;
		RegValue = cve5_reg_read(0x2);
		RegValue |= (1 << 5);
		cve5_reg_write(0x2,RegValue);
	}
	cve5_reg_write(CVE5_REG[0][REG_ADDR],0x0);
	return rt;
}
#ifdef WIN32_PCI_TEST
int SetVideoDACOutPutMode( int IsDACYPbPr  )
{
	int rt = 0;
	int i = 0;
	U8 CVE5FmtValue = 0x08;
	//rt = SetVideoDACMux(IsDACYPbPr);, //Do not need in Orion1.4
	if (!rt) return rt;
	if (IsDACYPbPr)
	{
		CVE5FmtValue = 0x09;//0x08; YPbPr and CVBs Output at the same time
	}
	else //CVBS
	{
		CVE5FmtValue = 0x09;
	}
	for (i = REG_ADDR + 1; i < TYPE_TOTAL; i++)
	{
		CVE5_REG[3][i] = CVE5FmtValue;
	}
	cve5_reg_write(0x4, CVE5FmtValue);
	assert(cve5_reg_read(0x4) == CVE5FmtValue);
	return rt;

}

//CVE5 Basic Test Case
int TestCVE5( int IsDACYPbPr )
{
	int VideoFmtTable[] = 
	{
		DISP_IS_PAL 	        , 
		DISP_IS_NTSC		    ,
		DISP_IS_1080I		    ,
		DISP_IS_1080I_25FPS     , 
		DISP_IS_720P	 	    , 
		DISP_IS_720P_50FPS	    , 
	    DISP_IS_480P            , 
	    DISP_IS_576P            , 
	};
	char *VideoFmtNameTable[]=
	{
		"DISP_IS_PAL 	    "    , 
		"DISP_IS_NTSC		"    ,
		"DISP_IS_1080I		"    ,
		"DISP_IS_1080I_25FPS"    , 
		"DISP_IS_720P	 	"    , 
		"DISP_IS_720P_50FPS	"    , 
	    "DISP_IS_480P       "    , 
	    "DISP_IS_576P       "    , 		
	};
	int i = 0;
	//Test all the supported display format by using internal color bar
	printf("CVE5: Different Display Format Test Start\n");
	
	SetVideoDACOutPutMode(IsDACYPbPr);
	
	for (i = 0; i < sizeof(VideoFmtTable)/ sizeof(int); i++)
	{
		InitCVE5Raw(VideoFmtTable[i], 1);
		printf("CVE5: Test Color Bar at %s\n", VideoFmtNameTable[i]);
		printf("CVE5: Press any key to test next format\n");
		getchar();
	}
	return i;
}
#endif
