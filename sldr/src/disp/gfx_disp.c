#include "../common/hw_if.h"
#include "../disp/df_hw_def.h"
#include "../cve5/cve5.h"
#include "gfx_disp.h"
#ifdef WIN32_PCI_TEST  
#include <stdio.h>
#include <assert.h>
#else 
#include "../stdlib/c_stdlib.h" 
#endif

void GfxDispImg(unsigned int ImgStartAddr, 
			    unsigned int ImgWidth, 
				unsigned int ImgHeight,
				unsigned int ImgFormat)
{
	unsigned int XMax = 0;
	unsigned int YMax = 0;
	unsigned int IsFieldDisplay = 0;
	unsigned int DispFormat = 0;
	int rt = 0;

	//Map 2D BLT Img Format to DF Gfx Layer Format
	ImgFormat = DF_GFX_RGB565;
	
	DispFormat = DISP_IS_NTSC;
	XMax = 720;
	YMax = 480;
	IsFieldDisplay = 1;
	
	rt = InitCVE5(DispFormat); assert(rt);	
     RegWriteCheck ( DISP_UPDATE_REG,    1) ;
     RegWriteCheck ( DISP_BYTE_ENDIAN,   2) ; //Need to Test to Find a Right Value
     RegWriteCheck ( DISP_NIBBLE_ENDIAN, 0) ;
     RegWriteCheck ( DISP_DISP_ENABLE  , 1) ;
	 RegWriteCheck ( DISP_VIDEO_FORMAT, DispFormat);
     RegWriteCheck ( DISP_VIDEO_ENABLE, 0); 

	//Set Back Groud to Red
	 RegWriteCheck ( DISP_BG_Y, 0x0000000 ) ; 
     RegWriteCheck ( DISP_BG_U, 0x0000000 ) ; 
     RegWriteCheck ( DISP_BG_V, 0x00000FF ) ; 
    
	 RegWriteCheck ( DISP_GFX1_EN, 0x01) ;
	 RegWriteCheck ( DISP_GFX1_FORMAT,  ImgFormat ) ;// clut4 : 0  clut8:1  565:2  4444:3  A0:4
	 RegWriteCheck ( DISP_GFX1_X_START, 0) ;
	 RegWriteCheck ( DISP_GFX1_X_END  , min(ImgWidth,XMax) - 1);//0x0+16*40-1 ) ;//0x0+719 ) ;
	 RegWriteCheck ( DISP_GFX1_Y_START, 0x0 ) ;
	 RegWriteCheck ( DISP_GFX1_Y_END  , min(ImgHeight,YMax)/(IsFieldDisplay + 1)) ;

	 RegWriteCheck ( DISP_GFX1_BUF_START, ImgStartAddr / 16) ;
	 RegWriteCheck ( DISP_GFX1_LINE_PITCH,ImgWidth * 2 / 16) ;


     RegWriteCheck ( DISP_GFX1_Z_ORDER , 0x2);
	 RegWriteCheck ( DISP_GFX1_DEFAULT_ALPHA , 0x000000ff ) ;
	
	 RegWriteCheck ( DISP_UPDATE_REG, 0 ) ;
}


void DispSetBackGround(int BGY, int BGU, int BGV)
{
	RegWriteCheck ( DISP_UPDATE_REG, 1) ;

	RegWriteCheck ( DISP_BG_Y, BGY ) ; 
    RegWriteCheck ( DISP_BG_U, BGU ) ; 
    RegWriteCheck ( DISP_BG_V, BGV ) ; 		
	RegWriteCheck ( DISP_UPDATE_REG, 0) ;
	
}
