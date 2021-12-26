/* MAX screen region (0, 0) ~ (719, 575) */
/* TV set is PAL */

#include <common.h>
#include <command.h>
#include <asm/byteorder.h>
#include <malloc.h>

#ifdef CFG_CMD_CSLOGO
#ifndef CFG_GFX
#error you should define "CFG_GFX" in configration file first
#endif

#include "gfx.h"

int DrawPicture(unsigned char *pic_buf, int pic_w, int pic_h, int dst_x, int dst_y);

int do_cslogo(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong addr, w, h, x, y;
	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtoul(argv[1], NULL, 16);
	if (argc == 2) 
	    return DrawPicture( (unsigned char*)addr, 720, 576, 0, 0);

	if (argc != 6)  {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	w = simple_strtoul(argv[2], NULL, 0);
	h = simple_strtoul(argv[3], NULL, 0);
	x = simple_strtoul(argv[4], NULL, 0);
	y = simple_strtoul(argv[5], NULL, 0);

	if(w > 720 || h > 576 || x > 720 || y > 576 || w + x > 720 || h + y > 576)
	{
	    printf("picture is too large!\n");
	    return 1;
	}
	else
	return DrawPicture( (unsigned char*)addr, (int)w, (int)h, (int)x, (int)y);
}

U_BOOT_CMD(
	logo,	CFG_MAXARGS/*5*/,	1,	do_cslogo,
	"logo    -display logo picture on TV Screen",
	"start width height dstX dstY\n"
	"    - display picture from flash addr 'start' to screen\n"
	"    - start(hex): 	flash start address\n"
	"    - width(decimal): 	picture width\n"
	"    - height(decimal): picture height\n"
	"    - dstX(decimal): 	the top-left X coordinate of screen\n"
	"    - dstY(decimal): 	the top-left Y coordinate of screen\n"
	"logo start: Example\n"
	"    - logo start 720 576 0 0 "
	);

int DrawPicture(unsigned char *pic_buf, int pic_w, int pic_h, int dst_x, int dst_y)
{
    int i;
    unsigned char *psrcbuf;
    unsigned char *pdstbuf;

    /* clear screen */
    memset((void*)GFXBASEADDRESS, 0, GFXLINEPITCHSIZE * GX_BOOT_V_SIZE);
    
    /* hardware init (gfx0,display,tve) */
    gfx_init(pic_w, pic_h, dst_x, dst_y);

    psrcbuf = (unsigned char*)pic_buf;
    pdstbuf = (unsigned char*)(GFXBASEADDRESS + (dst_y * GFXLINEPITCHSIZE) + dst_x * 2);
    for(i = 0; i < pic_h; i++)
    {
		memcpy(pdstbuf, psrcbuf, pic_w * 2);

		psrcbuf += (pic_w*2);
		pdstbuf += GFXLINEPITCHSIZE;
    }

    return 0;
}

#endif /* CFG_CMD_CSLOGO */

