/*
 * =====================================================================================
 * 
 *       Filename:  sketch.h
 * 
 *    Description:  header file for sketch.c
 * 
 *        Version:  1.0
 *        Created:  2008年04月2日 15时30分35秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:   (xm.chen) 
 *        Company:  
 * 
 * =====================================================================================
 */

#ifndef __SKETCH_H__
#define __SKETCH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <type_def.h>     // by kb :20100416
#if 0     // by kb :20100416
#ifndef U32
typedef unsigned int U32;
#endif
#ifndef U16
typedef unsigned short U16;
#endif
#ifndef U8
typedef unsigned char U8;
#endif
#ifndef S32
typedef int S32;
#endif
#ifndef S16
typedef short S16;
#endif
#endif

typedef enum ColorType_e
{
    COLOR_TYPE_RGB565 = 2,
    COLOR_TYPE_ARGB4444 = 3,
    COLOR_TYPE_ARGB1555 = 5,
} ColorType_t;

typedef struct ColorRGB_s
{
    U8 R;
    U8 G;
    U8 B;
} ColorRGB_t;

typedef struct ColorARGB_s
{
    U8 A;
    U8 R;
    U8 G;
    U8 B;
} ColorARGB_t;

typedef union ColorValue_u
{
    ColorRGB_t            RGB565;
    ColorARGB_t           ARGB1555;
    ColorARGB_t           ARGB4444;
} ColorValue_t;

typedef struct Color_s
{
  ColorType_t            Type;
  ColorValue_t           Value;
} Color_t;

/* draw one pixel */
int DrawPixel( S32             X,
               S32             Y,
               Color_t*        Color_p );

/* draw one horizontal line */
int DrawHLine( S32             StartX,
               S32             StartY,
               U32             Width,
               Color_t*        Color_p );

/* draw one vertical line */
int DrawVLine( S32             StartX,
               S32             StartY,
               U32             Height,
               Color_t*        Color_p );

/* draw one normal line (horizontal, vertical or inclined) */
int DrawLine( S32             StartX,
              S32             StartY,
              S32             EndX,
              S32             EndY,
              Color_t*        Color_p );

/* draw one rectangle */
int DrawRectangle( S32             StartX,  
                   S32             StartY,  
                   S32             EndX,
                   S32             EndY,
		   S32		   LineWidth,
                   Color_t*        Color_p );

/* fill one rectangle */
int FillRectangle( S32             StartX,  
                   S32             StartY,  
                   S32             EndX,
                   S32             EndY,
                   Color_t*        Color_p );

/* draw ASCII string 
   BgColor_p: background color
   FgColor_p: foreground color
*/
int DrawText( S32             StartX,
              S32             StartY,
	      U8*	      Text,
              Color_t*        BgColor_p,
              Color_t*        FgColor_p );

#ifdef __cplusplus
}
#endif

#endif //

