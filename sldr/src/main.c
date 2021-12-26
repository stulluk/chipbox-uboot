#include "./common/init_ddr.h"
#include "./disp/gfx_disp.h"
#include "./sys/sys_func.h"



int main( int argc, char *argv[] )
{
	int rt = 0;

/* 1. Initial DDR*/
	InitDDR();
#if 0
	TestDDR();
/* 2. Initial Display and Set Back Ground 	*/
/* 3. Display the Context of DDR at 0x02000000 to Gfx Layer*/
 	{
 		unsigned int ImgStartAddr = 0;
		unsigned int ImgWidth     = 0;
		unsigned int ImgHeight    = 0;
		unsigned int ImgFormat    = 0;
 		GfxDispImg
		(
			ImgStartAddr = 0x02000000,
			ImgWidth  = 512, 
			ImgHeight = 320,
			ImgFormat = GFX_CF_RGB565
		);
 	}
#endif
/* 4. Copy the U-BOOT Region*/
 	SysImgCopyToDDR();
/* 5. Set BackGround to Blue*/
/*         DispSetBackGround(0x0, 0xFF, 0x0); 	*/
 	return 1;
}
