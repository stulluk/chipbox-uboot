/*
 * =====================================================================================
 *
 *       Filename:  sketch.c
 *
 *    Description:  draw lines/rectangles and so on
 *    		    ** Only for PAL **
 *
 *        Version:  1.0
 *        Created:  2008年04月2日 15时18分40秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (xm.chen) 
 *        Company:  CelestialSemi Corp. 
 *
 * =====================================================================================
 */
#include <common.h>
#include "sketch.h"
#include "font.h"

#ifdef CFG_SKETCH
#ifndef CFG_GFX
#error you should define "CFG_GFX" in configration file first
#endif

#include "gfx.h"

#define SWAP_TWO_BYTES(c) c
//#define SWAP_TWO_BYTES(c) (((c) >> 8) | (((c) & 0xff) << 8))

static U16 getcolor(Color_t* Color_p)
{
    U16 c;
    switch(Color_p->Type)
    {
	case COLOR_TYPE_RGB565:
	    c = SWAP_TWO_BYTES(
		    (Color_p->Value.RGB565.R >> 3) << 11 |
		    (Color_p->Value.RGB565.G >> 2) << 5  |
		    (Color_p->Value.RGB565.B >> 3));
	    break;
	case COLOR_TYPE_ARGB4444:
	    c = SWAP_TWO_BYTES(
		(Color_p->Value.ARGB4444.A >> 4) << 12 |
		(Color_p->Value.ARGB4444.R >> 4) << 8  |
		(Color_p->Value.ARGB4444.G >> 4) << 4  |
		(Color_p->Value.ARGB4444.B >> 4));
	    break;
	case COLOR_TYPE_ARGB1555:
	    c = SWAP_TWO_BYTES(
		(Color_p->Value.ARGB1555.A) << 15 |
		(Color_p->Value.ARGB1555.R >> 3) << 10  |
		(Color_p->Value.ARGB1555.G >> 3) << 5  |
		(Color_p->Value.ARGB1555.B >> 3));
	    break;
    }

    return c;
}

int DrawPixel( S32             X,
               S32             Y,
               Color_t*        Color_p )
{
    U16	*dst;
    U16 c;

    if(X < 0 || X > 719 || Y < 0 || Y > 575)
	return -1;
    if(Color_p == NULL)
	return -1;

    c = getcolor(Color_p);

    dst = (U16 *)(GFXBASEADDRESS) + Y * 720 + X;
    *dst = c;

    return 0;
}

int DrawHLine( S32             StartX,
               S32             StartY,
               U32             Width,
               Color_t*        Color_p )
{
    U16	*dst;
    U16 c;

    if(StartX < 0 || StartX + Width -1 > 719)
	return -1;
    if(StartY < 0)
	return -1;
    if(StartX > 719 || StartY > 575 || Width == 0)
	return -1;
    if(Color_p == NULL)
	return -1;

    c = getcolor(Color_p);

    dst = (U16 *)(GFXBASEADDRESS) + StartY * 720 + StartX;

    while (Width --)
	*dst++ = c;

    return 0;
}

int DrawVLine( S32             StartX,
               S32             StartY,
               U32             Height,
               Color_t*        Color_p )
{
    U16	*dst;
    U16 c;

    if(StartX < 0)
	return -1;
    if(StartY < 0 || StartY + Height > 576)
	return -1;
    if(StartX > 719 || StartY > 575 || Height == 0)
	return -1;
    if(Color_p == NULL)
	return -1;

    c = getcolor(Color_p);

    dst = (U16 *)(GFXBASEADDRESS) + StartY * 720 + StartX;

    while (Height --) 
    {
	*dst = c;
	dst += 720;
    }

    return 0;
}

int DrawLine( S32             StartX,
              S32             StartY,
              S32             EndX,
              S32             EndY,
              Color_t*        Color_p )
{
    S32 X, Y, w, h;
    if(StartX < 0 || StartX > 719 || StartY < 0 || StartY > 575)
	return -1;
    if(EndX < 0 || EndX > 719 || EndY < 0 || EndY > 575)
	return -1;
    if(EndX < StartX)
	return -1;
    if(StartX == EndX)
	return DrawVLine(StartX, StartY, (EndY - StartY + 1), Color_p);
    else if(StartY == EndY)
	return DrawHLine(StartX, StartY, (EndX - StartX + 1), Color_p);
    else {
	w = EndX - StartX;
	h = (EndY - StartY >= 0) ? (EndY - StartY) : (StartY - EndY);
	if(w >= h)
	    for(X = StartX; X <= EndX; X++)
		DrawPixel(X, (StartY + (EndY - StartY) * (X - StartX) / (EndX - StartX)), Color_p);
	else
	    for(Y = StartY; Y <= EndY; Y++)
		DrawPixel((StartX + (EndX - StartX) * (Y - StartY) / (EndY - StartY)), Y, Color_p);
    }
    return 0;
}

static int DrawSingleRectangle( S32             StartX,  
                   S32             StartY,  
                   S32             EndX,
                   S32             EndY,
                   Color_t*        Color_p )
{
    U32 w, h;
    if(StartX >= EndX || StartY >= EndY)
	return -1;

    w = EndX - StartX + 1;
    h = EndY - StartY + 1;

    DrawHLine(StartX, StartY, w, Color_p);
    DrawHLine(StartX, EndY, w, Color_p);
    DrawVLine(StartX, StartY, h, Color_p);
    DrawVLine(EndX, StartY, h, Color_p);

    return 0;
}

int DrawRectangle( S32             StartX,  
                   S32             StartY,  
                   S32             EndX,
                   S32             EndY,
		   S32		   LineWidth,
                   Color_t*        Color_p )
{
    S32 lw;
    if(StartX >= EndX || StartY >= EndY)
	return -1;
    if(LineWidth <= 0)
	return 0;
    for(lw = LineWidth - 1; lw >= 0; lw--)
	DrawSingleRectangle( StartX + lw,  StartY + lw,  EndX - lw, EndY - lw, Color_p );

    return 0;
}

int FillRectangle( S32             StartX,  
                   S32             StartY,  
                   S32             EndX,
                   S32             EndY,
                   Color_t*        Color_p )
{
    U32 w;
    int i;
    if(StartX >= EndX || StartY >= EndY)
	return -1;

    w = EndX - StartX + 1;
    for(i = StartY; i <= EndY; i++)
	DrawHLine(StartX, i, w, Color_p);

    return 0;
}

/* draw ASCII font */
static int DrawAChar( S32             StartX,
              S32             StartY,
	      U8*	      AChar,
              Color_t*        BgColor_p,
              Color_t*        FgColor_p )
{
    U16 bitmap[FONT_HEIGHT*FONT_WIDTH/2];
    U16 *pbitmap;
    U16	*dst;
    U16 bg, fg;		//background color, foreground color
    U8* font;
    U16 bit;		//bit of ASCII bitmap
    int x,y,idx;
    int ascii_amount;
    int i;

    if(StartX < 0 || StartY < 0 || StartX > 719 || StartY > 575 )
	return -1;
    if(BgColor_p == NULL || FgColor_p == NULL)
	return -1;
    if(AChar == NULL)
	return -1;
    if(*AChar == '\0')
	return 0;

    pbitmap = &bitmap[0];
    bg = getcolor(BgColor_p);
    fg = getcolor(FgColor_p);

    dst = (U16 *)(GFXBASEADDRESS) + StartY * 720 + StartX;

    ascii_amount = Get_ASCII_Amount();
    font = NULL;
    for(i = 0; i < ascii_amount; i++)
    {
	if( ascii_char_set[i].ascii_code == *AChar )
	{
	    font = &ascii_char_set[i].bitmap[0];
/*--------------------------------------------------
* printf("find a AChar!%d\n",ascii_char_set[i].ascii_code);
*--------------------------------------------------*/
	    break;
	}
    }

    if(font == NULL)
	return -1;

    /* draw bitmap to the buffer 'bitmap' */

    for (y = 0; y < FONT_HEIGHT/2; y++)
	for (x = 0; x < FONT_WIDTH; x++) 
	{
	    idx = x & 0x7;
	    if (idx == 0)
		bit = *font++;

	    if (bit & (128 >> idx)) 
		*pbitmap++ = fg;
	    else
		*pbitmap++ = bg;
	}		

    pbitmap = (U16*)bitmap; //rewind the pointer

    /* draw the bitmap to screen */

    for(i = 0; i < FONT_HEIGHT/8; i++)
	for(x = 0; x < FONT_WIDTH/2; x++)
	    for(y = 7; y >= 0; y--)
		*(dst + 720*(y+8*i) + x) = *pbitmap++;

    return 0;
}

int DrawText( S32             StartX,
              S32             StartY,
	      U8*	      Text,
              Color_t*        BgColor_p,
              Color_t*        FgColor_p )
{
    U8* pchar = Text;
    if(StartX < 0 || StartY < 0 || StartX > 719 || StartY > 575 )
	return -1;
    if(BgColor_p == NULL || FgColor_p == NULL)
	return -1;
    if(Text == NULL)
	return -1;
    if(*Text == '\0')
	return 0;
    do {
	if(*pchar < 128)	/* ASCII code */
	{
	    DrawAChar( StartX, StartY, pchar, BgColor_p, FgColor_p );
	    StartX += FONT_WIDTH/2;
	    pchar++;
	}
    } while(pchar != NULL && *pchar != 0);

    return 0;
}

#endif /* CFG_SKETCH */
