#include "TVE0.h"

//#define DEBUG_TVE0
#include <type_def.h>
#if 0
typedef unsigned char U8;
typedef unsigned int  U32;
#endif

#define WriteReg32(a,b) 	(*(volatile unsigned int*)(a) = (b))
#define WriteReg16(a,b) 	(*(volatile unsigned short*)(a) = (b))
#define WriteReg8(a,b) 		(*(volatile unsigned char*)(a) = (b))

#define ReadReg32(a) 		(*(volatile unsigned int*)(a))
#define ReadReg16(a) 		(*(volatile unsigned short*)(a))
#define ReadReg8(a) 		(*(volatile unsigned char*)(a))

#define  TVE0_REG_BASE_ADDR (0x10168000)//???
#define  REG_NUM     (45)//(49)

enum _TVE_VIDEO_FORMAT_
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
	DISP_IS_576I            =  ((3<< 4) | 7), //????
	DISP_IS_480I            =  ((4<< 4) | 7), //???? 
}TVE_VIDEO_FORMAT;

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
#define  TYPE_TOTAL  (10)

#define TVE0_REG_ACCESS_BYTE //For Orion1.3.0/1.3.1

//#define DEBUG_TVE0
//dont change this table
static U8 TVE0_REG[REG_NUM][TYPE_TOTAL] =
{
/*	   address		      ntsc		pal		   PAL_M 		PAL-N		PAL-CN		SECAM		480p		720p		1080i	*/
/*0*/	{ 0x0,        	0x00,		0x00,  		0x00,		0x00,		0x00,		0x00,		0x00,		0x00,		0x00	 	},      	//CVE_SOFT_RESET_VAL
/*1*/ 	{ 0x1,        	0x50,		0x50,		0x50,		0x50,		0x50,		0x50,		0x50,		0x50,		0x50		},      	//CVE_REV_ID_VAL
        //Why by pass the Y output clip?
/*2*/ 	{ 0x2,        	0x11,		0x11,  		0x11,		0x11,		0x11,		0x11,		0x11,		0x11,		0x11	 	},     		//CVE_INPUT_CTL_VAL         (clrbar_mode,yc_delay,uv_order,bypass_yclamp)
/*3*/ 	{ 0x4,        	0x09,		0x09,  		0x09,		0x09,		0x09,		0x09,		0x09,		0x09,		0x09	 	},      	//CVE_OUT_CTL_VAL           (rgb_sync,rgb_setup,comp_yuv,compchgain,cvbs_enable)
/*4*/ 	{ 0x6,        	0x01,		0x01,   	0x01,		0x01,		0x01,		0x01,		0x01,		0x01,		0x01	 	},      	//CVE_SLV_MD_TH_VAL         (slave_thresh,slave_mode)
/*5*/ 	{ 0x7,        	0x00,		0x34,  		0x24,		0x24,		0x34,		0x3C,		0x01,		0x03,		0x02	 	},     		//CVE_VID_STD_VAL           (invert_top,vsync5,secam_mode,pal_mode,trisync_mode,progressive_mode)
/*6*/ 	{ 0x8,         	0x02,		0x02,   	0x02,		0x02,		0x02,		0x02,		0x02,		0x02,		0x04	 	},      	//CVE_NUM_LINES_H_VAL
/*7*/ 	{ 0x9,         	0x0D,		0x71, 		0x0D,		0x71,		0x71,		0x71,		0x0D,		0xEE,		0x65 		},    		//CVE_NUM_LINES_L_VAL
/*8*/ 	{ 0xa,         	21,		      23,  		  21,		  23,		  23,		  23,		  42,		  25,		  20	 	},     		//CVE_FST_VID_LINE_VAL
/*9*/ 	{ 0xb,         	126,		126, 		126,		126,		126,		126,		 126,		  80,		88	 	},    		//CVE_HSYNC_WIDTH_VAL
/*10*/ 	{ 0xd,         	118,		138, 		118,		138,		138,		138,		 118,		 220,		148 		},    		//CVE_BACK_PORCH_VAL
/*11*/ 	{ 0xe,         	0x00,		0x00,   	0x00,		0x00,		0x00,		0x00,		0x00,		   0,		0	 	},      	//CVE_FRONT_PORCH_H_VAL
/*12*/ 	{ 0xf,         	  32,		  24,  		  32,		  24,		  24,		  24,		  32,		  70,		44	 	},     		//CVE_FRONT_PORCH_L_VAL
/*13*/ 	{ 0x10,        	5 ,		5,   		5,		5,		5,		5,		5,		5,		7	 	},      	//CVE_ACTIVELINE_H_VAL
/*14*/ 	{ 0x11,        	160,		160, 		160,		160,		160,		160,		160,		0,		128 		},    		//CVE_ACTIVELINE_L_VAL
/*15*/ 	{ 0x12,        	68,		64,  		68,		64,		68,		64,		68,		8,		8	 	},     		//CVE_BURST_WDITH_VAL
/*16*/ 	{ 0x13,        	22,		26,  		18,		26,		26,		26,		22,		8,		8	 	},     		//CVE_BREEZE_WAY_VAL
/*17*/ 	{ 0x16,        	0x10,		0x15,		0x10,		0x15,		0x10,		0x14,		0x10,		0x10,      	0x10		},      	//CVE_CHROMA_FREQ_3_VAL 
/*18*/ 	{ 0x17,        	0xf8,		0x04,		0xF3,		0x04,		0xFB,		0xE3,		0xf8,		0xf8,      	0xf8		},      	//CVE_CHROMA_FREQ_2_VAL
/*19*/ 	{ 0x18,        	0x3e,		0xc5,		0x77,		0xc5,		0x4A,		0x8E,		0x3e,		0x3e,      	0x3e		},      	//CVE_CHROMA_FREQ_1_VAL
/*20*/ 	{ 0x19,        	0x0f,		0x66,		0xD2,		0x66,		0x23,		0x39,		0x0f,		0x0f,      	0x0f		},      	//CVE_CHROMA_FREQ_0_VAL
/*21*/	//{ 0x1a,		0x00,		0x00,		0x00,		0x00,		0x00,		0x14,		0x00,		0x00,		0x00,		},
/*22*/	//{ 0x1b,		0x00,		0x00,		0x00,		0x00,		0x00,		0x25,		0x00,		0x00,		0x00,		},
/*23*/	//{ 0x1c,		0x00,		0x00,		0x00,		0x00,		0x00,		0xed,		0x00,		0x00,		0x00,		},
/*24*/	//{ 0x1d,		0x00,		0x00,		0x00,		0x00,		0x00,		0x09,		0x00,		0x00,		0x00,		},
/*25*/ 	{ 0x1e,        	0x00,		0x00, 		0x00,		0x00,		0x00,		0x00,		0x00,		0x00,       	0x00 		},       	//CVE_CHROMA_PHASE_VAL
/*26*/ 	{ 0x1f,        	0x00,		0x00,   	0x00,		0x00,		0x00,		0x00,		0x00,		0x00,		0x00,	 	},      	//CVE_TINT_VAL      
/*27*/ 	{ 0x20,        	32,		4,   		4,		4,		4,		4,		4,		36,		36	 	},      	//CVE_NA_WI_CH_CH_CP_VAL    (cphase_rst,chroma_bw_modulated,chroma_bw_yuv,wide_bw_modulated,narrow_bw_yuv)
/*28*/ 	{ 0x21,        	60,		44,  		42,		42,		44,		0,		0,		60,		60	 	},     		//CVE_CB_BURST_AMP_VAL
/*29*/ 	{ 0x22,        	0,		31,  		29,		29,		31,		0, 		0,		0,		0	 	},     		//CVE_CR_BURST_AMP_VAL
/*30*/ 	{ 0x23,        	139,		165, 		139, 		139, 		148,		96,		139,		139,		180 		},    		//CVE_CB_GAIN_VAL            
/*31*/ 	{ 0x24,        	139,		165, 		139,		139,		148,		117,		139,		139,		180 		},    		//CVE_CR_GAIN_VAL            
/*32*/ 	{ 0x27,        	3,		3,   		3,		3,		3,		3,		3,		3,		3 		},      	//CVE_WHITE_LEVEL_H_VAL     
/*33*/ 	{ 0x28,        	32,		32,  		32,		32,		32,		32,		32,		32,		32 		},     		//CVE_WHITE_LEVEL_L_VAL        
/*34*/ 	{ 0x29,        	1,		0,   		0,		0,		0,		0,		0,		0,		0 		},      	//CVE_BLACK_LEVEL_H_VAL      
/*35*/ 	{ 0x2a,        	26,		208, 		251,		251,		251,		251,		251,		251,		208 		},    		//CVE_BLACK_LEVEL_L_VAL
/*36*/ 	{ 0x2b,        	0,		0,   		0,		0,		0,		0,		0,		0,		0 		},      	//CVE_BLANK_LEVEL_H_VAL
/*37*/ 	{ 0x2c,        	240,		251, 		240,		240,		251,		251,		251,		251,		251 		},      	//CVE_BLANK_LEVEL_L_VAL      
/*38*/ 	{ 0x2d,        	0,		0,   		0,		0,		0,		0,		0,		0,		0 		},      	//CVE_VBI_BLANK_LEVEL_H_VAL 
/*39*/ 	{ 0x2e,        	240,		251, 		240,		240,		251,		251,		251,		251,		251 		},    		//CVE_VBI_BLANK_LEVEL_L_VAL  
/*40*/ 	{ 0x2f,        	0,		0,   		0,		0,		0,		0,		0,		0,		0	 	},      	//CVE_CLAMP_LEVEL_H_VAL 
/*41*/ 	{ 0x30,        	0,		0,   		0,		0,		0,		0,		0,		0,		0	 	},      	//CVE_CLAMP_LEVEL_L_VAL
/*42*/ 	{ 0x31,        	16,		16,  		16,		16,		16,		16,		16,		16,		16	 	},     		//CVE_SYNC_LEVEL_VAL
/*43*/ 	{ 0x33,        	1,		1,   		1,		1,		1,		1,		1,		1,		1	 	},      	//CVE_SYNC_HIGH_LEVEL_H_VAL
/*44*/ 	{ 0x34,        	230,		230, 		230,		230,		230,		230,		230,		230,		230 		},    		//CVE_SYNC_HIGH_LEVEL_L_VAL  
/*45*/ 	{ 0x37,        	8,		8,   		8,		8,		8,		8,		8,		8,		8	 	},      	//CVE_NOTCH_EN_WI_FR_VAL    (notch_freq,notch_wide,notch_en)
/*46*/ 	{ 0x38,        	0,		0,   		0,		0,		0,		0,		0,		0,		0	 	},      	//CVE_NO_SH_NO_VAL          (noise_gain,sharpen_gain,noise_filter) 
/*47*/ 	{ 0x39,        	5,		5,   		5,		5,		5,		5,		5,		5,		5	 	},      	//CVE_NOISE_THRESHOLD_VAL   
/*48*/ 	{ 0x3a,        	10,		10,  		10,		10,		10,		10,		10,		10,		10	 	},     		//CVE_SHARPEN_THRESHOLD_VAL 
};

static U8 TVE0_reg_read(U8 addr_base)
{
	U8 data_rd;
	U32 addr ;
	addr = TVE0_REG_BASE_ADDR +(addr_base);
	data_rd = ReadReg8(addr);
    return data_rd;
}

static void TVE0_reg_write(U8 addr_base, U8 data)
{
	U8 data_rd;
	U32 addr ;
	addr = TVE0_REG_BASE_ADDR +(addr_base);

	WriteReg8(addr,data);
	data_rd = ReadReg8(addr);

    if(data_rd != data)
	{
//	  assert(data_rd == data);
  	  printf("TVE0: Reg Write ERROR: addr_base = %02x, wr_data =%08x,rd_data = %08x\n",
  	                                 addr_base,data,data_rd);
	}
}

int InitTVE0( int DispFormat)
{
	int rt = 1;
	static int PreDispFormat = ~0;
	if (PreDispFormat != DispFormat)
	{
		rt  = TVE0SetClk(DispFormat , 0);
		rt  = InitTVE0Raw(DispFormat , 0);
	}
	PreDispFormat = DispFormat;
	return rt;
}	


int TVE0SetClk( int DispFormat , int EnableColorBar)
{
    U8 data = 0;
	int i = 0;
	int VIDEO_TYPE  = TYPE_PAL;
	int IsHD = 0;
	int IsProgressive = 0;
	int rt = 1;

	//Map the Display Format Value to TVE0 Format Value
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
		case DISP_IS_1080I:
			VIDEO_TYPE = TYPE_1080i;
			IsHD = 1;
			IsProgressive = 0;
			break;
		case DISP_IS_1080I_25FPS:
			VIDEO_TYPE = TYPE_1080i;
			IsHD = 1;
			IsProgressive = 0;
			break;
		case DISP_IS_720P: 
			VIDEO_TYPE = TYPE_720p;
			IsHD = 1;
			IsProgressive = 1;
			break;
		case DISP_IS_720P_50FPS:
			VIDEO_TYPE = TYPE_720p;
			IsHD = 1;
			IsProgressive = 1;
			break;
		case DISP_IS_480P:
			VIDEO_TYPE = TYPE_480p;
			IsHD = 0;
			IsProgressive = 1;
			break;
		case DISP_IS_576P:
			VIDEO_TYPE = TYPE_480p; //???
			IsHD = 0;
			IsProgressive = 1;
			break;
		default:
			VIDEO_TYPE = TYPE_PAL;
			IsHD = 0;
			IsProgressive = 0;
			break;
	}
	//Set Clk input frequecy
	rt &= SetTVEncoderClk_TVE0(IsHD, IsProgressive);
	return rt;
}


int InitTVE0Raw( int DispFormat , int EnableColorBar)
{
    U8 data = 0;
	int i = 0;
	int VIDEO_TYPE  = TYPE_PAL;
	int IsHD = 0;
	int IsProgressive = 0;
	int rt = 1;

	printf("DispFormat = 0x%x\n",DispFormat);
	//Map the Display Format Value to TVE0 Format Value
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
		case DISP_IS_1080I:
			VIDEO_TYPE = TYPE_1080i;
			IsHD = 1;
			IsProgressive = 0;
			break;
		case DISP_IS_1080I_25FPS:
			VIDEO_TYPE = TYPE_1080i;
			IsHD = 1;
			IsProgressive = 0;
			break;
		case DISP_IS_720P: 
			VIDEO_TYPE = TYPE_720p;
			IsHD = 1;
			IsProgressive = 1;
			break;
		case DISP_IS_720P_50FPS:
			VIDEO_TYPE = TYPE_720p;
			IsHD = 1;
			IsProgressive = 1;
			break;
		case DISP_IS_480P:
			VIDEO_TYPE = TYPE_480p;
			IsHD = 0;
			IsProgressive = 1;
			break;
		case DISP_IS_576P:
			VIDEO_TYPE = TYPE_480p; //???
			IsHD = 0;
			IsProgressive = 1;
			break;
		default:
			VIDEO_TYPE = TYPE_PAL;
			IsHD = 0;
			IsProgressive = 0;
			break;
	}
	//Set Clk input frequecy
	//rt &= SetTVEncoderClk_TVE0(IsHD, IsProgressive);

	//Hold TVE0 Timing
	TVE0_reg_write(TVE0_REG[0][REG_ADDR],0x1);
#ifdef DEBUG_TVE0	
	printf("[TVE0: INFO]probe REV_ID ..%d\n",TVE0_REG[1][REG_ADDR]);
#endif
	data = TVE0_reg_read(TVE0_REG[1][REG_ADDR]);
#ifdef DEBUG_TVE0
	if(data == 0x50)
		printf("[TVE0: OK]REV_ID OK,data is %d\n",data);
	else 
		printf("[TVE0: WARNING]REV_ID not expected, data is %d\n",data);
	//assert(data == 0x50);
#endif
    for (i=2; i<REG_NUM; i++)
    {
#ifdef DEBUG_TVE0
        printf("TVE0: ADDR=%03x,WriteDATA=%03x\n",TVE0_REG[i][REG_ADDR],TVE0_REG[i][VIDEO_TYPE]);
#endif
        TVE0_reg_write(TVE0_REG[i][REG_ADDR],TVE0_REG[i][VIDEO_TYPE]);
    }
#ifdef DEBUG_TVE0
	printf("[TVE0: INFO]start read registers\n");
    for (i=2; i<REG_NUM; i++)
    {
#ifdef DEBUG_TVE0
        printf("TVE0: ADDR=%03x,ReadDATA=%03x\n",TVE0_REG[i][REG_ADDR],TVE0_reg_read(TVE0_REG[i][REG_ADDR]));
#endif
		if (!(TVE0_REG[i][VIDEO_TYPE] == TVE0_reg_read(TVE0_REG[i][REG_ADDR])))
		{
			printf("TVE0_REG[i][VIDEO_TYPE] != TVE0_reg_read(TVE0_REG[i][REG_ADDR]\n");
			rt = 0;	
			return rt;
		}
    }
#endif
	if (EnableColorBar)
	{
		U8 RegValue = 0;
		RegValue = TVE0_reg_read(0x2);
		RegValue |= (1 << 5);
		TVE0_reg_write(0x2,RegValue);
	}
	TVE0_reg_write(TVE0_REG[0][REG_ADDR],0x0);
	return rt;
}

#if 0
int SetVideoDACOutPutMode_TVE0( int IsDACYPbPr  )
{
	int rt = 0;
	int i = 0;
	U8 TVE0FmtValue = 0x08;
	//rt = SetVideoDACMux(IsDACYPbPr);, //Do not need in Orion1.4
	if (!rt) return rt;
	if (IsDACYPbPr)
	{
		TVE0FmtValue = 0x09;//0x08; YPbPr and CVBs Output at the same time
	}
	else //CVBS
	{
		TVE0FmtValue = 0x09;
	}
	for (i = REG_ADDR + 1; i < TYPE_TOTAL; i++)
	{
		TVE0_REG[3][i] = TVE0FmtValue;
	}
	TVE0_reg_write(0x4, TVE0FmtValue);
	if(TVE0_reg_read(0x4) != TVE0FmtValue){
		printf("SetVideoDACOutPutMode_TVE0 error\n");
	}
	return rt;
}

void DACOutConfig(int Cfg_id)
{
	unsigned int dac0_cfg;
	unsigned int dac1_cfg;
	unsigned int read_data;
	unsigned int write_data;
	
	switch(Cfg_id)
	{
	case 0:   //DAC0: tve1_cvbs DAC1: tve0_r/g/b
		dac0_cfg = 0;
		dac1_cfg = 0;
		break;
	case 1:   //DAC0: tve1_cvbs DAC1: DF_r/g/b
	    dac0_cfg = 0;
		dac1_cfg = 2;
		break;
 	case 2:   //DAC0: tve1_cvbs DAC1: tve1_r/g/b
		dac0_cfg = 0;
		dac1_cfg = 1;		
		break;
	case 4:   //DAC0: tve1_r/g/b DAC1: tve0_r/g/b
    	dac0_cfg = 1;
		dac1_cfg = 0;
		break;
	case 5:   //DAC0: tve1_r/g/b DAC1: DF_r/g/b
		dac0_cfg = 1;
		dac1_cfg = 2;
		break;
 	case 6:   //DAC0: tve1_r/g/b DAC1: tve1_r/g/b
		dac0_cfg = 1;
		dac1_cfg = 1;
		break;
	case 7:   //DAC0: tve1_cvbs DAC1: tve1_r/g/b
		dac0_cfg = 0;
		dac1_cfg = 3;
		break;
	case 8:   //DAC0: tve1_r/g/b DAC1: tve1_r/g/b
		dac0_cfg = 1;
		dac1_cfg = 3;
		break;
	default:
        dac0_cfg = 0;
		dac1_cfg = 0;
		break;
	}

    read_data = pin_mux0_read();
	write_data = (read_data&0xfffe)|dac0_cfg;
	pin_mux0_write(write_data);
	read_data = pin_mux0_read();


	read_data = pin_mux1_read();
	write_data = (read_data&0xfff3)|(dac1_cfg<<2);
	pin_mux1_write(write_data);	
	read_data = pin_mux1_read();

}



//TVE0 Basic Test Case
int TestTVE0( int IsDACYPbPr )
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
	printf("TVE0: Different Display Format Test Start\n");
	
	SetVideoDACOutPutMode_TVE0(IsDACYPbPr); //TVE0 only output RGB

	//Need call pinmux function
	DACOutConfig(0);
	
	for (i = 0; i < sizeof(VideoFmtTable)/ sizeof(int); i++)
	{
		TVE0SetClk(VideoFmtTable[i], 1);
		InitTVE0Raw(VideoFmtTable[i], 1);		
		printf("TVE0: Test Color Bar at %s\n", VideoFmtNameTable[i]);
		printf("TVE0: Press any key to test next format\n");

		getc();
	}
	return i;
}
#endif

