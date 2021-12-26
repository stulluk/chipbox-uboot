#ifndef _GFX_DISP_H_
#define _GFX_DISP_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "gfx_cmd_reg_def.h"
void GfxDispImg(unsigned int ImgStartAddr, 
			    unsigned int ImgWidth, 
				unsigned int ImgHeight,
				unsigned int ImgFormat);
void DispSetBackGround(int BGY, int BGU, int BGV);

#ifdef __cplusplus
}
#endif
#endif
