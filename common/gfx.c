
#include <common.h>
#include <command.h>
#include <asm/byteorder.h>
#include <malloc.h>

#ifdef CFG_GFX
//#include "TVE1.h"
#include "gfx.h"

#define DF_GFX_TRUE_COLOR_SUPPORT

#define WriteReg32(a,b) 	(*(volatile unsigned int*)(a) = (b))
#define WriteReg16(a,b) 	(*(volatile unsigned short*)(a) = (b))
#define WriteReg8(a,b) 		(*(volatile unsigned char*)(a) = (b))

#define ReadReg32(a) 		(*(volatile unsigned int*)(a))
#define ReadReg16(a) 		(*(volatile unsigned short*)(a))
#define ReadReg8(a) 		(*(volatile unsigned char*)(a))

#ifdef CONFIG_ARCH_CSM1201
static int gfxid = 0;
static int videoid = 0;
static int outifid = 1;
static int compid = 1;

static df_reg dfreg;

static df_out_if_timing outputFomatInfo[] = 
{
	{1,  720, 480, 30, 27000,0,1,1,1,1,0,0,1,0,0,1, 858,138, 525,21, 261,284, 524,3,265, 19, 81, 3, 6,265,268, DISP_YUV_NTSC,            "DISP_YUV_NTSC"},
	{1, 1280, 720, 60, 74250,1,0,0,0,0,0,0,0,1,1,1,1650,370, 750,25, 745, 25, 745,0,  0,110,150, 0, 5,  0,  5, DISP_YUV_720P_60FPS,      "DISP_YUV_720P_60FPS"},
	{1, 1280, 720, 50, 74250,1,0,0,0,0,0,0,0,1,1,1,1980,700, 750,25, 745, 25, 745,0,  0,440,480, 0, 5,  0,  5, DISP_YUV_720P_50FPS,      "DISP_YUV_720P_50FPS"},
	{1, 1920,1080, 30, 74250,1,1,0,0,1,0,0,1,1,1,1,2200,280,1125,20, 560,583,1123,0,563, 88,132, 0, 5,562,567, DISP_YUV_1080I_30FPS,     "DISP_YUV_1080I_30FPS"},
	{1, 1920,1080, 25, 74250,1,1,0,0,1,0,0,1,1,1,1,2640,720,1125,20, 560,583,1123,0,563,528,572, 0, 5,562,567, DISP_YUV_1080I_25FPS,     "DISP_YUV_1080I_25FPS"},
	{1,  720, 576, 50, 27000,0,1,1,1,0,1,0,1,0,0,1, 864,144, 625,22, 310,335, 623,0,312, 12, 75, 0, 3,312,315, DISP_YUV_PAL,             "DISP_YUV_PAL"},
	{1,  720, 480, 60, 27000,0,0,0,0,0,0,0,0,0,0,1, 858,138, 525,42, 522, 42, 522,0,  0, 16, 78, 6,12,  6, 12, DISP_YUV_480P,            "DISP_YUV_480P"},
	{1,  720, 576, 50, 27000,0,0,0,0,0,0,0,0,0,0,1 ,864,144, 625,44, 620, 44, 620,0,  0, 12, 76, 0, 5,  0,  5, DISP_YUV_576P,            "DISP_YUV_576P"},
	{1, 1920,1080, 30, 74250,1,0,0,0,0,0,0,0,1,1,1,2200,280,1125,41,1121, 41,1121,0,  0, 88,132, 0, 5,  0,  5, DISP_YUV_1080P_30FPS,     "DISP_YUV_1080P_30FPS"},
	{1, 1920,1080, 25, 74250,1,0,0,0,0,0,0,0,1,1,1,2640,720,1125,41,1121, 41,1121,0,  0,528,572, 0, 5,  0,  5, DISP_YUV_1080P_25FPS,     "DISP_YUV_1080P_25FPS"},
	{1, 1920,1080, 60,148500,1,0,0,0,0,0,0,0,1,1,1,2200,280,1125,41,1121, 41,1121,0,  0, 88,132, 0, 5,  0,  5, DISP_YUV_1080P_60FPS,     "DISP_YUV_1080P_60FPS"},
	{1, 1920,1080, 50,148500,1,0,0,0,0,0,0,0,1,1,1,2640,720,1125,41,1121, 41,1121,0,  0,528,572, 0, 5,  0,  5, DISP_YUV_1080P_50FPS,     "DISP_YUV_1080P_50FPS"},
	{0,  640, 480, 60, 25170,0,0,0,0,0,0,0,0,0,0,1, 800,160, 524,44,   0, 44,   0,0,  0, 16,112,11,13, 11, 13, DISP_RGB_640X480_60FPS,   "DISP_RGB_640X480_60FPS"},
	{0,  800, 600, 60, 40000,0,0,0,0,0,0,0,0,0,0,1,1056,256, 628,28,   0, 28,   0,0,  0, 40,168, 1, 5,  1,  5, DISP_RGB_800X600_60FPS,   "DISP_RGB_800X600_60FPS"},
	{0,  800, 600, 72, 50000,0,0,0,0,0,0,0,0,0,0,1,1040,240, 666,64, 664, 64, 664,0,  0, 56,176,35,41, 35, 41, DISP_RGB_800X600_72FPS,   "DISP_RGB_800X600_72FPS"},
	{0, 1024, 768, 60, 65000,1,0,0,0,0,0,0,0,0,0,1,1344,320, 806,38,   0, 38,   0,0,  0, 24,160, 3, 9,  3,  9, DISP_RGB_1024X768_60FPS,  "DISP_RGB_1024X768_60FPS"},
	{0, 1280,1024, 50, 89375,1,0,0,0,0,0,0,0,0,0,1,1696,416,1054,30,   0, 30,   0,0,  0, 72,208, 1, 4,  1,  4, DISP_RGB_1280X1024_50FPS, "DISP_RGB_1280X1024_50FPS"},
	{0, 1600,1000, 60,133125,1,0,0,0,0,0,0,0,0,0,1,2144,544,1035,35,   0, 35,   0,0,  0, 88,256, 1, 4,  1,  4, DISP_RGB_1600X1000_60FPS, "DISP_RGB_1600X1000_60FPS"},
	{0, 1280,1024, 60, 89000,1,0,0,0,0,0,0,0,0,0,1,1408,128,1054,30,   0, 30,   0,0,  0, 32,64, 1, 4,  1,  4, DISP_RGB_1280X1024_60FPS, "DISP_RGB_1280X1024_60FPS"},
};

enum  _DF_REGISTER_BANK_
{
	DISP_REG_BASE             = 0x41800000,	
	DISP_REG_COMMON_BASE      = DISP_REG_BASE + (0x00 << 2),//0x0
	DISP_REG_GFX1_BASE        = DISP_REG_BASE + (0x10 << 2),//0x40
	DISP_REG_GFX2_BASE        = DISP_REG_BASE + (0x20 << 2),//0x80
	DISP_REG_VIDEO1_BASE      = DISP_REG_BASE + (0x30 << 2),//0xc0
	DISP_REG_VIDEO2_BASE      = DISP_REG_BASE + (0x50 << 2),//0x140
	DISP_REG_COMP1_BASE       = DISP_REG_BASE + (0x70 << 2),//0x1c0
	DISP_REG_COMP2_BASE       = DISP_REG_BASE + (0x80 << 2),//0x200
	DISP_REG_HD2SD_BASE       = DISP_REG_BASE + (0x90 << 2),//0x240
	DISP_REG_OUTIF1_BASE      = DISP_REG_BASE + (0xA0 << 2),//0x280
	DISP_REG_OUTIF2_BASE      = DISP_REG_BASE + (0xB0 << 2),//0x2c0
	/***************COMMON Register******************/
	DISP_UPDATE_REG           = ( DISP_REG_COMMON_BASE ) + ( 0 << 2),
	DISP_STATUS               = ( DISP_REG_COMMON_BASE ) + ( 1 << 2),	
	DISP_OUTIF1_INT_CLEAR     = ( DISP_REG_COMMON_BASE ) + ( 2 << 2),
	DISP_OUTIF2_INT_CLEAR     = ( DISP_REG_COMMON_BASE ) + ( 3 << 2),
	DISP_OUTIF1_ERR_CLEAR     = ( DISP_REG_COMMON_BASE ) + ( 4 << 2),
	DISP_OUTIF2_ERR_CLEAR     = ( DISP_REG_COMMON_BASE ) + ( 5 << 2),	
	DISP_SCA_COEF_IDX         = ( DISP_REG_COMMON_BASE ) + ( 6 << 2),
	DISP_SCA_COEF_DATA        = ( DISP_REG_COMMON_BASE ) + ( 7 << 2),
	/***************GFX1 Register******************/
	DISP_GFX1_CTRL            = ( DISP_REG_GFX1_BASE ) + ( 0 << 2),
 	DISP_GFX1_FORMAT          = ( DISP_REG_GFX1_BASE ) + ( 1 << 2),
 	DISP_GFX1_ALPHA_CTRL       = ( DISP_REG_GFX1_BASE ) + ( 2 << 2),
 	DISP_GFX1_KEY_RED         = ( DISP_REG_GFX1_BASE ) + ( 3 << 2), 
 	DISP_GFX1_KEY_BLUE        = ( DISP_REG_GFX1_BASE ) + ( 4 << 2), 
 	DISP_GFX1_KEY_GREEN       = ( DISP_REG_GFX1_BASE ) + ( 5 << 2), 
 	DISP_GFX1_BUF_START       = ( DISP_REG_GFX1_BASE ) + ( 6 << 2), 
 	DISP_GFX1_LINE_PITCH      = ( DISP_REG_GFX1_BASE ) + ( 7 << 2), 
 	DISP_GFX1_X_POSITON       = ( DISP_REG_GFX1_BASE ) + ( 8 << 2), 
 	DISP_GFX1_Y_POSITON       = ( DISP_REG_GFX1_BASE ) + ( 9 << 2), 
 	DISP_GFX1_SCL_X_POSITON   = ( DISP_REG_GFX1_BASE ) + (10 << 2), 
 	DISP_GFX1_CLUT_ADDR       = ( DISP_REG_GFX1_BASE ) + (11 << 2),
 	DISP_GFX1_CLUT_DATA       = ( DISP_REG_GFX1_BASE ) + (12 << 2), 
 	/***************GFX2 Register******************/
	DISP_GFX2_CTRL            = ( DISP_REG_GFX2_BASE ) + ( 0 << 2),
	DISP_GFX2_FORMAT          = ( DISP_REG_GFX2_BASE ) + ( 1 << 2),
	DISP_GFX2_ALPHA_CTRL      = ( DISP_REG_GFX2_BASE ) + ( 2 << 2),
	DISP_GFX2_KEY_RED         = ( DISP_REG_GFX2_BASE ) + ( 3 << 2), 
	DISP_GFX2_KEY_BLUE        = ( DISP_REG_GFX2_BASE ) + ( 4 << 2), 
	DISP_GFX2_KEY_GREEN       = ( DISP_REG_GFX2_BASE ) + ( 5 << 2), 
	DISP_GFX2_BUF_START       = ( DISP_REG_GFX2_BASE ) + ( 6 << 2), 
	DISP_GFX2_LINE_PITCH      = ( DISP_REG_GFX2_BASE ) + ( 7 << 2), 
	DISP_GFX2_X_POSITON       = ( DISP_REG_GFX2_BASE ) + ( 8 << 2), 
	DISP_GFX2_Y_POSITON       = ( DISP_REG_GFX2_BASE ) + ( 9 << 2), 
	DISP_GFX2_SCL_X_POSITON   = ( DISP_REG_GFX2_BASE ) + (10 << 2), 
	DISP_GFX2_CLUT_ADDR       = ( DISP_REG_GFX2_BASE ) + (11 << 2),
	DISP_GFX2_CLUT_DATA       = ( DISP_REG_GFX2_BASE ) + (12 << 2),
	/***************VIDEO1 Register******************/
	DISP_VIDEO1_CTRL          = ( DISP_REG_VIDEO1_BASE ) + ( 0 << 2),
	DISP_VIDEO1_ALPHA_CTRL    = ( DISP_REG_VIDEO1_BASE ) + ( 1 << 2),
 	DISP_VIDEO1_KEY_LUMA      = ( DISP_REG_VIDEO1_BASE ) + ( 2 << 2), 
 	DISP_VIDEO1_X_POSITON     = ( DISP_REG_VIDEO1_BASE ) + ( 3 << 2), 
 	DISP_VIDEO1_Y_POSITON     = ( DISP_REG_VIDEO1_BASE ) + ( 4 << 2), 
 	DISP_VIDEO1_SRC_X_CROP    = ( DISP_REG_VIDEO1_BASE ) + ( 5 << 2),     
  	DISP_VIDEO1_SRC_Y_CROP    = ( DISP_REG_VIDEO1_BASE ) + ( 6 << 2),
 	DISP_VIDEO1_CM_COEF0_012  = ( DISP_REG_VIDEO1_BASE ) + ( 7 << 2),
 	DISP_VIDEO1_CM_COEF0_3    = ( DISP_REG_VIDEO1_BASE ) + ( 8 << 2),
 	DISP_VIDEO1_CM_COEF1_012  = ( DISP_REG_VIDEO1_BASE ) + ( 9 << 2),
 	DISP_VIDEO1_CM_COEF1_3    = ( DISP_REG_VIDEO1_BASE ) + (10 << 2),
 	DISP_VIDEO1_CM_COEF2_012  = ( DISP_REG_VIDEO1_BASE ) + (11 << 2),
 	DISP_VIDEO1_CM_COEF2_3    = ( DISP_REG_VIDEO1_BASE ) + (12 << 2),
	/***************Video 1 STATUS(Command Info) Register******************/
 	DISP_VIDEO1_STA_IMG_SIZE   = ( DISP_REG_VIDEO1_BASE ) + (13 << 2),
 	DISP_VIDEO1_STA_FRM_INFO   = ( DISP_REG_VIDEO1_BASE ) + (14 << 2),
	DISP_VIDEO1_STA_Y_TOPADDR  = ( DISP_REG_VIDEO1_BASE ) + (15 << 2),
	DISP_VIDEO1_STA_Y_BOTADDR  = ( DISP_REG_VIDEO1_BASE ) + (16 << 2),
	DISP_VIDEO1_STA_C_TOPADDR  = ( DISP_REG_VIDEO1_BASE ) + (17 << 2),
	DISP_VIDEO1_STA_C_BOTADDR  = ( DISP_REG_VIDEO1_BASE ) + (18 << 2),
	DISP_VIDEO1_STA_DISP_NUM   = ( DISP_REG_VIDEO1_BASE ) + (19 << 2),
	/***************VIDEO2 Register******************/
	DISP_VIDEO2_CTRL           = ( DISP_REG_VIDEO2_BASE ) + ( 0 << 2), 	
 	DISP_VIDEO2_ALPHA_CTRL     = ( DISP_REG_VIDEO2_BASE ) + ( 1 << 2),
 	DISP_VIDEO2_KEY_LUMA       = ( DISP_REG_VIDEO2_BASE ) + ( 2 << 2),
 	DISP_VIDEO2_X_POSITON      = ( DISP_REG_VIDEO2_BASE ) + ( 3 << 2), 
 	DISP_VIDEO2_Y_POSITON      = ( DISP_REG_VIDEO2_BASE ) + ( 4 << 2), 
 	DISP_VIDEO2_SRC_X_CROP     = ( DISP_REG_VIDEO2_BASE ) + ( 5 << 2),
 	DISP_VIDEO2_SRC_Y_CROP     = ( DISP_REG_VIDEO2_BASE ) + ( 6 << 2),
 	DISP_VIDEO2_CM_COEF0_012   = ( DISP_REG_VIDEO2_BASE ) + ( 7 << 2),
 	DISP_VIDEO2_CM_COEF0_3     = ( DISP_REG_VIDEO2_BASE ) + ( 8 << 2),
 	DISP_VIDEO2_CM_COEF1_012   = ( DISP_REG_VIDEO2_BASE ) + ( 9 << 2),
 	DISP_VIDEO2_CM_COEF1_3     = ( DISP_REG_VIDEO2_BASE ) + (10 << 2),
 	DISP_VIDEO2_CM_COEF2_012   = ( DISP_REG_VIDEO2_BASE ) + (11 << 2),
 	DISP_VIDEO2_CM_COEF2_3     = ( DISP_REG_VIDEO2_BASE ) + (12 << 2),
 	DISP_VIDEO2_STA_IMG_SIZE   = ( DISP_REG_VIDEO2_BASE ) + (13 << 2),
	DISP_VIDEO2_STA_FRM_INFO   = ( DISP_REG_VIDEO2_BASE ) + (14 << 2),
	DISP_VIDEO2_STA_Y_TOPADDR  = ( DISP_REG_VIDEO2_BASE ) + (15 << 2),
	DISP_VIDEO2_STA_Y_BOTADDR  = ( DISP_REG_VIDEO2_BASE ) + (16 << 2),
	DISP_VIDEO2_STA_C_TOPADDR  = ( DISP_REG_VIDEO2_BASE ) + (17 << 2),
	DISP_VIDEO2_STA_C_BOTADDR  = ( DISP_REG_VIDEO2_BASE ) + (18 << 2),	
	DISP_VIDEO2_STA_DISP_NUM   = ( DISP_REG_VIDEO2_BASE ) + (19 << 2),
 	/***************COMPOSITOR1 Register******************/
 	DISP_COMP1_CLIP           = ( DISP_REG_COMP1_BASE ) + ( 0 << 2),
 	DISP_COMP1_BACK_GROUND    = ( DISP_REG_COMP1_BASE ) + ( 1 << 2),
 	DISP_COMP1_Z_ORDER        = ( DISP_REG_COMP1_BASE ) + ( 2 << 2),
 	/***************COMPOSITOR2 Register******************/
 	DISP_COMP2_CLIP           = ( DISP_REG_COMP2_BASE ) + ( 0 << 2),
 	DISP_COMP2_BACK_GROUND    = ( DISP_REG_COMP2_BASE ) + ( 1 << 2),
 	DISP_COMP2_Z_ORDER        = ( DISP_REG_COMP2_BASE ) + ( 2 << 2), 	
 	/***************HD2SD_CAPTURE Register******************/
 	DISP_HD2SD_CTRL       	  = ( DISP_REG_HD2SD_BASE ) + ( 0 << 2),
 	DISP_HD2SD_DES_SIZE       = ( DISP_REG_HD2SD_BASE ) + ( 1 << 2),
 	DISP_HD2SD_ADDR_Y         = ( DISP_REG_HD2SD_BASE ) + ( 2 << 2),
 	DISP_HD2SD_ADDR_C         = ( DISP_REG_HD2SD_BASE ) + ( 3 << 2),
 	DISP_HD2SD_BUF_PITCH      = ( DISP_REG_HD2SD_BASE ) + ( 4 << 2),
 	DISP_HD2SD_STATUS         = ( DISP_REG_HD2SD_BASE ) + ( 5 << 2),
 	/***************OUTIF1 Register******************/
 	DISP_OUTIF1_CTRL          = ( DISP_REG_OUTIF1_BASE ) + ( 0 << 2),
 	DISP_OUTIF1_X_SIZE        = ( DISP_REG_OUTIF1_BASE ) + ( 1 << 2),
 	DISP_OUTIF1_Y_TOTAL       = ( DISP_REG_OUTIF1_BASE ) + ( 2 << 2),
 	DISP_OUTIF1_ACTIVE_TOP    = ( DISP_REG_OUTIF1_BASE ) + ( 3 << 2),
 	DISP_OUTIF1_ACTIVE_BOT    = ( DISP_REG_OUTIF1_BASE ) + ( 4 << 2), 	 	
 	DISP_OUTIF1_BLANK_LEVEL   = ( DISP_REG_OUTIF1_BASE ) + ( 5 << 2),
 	DISP_OUTIF1_CCIR_F_START  = ( DISP_REG_OUTIF1_BASE ) + ( 6 << 2),
 	DISP_OUTIF1_HSYNC         = ( DISP_REG_OUTIF1_BASE ) + ( 7 << 2),
 	DISP_OUTIF1_VSYNC_TOP     = ( DISP_REG_OUTIF1_BASE ) + ( 8 << 2),
 	DISP_OUTIF1_VSYNC_BOT     = ( DISP_REG_OUTIF1_BASE ) + ( 9 << 2),
 	DISP_OUTIF1_STA_DISP_SIZE = ( DISP_REG_OUTIF1_BASE ) + (10 << 2),
 	DISP_OUTIF1_STA_LINE      = ( DISP_REG_OUTIF1_BASE ) + (11 << 2),
 	/***************OUTIF2 Register******************/
 	DISP_OUTIF2_CTRL          = ( DISP_REG_OUTIF2_BASE ) + ( 0 << 2),
 	DISP_OUTIF2_X_SIZE        = ( DISP_REG_OUTIF2_BASE ) + ( 1 << 2),
 	DISP_OUTIF2_Y_TOTAL       = ( DISP_REG_OUTIF2_BASE ) + ( 2 << 2),
 	DISP_OUTIF2_ACTIVE_TOP    = ( DISP_REG_OUTIF2_BASE ) + ( 3 << 2),
 	DISP_OUTIF2_ACTIVE_BOT    = ( DISP_REG_OUTIF2_BASE ) + ( 4 << 2),
 	DISP_OUTIF2_BLANK_LEVEL   = ( DISP_REG_OUTIF2_BASE ) + ( 5 << 2),
 	DISP_OUTIF2_CCIR_F_START  = ( DISP_REG_OUTIF2_BASE ) + ( 6 << 2),
 	DISP_OUTIF2_HSYNC         = ( DISP_REG_OUTIF2_BASE ) + ( 7 << 2),
 	DISP_OUTIF2_VSYNC_TOP     = ( DISP_REG_OUTIF2_BASE ) + ( 8 << 2),
 	DISP_OUTIF2_VSYNC_BOT     = ( DISP_REG_OUTIF2_BASE ) + ( 9 << 2),
 	DISP_OUTIF2_STA_DISP_SIZE = ( DISP_REG_OUTIF2_BASE ) + (10 << 2),
 	DISP_OUTIF2_STA_LINE      = ( DISP_REG_OUTIF2_BASE ) + (11 << 2),
};

#else

enum
{
	DISP_BYTE_ENDIAN 				= 0x41800000,	/* Byte endian register */
	DISP_NIBBLE_ENDIAN				= 0x41800004, 	/* Nibble endian register */ 	
	DISP_GFX1_BUF_START				= 0x41800008,	/* GFX1 buffer start address */
	DISP_GFX1_LINE_PITCH 				= 0x4180000C,	/* GFX1 pitch per line */
	DISP_GFX1_BLANK_PXL	 			= 0x41800010,	/* Blank pixel number for GFX1 */
	DISP_GFX2_BUF_START				= 0x41800014,	/* GFX2 buffer start address */
	DISP_GFX2_LINE_PITCH 				= 0x41800018,	/* GFX2 pitch per line */
	DISP_GFX2_BLANK_PXL				= 0x4180001C,	/* Blank pixel number for GFX2 */
	DISP_GFX1_X_START 				= 0x41800020,	/* GFX1 start position in the X axis */
	DISP_GFX1_X_END 				= 0x41800024,	/* GFX1 end position in the X axis */
	DISP_GFX1_Y_START 				= 0x41800028,	/* GFX1 start position in the Y axis */
	DISP_GFX1_Y_END 				= 0x4180002C,	/* GFX1 end position in the Y axis */
	DISP_GFX2_X_START 				= 0x41800030,	/* GFX2 start position in the X axis */
	DISP_GFX2_X_END 				= 0x41800034,	/* GFX2 end position in the X axis */
	DISP_GFX2_Y_START 				= 0x41800038,	/* GFX2 start position in the Y axis */
	DISP_GFX2_Y_END 				= 0x4180003C,	/* GFX2 end position in the Y axis */
	DISP_VIDEO_X_START 				= 0x41800040,	/* Video start position in the X axis */
	DISP_VIDEO_X_END 				= 0x41800044,	/* Video end position in the X axis	*/
	DISP_VIDEO_Y_START 				= 0x41800048,	/* Video start position in the Y axis */
	DISP_VIDEO_Y_END 				= 0x4180004C,	/* Video end position in the Y axis	*/
	DISP_DISP_ENABLE 				= 0x41800050,	/* Display Enable register */
	DISP_SYNC_MODE 					= 0x41800054,	/* Sync mode register */
	DISP_VIDEO_FORMAT 				= 0x41800058,	/* Video format register */
	DISP_GFX1_EN 					= 0x4180005C,	/* GFX1 display enable register */
	DISP_GFX1_FORMAT 				= 0x41800060,	/* GFX1 format register	*/
	DISP_GFX1_DEFAULT_ALPHA 			= 0x41800064,	/* GFX1 default alpha value	*/
	DISP_GFX1_Z_ORDER 				= 0x41800068,	/* GFX1 Z-order register */
	DISP_GFX1_KEYING_EN 				= 0x4180006C,	/* GFX1 color key enable register */
	DISP_GFX1_R_MIN 				= 0x41800070,	/* GFX1 red min value register */
	DISP_GFX1_R_MAX 				= 0x41800074,	/* GFX1 red max value register */
	DISP_GFX1_G_MIN 				= 0x41800078,	/* GFX1 green min value register */
	DISP_GFX1_G_MAX 				= 0x4180007C,	/* GFX1 green max value register */
	DISP_GFX1_B_MIN 				= 0x41800080,	/* GFX1 blue min value register	*/
	DISP_GFX1_B_MAX 				= 0x41800084,	/* GFX1 blue max value register	*/
	DISP_GFX2_EN 					= 0x41800088,	/* GFX2 display enable register	*/
	DISP_GFX2_FORMAT 				= 0x4180008C,	/* GFX2 format register	*/
	DISP_GFX2_DEFAULT_ALPHA 			= 0x41800090,	/* GFX2 default alpha value	*/
	DISP_GFX2_Z_ORDER 				= 0x41800094,	/* GFX2 Z-order register */
	DISP_GFX2_KEYING_EN 				= 0x41800098,	/* GFX2 color key enable register */
	DISP_GFX2_R_MIN 				= 0x4180009C,	/* GFX2 red min value register */
	DISP_GFX2_R_MAX 				= 0x418000A0,	/* GFX2 red max value register */
	DISP_GFX2_G_MIN 				= 0x418000A4,	/* GFX2 green min value register */
	DISP_GFX2_G_MAX 				= 0x418000A8,	/* GFX2 green max value register */
	DISP_GFX2_B_MIN 				= 0x418000AC,	/* GFX2 blue min value register	*/
	DISP_GFX2_B_MAX 				= 0x418000B0,	/* GFX2 blue max value register	*/
	DISP_VIDEO_ENABLE 				= 0x418000B4,	/* Video display enable register */
	DISP_VIDEO_DEFAULT_ALPHA 			= 0x418000B8,	/* Video default alpha register */
	DISP_VIDEO_Z_ORDER 				= 0x418000BC,	/* Video Z-order register */
	DISP_BG_Y 					= 0x418000C0,	/* Display background Y value register */
	DISP_BG_U 					= 0x418000C4,	/* Display background U value register */
	DISP_BG_V 					= 0x418000C8,	/* Display background V value register */	
	DISP_UPDATE_REG 				= 0x418000CC,	/* Display update register */
	DISP_CLUT1_ADDR 				= 0x418000D0,	/* CLUT1 address register */
	DISP_CLUT1_WE 					= 0x418000D4,	/* CLUT1 data register */
	DISP_CLUT2_ADDR 				= 0x418000D8,	/* CLUT2 address register */
	DISP_CLUT2_WE 					= 0x418000DC,	/* CLUT2 data register */	
	DISP_GFX1_DRAM_MAPPING 				= 0x418000E0,	/* GFX1 DRAM mapping mode register */
	DISP_GFX2_DRAM_MAPPING 				= 0x418000E4,	/* GFX2 DRAM mapping mode register */
	DISP_CLIP_EN 					= 0x418000E8	/* Display clipping register */
};
#endif

#ifdef CONFIG_ARCH_CSM1201
#define PIN_MUX_BASE_ADDR         0x10171400
#define PIN_MUX0_OFFSET_ADDR      0x00
#define PIN_MUX1_OFFSET_ADDR      0x04
#define GPIO_PIN_MUX0_ADDR    0x10260030
#define GPIO_PIN_MUX1_ADDR    0x10260044

enum 
{
	REG_CLK_BASE = 0x10171000,
	REG_CLK_IDS_ID_LO   = REG_CLK_BASE + 0x000,
	REG_CLK_IDS_ID_HI   = REG_CLK_BASE + 0x004,
	REG_CLK_PLL_DDR_DIV = REG_CLK_BASE + 0x100,
	REG_CLK_PLL_BP      = REG_CLK_BASE + 0x104,
	REG_CLK_FREQ_CTRL   = REG_CLK_BASE + 0x108,
	REG_CLK_AUD_CLK_GEN_HPD    = REG_CLK_BASE + 0x10c,
	REG_CLK_AUD_CLK_GEN_FREQ   = REG_CLK_BASE + 0x118,
	REG_CLK_AUD_CLK_GEN_JITTER = REG_CLK_BASE + 0x11c,
	REG_CLK_SOFT_RESET  = REG_CLK_BASE + 0x200,
	REG_CLK_OPROC_CTRL  = REG_CLK_BASE + 0x400,

// ----------Test set registers ----------
    IDS_MBIST_DONE      = REG_CLK_BASE +  0x304,
    IDS_MBIST_RESULT0   = REG_CLK_BASE +  0x308,
    IDS_MBIST_RESULT1   = REG_CLK_BASE +  0x30c,
    IDS_TEST_CTRL       = REG_CLK_BASE +  0x310,

    // ----------System control register ----------
    IDS_OPROC_LOCTRL    = REG_CLK_BASE +  0x400,
    IDS_OPROC_HICTRL    = REG_CLK_BASE +  0x404,
    IDS_PCTL_HI         = REG_CLK_BASE +  0x408,
    IDS_PCTL_LO         = REG_CLK_BASE +  0x40c,

    //AHB error capturer
    IDS_HER_IND         = REG_CLK_BASE +  0x410,
    IDS_HER0_STAT0      = REG_CLK_BASE +  0x414,
    IDS_HER0_STAT1      = REG_CLK_BASE +  0x418,
    IDS_HER1_STAT0      = REG_CLK_BASE +  0x41c,
    IDS_HER1_STAT1      = REG_CLK_BASE +  0x420,
    IDS_HER2_STAT0      = REG_CLK_BASE +  0x424,
    IDS_HER2_STAT1      = REG_CLK_BASE +  0x428,
};

// 1,1 HD, Progressive
// 0,1 SD, Progressive
// 0,0 SD, Interlacer
int SetTVEncoderClk_TVE0(int IsHD, int IsProgressive)
{
	unsigned short FreqCtlValue = 0;
	unsigned short TVEPLLValue  = 0;

	FreqCtlValue = ReadReg16(REG_CLK_FREQ_CTRL);
	FreqCtlValue  &= (~(1 << 3)); //Disable the TVE Clock
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);
	//Delay for A while
	udelay(1024);
	//Set TVE1 clk pll value
	if (IsHD)
	{
		TVEPLLValue = 0x4621; 
	}
	else
	{
		TVEPLLValue = 0x8618;
	}
	WriteReg16(IDS_PCTL_LO, TVEPLLValue);
	//Delay for A while
	udelay(1024);

	//Enable the TVE Clk sys REG_CLK_FREQ_CTRL[3]
	FreqCtlValue |= (0x1 << 3);
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue); 
	
	//Set tve0_clk6 gating:
	//[5:4]=2'b00,
	FreqCtlValue &= (~( 0x3 << 4));
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);

	if (!IsHD && IsProgressive)
	{
		FreqCtlValue |= (0x2 << 4);
	}
	else // HD and SD_i share this branch
	{
		FreqCtlValue |= (0x1 << 4);
	}
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);
	//Read Back to Check
	{
		unsigned short ReadValue = 0;
		ReadValue = ReadReg16(REG_CLK_FREQ_CTRL);
		if(!(FreqCtlValue == ReadValue)) printf("FreqCtlValue = 0x%x,ReadValue = 0x%x\n",FreqCtlValue,ReadValue);
	}

	FreqCtlValue = ReadReg16(REG_CLK_FREQ_CTRL);
	printf("clock TVE0 REG_CLK_FREQ_CTRL=%x\n",FreqCtlValue);
	printf("clock TVE0 IDS_PCTL_LO=%x\n",ReadReg16(IDS_PCTL_LO));
	return 1;
}

//IsHd IsP
// 1,  1 :HD, Progressive
// 0,  1 :SD, Progressive
// 0,  0 :SD, Interlacer
// Clock Source
// 1: PLL_SYS
// 0: PLL_TVE
int SetTVEncoderClk_TVE1(int IsHD, int IsProgressive, int ClockSrc)
{
	unsigned int  FreqCtlValue = 0;
	unsigned int  TVEPLLValue  = 0;
	unsigned int  temp         = 0;
	// Under PLL_SYS, the TVE_PLL will be configure in SetTVEncoderClk_TVE0()
	if (!ClockSrc){
		FreqCtlValue = ReadReg16(REG_CLK_FREQ_CTRL);
		FreqCtlValue  &= (~(1 << 3)); //Disable the TVE Clock
		WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);
		//Delay for A while
		udelay(1024);
		//Set TVE1 clk pll value
		if (IsHD)
		{
			TVEPLLValue = 0x4621; //???
		}
		else
		{
			TVEPLLValue = 0x8618;
		}
		WriteReg16(IDS_PCTL_LO, TVEPLLValue);
		//Delay for A while
		udelay(1024);

		//Enable the TVE Clk sys REG_CLK_FREQ_CTRL[3]
		FreqCtlValue |= (1 << 3);
		WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue); 
	}
	// tve1_clk0 ~ tve1_clk5 select: tve1_clk0_sel==freq_ctrl[11,10]
	// (default:freq_ctrl =0x031a)
	// default:tve1_clk0_sel=2'b00,  clock0-5 are gating 
	// 2'b01 clock0-5 the from PLL_SYS/4
	// 2'b10 clock0~5 from the o_plltve
	// Set tve1_clk0_sel switch to PLLTVE
	// [11:10]=2'b10,
	FreqCtlValue = ReadReg16(REG_CLK_FREQ_CTRL);
	printf("1 gating TVE1 REG_CLK_FREQ_CTRL=%x\n",FreqCtlValue);
	temp = ~(0x3 << 10);
	printf("temp =%x\n",temp);
	//FreqCtlValue  &= (~(0x3 << 10)); //gating the tve1_clk0_sel Clock
	FreqCtlValue  &= temp; //gating the tve1_clk0_sel Clock
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue); 
    FreqCtlValue = ReadReg16(REG_CLK_FREQ_CTRL);
	printf("2 gating TVE1 REG_CLK_FREQ_CTRL=%x\n",FreqCtlValue);

	if(ClockSrc){ //PLL_SYS
	    FreqCtlValue |= (0x1 << 10); //[11:10]=2'b01
	    WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);
	    udelay(1024);
	} else {	//PLL_TVE
	    FreqCtlValue |= (0x1 << 11); //[11:10]=2'b10
	    WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);
	    udelay(1024);
	}

	//Set TVE1_CLK6 gating:
	//[13:12]=2'b00,
	FreqCtlValue &= (~( 0x3 << 12));
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);

	if (!IsHD && IsProgressive)
	{
		FreqCtlValue |= (0x2 << 12);
	}
	else // HD and SD_i share this branch
	{
		FreqCtlValue |= (0x1 << 12);
	}
	WriteReg16(REG_CLK_FREQ_CTRL, FreqCtlValue);
	//Read Back to Check
	{
		unsigned int  ReadValue = 0;
		ReadValue = ReadReg16( REG_CLK_FREQ_CTRL);
	}
	FreqCtlValue = ReadReg16(REG_CLK_FREQ_CTRL);
	printf("clock TVE1 REG_CLK_FREQ_CTRL=%x\n",FreqCtlValue);
	//For Test
	//Enable CCIR 656 Clk

	return 1;
}


void gpio_pin_mux0_write(unsigned int value)
{
   WriteReg32(GPIO_PIN_MUX0_ADDR, value);
}
unsigned int gpio_pin_mux0_read(void)
{
 return (ReadReg32(GPIO_PIN_MUX0_ADDR));
}
void gpio_pin_mux1_write(unsigned int value)
{
   WriteReg32(GPIO_PIN_MUX1_ADDR, value);
}
unsigned int gpio_pin_mux1_read(void)
{
 return (ReadReg32(GPIO_PIN_MUX1_ADDR));
}

void pin_mux0_write(unsigned int value)
{
  WriteReg32(PIN_MUX_BASE_ADDR+PIN_MUX0_OFFSET_ADDR, value);
}

void pin_mux1_write(unsigned int value)
{
  WriteReg32(PIN_MUX_BASE_ADDR+PIN_MUX1_OFFSET_ADDR, value);
}

unsigned int pin_mux0_read(void)
{
  return (ReadReg32(PIN_MUX_BASE_ADDR+PIN_MUX0_OFFSET_ADDR));
}

unsigned int pin_mux1_read(void)
{
  return (ReadReg32(PIN_MUX_BASE_ADDR+PIN_MUX1_OFFSET_ADDR));
}

void DFInit(void)
{
	unsigned int RegVal;
	RegVal=gpio_pin_mux0_read();
	RegVal&=0x18ffffff;
	gpio_pin_mux0_write(RegVal);
	RegVal=gpio_pin_mux1_read();
	RegVal&=0xffffc000;
	gpio_pin_mux1_write(RegVal);

	//RegVal |= 0x3fc0;
	//gpio_pin_mux1_write(RegVal);
	
	RegVal=pin_mux1_read();
	RegVal |=0x1000;
	pin_mux1_write(RegVal);

	printf("gpio_pin_mux1_read 0x%x\n", gpio_pin_mux1_read());
}

void DFSetDigitalOutFmt(unsigned int OutIFID,unsigned int FmtVal)
{
    unsigned int RegVal;
    OutIFID =!(!OutIFID);
	RegVal=pin_mux0_read();
	
	if(OutIFID)
		RegVal &=0xefff;
	else
		RegVal |=0x1000;

	switch(FmtVal)
	{
	case 0:
		RegVal &=0xfbff;
		RegVal |=0x800;
		break;
	case 1:
        RegVal |=0xcff;
		RegVal &=0xdfff;
		break;
	default:
		RegVal &=0xf3ff;
	}
    pin_mux0_write(RegVal);
}
void DFSetAnalogOutFmt(unsigned int DACID,unsigned int FmtID)
{
	unsigned int RegVal = 0;
	DACID=!(!DACID);
	if(DACID)
	{
        RegVal=pin_mux1_read();
		printf("pin_mux1_read1 0x%x\n",RegVal);
        switch(FmtID)
		{
		case 0:
            RegVal &=0xfff3;
			break;
		case 1:
            RegVal |=0x8;
			RegVal &=0xfffb;
			break;
		default:
			RegVal |=0xc;
		}
		printf("pin_mux1_read2-1 0x%x\n",RegVal);
        	pin_mux1_write(RegVal);
		printf("pin_mux1_read3 0x%x\n",RegVal);
	}
	else
	{
		RegVal=pin_mux0_read();
		if(FmtID==0)
		{
          RegVal &=0xfffe;
		}
		else
		{
          RegVal |=0x1;
		}
        pin_mux0_write(RegVal);
	}
}
#endif

typedef enum __VIDEOFMT_E__
{
	VIDENC_NTSC = 0,
	VIDENC_PAL,
//#ifdef CONFIG_ARCH_CSM1201
	VIDENC_PAL_M,	
	VIDENC_PAL_N,	
	VIDENC_PAL_CN,	
	VIDENC_SECAM,
	VIDENC_P480,
	VIDENC_P720,
	VIDENC_I1080,
	VIDENC_NONE
//#endif
} VIDEOFMT_E;

typedef enum __VIDOUTPORT_E__
{
    VIDOUTPORT_CVBS_YC = 0,
    VIDOUTPORT_YUV,
#ifdef CONFIG_ARCH_CSM1201   
    VIDOUTPORT_RGB
#endif
} VIDOUTPORT_E;

#ifdef CONFIG_ARCH_CSM1201
static int dfreg_ctrl[8] =
{
	DISP_UPDATE_REG,DISP_STATUS,DISP_OUTIF1_INT_CLEAR,DISP_OUTIF2_INT_CLEAR,DISP_OUTIF1_ERR_CLEAR,
	DISP_OUTIF2_ERR_CLEAR,DISP_SCA_COEF_IDX,DISP_SCA_COEF_DATA
};
static int dfreg_gfx[2][13] =
{
	{DISP_GFX1_CTRL,DISP_GFX1_FORMAT,DISP_GFX1_ALPHA_CTRL,DISP_GFX1_KEY_RED,DISP_GFX1_KEY_BLUE,DISP_GFX1_KEY_GREEN,
	DISP_GFX1_BUF_START,DISP_GFX1_LINE_PITCH,DISP_GFX1_X_POSITON,DISP_GFX1_Y_POSITON,DISP_GFX1_SCL_X_POSITON,
	DISP_GFX1_CLUT_ADDR,DISP_GFX1_CLUT_DATA
	},
	{DISP_GFX2_CTRL,DISP_GFX2_FORMAT,DISP_GFX2_ALPHA_CTRL,DISP_GFX2_KEY_RED,DISP_GFX2_KEY_BLUE,DISP_GFX2_KEY_GREEN,
	DISP_GFX2_BUF_START,DISP_GFX2_LINE_PITCH,DISP_GFX2_X_POSITON,DISP_GFX2_Y_POSITON,DISP_GFX2_SCL_X_POSITON,
	DISP_GFX2_CLUT_ADDR,DISP_GFX2_CLUT_DATA
	},
};

static int dfreg_outif[2][12] =
{
	{DISP_OUTIF1_CTRL,DISP_OUTIF1_X_SIZE,DISP_OUTIF1_Y_TOTAL,DISP_OUTIF1_ACTIVE_TOP,DISP_OUTIF1_ACTIVE_BOT,
	DISP_OUTIF1_BLANK_LEVEL,DISP_OUTIF1_CCIR_F_START,DISP_OUTIF1_HSYNC,DISP_OUTIF1_VSYNC_TOP,DISP_OUTIF1_VSYNC_BOT,
	DISP_OUTIF1_STA_DISP_SIZE,DISP_OUTIF1_STA_LINE
	},
	{DISP_OUTIF2_CTRL,DISP_OUTIF2_X_SIZE,DISP_OUTIF2_Y_TOTAL,DISP_OUTIF2_ACTIVE_TOP,DISP_OUTIF2_ACTIVE_BOT,
	DISP_OUTIF2_BLANK_LEVEL,DISP_OUTIF2_CCIR_F_START,DISP_OUTIF2_HSYNC,DISP_OUTIF2_VSYNC_TOP,DISP_OUTIF2_VSYNC_BOT,
	DISP_OUTIF2_STA_DISP_SIZE,DISP_OUTIF1_STA_LINE
	},
};
#endif
/* BYTE_ENDIAN */
/* 16bit	128bit	32bit 	correct */
/* 1		0	0	NO
 * 0		0	0	NO
 * 0		1	0	Yes
 * 0		1	1	NO
 */
#define BYTE_ENDIAN	3

/* 7..4 3..0 -> 3..0 7..4 bits */
#define NIBBLE_ENDIAN	0

#ifdef CONFIG_ARCH_CSM1201
static int BeginUpdateDispReg ( void )
{
	WriteReg32 ( DISP_UPDATE_REG, 1);
	return 0;
}

static int EndUpdateDispReg ( void )
{
	WriteReg32 ( DISP_UPDATE_REG, 0 );
	return 0;
}


int __DFOutIFTimingCfg(df_outif_reg *Reg, DF_VIDEO_FORMAT Format)
{
	if(outputFomatInfo[Format].IsYUVorRGB == 1)
	{
		Reg->df_outif_control.bits.iClkOutSel = 7;
	}
	else if(outputFomatInfo[Format].IsYUVorRGB == 0)
	{
		Reg->df_outif_control.bits.iClkOutSel = 4;
	}
	
	Reg->df_outif_control.bits.is_rgb_fmt = !outputFomatInfo[Format].IsYUVorRGB;	
	Reg->df_outif_control.bits.iIsHD = outputFomatInfo[Format].iIsHD;
	Reg->df_outif_control.bits.iIsInterlaced = outputFomatInfo[Format].iIsInterlaced;
	Reg->df_outif_control.bits.iNeedRepeat = outputFomatInfo[Format].iNeedRepeat;
	Reg->df_outif_control.bits.iNeedMux = outputFomatInfo[Format].iNeedMux;
	Reg->df_outif_control.bits.iCVE5F0IsHalfLine = outputFomatInfo[Format].iCVE5F0IsHalfLine;
	Reg->df_outif_control.bits.iCVE5F1IsHalfLine = outputFomatInfo[Format].iCVE5F1IsHalfLine;
	Reg->df_outif_control.bits.iVgaF0IsHalfLine = outputFomatInfo[Format].iVgaF0IsHalfLine;
	Reg->df_outif_control.bits.iVgaF1IsHalfLine = outputFomatInfo[Format].iVgaF1IsHalfLIne;
	Reg->df_outif_control.bits.iHSyncPolarity = outputFomatInfo[Format].iHSyncPolarity;
	Reg->df_outif_control.bits.iVSyncPolarity = outputFomatInfo[Format].iVSyncPolarity;
	Reg->df_outif_control.bits.iDEPolarity = outputFomatInfo[Format].iDEPolarity;
	
	Reg->df_outif_x_size.bits.iXTotal = outputFomatInfo[Format].iXTotal;
	Reg->df_outif_x_size.bits.iXActiveStart = outputFomatInfo[Format].iXActiveStart;
	Reg->df_outif_y_size.bits.iYTotal = outputFomatInfo[Format].iYTotal;
	Reg->df_outif_active_top.bits.iYTopActiveStart = outputFomatInfo[Format].iYTopActiveStart;
	Reg->df_outif_active_top.bits.iYTopActiveEnd = outputFomatInfo[Format].iYTopActiveEnd;
	Reg->df_outif_active_bot.bits.iYBotActiveStart = outputFomatInfo[Format].iYBotActiveStart;
	Reg->df_outif_active_bot.bits.iYBotActiveEnd = outputFomatInfo[Format].iYBotActiveEnd;
	Reg->df_outif_ccir_f_start.bits.iCCIRF0Start = outputFomatInfo[Format].iCCIRF0Start;
	Reg->df_outif_ccir_f_start.bits.iCCIRF1Start = outputFomatInfo[Format].iCCIRF1Start;
	Reg->df_outif_h_sync.bits.iHSyncStart = outputFomatInfo[Format].iHSyncStart;
	Reg->df_outif_h_sync.bits.iHSyncEnd = outputFomatInfo[Format].iHSyncEnd;
	Reg->df_outif_v_sync_top.bits.iVSyncTopStart = outputFomatInfo[Format].iVSyncTopStart;
	Reg->df_outif_v_sync_top.bits.iVSyncTopEnd = outputFomatInfo[Format].iVSyncTopEnd;
	Reg->df_outif_v_sync_bot.bits.iVSyncBotStart = outputFomatInfo[Format].iVSyncBotStart;
	Reg->df_outif_v_sync_bot.bits.iVSyncBotEnd = outputFomatInfo[Format].iVSyncBotEnd;
	if (Format == DISP_YUV_PAL)
		Reg->df_outif_control.bits.iIsPal = 1;
	else
		Reg->df_outif_control.bits.iIsPal = 0;
	return Format;
}
#endif

int IsRectValid( unsigned int PixelWidth, unsigned int PixelHeight, RECT *Rect) //Boolean
{
	if (Rect == NULL) 
	{
		return 0;
	}
	else if (Rect->left   < 0 || Rect->left   > PixelWidth  ||
		     Rect->top    < 0 || Rect->top    > PixelHeight ||
		     Rect->right  < 0 || Rect->right  > PixelWidth  ||
		     Rect->bottom < 0 || Rect->bottom > PixelHeight )
	{
		return 0;
	}
	else if (!(Rect->left  <= Rect->right) || 
		     !(Rect->top   <= Rect->bottom) )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static int	CS_LoadDisplay ( VIDEOFMT_E vefmt,  DF_GFX_IMG* ColorImg,RECT *SrcRect, RECT *DispRect,
							int IsGfxScalerEna,int IsFetchOneField, int IsFetchTopOnly,
	               				int DispScreenWidth, int DispScreenHeight, int IsDispInterlaced)
{
#ifdef CONFIG_ARCH_CSM1201
//#if 1
	unsigned int Pitch = 0;
	unsigned int ImgWidth   = 0;
	unsigned int ImgHeight  = 0;
	unsigned int SrcXStart  = 0;
	unsigned int SrcXBlank  = 0;
	unsigned int SrcYStart  = 0;
	unsigned int DispWidth  = 0;
	unsigned int DispHeight = 0;
	unsigned int DispXStart = 0;
	unsigned int DispYStart = 0;
	unsigned int PixelBitWidth = 0;
	DF_VIDEO_FORMAT dfVideoFormat;

	DF_GFX_COLOR_FORMAT DFImgFormat = ColorImg->ColorFormat;

	switch(vefmt)
	{
		case VIDENC_NTSC :
			dfVideoFormat = DISP_YUV_NTSC;
			break;
		case VIDENC_PAL :
			dfVideoFormat = DISP_YUV_PAL;
			break;
		case VIDENC_I1080 :
			dfVideoFormat = DISP_YUV_1080I_25FPS;
			break;
		case VIDENC_P720 :
		default :
			dfVideoFormat = DISP_YUV_720P_50FPS;
			break;
	}

	dfVideoFormat = DISP_YUV_PAL;

	dfreg.Gfx[gfxid].df_gfx_control_reg.val = ReadReg32(DISP_GFX1_CTRL);
	dfreg.Gfx[gfxid].df_gfx_format_reg.val = ReadReg32(DISP_GFX1_FORMAT);
	dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.val = ReadReg32(DISP_GFX1_BUF_START);
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.val = ReadReg32(DISP_GFX1_LINE_PITCH);
	dfreg.Gfx[gfxid].df_gfx_x_position_reg.val = ReadReg32(DISP_GFX1_X_POSITON);
	dfreg.Gfx[gfxid].df_gfx_y_position_reg.val = ReadReg32(DISP_GFX1_Y_POSITON);
	dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.val = ReadReg32(DISP_GFX1_SCL_X_POSITON);
	dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.val = ReadReg32(DISP_GFX1_ALPHA_CTRL);	
	dfreg.OutIF[outifid].df_outif_control.val = ReadReg32(DISP_OUTIF2_CTRL);
	dfreg.OutIF[outifid].df_outif_x_size.val = ReadReg32(DISP_OUTIF2_X_SIZE);
	dfreg.OutIF[outifid].df_outif_y_size.val = ReadReg32(DISP_OUTIF2_Y_TOTAL);
	dfreg.OutIF[outifid].df_outif_active_top.val = ReadReg32(DISP_OUTIF2_ACTIVE_TOP);
	dfreg.OutIF[outifid].df_outif_active_bot.val = ReadReg32(DISP_OUTIF2_ACTIVE_BOT);
	dfreg.OutIF[outifid].df_outif_blank_level.val = ReadReg32(DISP_OUTIF2_BLANK_LEVEL);
	dfreg.OutIF[outifid].df_outif_ccir_f_start.val = ReadReg32(DISP_OUTIF2_CCIR_F_START);
	dfreg.OutIF[outifid].df_outif_h_sync.val = ReadReg32(DISP_OUTIF2_HSYNC);
	dfreg.OutIF[outifid].df_outif_v_sync_top.val = ReadReg32(DISP_OUTIF2_VSYNC_TOP);
	dfreg.OutIF[outifid].df_outif_v_sync_bot.val = ReadReg32(DISP_OUTIF2_VSYNC_BOT);

	printf("ColorImg->LinePitch 0x%x,ColorImg->StartAddress 0x%x,ColorImg->PixelHeight %d,ColorImg->PixelWidth %d\n\n",
		ColorImg->LinePitch,ColorImg->StartAddress,ColorImg->PixelHeight,ColorImg->PixelWidth);

	printf("ColorImg->ColorFormat %d\n\n",ColorImg->ColorFormat);
	printf("DispScreenWidth = %d,DispScreenHeight %d\n",DispScreenWidth,DispScreenHeight);
	dfreg.Gfx[gfxid].df_gfx_format_reg.bits.iColorFormat = ColorImg->ColorFormat;// RGB565
	//Check DFImgFormat
	if (DFImgFormat == 	DF_GFX_RGB565   ||
	    DFImgFormat == 	DF_GFX_ARGB4444 ||
	    DFImgFormat == 	DF_GFX_ARGB1555 )
	{
		PixelBitWidth = 16;
	}
	else if (DFImgFormat == DF_GFX_ARGB8888)
	{
		PixelBitWidth = 32;
	}
	else if (DFImgFormat == DF_GFX_CLUT8)//Clut TBD
	{
		PixelBitWidth = 8;
	}
	else if (DFImgFormat == DF_GFX_CLUT4)
	{
		PixelBitWidth = 4;
	}
	else if (DFImgFormat == DF_GFX_A0)
	{
#ifdef DF_GFX_TRUE_COLOR_SUPPORT
			PixelBitWidth = 32;
#else
			PixelBitWidth = 16;
#endif
	}

	printf("PixelBitWidth = %d\n",PixelBitWidth);
	
	//Caculate the Disp Width and Hight Para
	if (!IsRectValid(ColorImg->PixelWidth, ColorImg->PixelHeight, SrcRect) ||
		SrcRect->right == 0 ||
		SrcRect->bottom == 0)
	{
		ImgWidth  = ColorImg->PixelWidth;
		ImgHeight = ColorImg->PixelHeight;
		SrcXStart = 0;
		SrcXBlank = 0;
		SrcYStart = 0;
	}
	else
	{
		//Do Word Align
		unsigned int Mask = 0;
		Mask = ~(128 / PixelBitWidth - 1);
		ImgWidth  = (SrcRect->right & Mask) - (SrcRect->left & Mask);
		ImgHeight = (SrcRect->bottom- SrcRect->top);
		SrcXStart = (SrcRect->left & Mask);
		SrcXBlank = SrcRect->left - SrcXStart;
		SrcYStart = (SrcRect->top);
	}

	if (!IsRectValid(DispScreenWidth, DispScreenHeight,DispRect) ||
		DispRect->right == 0 ||
		DispRect->bottom == 0)
	{
		DispWidth  = DispScreenWidth;
		DispHeight = DispScreenHeight;
		DispXStart = 0;
		DispYStart = 0;
	}
	else
	{
		unsigned int Mask = 0;
		Mask = ~(128 / PixelBitWidth - 1);
		DispWidth  = (DispRect->right & Mask) - (DispRect->left & Mask);
		DispHeight = (DispRect->bottom- DispRect->top);
		DispXStart = (DispRect->left & Mask);
		DispYStart = (DispRect->top);
	}
	//Do SrcRect and DesRect Compare
	if (!IsGfxScalerEna)
	{
		ImgWidth  = min(ImgWidth,  DispWidth);
		ImgHeight = min(ImgHeight, DispHeight);
	}

	dfreg.Gfx[gfxid].df_gfx_control_reg.bits.iGfxScalerEnable = IsGfxScalerEna;
	dfreg.Gfx[gfxid].df_gfx_control_reg.bits.iFetchType = IsFetchOneField ? (IsFetchTopOnly ? 1:2): 0;
	dfreg.Gfx[gfxid].df_gfx_format_reg.bits.iColorFormat = ColorImg->ColorFormat;
	dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.bits.iStartAddr = ((ColorImg->StartAddress + 
		               SrcYStart * ColorImg->LinePitch +
		               SrcXStart * PixelBitWidth / 8) >> 4); //WordAddr
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.bits.iLinePitch = ColorImg->LinePitch >> 4; //WordAddr
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.bits.iBlankPixel = SrcXBlank;
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.bits.reserved_2 = 0;
	//IsDispInterlaced = 1;
	if (!IsGfxScalerEna)
	{
		dfreg.Gfx[gfxid].df_gfx_x_position_reg.bits.iXStart = DispXStart;
		dfreg.Gfx[gfxid].df_gfx_x_position_reg.bits.iXEnd = ImgWidth + DispXStart - 1;
		dfreg.Gfx[gfxid].df_gfx_y_position_reg.bits.iYStart  = DispYStart;
		dfreg.Gfx[gfxid].df_gfx_y_position_reg.bits.iYEnd    = (ImgHeight + DispYStart)/ (IsDispInterlaced ? 2:1);
		dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.bits.iScaleXStart  = 0;
		dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.bits.iScaleXEnd    = 0;
	}
	else //if(IsGfxScalerEna)
	{
		dfreg.Gfx[gfxid].df_gfx_x_position_reg.bits.iXStart  = 0;
		dfreg.Gfx[gfxid].df_gfx_x_position_reg.bits.iXEnd   = ImgWidth - 1;
		dfreg.Gfx[gfxid].df_gfx_y_position_reg.bits.iYStart  = DispYStart;
		dfreg.Gfx[gfxid].df_gfx_y_position_reg.bits.iYEnd   = (ImgHeight + DispYStart)/ (IsDispInterlaced ? 2:1);
		dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.bits.iScaleXStart = DispXStart;
		dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.bits.iScaleXEnd  = DispWidth + DispXStart - 1;
	}

#endif	

#ifdef CONFIG_ARCH_CSM1201
	BeginUpdateDispReg ( );
#if 0
	dfreg.Gfx[gfxid].df_gfx_control_reg.val = ReadReg32(DISP_GFX1_CTRL);
	printf("DISP_GFX1_CTRL : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_control_reg.val);
	dfreg.Gfx[gfxid].df_gfx_control_reg.bits.iGfxEnable = 1;// gfx0 on outif0
	dfreg.Gfx[gfxid].df_gfx_control_reg.bits.iGfxScalerEnable= 0;
	
	dfreg.Gfx[gfxid].df_gfx_format_reg.val = ReadReg32(DISP_GFX1_FORMAT);
	printf("DISP_GFX1_FORMAT : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_format_reg.val);
	dfreg.Gfx[gfxid].df_gfx_format_reg.bits.iColorFormat = 2;
	
	dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.val = ReadReg32(DISP_GFX1_BUF_START);
	printf("DISP_GFX1_BUF_START : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.val);
	dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.bits.iStartAddr = GFXBASEADDRESS >> 4;

	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.val = ReadReg32(DISP_GFX1_LINE_PITCH);
	printf("DISP_GFX1_LINE_PITCH : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.val);
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.bits.iLinePitch = GFXLINEPITCHSIZE >> 4;
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.bits.iBlankPixel = 0;
	dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.bits.reserved_2 = 0;

	dfreg.Gfx[gfxid].df_gfx_x_position_reg.val = ReadReg32(DISP_GFX1_X_POSITON);
	printf("DISP_GFX1_X_POSITON : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_x_position_reg.val);
	dfreg.Gfx[gfxid].df_gfx_x_position_reg.bits.iXStart = 0;
	dfreg.Gfx[gfxid].df_gfx_x_position_reg.bits.iXEnd = 719;

	dfreg.Gfx[gfxid].df_gfx_y_position_reg.val = ReadReg32(DISP_GFX1_Y_POSITON);
	printf("DISP_GFX1_Y_POSITON : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_y_position_reg.val);
	dfreg.Gfx[gfxid].df_gfx_y_position_reg.bits.iYStart = 0;
	dfreg.Gfx[gfxid].df_gfx_y_position_reg.bits.iYEnd = 288;

	dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.val = ReadReg32(DISP_GFX1_SCL_X_POSITON);
	printf("DISP_GFX1_SCL_X_POSITON : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.val);
	dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.bits.iScaleXStart = 0;
	dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.bits.iScaleXEnd = 0;	

	dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.val = ReadReg32(DISP_GFX1_ALPHA_CTRL);
	printf("DISP_GFX1_ALPHA_CTRL : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.val);
#endif

	dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.bits.iDefaultAlpha = 0xf0;
	dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.bits.iArgb1555Alpha0 = 0;
	dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.bits.iArgb1555Alpha1 = 0;

	dfreg.Gfx[gfxid].df_gfx_format_reg.bits.i16BitEndian = 0;
	dfreg.Gfx[gfxid].df_gfx_format_reg.bits.i128BitEndian = 1;
	dfreg.Gfx[gfxid].df_gfx_format_reg.bits.iByteEndian = 1;

#if 1
	WriteReg32(DISP_GFX1_FORMAT, dfreg.Gfx[gfxid].df_gfx_format_reg.val);
	WriteReg32(DISP_GFX1_BUF_START, dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.val);
	WriteReg32(DISP_GFX1_LINE_PITCH, dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.val);
	WriteReg32(DISP_GFX1_X_POSITON, dfreg.Gfx[gfxid].df_gfx_x_position_reg.val);
	WriteReg32(DISP_GFX1_Y_POSITON, dfreg.Gfx[gfxid].df_gfx_y_position_reg.val);
	WriteReg32(DISP_GFX1_SCL_X_POSITON, dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.val);
	WriteReg32(DISP_GFX1_ALPHA_CTRL, dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.val);
#endif
	__DFOutIFTimingCfg(&(dfreg.OutIF[outifid]), dfVideoFormat);
	WriteReg32(DISP_OUTIF2_CTRL, dfreg.OutIF[outifid].df_outif_control.val);
 	WriteReg32(DISP_OUTIF2_X_SIZE, dfreg.OutIF[outifid].df_outif_x_size.val);
	WriteReg32(DISP_OUTIF2_Y_TOTAL, dfreg.OutIF[outifid].df_outif_y_size.val);
	WriteReg32(DISP_OUTIF2_ACTIVE_TOP, dfreg.OutIF[outifid].df_outif_active_top.val);
	WriteReg32(DISP_OUTIF2_ACTIVE_BOT, dfreg.OutIF[outifid].df_outif_active_bot.val);
	WriteReg32(DISP_OUTIF2_BLANK_LEVEL, dfreg.OutIF[outifid].df_outif_blank_level.val);
	WriteReg32(DISP_OUTIF2_CCIR_F_START, dfreg.OutIF[outifid].df_outif_ccir_f_start.val);
	WriteReg32(DISP_OUTIF2_HSYNC, dfreg.OutIF[outifid].df_outif_h_sync.val);
	WriteReg32(DISP_OUTIF2_VSYNC_TOP, dfreg.OutIF[outifid].df_outif_v_sync_top.val);
	WriteReg32(DISP_OUTIF2_VSYNC_BOT, dfreg.OutIF[outifid].df_outif_v_sync_bot.val);


	//enable gfx and outif
	dfreg.Gfx[gfxid].df_gfx_control_reg.bits.iGfxEnable = 2;// gfx0 on outif1
	dfreg.Gfx[gfxid].df_gfx_control_reg.bits.iRGB2YUVConvertEna = 1;
	dfreg.OutIF[outifid].df_outif_control.bits.iDispEna = 1;
	WriteReg32(DISP_OUTIF2_CTRL, dfreg.OutIF[outifid].df_outif_control.val);
	//dfreg.Comp[outifid].df_comp_clip.bits.iClipEna	= 1;
	//WriteReg32(DISP_COMP1_CLIP, dfreg.Comp[outifid].df_comp_clip.val);

	EndUpdateDispReg();

	sleep(1);
	
	BeginUpdateDispReg();
	
	WriteReg32(DISP_GFX1_CTRL, dfreg.Gfx[gfxid].df_gfx_control_reg.val);


#else
	WriteReg32 ( DISP_BYTE_ENDIAN,  BYTE_ENDIAN );
	WriteReg32 ( DISP_NIBBLE_ENDIAN, NIBBLE_ENDIAN );	

	if(vefmt == VIDENC_PAL)
	{
		/* set video layer and display output type. PAL */
		/* video output format in TVEncoder init */
		WriteReg32 ( DISP_VIDEO_FORMAT, 3 );
		WriteReg32 ( DISP_GFX1_X_START, 0 );
		WriteReg32 ( DISP_GFX1_X_END, 719 ); /* 720 - 1 */
		WriteReg32 ( DISP_GFX1_Y_START, 0 );
		WriteReg32 ( DISP_GFX1_Y_END, 287 ); /* 576/2  - 1 */
	}
	else if(vefmt == VIDENC_NTSC)
	{
		/* set video layer and display output type. 720P */
		/* video output format in TVEncoder init */
		WriteReg32 ( DISP_VIDEO_FORMAT, 0 );
		
		WriteReg32 ( DISP_GFX1_X_START, 0 );
		WriteReg32 ( DISP_GFX1_X_END, 719 ); 
		WriteReg32 ( DISP_GFX1_Y_START, 0 );
		WriteReg32 ( DISP_GFX1_Y_END, 239 ); 
	}			

	/* set video param */
	WriteReg32 ( DISP_VIDEO_ENABLE, 0 );
	WriteReg32 ( DISP_GFX1_FORMAT, 2 ); //RGB565 mode
	WriteReg32 ( DISP_GFX1_DEFAULT_ALPHA, /*0x7f*/ 255);
	WriteReg32 ( DISP_GFX1_Z_ORDER, 1 );
	WriteReg32 ( DISP_GFX1_EN, 1 );
	WriteReg32 ( DISP_GFX1_BUF_START, GFXBASEADDRESS >> 4 );
	WriteReg32 ( DISP_GFX1_LINE_PITCH, GFXLINEPITCHSIZE >> 4 );
	WriteReg32 ( DISP_GFX1_BLANK_PXL, 0 );
#endif

#if USE_DISP_GFX2
	if(vefmt == VIDENC_PAL)
	{
		/* set video layer and display output type. PAL */
		/* video output format in TVEncoder init */
		WriteReg32 ( DISP_VIDEO_FORMAT, 3 );
		
		WriteReg32 ( DISP_GFX2_X_START, 0 );
		WriteReg32 ( DISP_GFX2_X_END, 719 ); /* 720 - 1 */
		WriteReg32 ( DISP_GFX2_Y_START, 0 );
		WriteReg32 ( DISP_GFX2_Y_END, 287 ); /* 576/2  - 1 */		
	}
	else if(vefmt == VIDENC_NTSC)
	{
		/* set video layer and display output type. 720P */
		/* video output format in TVEncoder init */
		WriteReg32 ( DISP_VIDEO_FORMAT, 0 );
		
		WriteReg32 ( DISP_GFX2_X_START, 0 );
		WriteReg32 ( DISP_GFX2_X_END, 719 ); 
		WriteReg32 ( DISP_GFX2_Y_START, 0 );
		WriteReg32 ( DISP_GFX2_Y_END, 239 ); 
	}
	/* set video param */
	WriteReg32 ( DISP_VIDEO_ENABLE, 0 );
	
	WriteReg32 ( DISP_GFX2_FORMAT, 2 ); //RGB565 mode
	WriteReg32 ( DISP_GFX2_DEFAULT_ALPHA, 0xfe /*255*/ );
	WriteReg32 ( DISP_GFX2_Z_ORDER, 1 );
	WriteReg32 ( DISP_GFX2_EN, 0 );
	
	WriteReg32 ( DISP_GFX2_BUF_START, GFX2BASEADDRESS >> 4 );
	WriteReg32 ( DISP_GFX2_LINE_PITCH, GFXLINEPITCHSIZE>> 4 );
	WriteReg32 ( DISP_GFX2_BLANK_PXL, 0 );

#endif

#ifdef CONFIG_ARCH_CSM1201
	EndUpdateDispReg ( );
	printf("Display configurate complete!\n\n\n");
#else
	/* enable limit y,u,v max to 255 */
	WriteReg32 ( DISP_CLIP_EN, 1 );
	/* enable display show */
	WriteReg32 ( DISP_DISP_ENABLE, 1 );
#endif

#if 0
	printf("DISP_GFX1_CTRL : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_control_reg.val);
	printf("DISP_GFX1_FORMAT : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_format_reg.val);
	printf("DISP_GFX1_BUF_START : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_buf_start_addr_reg.val);
	printf("DISP_GFX1_LINE_PITCH : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_line_pitch_reg.val);
	printf("DISP_GFX1_X_POSITON : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_x_position_reg.val);
	printf("DISP_GFX1_Y_POSITON : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_y_position_reg.val);
	printf("DISP_GFX1_SCL_X_POSITON : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_scl_x_position_reg.val);
	printf("DISP_GFX1_ALPHA_CTRL : 0x%x\n",dfreg.Gfx[gfxid].df_gfx_alpha_control_reg.val);


	printf("DISP_OUTIF1_CTRL : 0x%x\n",dfreg.OutIF[outifid].df_outif_control.val);
	printf("DISP_OUTIF1_X_SIZE : 0x%x\n",dfreg.OutIF[outifid].df_outif_x_size.val);
	printf("DISP_OUTIF1_Y_TOTAL : 0x%x\n",dfreg.OutIF[outifid].df_outif_y_size.val);
	printf("DISP_OUTIF1_ACTIVE_TOP : 0x%x\n",dfreg.OutIF[outifid].df_outif_active_top.val);
	printf("DISP_OUTIF1_ACTIVE_BOT : 0x%x\n",dfreg.OutIF[outifid].df_outif_active_bot.val);
	printf("DISP_OUTIF1_BLANK_LEVEL : 0x%x\n",dfreg.OutIF[outifid].df_outif_blank_level.val);
	printf("DISP_OUTIF1_CCIR_F_START : 0x%x\n",dfreg.OutIF[outifid].df_outif_ccir_f_start.val);
	printf("DISP_OUTIF1_HSYNC : 0x%x\n",dfreg.OutIF[outifid].df_outif_h_sync.val);
	printf("DISP_OUTIF1_VSYNC_TOP : 0x%x\n",dfreg.OutIF[outifid].df_outif_v_sync_top.val);
	printf("DISP_OUTIF1_VSYNC_BOT : 0x%x\n",dfreg.OutIF[outifid].df_outif_v_sync_bot.val);
	printf("DISP_OUTIF1_STA_DISP_SIZE : 0x%x\n",dfreg.OutIF[outifid].df_outif_status_disp_size.val);
	printf("DISP_OUTIF1_STA_LINE : 0x%x\n",dfreg.OutIF[outifid].df_outif_status_line.val);

	{
		int i = 0;
		for(i = 0;i <12;i++){
			printf("outif %d: 0x%x\n",i,ReadReg32(dfreg_outif[1][i]));
		}

		for(i = 0;i <8;i++){
			printf("ctl %d: 0x%x\n",i,ReadReg32(dfreg_ctrl[i]));
		}

		for(i = 0;i <13;i++){
			printf("gfx %d: 0x%x\n",i,ReadReg32(dfreg_gfx[0][i]));
		}
	}
#endif
	return 0;
}


#define     CVE_REG_BASE     0x10160000
static int	CS_SetVideoFormat ( VIDEOFMT_E vefmt )
{
	int iErrorCode = 0;
	unsigned int vfmt = 0;

	switch ( vefmt )
	{
		case VIDENC_NTSC:
			vfmt = 0;
			break;	
		case VIDENC_PAL:
			vfmt = 3;
			break;
#ifdef CONFIG_ARCH_CSM1201
		case VIDENC_P720:
			vfmt = 1;
			break;	
		case VIDENC_I1080:
			vfmt = 2;
			break;
		case VIDENC_PAL_M:	
		case VIDENC_PAL_N:	
		case VIDENC_PAL_CN:	
		case VIDENC_SECAM:
		case VIDENC_P480:
#endif
		default:
			/* no support video format */
			return -1;
			break;
	}

	/* set format */
#ifdef CONFIG_ARCH_CSM1201
	BeginUpdateDispReg ( );

	//WriteReg32 ( DISP_VIDEO_FORMAT, vfmt );
	EndUpdateDispReg ( );

#else
	WriteReg32 ( DISP_VIDEO_FORMAT, vfmt );
#endif
	
	return iErrorCode;
}

typedef struct _CVE_REG_S
{
	unsigned char  	offset;
	unsigned char   ntsc_val;
	unsigned char   pal_val;
#ifdef CONFIG_ARCH_CSM1201
	unsigned char   pal_m_val;	
	unsigned char   pal_n_val;	
	unsigned char   pal_cn_val;	
	unsigned char   secam_val;
	unsigned char   p480_val;
	unsigned char   p720_val;
	unsigned char   i1080_val;
#endif
} CVE_REG_S;

#ifdef CONFIG_ARCH_CSM1201
static CVE_REG_S stTVE0ConfReg [ ] =
{
/*		address			ntsc		pal			PAL_M 		PAL-N		PAL-CN		SECAM		480p		720p		1080i	*/
/*0*/	{ 0x0,        	0x00,		0x00,  		0x00,		0x00,		0x00,		0x00,		0x00,		0x00,		0x00	 	},      	//CVE_SOFT_RESET_VAL
/*1*/ 	{ 0x1,        	0x50,		0x50,		0x50,		0x50,		0x50,		0x50,		0x50,		0x50,		0x50		},      	//CVE_REV_ID_VAL
/*2*/ 	{ 0x2,        	0x11,		0x11,  		0x11,		0x11,		0x11,		0x11,		0x11,		0x11,		0x11	 	},     		//CVE_INPUT_CTL_VAL         (clrbar_mode,yc_delay,uv_order,bypass_yclamp)
/*3*/ 	{ 0x4,        	0x09,		0x09,  		0x09,		0x09,		0x09,		0x09,		0x09,		0x09,		0x09	 	},      	//CVE_OUT_CTL_VAL           (rgb_sync,rgb_setup,comp_yuv,compchgain,cvbs_enable)
/*4*/ 	{ 0x6,        	0x01,		0x01,   	0x01,		0x01,		0x01,		0x01,		0x01,		0x01,		0x01	 	},      	//CVE_SLV_MD_TH_VAL         (slave_thresh,slave_mode)
/*5*/ 	{ 0x7,        	0x00,		0x34,  		0x24,		0x24,		0x34,		0x3C,		0x01,		0x03,		0x02	 	},     		//CVE_VID_STD_VAL           (invert_top,vsync5,secam_mode,pal_mode,trisync_mode,progressive_mode)
/*6*/ 	{ 0x8,         	0x02,		0x02,   	0x02,		0x02,		0x02,		0x02,		0x02,		0x02,		0x04	 	},      	//CVE_NUM_LINES_H_VAL
/*7*/ 	{ 0x9,         	0x0D,		0x71, 		0x0D,		0x71,		0x71,		0x71,		0x0D,		0xEE,		0x65 		},    		//CVE_NUM_LINES_L_VAL
/*8*/ 	{ 0xa,         	21,			23,  		21,			23,			23,			23,			42,			25,			20	 		},     		//CVE_FST_VID_LINE_VAL
/*9*/ 	{ 0xb,         	126,		126, 		126,		126,		126,		126,		126,		80,			88	 		},    		//CVE_HSYNC_WIDTH_VAL
/*10*/ 	{ 0xd,         	118,		138, 		118,		138,		138,		138,		118,		220,		148 		},    		//CVE_BACK_PORCH_VAL
/*11*/ 	{ 0xe,         	0x00,		0x00,   	0x00,		0x00,		0x00,		0x00,		0x00,		0,			0	 		},      	//CVE_FRONT_PORCH_H_VAL
/*12*/ 	{ 0xf,         	32,			24,  		32,			24,			24,			24,			32,			70,			44	 		},     		//CVE_FRONT_PORCH_L_VAL
/*13*/ 	{ 0x10,        	5 ,			5,   		5,			5,			5,			5,			5,			5,			7	 		},      	//CVE_ACTIVELINE_H_VAL
/*14*/ 	{ 0x11,        	160,		160, 		160,		160,		160,		160,		160,		0,			128 		},    		//CVE_ACTIVELINE_L_VAL
/*15*/ 	{ 0x12,        	68,			64,  		68,			64,			68,			64,			68,			8,			8	 		},     		//CVE_BURST_WDITH_VAL
/*16*/ 	{ 0x13,        	22,			26,  		18,			26,			26,			26,			22,			8,			8	 		},     		//CVE_BREEZE_WAY_VAL
/*17*/ 	{ 0x16,        	0x10,		0x15,		0x10,		0x15,		0x10,		0x14,		0x10,		0x10,      	0x10		},      	//CVE_CHROMA_FREQ_3_VAL 
/*18*/ 	{ 0x17,        	0xf8,		0x04,		0xF3,		0x04,		0xFB,		0xE3,		0xf8,		0xf8,      	0xf8		},      	//CVE_CHROMA_FREQ_2_VAL
/*19*/ 	{ 0x18,        	0x3e,		0xc5,		0x77,		0xc5,		0x4A,		0x8E,		0x3e,		0x3e,      	0x3e		},      	//CVE_CHROMA_FREQ_1_VAL
/*20*/ 	{ 0x19,        	0x0f,		0x66,		0xD2,		0x66,		0x23,		0x39,		0x0f,		0x0f,      	0x0f		},      	//CVE_CHROMA_FREQ_0_VAL
/*21*/	{ 0x1a,			0x00,		0x00,		0x00,		0x00,		0x00,		0x14,		0x00,		0x00,		0x00,		},
/*22*/	{ 0x1b,			0x00,		0x00,		0x00,		0x00,		0x00,		0x25,		0x00,		0x00,		0x00,		},
/*23*/	{ 0x1c,			0x00,		0x00,		0x00,		0x00,		0x00,		0xed,		0x00,		0x00,		0x00,		},
/*24*/	{ 0x1d,			0x00,		0x00,		0x00,		0x00,		0x00,		0x09,		0x00,		0x00,		0x00,		},
/*25*/ 	{ 0x1e,        	0x00,		0x00, 		0x00,		0x00,		0x00,		0x00,		0x00,		0x00,       0x00 		},       	//CVE_CHROMA_PHASE_VAL
/*26*/ 	{ 0x1f,        	0x00,		0x00,   	0x00,		0x00,		0x00,		0x00,		0x00,		0x00,		0x00,	 	},      	//CVE_TINT_VAL      
/*27*/ 	{ 0x20,        	32,			4,   		4,			4,			4,			4,			4,			36,			36	 		},      	//CVE_NA_WI_CH_CH_CP_VAL    (cphase_rst,chroma_bw_modulated,chroma_bw_yuv,wide_bw_modulated,narrow_bw_yuv)
/*28*/ 	{ 0x21,        	60,			44,  		42,			42,			44,			0,			0,			60,			60	 		},     		//CVE_CB_BURST_AMP_VAL
/*29*/ 	{ 0x22,        	0,			31,  		29,			29,			31,			0, 			0,			0,			0	 		},     		//CVE_CR_BURST_AMP_VAL
/*30*/ 	{ 0x23,        	139,		168, 		139, 		139, 		148,		96,			139,		139,		182 		},    		//CVE_CB_GAIN_VAL            
/*31*/ 	{ 0x24,        	139,		168, 		139,		139,		148,		117,		139,		139,		178 		},    		//CVE_CR_GAIN_VAL            
/*32*/ 	{ 0x27,        	3,			3,   		3,			3,			3,			3,			3,			3,			3 			},      	//CVE_WHITE_LEVEL_H_VAL     
/*33*/ 	{ 0x28,        	32,			32,  		32,			32,			32,			32,			32,			32,			32 			},     		//CVE_WHITE_LEVEL_L_VAL        
/*34*/ 	{ 0x29,        	1,			0,   		0,			0,			0,			0,			0,			0,			0 			},      	//CVE_BLACK_LEVEL_H_VAL      
/*35*/ 	{ 0x2a,        	26,			208, 		251,		251,		251,		251,		251,		251,		251 		},    		//CVE_BLACK_LEVEL_L_VAL
/*36*/ 	{ 0x2b,        	0,			0,   		0,			0,			0,			0,			0,			0,			0 			},      	//CVE_BLANK_LEVEL_H_VAL
/*37*/ 	{ 0x2c,        	240,		251, 		240,		240,		251,		251,		251,		251,		251 		},      	//CVE_BLANK_LEVEL_L_VAL      
/*38*/ 	{ 0x2d,        	0,			0,   		0,			0,			0,			0,			0,			0,			0 			},      	//CVE_VBI_BLANK_LEVEL_H_VAL 
/*39*/ 	{ 0x2e,        	240,		251, 		240,		240,		251,		251,		251,		251,		251 		},    		//CVE_VBI_BLANK_LEVEL_L_VAL  
/*40*/ 	{ 0x2f,        	0,			0,   		0,			0,			0,			0,			0,			0,			0	 		},      	//CVE_CLAMP_LEVEL_H_VAL 
/*41*/ 	{ 0x30,        	0,			0,   		0,			0,			0,			0,			0,			0,			0	 		},      	//CVE_CLAMP_LEVEL_L_VAL
/*42*/ 	{ 0x31,        	16,			8,  		16,			16,			16,			16,			16,			16,			14	 		},     		//CVE_SYNC_LEVEL_VAL
/*43*/ 	{ 0x33,        	1,			1,   		1,			1,			1,			1,			1,			1,			1	 		},      	//CVE_SYNC_HIGH_LEVEL_H_VAL
/*44*/ 	{ 0x34,        	230,		230, 		230,		230,		230,		230,		230,		230,		230 		},    		//CVE_SYNC_HIGH_LEVEL_L_VAL  
/*45*/ 	{ 0x37,        	8,			8,   		8,			8,			8,			8,			8,			8,			8	 		},      	//CVE_NOTCH_EN_WI_FR_VAL    (notch_freq,notch_wide,notch_en)
/*46*/ 	{ 0x38,        	0,			0,   		0,			0,			0,			0,			0,			0,			0	 		},      	//CVE_NO_SH_NO_VAL          (noise_gain,sharpen_gain,noise_filter) 
/*47*/ 	{ 0x39,        	5,			5,   		5,			5,			5,			5,			5,			5,			5	 		},      	//CVE_NOISE_THRESHOLD_VAL   
/*48*/ 	{ 0x3a,        	10,			10,  		10,			10,			10,			10,			10,			10,			10	 		},     		//CVE_SHARPEN_THRESHOLD_VAL 
	///////////////////////////////////////////////////////////////////////////////////////
	//                        part mix-registers value description                       //
	//-----------------------------------------------------------------------------------//
	//                       ntsc           pal            720p             1080i        //
	//-----------------------------------------------------------------------------------//  
	//clrbar_mode,                                 
	//yc_delay,
	//uv_order,
	//bypass_yclamp:         0,4,0,1        0,4,0,1        0,4,0,1          0,4,0,1
	//rgb_sync,
	//rgb_setup,
	//comp_yuv,
	//compchgain,
	//cvbs_enable:           7,1,0,0,1      0,0,0,0,1      000,0,1,00,1     000,0,1,00,1
	//slave_thresh,
	//slave_mode:            0,1            0,1            0,1              0,1
	//invert_top,
	//vsync5,
	//secam_mode,
	//pal_mode,
	//trisync_mode,
	//progressive_mode:      0,0,0,0,0,0    1,1,0,1,0,0    0,0,0,0,1,1      0,0,0,0,1,0
	//cphase_rst,
	//chroma_bw_modulated,
	//chroma_bw_yuv,
	//wide_bw_modulated,
	//narrow_bw_yuv:         2,0,0,0,0      0,0,1,0,0      2,0,1,0,0        2,0,1,0,0
	//notch_freq,
	//notch_wide,
	//notch_en:              2,0,0          2,0,0          2,0,0            2,0,0
	//noise_gain,
	//sharpen_gain,
	//noise_filter:          0,0,0          0,0,0          0,0,0            0,0,0 
};
#else
static CVE_REG_S stCve5ConfReg [ ] =
{
/*	address		ntsc		pal	*/		
/*0*/	{ 0x0,        	0x00,		0x00,  	},      	//CVE_SOFT_RESET_VAL
/*1*/ 	{ 0x1,        	0x50,		0x50,	},      	//CVE_REV_ID_VAL
/*2*/ 	{ 0x2,        	0x11,		0x11,  	},     		//CVE_INPUT_CTL_VAL         (clrbar_mode,yc_delay,uv_order,bypass_yclamp)
/*3*/ 	{ 0x4,        	0x09,		0x09,  	},      	//CVE_OUT_CTL_VAL           (rgb_sync,rgb_setup,comp_yuv,compchgain,cvbs_enable)
/*4*/ 	{ 0x6,        	0x01,		0x01,   },      	//CVE_SLV_MD_TH_VAL         (slave_thresh,slave_mode)
/*5*/ 	{ 0x7,        	0x00,		0x34,  	},     		//CVE_VID_STD_VAL           (invert_top,vsync5,secam_mode,pal_mode,trisync_mode,progressive_mode)
/*6*/ 	{ 0x8,         	0x02,		0x02,   },      	//CVE_NUM_LINES_H_VAL
/*7*/ 	{ 0x9,         	0x0D,		0x71, 	},    		//CVE_NUM_LINES_L_VAL
/*8*/ 	{ 0xa,         	21,		23,  	},     		//CVE_FST_VID_LINE_VAL
/*9*/ 	{ 0xb,         	126,		126, 	},    		//CVE_HSYNC_WIDTH_VAL
/*10*/ 	{ 0xd,         	118,		138, 	},    		//CVE_BACK_PORCH_VAL
/*11*/ 	{ 0xe,         	0x00,		0x00,   },      	//CVE_FRONT_PORCH_H_VAL
/*12*/ 	{ 0xf,         	32,		24,  	},     		//CVE_FRONT_PORCH_L_VAL
/*13*/ 	{ 0x10,        	5 ,		5,   	},      	//CVE_ACTIVELINE_H_VAL
/*14*/ 	{ 0x11,        	160,		160, 	},    		//CVE_ACTIVELINE_L_VAL
/*15*/ 	{ 0x12,        	68,		64,  	},     		//CVE_BURST_WDITH_VAL
/*16*/ 	{ 0x13,        	22,		26,  	},     		//CVE_BREEZE_WAY_VAL
/*17*/ 	{ 0x16,        	0x10,		0x15,	},      	//CVE_CHROMA_FREQ_3_VAL 
/*18*/ 	{ 0x17,        	0xf8,		0x04,	},      	//CVE_CHROMA_FREQ_2_VAL
/*19*/ 	{ 0x18,        	0x3e,		0xc5,	},      	//CVE_CHROMA_FREQ_1_VAL
/*20*/ 	{ 0x19,        	0x0f,		0x66,	},      	//CVE_CHROMA_FREQ_0_VAL
/*21*/	{ 0x1a,		0x00,		0x00,	},
/*22*/	{ 0x1b,		0x00,		0x00,	},
/*23*/	{ 0x1c,		0x00,		0x00,	},
/*24*/	{ 0x1d,		0x00,		0x00,	},
/*25*/ 	{ 0x1e,        	0x00,		0x00, 	},       	//CVE_CHROMA_PHASE_VAL
/*26*/ 	{ 0x1f,        	0x00,		0x00,   },      	//CVE_TINT_VAL      
/*27*/ 	{ 0x20,        	32,		4,   	},      	//CVE_NA_WI_CH_CH_CP_VAL    (cphase_rst,chroma_bw_modulated,chroma_bw_yuv,wide_bw_modulated,narrow_bw_yuv)
/*28*/ 	{ 0x21,        	60,		44,  	},     		//CVE_CB_BURST_AMP_VAL
/*29*/ 	{ 0x22,        	0,		31,  	},     		//CVE_CR_BURST_AMP_VAL
/*30*/ 	{ 0x23,        	139,		168, 	},    		//CVE_CB_GAIN_VAL            
/*31*/ 	{ 0x24,        	139,		168, 	},    		//CVE_CR_GAIN_VAL            
/*32*/ 	{ 0x27,        	3,		3,   	},      	//CVE_WHITE_LEVEL_H_VAL     
/*33*/ 	{ 0x28,        	32,		32,  	},     		//CVE_WHITE_LEVEL_L_VAL        
/*34*/ 	{ 0x29,        	1,		0,   	},      	//CVE_BLACK_LEVEL_H_VAL      
/*35*/ 	{ 0x2a,        	26,		208, 	},    		//CVE_BLACK_LEVEL_L_VAL
/*36*/ 	{ 0x2b,        	0,		0,   	},      	//CVE_BLANK_LEVEL_H_VAL
/*37*/ 	{ 0x2c,        	240,		251, 	},      	//CVE_BLANK_LEVEL_L_VAL      
/*38*/ 	{ 0x2d,        	0,		0,   	},      	//CVE_VBI_BLANK_LEVEL_H_VAL 
/*39*/ 	{ 0x2e,        	240,		251, 	},    		//CVE_VBI_BLANK_LEVEL_L_VAL  
/*40*/ 	{ 0x2f,        	0,		0,   	},      	//CVE_CLAMP_LEVEL_H_VAL 
/*41*/ 	{ 0x30,        	0,		0,   	},      	//CVE_CLAMP_LEVEL_L_VAL
/*42*/ 	{ 0x31,        	16,		8,  	},     		//CVE_SYNC_LEVEL_VAL
/*43*/ 	{ 0x33,        	1,		1,   	},      	//CVE_SYNC_HIGH_LEVEL_H_VAL
/*44*/ 	{ 0x34,        	230,		230, 	},    		//CVE_SYNC_HIGH_LEVEL_L_VAL  
/*45*/ 	{ 0x37,        	8,		8,   	},      	//CVE_NOTCH_EN_WI_FR_VAL    (notch_freq,notch_wide,notch_en)
/*46*/ 	{ 0x38,        	0,		0,   	},      	//CVE_NO_SH_NO_VAL          (noise_gain,sharpen_gain,noise_filter) 
/*47*/ 	{ 0x39,        	5,		5,   	},      	//CVE_NOISE_THRESHOLD_VAL   
/*48*/ 	{ 0x3a,        	10,		10,  	},     		
};
#endif

typedef struct __VIDENCINFO_S_
{
	/* current videnc module video output format; */
	VIDEOFMT_E 	vfmt;
	/* current video oputput port; */
	VIDOUTPORT_E	vop;
	/* current videnc module output color bar id; */
	int			uiClrBar;
	
} VIDENDINFO_S;

static VIDENDINFO_S VideoEncInfo =
{
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF
};

static int CS_VidEnc_OutputClrBar ( int bClrBar )
{
	unsigned char RegVal = 0;
	unsigned int RegOffset = 0x02;
	
	if ( 1 == bClrBar )
	{
		/* show color bar */
		RegVal |= 0x20;
	}
	else
	{
		/* clear up color bar */
		RegVal &= ~0x20;
	}
	
	WriteReg8 ( CVE_REG_BASE + RegOffset, RegVal );
	/* save we setting */
	
	return 0;
}

static int CS_SetVideoTimeFmt ( VIDEOFMT_E vfmt )
{
#ifdef CONFIG_ARCH_CSM1201
	unsigned int val32 = 0;
	
	if ( ( VIDENC_NTSC == vfmt ) ||
	     ( VIDENC_PAL == vfmt ) ||
		 ( VIDENC_PAL_M == vfmt ) ||
		 ( VIDENC_PAL_N == vfmt ) ||
		 ( VIDENC_PAL_CN == vfmt ) ||
	     ( VIDENC_SECAM == vfmt ) )
	{	
		val32 = 0x145;
	}
	else if ( VIDENC_P480 == vfmt )
	{
		val32 = 0x126;
		return -1;
	}
	else if ( ( VIDENC_P720 == vfmt ) || ( VIDENC_I1080 == vfmt ) )
	{
		val32 = 0x14A;
	}
	else
	{
		return -1;
	}
	WriteReg32 ( 0x10171108, (unsigned short)val32 );
#else
	unsigned int val16 = 0;

	val16 = ReadReg16(0x10171108);

    if ( ( VIDENC_NTSC == vfmt ) || ( VIDENC_PAL == vfmt ) )
    {	
	val16 &= (~(0x3 << 4));
	val16 &= (~(0x1 << 3));
	WriteReg16 ( 0x10171108, (unsigned short)val16 );

	WriteReg16 ( 0x10171000 + 0x40C, 0x8618 );

	val16 |= ((0x1 << 3));
	val16 |= ((0x1 << 4));
	WriteReg16 ( 0x10171108, (unsigned short)val16 );
    }
    else
    {
	return -1;
    }
	WriteReg16 ( 0x10171108, (unsigned short)val16 );
#endif
    

    return 0;
}

static int CS_VidEnc_SetVidFmt ( VIDEOFMT_E vfmt )
{
	unsigned char val8 = 0;
	unsigned int	iCnt = 0;

	if ( 0 != CS_SetVideoTimeFmt ( vfmt ) )
	{
		printf("CS_SetVideoTimeFmt : error\n");
		return -1;
	}

	WriteReg8 ( CVE_REG_BASE, 1 );
#ifdef CONFIG_ARCH_CSM1201
	for ( iCnt = 0; iCnt < sizeof ( stTVE0ConfReg ) / sizeof ( CVE_REG_S ); iCnt++ )
	{
		switch ( vfmt )
		{
			case VIDENC_NTSC:
				val8 = stTVE0ConfReg[iCnt].ntsc_val;
				break;
			case VIDENC_PAL:
				val8 = stTVE0ConfReg[iCnt].pal_val;
				break;
			case VIDENC_PAL_M:	
				val8 = stTVE0ConfReg[iCnt].pal_m_val;
				break;
			case VIDENC_PAL_N:	
				val8 = stTVE0ConfReg[iCnt].pal_n_val;
				break;
			case VIDENC_PAL_CN:	
				val8 = stTVE0ConfReg[iCnt].pal_cn_val;
				break;
			case VIDENC_SECAM:
				val8 = stTVE0ConfReg[iCnt].secam_val;
				break;
			case VIDENC_P480:
				val8 = stTVE0ConfReg[iCnt].p480_val;
				break;
			case VIDENC_P720:
				val8 = stTVE0ConfReg[iCnt].p720_val;
				break;
			case VIDENC_I1080:
				val8 = stTVE0ConfReg[iCnt].i1080_val;
				break;
			default:
				return -1;
				break;
		}
		WriteReg8 ( CVE_REG_BASE + stTVE0ConfReg[iCnt].offset, val8 );
	}
#else
	for ( iCnt = 0; iCnt < sizeof ( stCve5ConfReg ) / sizeof ( CVE_REG_S ); iCnt++ )
	{
		switch ( vfmt )
		{
			case VIDENC_NTSC:
				val8 = stCve5ConfReg[iCnt].ntsc_val;
				break;
			case VIDENC_PAL:
				val8 = stCve5ConfReg[iCnt].pal_val;
				break;
			default:
				return -1;
				break;
		}
		WriteReg8 ( CVE_REG_BASE + stCve5ConfReg[iCnt].offset, val8 );
	}
	WriteReg8 ( CVE_REG_BASE, 0 );
#endif

	if ( 0 != CS_SetVideoFormat ( vfmt ) )
	{
		return -1;
	}

	return 0;
}

static int CS_VidEnc_SetOutputPort ( VIDOUTPORT_E vop )
{
	unsigned char val8 = 0;
	unsigned char val32 = 0;
	unsigned int RegOffset = 0x04;

	switch ( vop )
	{
	    case VIDOUTPORT_CVBS_YC:
		val8 = 0x09;
		val32 = 0;
		break;
	    case VIDOUTPORT_YUV:
		val8 = 0x08;
		val32 = 0;
		break;
#ifdef CONFIG_ARCH_CSM1201
	case VIDOUTPORT_RGB:
		val8 = 0xE1;
		val32 = 0;
		break;
#endif
	    default:
		return -1;
		break;
	}
	WriteReg32 ( 0x10171400, val32 );
	WriteReg8 ( CVE_REG_BASE + RegOffset, val8 );
	
	return 0;
}

static int initstatus;
int gfx_init(int pic_w, int pic_h, int dst_x, int dst_y)
{
	DF_GFX_IMG ColorImg;
	RECT SrcRect;
	RECT DispRect;
	int IsGfxScalerEna;
	int IsFetchOneField;
	int IsFetchTopOnly;
	int DispScreenWidth;
	int DispScreenHeight;
	int IsDispInterlaced;
	int IsProgressive;

	if(initstatus){	/* already initialized */
		printf("DF and TVE have been already initialized\n");
		return -1;
		
	}

#ifdef CONFIG_ARCH_CSM1201
//#if 1

	pic_w = GX_BOOT_H_SIZE;
	pic_h = GX_BOOT_V_SIZE;
	dst_x = 0;
	dst_y = 0;
	IsGfxScalerEna = 0;
	ColorImg.StartAddress = GFXBASEADDRESS;
	ColorImg.PixelWidth = GX_BOOT_H_SIZE;
	ColorImg.PixelHeight = GX_BOOT_V_SIZE;
	ColorImg.ColorFormat = DF_GFX_RGB565;
	ColorImg.DefaultAlpha = 0xb0;
	ColorImg.LinePitch = ColorImg.PixelWidth * 16 / 8;
	ColorImg.ColorFormat = DF_GFX_RGB565;

	DispScreenWidth = 720;
	DispScreenHeight = 576;
	IsDispInterlaced = 1;
	IsFetchOneField = 0;
	IsFetchTopOnly = 0;
	// IsProgressive = 0;
	
	if (GX_BOOT_INTERLACED)
	{
		IsProgressive = 0;
	}
	else
	{
		IsProgressive = 1;
	}
	

	SrcRect.left    = 0;
	SrcRect.top     = 0;
	SrcRect.right   = pic_w;
	SrcRect.bottom  = pic_h;
	DispRect.left   = 0;
	DispRect.top    = 0;
	DispRect.right  = DispScreenWidth;
	DispRect.bottom = DispScreenHeight;


	gpio_hw_set_direct(3, 1);
	gpio_hw_write(3, 0);
	gpio2_hw_set_direct(47, 1);
	gpio2_hw_write(47, 0);
	gpio2_hw_set_direct(48, 1);
	gpio2_hw_write(48, 0);
	printf("pic_w = %d,pic_h = %d,dst_x = %d,dst_y = %d\n",pic_w,pic_h,dst_x,dst_y);
	//DFInit();
	DFSetAnalogOutFmt(0, 0);
	DFSetAnalogOutFmt(1, 2);
	DFSetDigitalOutFmt(1, 0);
	#if 0
	SetTVEncoderClk_TVE1(GX_BOOT_HD, IsProgressive, 1);
	InitTVE1Raw(GX_BOOT_DISP_FORMAT, 0, 1);
	SetTVEncoderClk_TVE0(GX_BOOT_HD, IsProgressive);
	InitTVE0Raw(GX_BOOT_DISP_FORMAT, 0);
	#else
	// SetTVEncoderClk_TVE1(GX_BOOT_HD, IsProgressive, 1);
	// InitTVE1Raw(GX_BOOT_DISP_FORMAT, 0, 1);
	SetTVEncoderClk_TVE1(0, 0, 1);
	InitTVE1Raw(3, 0, 1);
	// SetTVEncoderClk_TVE0(GX_BOOT_HD, IsProgressive);
	// InitTVE0Raw(GX_BOOT_DISP_FORMAT, 0);
	SetTVEncoderClk_TVE0(0, 0);
	InitTVE0Raw(3, 0);
	#endif
#endif
    /* init display */
    CS_LoadDisplay (VIDENC_PAL, &ColorImg,&SrcRect, &DispRect,IsGfxScalerEna,IsFetchOneField, IsFetchTopOnly,
	               				DispScreenWidth, DispScreenHeight, IsDispInterlaced);			/*VIDENC_PAL*/ /*VIDENC_NTSC*/ 

    /* init cve5 */
#ifdef CONFIG_ARCH_CSM1201
	//TestTVE0(0);
	//SetTVEncoderClk_TVE1(2, 1, 1);
	//InitTVE1Raw(2, 1, 1);
#else
	CS_VidEnc_SetVidFmt ( VIDENC_PAL );		/*	*/
	CS_VidEnc_SetOutputPort ( VIDOUTPORT_CVBS_YC );	/* VIDOUTPORT_CVBS_YC */
#endif

    initstatus = 1;
    return 0;
}

#endif

