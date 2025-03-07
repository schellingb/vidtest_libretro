/*
graphics primitives taken from SDL_gfxPrimitives
(minimal implementation for use with RSDL_wrapper)

Altered source only for use with RSDL_wrapper
RETRO HACK This is just an SDL wrapper for liretro core.

original copyright follows:

RSDL_gfxPrimitives.h: graphics primitives for SDL

Copyright (C) 2001-2012  Andreas Schiffler

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#ifndef M_PI
#define M_PI	3.1415926535897932384626433832795
#endif

#ifndef NK_RSDL_H_
#define NK_RSDL_H_

#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif

#ifndef NK_RSDL_MAX_POINTS
#define NK_RSDL_MAX_POINTS 128
#endif

/* RETRO HACK
 * warning just an SDL wrapper for use SDL surface in the core.
 */

/* The number of elements in a table */
#define RSDL_TABLESIZE(table)	(sizeof(table)/sizeof(table[0]))

typedef unsigned char	Uint8;
typedef signed char	Sint8;
typedef unsigned short	Uint16;
typedef signed short	Sint16;
typedef unsigned int	Uint32;
typedef signed int	Sint32;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int   int32;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int  uint32;

/* Figure out how to support 64-bit datatypes */
#if !defined(__STRICT_ANSI__)
#if defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_C)
#define RSDL_HAS_64BIT_TYPE	 long
#elif defined(_MSC_VER) /* VC++ */
#define RSDL_HAS_64BIT_TYPE	__int64
#endif
#endif


#define Uint64 unsigned long long 
#define Sint64 signed long long 

#undef RSDL_HAS_64BIT_TYPE

/* General keyboard/mouse state definitions */
enum { RSDL_PRESSED = 0x01, RSDL_RELEASED = 0x00 };


typedef struct{
     Sint16 x, y;
     Uint16 w, h;
} RSDL_Rect;


typedef struct RSDL_Color
{
 Uint8 r;
 Uint8 g;
 Uint8 b;
 Uint8 a;
} RSDL_Color;

#define RSDL_Colour RSDL_Color
typedef struct RSDL_Palette
{
  int ncolors;
  RSDL_Color *colors;
  Uint32 version;
  int refcount;
} RSDL_Palette;
 
typedef struct {
      RSDL_Palette *palette;
      Uint8  BitsPerPixel;
      Uint8  BytesPerPixel;
      Uint8  Rloss, Gloss, Bloss, Aloss;
      Uint8  Rshift, Gshift, Bshift, Ashift;
      Uint32 Rmask, Gmask, Bmask, Amask;
      Uint32 colorkey;
      Uint8  alpha;
} RSDL_PixelFormat;

typedef struct RSDL_Surface {
        Uint32 flags;                           /* Read-only */
        RSDL_PixelFormat *format;                /* Read-only */
        int w, h;                               /* Read-only */
        Uint16 pitch;                           /* Read-only */
        void *pixels;                           /* Read-write */

        /* clipping information */
        RSDL_Rect clip_rect;                     /* Read-only */

        /* Reference count -- used when freeing surface */
        int refcount;                           /* Read-mostly */

	/* This structure also contains private fields not shown here */
} RSDL_Surface;


static unsigned short RSDL_Swap16(unsigned short x){
	unsigned short result= ((x<<8)|(x>>8)); 
return result;
}
static unsigned RSDL_Swap32(unsigned x){
	unsigned result= ((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
 return result;
}


#ifdef WORDS_BIGENDIAN
#define RSDL_SwapLE16(X)	RSDL_Swap16(X)
#define RSDL_SwapLE32(X)   RSDL_Swap32(X)

#define RSDL_SwapBE16(X)   (X)
#define RSDL_SwapBE32(X)   (X)
#else
#define RSDL_SwapLE16(X)	(X)
#define RSDL_SwapLE32(X)   (X)

#define RSDL_SwapBE16(X)   RSDL_Swap16(X)
#define RSDL_SwapBE32(X)   RSDL_Swap32(X)

#endif

#define RSDL_LIL_ENDIAN	1234
#define RSDL_BIG_ENDIAN	4321

#ifdef WORDS_BIGENDIAN
#define RSDL_BYTEORDER RSDL_BIG_ENDIAN
#else
#define RSDL_BYTEORDER RSDL_LIL_ENDIAN         
#endif

#ifdef __cplusplus
typedef bool RSDL_bool;
#define RSDL_FALSE false
#define RSDL_TRUE  true
#else
typedef enum {
	RSDL_FALSE = 0,
	RSDL_TRUE  = 1
} RSDL_bool;
#endif

#define RSDL_MapRGB(fmt, r, g, b) Retro_MapRGB(fmt, r, g, b)
#define RSDL_MapRGBA(fmt, r, g, b,a) Retro_MapRGBA(fmt, r, g, b,a)

static const char *cross[] = {
  "X                               ",
  "XX                              ",
  "X.X                             ",
  "X..X                            ",
  "X...X                           ",
  "X....X                          ",
  "X.....X                         ",
  "X......X                        ",
  "X.......X                       ",
  "X........X                      ",
  "X.....XXXXX                     ",
  "X..X..X                         ",
  "X.X X..X                        ",
  "XX  X..X                        ",
  "X    X..X                       ",
  "     X..X                       ",
  "      X..X                      ",
  "      X..X                      ",
  "       XX                       ",
  "                                ",
};

#ifdef M16B
RSDL_GFXPRIMITIVES_SCOPE void DrawPointBmp(unsigned short int *buffer,int x, int y, unsigned short int color,int rwidth,int rheight)
#else
RSDL_GFXPRIMITIVES_SCOPE void DrawPointBmp(unsigned int *buffer,int x, int y, unsigned int color,int rwidth,int rheight)
#endif

{
   int idx;

   idx=x+y*rwidth;
   if(idx>=0 && idx<rwidth*rheight)
   	buffer[idx]=color;	
}

RSDL_GFXPRIMITIVES_SCOPE void draw_cross(RSDL_Surface *surface,int x,int y) {

	int i,j,idx;
	int dx=32,dy=20;
#ifdef M16B
	unsigned short int col=0xffff;
#else
	unsigned int col=0xffffffff;
#endif
	int w=surface->w;
	int h=surface->h;

	for(j=y;j<y+dy;j++){
		idx=0;
		for(i=x;i<x+dx;i++){

			if(cross[j-y][idx]=='.')DrawPointBmp((unsigned int*)surface->pixels,i,j,col,w,h);
			else if(cross[j-y][idx]=='X')DrawPointBmp((unsigned int*)surface->pixels,i,j,0,w,h);
			idx++;			
		}
	}
}

RSDL_GFXPRIMITIVES_SCOPE unsigned int Retro_MapRGB(RSDL_PixelFormat *a, int r, int g, int b){

	return (r >> a->Rloss) << a->Rshift
		| (g >> a->Gloss) << a->Gshift
		| (b >> a->Bloss) << a->Bshift
		| a->Amask;
}

RSDL_GFXPRIMITIVES_SCOPE unsigned int Retro_MapRGBA(RSDL_PixelFormat *a, int r, int g, int b,int alpha){

	return (r >> a->Rloss) << a->Rshift
		| (g >> a->Gloss) << a->Gshift
		| (b >> a->Bloss) << a->Bshift
		| ((alpha >> a->Aloss) << a->Ashift & a->Amask);
}

static
RSDL_bool RSDL_IntersectRect(const RSDL_Rect *A, const RSDL_Rect *B, RSDL_Rect *intersection)
{
	int Amin, Amax, Bmin, Bmax;

	/* Horizontal intersection */
	Amin = A->x;
	Amax = Amin + A->w;
	Bmin = B->x;
	Bmax = Bmin + B->w;
	if(Bmin > Amin)
	        Amin = Bmin;
	intersection->x = Amin;
	if(Bmax < Amax)
	        Amax = Bmax;
	intersection->w = Amax - Amin > 0 ? Amax - Amin : 0;

	/* Vertical intersection */
	Amin = A->y;
	Amax = Amin + A->h;
	Bmin = B->y;
	Bmax = Bmin + B->h;
	if(Bmin > Amin)
	        Amin = Bmin;
	intersection->y = Amin;
	if(Bmax < Amax)
	        Amax = Bmax;
	intersection->h = Amax - Amin > 0 ? Amax - Amin : 0;

	return (intersection->w && intersection->h);
}

/*
 * Set the clipping rectangle for a blittable surface
 */
RSDL_GFXPRIMITIVES_SCOPE RSDL_bool RSDL_SetClipRect(RSDL_Surface *surface, const RSDL_Rect *rect)
{
	RSDL_Rect full_rect;

	/* Don't do anything if there's no surface to act on */
	if ( ! surface ) {
		return RSDL_FALSE;
	}

	/* Set up the full surface rectangle */
	full_rect.x = 0;
	full_rect.y = 0;
	full_rect.w = surface->w;
	full_rect.h = surface->h;

	/* Set the clipping rectangle */
	if ( ! rect ) {
		surface->clip_rect = full_rect;
		return 1;
	}
	return RSDL_IntersectRect(rect, &full_rect, &surface->clip_rect);
}

RSDL_GFXPRIMITIVES_SCOPE void RSDL_GetClipRect(RSDL_Surface *surface, RSDL_Rect *rect)
{
	if ( surface && rect ) {
		*rect = surface->clip_rect;
	}
}

RSDL_GFXPRIMITIVES_SCOPE void Retro_FreeSurface(RSDL_Surface *surf )
{   

//printf("free surf format palette color\n");
   if(surf->format->palette->colors)	
      free(surf->format->palette->colors);

//printf("free surf format palette \n");
   if(surf->format->palette)	
      free(surf->format->palette);
//printf("free surf format  \n");
   if(surf->format)	
      free(surf->format);
//printf("free surf pixel  \n"); 
   if(surf->pixels)
      free(surf->pixels);       
//printf("free surf  \n"); 
   if(surf)	
      free(surf);	

}

RSDL_GFXPRIMITIVES_SCOPE RSDL_Surface *Retro_CreateRGBSurface32( int w,int h, int d, int rm,int rg,int rb,int ra)
{
   //printf("s(%d,%d,%d) (%x,%x,%x,%x)\n",w,h,d,rm,rg,rb,ra);

   RSDL_Surface *bitmp;

   bitmp = (RSDL_Surface *) calloc(1, sizeof(*bitmp));
   if (bitmp == NULL)
   {
      printf("tex surface failed");
      return NULL;
   }

   bitmp->format = (RSDL_PixelFormat *) calloc(1,sizeof(*bitmp->format));
   if (bitmp->format == NULL)
   {
      printf("tex format failed");
      return NULL;
   }

   bitmp->format->palette =(RSDL_Palette *) calloc(1,sizeof(*bitmp->format->palette));
   if (bitmp->format->palette == NULL)
   {
      printf("tex format palette failed");
      return NULL;
   }

//printf("create surface XR8G8B8 libretro\n");
//FIXME: why pal for 32 bits surface ?
  bitmp->format->palette->ncolors=256;
  bitmp->format->palette->colors=(RSDL_Color *)malloc(1024);
  bitmp->format->palette->version=0;
  bitmp->format->palette->refcount=0;
  memset(bitmp->format->palette->colors,0x0,1024);

   bitmp->format->BitsPerPixel = 32;
   bitmp->format->BytesPerPixel = 4;
   bitmp->format->Rloss=0;
   bitmp->format->Gloss=0;
   bitmp->format->Bloss=0;
   bitmp->format->Aloss=0;

   bitmp->format->Rshift=16;
   bitmp->format->Gshift=8;
   bitmp->format->Bshift=0;
   bitmp->format->Ashift=24;

   bitmp->format->Rmask=0x00ff0000;   
   bitmp->format->Gmask=0x0000ff00;
   bitmp->format->Bmask=0x000000ff;
   bitmp->format->Amask=0xff000000;

/*
   bitmp->format->Rshift=16;
   bitmp->format->Gshift=8;
   bitmp->format->Bshift=0;
   bitmp->format->Ashift=24;

   bitmp->format->Rmask=0x00ff0000;
   bitmp->format->Gmask=0x0000ff00;
   bitmp->format->Bmask=0x000000ff;
   bitmp->format->Amask=0xff000000;
*/   

   bitmp->format->colorkey=0;
   bitmp->format->alpha=255;//0;
   //bitmp->format->palette = NULL;

   bitmp->flags=0;
   bitmp->w=w;
   bitmp->h=h;
   bitmp->pitch=w*4;
   bitmp->pixels=malloc(sizeof(unsigned char)*h*w*4);//  (unsigned char *)&Retro_Screen[0];
   if (!bitmp->pixels) {
	    printf("failed alloc pixels\n");	
            Retro_FreeSurface(bitmp);            
            return NULL;
   }
   memset(bitmp->pixels,0, h*w*4);

   bitmp->clip_rect.x=0;
   bitmp->clip_rect.y=0;
   bitmp->clip_rect.w=w;
   bitmp->clip_rect.h=h;

   //printf("fin prepare tex:%dx%dx%d\n",bitmp->w,bitmp->h,bitmp->format->BytesPerPixel);
   return bitmp;

}

RSDL_GFXPRIMITIVES_SCOPE RSDL_Surface *Retro_CreateRGBSurface16( int w,int h, int d, int rm,int rg,int rb,int ra)
{
   //printf("s(%d,%d,%d) (%x,%x,%x,%x)\n",w,h,d,rm,rg,rb,ra);

   RSDL_Surface *bitmp;

   bitmp = (RSDL_Surface *) calloc(1, sizeof(*bitmp));
   if (bitmp == NULL)
   {
      printf("tex surface failed");
      return NULL;
   }

   bitmp->format =(RSDL_PixelFormat *) calloc(1,sizeof(*bitmp->format));
   if (bitmp->format == NULL)
   {
      printf("tex format failed");
      return NULL;
   }

   bitmp->format->palette = (RSDL_Palette *)calloc(1,sizeof(*bitmp->format->palette));
   if (bitmp->format->palette == NULL)
   {
      printf("tex format palette failed");
      return NULL;
   }

//printf("create surface RGB565 libretro\n");
//FIXME: why pal for 32 bits surface ?
  bitmp->format->palette->ncolors=256;
  bitmp->format->palette->colors=(RSDL_Color *)malloc(256*2);
  bitmp->format->palette->version=0;
  bitmp->format->palette->refcount=0;
  memset(bitmp->format->palette->colors,0x0,256*2);

   bitmp->format->BitsPerPixel = 16;
   bitmp->format->BytesPerPixel = 2;
   bitmp->format->Rloss=3;
   bitmp->format->Gloss=2;
   bitmp->format->Bloss=3;
   bitmp->format->Aloss=0;

   bitmp->format->Rshift=11;
   bitmp->format->Gshift=5;
   bitmp->format->Bshift=0;
   bitmp->format->Ashift=0;

   bitmp->format->Rmask=0x0000F800;
   bitmp->format->Gmask=0x000007E0;
   bitmp->format->Bmask=0x0000001F;
   bitmp->format->Amask=0x00000000;

/*
   bitmp->format->Rshift=16;
   bitmp->format->Gshift=8;
   bitmp->format->Bshift=0;
   bitmp->format->Ashift=24;

   bitmp->format->Rmask=0x00ff0000;
   bitmp->format->Gmask=0x0000ff00;
   bitmp->format->Bmask=0x000000ff;
   bitmp->format->Amask=0xff000000;
*/   

   bitmp->format->colorkey=0;
   bitmp->format->alpha=255;//0;
   //bitmp->format->palette = NULL;

   bitmp->flags=0;
   bitmp->w=w;
   bitmp->h=h;
   bitmp->pitch=w*2;
   bitmp->pixels=malloc(sizeof(unsigned char)*h*w*2);//  (unsigned char *)&Retro_Screen[0];
   if (!bitmp->pixels) {
	    printf("failed alloc pixels\n");	
            Retro_FreeSurface(bitmp);            
            return NULL;
   }
   memset(bitmp->pixels,0, h*w*2);

   bitmp->clip_rect.x=0;
   bitmp->clip_rect.y=0;
   bitmp->clip_rect.w=w;
   bitmp->clip_rect.h=h;

   //printf("fin prepare tex:%dx%dx%d\n",bitmp->w,bitmp->h,bitmp->format->BytesPerPixel);
   return bitmp;

}

/****************************************************************/
/*    	Apple IIgs emulator                                     */
/*                                                              */
/*    Apple 2GS Original fonts.				        */
/*    All the characters are coded in their original set. 	*/
/*    Only the USA keyboard is recognized with ROM 01.          */ 
/*                                                              */
/****************************************************************/

unsigned char font_array[256*8] 
= {    
0xc7, 0xbb, 0xab, 0xa3, 0xa7, 0xbf, 0xc3, 0xff, 
0xef, 0xd7, 0xbb, 0xbb, 0x83, 0xbb, 0xbb, 0xff, 
0x87, 0xbb, 0xbb, 0x87, 0xbb, 0xbb, 0x87, 0xff, 
0xc7, 0xbb, 0xbf, 0xbf, 0xbf, 0xbb, 0xc7, 0xff, 
0x87, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x87, 0xff, 
0x83, 0xbf, 0xbf, 0x87, 0xbf, 0xbf, 0x83, 0xff, 
0x83, 0xbf, 0xbf, 0x87, 0xbf, 0xbf, 0xbf, 0xff, 
0xc3, 0xbf, 0xbf, 0xbf, 0xb3, 0xbb, 0xc3, 0xff, 
0xbb, 0xbb, 0xbb, 0x83, 0xbb, 0xbb, 0xbb, 0xff, 
0xc7, 0xef, 0xef, 0xef, 0xef, 0xef, 0xc7, 0xff, 
0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xbb, 0xc7, 0xff, 
0xbb, 0xb7, 0xaf, 0x9f, 0xaf, 0xb7, 0xbb, 0xff, 
0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0x83, 0xff, 
0xbb, 0x93, 0xab, 0xab, 0xbb, 0xbb, 0xbb, 0xff, 
0xbb, 0xbb, 0x9b, 0xab, 0xb3, 0xbb, 0xbb, 0xff, 
0xc7, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xc7, 0xff, 
0x87, 0xbb, 0xbb, 0x87, 0xbf, 0xbf, 0xbf, 0xff, 
0xc7, 0xbb, 0xbb, 0xbb, 0xab, 0xb7, 0xcb, 0xff, 
0x87, 0xbb, 0xbb, 0x87, 0xaf, 0xb7, 0xbb, 0xff, 
0xc7, 0xbb, 0xbf, 0xc7, 0xfb, 0xbb, 0xc7, 0xff, 
0x83, 0xef, 0xef, 0xef, 0xef, 0xef, 0xef, 0xff, 
0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xc7, 0xff, 
0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xd7, 0xef, 0xff, 
0xbb, 0xbb, 0xbb, 0xab, 0xab, 0x93, 0xbb, 0xff, 
0xbb, 0xbb, 0xd7, 0xef, 0xd7, 0xbb, 0xbb, 0xff, 
0xbb, 0xbb, 0xd7, 0xef, 0xef, 0xef, 0xef, 0xff, 
0x83, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x83, 0xff, 
0x83, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x83, 0xff, 
0xff, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xff, 0xff, 
0x83, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0x83, 0xff, 
0xff, 0xff, 0xef, 0xd7, 0xbb, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xef, 0xef, 0xef, 0xef, 0xef, 0xff, 0xef, 0xff, 
0xd7, 0xd7, 0xd7, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xd7, 0xd7, 0x83, 0xd7, 0x83, 0xd7, 0xd7, 0xff, 
0xef, 0xc3, 0xaf, 0xc7, 0xeb, 0x87, 0xef, 0xff, 
0x9f, 0x9b, 0xf7, 0xef, 0xdf, 0xb3, 0xf3, 0xff, 
0xdf, 0xaf, 0xaf, 0xdf, 0xab, 0xb7, 0xcb, 0xff, 
0xef, 0xef, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xef, 0xdf, 0xbf, 0xbf, 0xbf, 0xdf, 0xef, 0xff, 
0xef, 0xf7, 0xfb, 0xfb, 0xfb, 0xf7, 0xef, 0xff, 
0xef, 0xab, 0xc7, 0xef, 0xc7, 0xab, 0xef, 0xff, 
0xff, 0xef, 0xef, 0x83, 0xef, 0xef, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xef, 0xef, 0xdf, 0xff, 
0xff, 0xff, 0xff, 0x83, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 
0xff, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0xff, 0xff, 
0xc7, 0xbb, 0xb3, 0xab, 0x9b, 0xbb, 0xc7, 0xff, 
0xef, 0xcf, 0xef, 0xef, 0xef, 0xef, 0xc7, 0xff, 
0xc7, 0xbb, 0xfb, 0xe7, 0xdf, 0xbf, 0x83, 0xff, 
0x83, 0xfb, 0xf7, 0xe7, 0xfb, 0xbb, 0xc7, 0xff, 
0xf7, 0xe7, 0xd7, 0xb7, 0x83, 0xf7, 0xf7, 0xff, 
0x83, 0xbf, 0x87, 0xfb, 0xfb, 0xbb, 0xc7, 0xff, 
0xe3, 0xdf, 0xbf, 0x87, 0xbb, 0xbb, 0xc7, 0xff, 
0x83, 0xfb, 0xf7, 0xef, 0xdf, 0xdf, 0xdf, 0xff, 
0xc7, 0xbb, 0xbb, 0xc7, 0xbb, 0xbb, 0xc7, 0xff, 
0xc7, 0xbb, 0xbb, 0xc3, 0xfb, 0xf7, 0x8f, 0xff, 
0xff, 0xff, 0xef, 0xff, 0xef, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xef, 0xff, 0xef, 0xef, 0xdf, 0xff, 
0xf7, 0xef, 0xdf, 0xbf, 0xdf, 0xef, 0xf7, 0xff, 
0xff, 0xff, 0x83, 0xff, 0x83, 0xff, 0xff, 0xff, 
0xdf, 0xef, 0xf7, 0xfb, 0xf7, 0xef, 0xdf, 0xff, 
0xc7, 0xbb, 0xf7, 0xef, 0xef, 0xff, 0xef, 0xff, 
0x08, 0x10, 0x6c, 0xfe, 0xfc, 0xfc, 0x7e, 0x6c, 
0x08, 0x10, 0x6c, 0x82, 0x84, 0x84, 0x52, 0x6c, 
0x00, 0x00, 0x40, 0x60, 0x70, 0x78, 0x6c, 0x42, 
0xfe, 0x44, 0x28, 0x10, 0x10, 0x28, 0x54, 0xfe, 
0x00, 0x02, 0x04, 0x88, 0x50, 0x20, 0x20, 0x00, 
0xfe, 0xfc, 0xfa, 0x36, 0xae, 0xde, 0xde, 0xfe, 
0xfc, 0xfc, 0xfc, 0xdc, 0x9c, 0x00, 0x9e, 0xde, 
0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0x00, 0xfe, 
0x10, 0x20, 0x40, 0xfe, 0x40, 0x20, 0x10, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 
0x10, 0x10, 0x10, 0x10, 0x92, 0x54, 0x38, 0x10, 
0x10, 0x38, 0x54, 0x92, 0x10, 0x10, 0x10, 0x10, 
0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x02, 0x02, 0x02, 0x22, 0x62, 0xfe, 0x60, 0x20, 
0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 
0xc8, 0x18, 0x38, 0x7e, 0x38, 0x18, 0x08, 0xf6, 
0x26, 0x30, 0x38, 0xfc, 0x38, 0x30, 0x20, 0xde, 
0x02, 0x12, 0x10, 0xfe, 0x7c, 0x38, 0x12, 0x02, 
0x02, 0x12, 0x38, 0x7c, 0xfe, 0x10, 0x12, 0x02, 
0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xfe, 
0x10, 0x08, 0x04, 0xfe, 0x04, 0x08, 0x10, 0x00, 
0x54, 0xaa, 0x54, 0xaa, 0x54, 0xaa, 0x54, 0xaa, 
0xaa, 0x54, 0xaa, 0x54, 0xaa, 0x54, 0xaa, 0x54, 
0x00, 0x7c, 0x82, 0x80, 0x80, 0x80, 0xfe, 0x00, 
0x00, 0x00, 0xfc, 0x02, 0x02, 0x02, 0xfe, 0x00, 
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00, 
0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 
0x28, 0x28, 0xee, 0x00, 0xee, 0x28, 0x28, 0x00, 
0xfe, 0x02, 0x02, 0x32, 0x32, 0x02, 0x02, 0xfe, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0xdf, 0xef, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xc7, 0xfb, 0xc3, 0xbb, 0xc3, 0xff, 
0xbf, 0xbf, 0x87, 0xbb, 0xbb, 0xbb, 0x87, 0xff, 
0xff, 0xff, 0xc3, 0xbf, 0xbf, 0xbf, 0xc3, 0xff, 
0xfb, 0xfb, 0xc3, 0xbb, 0xbb, 0xbb, 0xc3, 0xff, 
0xff, 0xff, 0xc7, 0xbb, 0x83, 0xbf, 0xc3, 0xff, 
0xe7, 0xdb, 0xdf, 0x87, 0xdf, 0xdf, 0xdf, 0xff, 
0xff, 0xff, 0xc7, 0xbb, 0xbb, 0xc3, 0xfb, 0xc7, 
0xbf, 0xbf, 0x87, 0xbb, 0xbb, 0xbb, 0xbb, 0xff, 
0xef, 0xff, 0xcf, 0xef, 0xef, 0xef, 0xc7, 0xff, 
0xf7, 0xff, 0xe7, 0xf7, 0xf7, 0xf7, 0xb7, 0xcf, 
0xbf, 0xbf, 0xbb, 0xb7, 0x8f, 0xb7, 0xbb, 0xff, 
0xcf, 0xef, 0xef, 0xef, 0xef, 0xef, 0xc7, 0xff, 
0xff, 0xff, 0x93, 0xab, 0xab, 0xab, 0xbb, 0xff, 
0xff, 0xff, 0x87, 0xbb, 0xbb, 0xbb, 0xbb, 0xff, 
0xff, 0xff, 0xc7, 0xbb, 0xbb, 0xbb, 0xc7, 0xff, 
0xff, 0xff, 0x87, 0xbb, 0xbb, 0x87, 0xbf, 0xbf, 
0xff, 0xff, 0xc3, 0xbb, 0xbb, 0xc3, 0xfb, 0xfb, 
0xff, 0xff, 0xa3, 0x9f, 0xbf, 0xbf, 0xbf, 0xff, 
0xff, 0xff, 0xc3, 0xbf, 0xc7, 0xfb, 0x87, 0xff, 
0xdf, 0xdf, 0x87, 0xdf, 0xdf, 0xdb, 0xe7, 0xff, 
0xff, 0xff, 0xbb, 0xbb, 0xbb, 0xb3, 0xcb, 0xff, 
0xff, 0xff, 0xbb, 0xbb, 0xbb, 0xd7, 0xef, 0xff, 
0xff, 0xff, 0xbb, 0xbb, 0xab, 0xab, 0x93, 0xff, 
0xff, 0xff, 0xbb, 0xd7, 0xef, 0xd7, 0xbb, 0xff, 
0xff, 0xff, 0xbb, 0xbb, 0xbb, 0xc3, 0xfb, 0xc7, 
0xff, 0xff, 0x83, 0xf7, 0xef, 0xdf, 0x83, 0xff, 
0xe3, 0xcf, 0xcf, 0x9f, 0xcf, 0xcf, 0xe3, 0xff, 
0xef, 0xef, 0xef, 0xef, 0xef, 0xef, 0xef, 0xef, 
0x8f, 0xe7, 0xe7, 0xf3, 0xe7, 0xe7, 0x8f, 0xff, 
0xcb, 0xa7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xab, 0xd7, 0xab, 0xd7, 0xab, 0xff, 0xff, 
0x38, 0x44, 0x54, 0x5c, 0x58, 0x40, 0x3c, 0x00, 
0x10, 0x28, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x00, 
0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x78, 0x00, 
0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00, 
0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x78, 0x00, 
0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7c, 0x00, 
0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x00, 
0x3c, 0x40, 0x40, 0x40, 0x4c, 0x44, 0x3c, 0x00, 
0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44, 0x00, 
0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x38, 0x00, 
0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00, 
0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x00, 
0x44, 0x6c, 0x54, 0x54, 0x44, 0x44, 0x44, 0x00, 
0x44, 0x44, 0x64, 0x54, 0x4c, 0x44, 0x44, 0x00, 
0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x00, 
0x38, 0x44, 0x44, 0x44, 0x54, 0x48, 0x34, 0x00, 
0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44, 0x00, 
0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, 0x00, 
0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 
0x44, 0x44, 0x44, 0x54, 0x54, 0x6c, 0x44, 0x00, 
0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00, 
0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x7c, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7c, 0x00, 
0x7c, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7c, 0x00, 
0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 
0x7c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x7c, 0x00, 
0x00, 0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 
0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x28, 0x28, 0x7c, 0x28, 0x7c, 0x28, 0x28, 0x00, 
0x10, 0x3c, 0x50, 0x38, 0x14, 0x78, 0x10, 0x00, 
0x60, 0x64, 0x08, 0x10, 0x20, 0x4c, 0x0c, 0x00, 
0x20, 0x50, 0x50, 0x20, 0x54, 0x48, 0x34, 0x00, 
0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x20, 0x40, 0x40, 0x40, 0x20, 0x10, 0x00, 
0x10, 0x08, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00, 
0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10, 0x00, 
0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, 
0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 
0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 
0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38, 0x00, 
0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x38, 0x44, 0x04, 0x18, 0x20, 0x40, 0x7c, 0x00, 
0x7c, 0x04, 0x08, 0x18, 0x04, 0x44, 0x38, 0x00, 
0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x08, 0x00, 
0x7c, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38, 0x00, 
0x1c, 0x20, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00, 
0x7c, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20, 0x00, 
0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00, 
0x38, 0x44, 0x44, 0x3c, 0x04, 0x08, 0x70, 0x00, 
0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x10, 0x00, 0x10, 0x10, 0x20, 0x00, 
0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x00, 
0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00, 0x00, 
0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x00, 
0x38, 0x44, 0x08, 0x10, 0x10, 0x00, 0x10, 0x00, 
0x38, 0x44, 0x54, 0x5c, 0x58, 0x40, 0x3c, 0x00, 
0x10, 0x28, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x00, 
0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x78, 0x00, 
0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00, 
0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x78, 0x00, 
0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7c, 0x00, 
0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x00, 
0x3c, 0x40, 0x40, 0x40, 0x4c, 0x44, 0x3c, 0x00, 
0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44, 0x00, 
0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x38, 0x00, 
0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00, 
0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x00, 
0x44, 0x6c, 0x54, 0x54, 0x44, 0x44, 0x44, 0x00, 
0x44, 0x44, 0x64, 0x54, 0x4c, 0x44, 0x44, 0x00, 
0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x00, 
0x38, 0x44, 0x44, 0x44, 0x54, 0x48, 0x34, 0x00, 
0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44, 0x00, 
0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, 0x00, 
0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 
0x44, 0x44, 0x44, 0x54, 0x54, 0x6c, 0x44, 0x00, 
0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00, 
0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x7c, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7c, 0x00, 
0x7c, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7c, 0x00, 
0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 
0x7c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x7c, 0x00, 
0x00, 0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 
0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x38, 0x04, 0x3c, 0x44, 0x3c, 0x00, 
0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x78, 0x00, 
0x00, 0x00, 0x3c, 0x40, 0x40, 0x40, 0x3c, 0x00, 
0x04, 0x04, 0x3c, 0x44, 0x44, 0x44, 0x3c, 0x00, 
0x00, 0x00, 0x38, 0x44, 0x7c, 0x40, 0x3c, 0x00, 
0x18, 0x24, 0x20, 0x78, 0x20, 0x20, 0x20, 0x00, 
0x00, 0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x38, 
0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00, 
0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x48, 0x30, 
0x40, 0x40, 0x44, 0x48, 0x70, 0x48, 0x44, 0x00, 
0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x00, 0x00, 0x6c, 0x54, 0x54, 0x54, 0x44, 0x00, 
0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00, 
0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 
0x00, 0x00, 0x3c, 0x44, 0x44, 0x3c, 0x04, 0x04, 
0x00, 0x00, 0x5c, 0x60, 0x40, 0x40, 0x40, 0x00, 
0x00, 0x00, 0x3c, 0x40, 0x38, 0x04, 0x78, 0x00, 
0x20, 0x20, 0x78, 0x20, 0x20, 0x24, 0x18, 0x00, 
0x00, 0x00, 0x44, 0x44, 0x44, 0x4c, 0x34, 0x00, 
0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 
0x00, 0x00, 0x44, 0x44, 0x54, 0x54, 0x6c, 0x00, 
0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 
0x00, 0x00, 0x44, 0x44, 0x44, 0x3c, 0x04, 0x38, 
0x00, 0x00, 0x7c, 0x08, 0x10, 0x20, 0x7c, 0x00, 
0x1c, 0x30, 0x30, 0x60, 0x30, 0x30, 0x1c, 0x00, 
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0x70, 0x18, 0x18, 0x0c, 0x18, 0x18, 0x70, 0x00, 
0x34, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x54, 0x28, 0x54, 0x28, 0x54, 0x00, 0x00, 
};

/* Well, this is finally done ! */

#ifdef M16B
RSDL_GFXPRIMITIVES_SCOPE void Retro_Draw_string(RSDL_Surface *surface, signed short int x, signed short int y, const  char *string,unsigned short maxstrlen,unsigned short xscale, unsigned short yscale, unsigned short fg, unsigned short bg)
#else
RSDL_GFXPRIMITIVES_SCOPE void Retro_Draw_string(RSDL_Surface *surface, signed short int x, signed short int y, const  char *string,unsigned short maxstrlen,unsigned short xscale, unsigned short yscale, unsigned  fg, unsigned  bg)
#endif
{
    	int strlen;
    	unsigned char *linesurf;

    	int col, bit;
    	unsigned char b;

    	int xrepeat, yrepeat;
#ifdef M16B
    	signed short int ypixel;
   	unsigned short *yptr; 

	unsigned short*mbuffer=(unsigned short*)surface->pixels;
#else
    	signed  int ypixel;
   	unsigned  *yptr; 

	unsigned *mbuffer=(unsigned*)surface->pixels;
#endif


	#define VIRTUAL_WIDTH surface->w

	if ((surface->clip_rect.w==0) || (surface->clip_rect.h==0)) {
		return;
	}


const Sint16 charWidthLocal = 8;
const Sint16 charHeightLocal = 8;

	Sint16 left, right, top, bottom;
	Sint16 x1, y1, x2, y2;

	left = surface->clip_rect.x;
	x2 = x + charWidthLocal;
	if (x2<left) {
		return;
	} 
	right = surface->clip_rect.x + surface->clip_rect.w - 1;
	x1 = x;
	if (x1>right) {
		return;
	} 
	top = surface->clip_rect.y;
	y2 = y + charHeightLocal;
	if (y2<top) {
		return;
	} 
	bottom = surface->clip_rect.y + surface->clip_rect.h - 1;
	y1 = y;
	if (y1>bottom) {
		return;
	} 


    	if(string==NULL)return;
    	for(strlen = 0; strlen<maxstrlen && string[strlen]; strlen++) {}


	int surfw=strlen * 7 * xscale;
	int surfh=8 * yscale;

#ifdef M16B	
        linesurf =(unsigned char *)malloc(sizeof(unsigned short)*surfw*surfh );
    	yptr = (unsigned short *)&linesurf[0];

#else
        linesurf =(unsigned char *)malloc(sizeof(unsigned )*surfw*surfh );
    	yptr = (unsigned *)&linesurf[0];

#endif
    //	yptr = (unsigned *)&linesurf[0];


	for(ypixel = 0; ypixel<8; ypixel++) {

        	for(col=0; col<strlen; col++) {

            		b = font_array[(string[col]^0x80)*8 + ypixel];

            		for(bit=0; bit<7; bit++, yptr++) {              
				*yptr = (b & (1<<(7-bit))) ? fg : bg;
                		for(xrepeat = 1; xrepeat < xscale; xrepeat++, yptr++)
                    			yptr[1] = *yptr;
                        }
        	}

        	for(yrepeat = 1; yrepeat < yscale; yrepeat++) 
            		for(xrepeat = 0; xrepeat<surfw; xrepeat++, yptr++)
                		*yptr = yptr[-surfw];
           
    	}

#ifdef M16B
    	yptr = (unsigned short*)&linesurf[0];
#else
    	yptr = (unsigned *)&linesurf[0];
#endif

    	for(yrepeat = y; yrepeat < y+ surfh; yrepeat++) 
        	for(xrepeat = x; xrepeat< x+surfw; xrepeat++,yptr++)
             		if(*yptr!=0 && (xrepeat+yrepeat*VIRTUAL_WIDTH) < surface->w*surface->h )mbuffer[xrepeat+yrepeat*VIRTUAL_WIDTH] = *yptr;

	free(linesurf);

}
#ifdef M16B
RSDL_GFXPRIMITIVES_SCOPE void Retro_Draw_char(RSDL_Surface *surface, signed short int x, signed short int y,  char string,unsigned short xscale, unsigned short yscale, unsigned short fg, unsigned short bg)
#else
RSDL_GFXPRIMITIVES_SCOPE void Retro_Draw_char(RSDL_Surface *surface, signed short int x, signed short int y,  char string,unsigned short xscale, unsigned short yscale, unsigned  fg, unsigned  bg)
#endif
{
    	int strlen;
    	unsigned char *linesurf;
  //  	signed  int ypixel;
    	int bit;
    	unsigned char b;

    	int xrepeat, yrepeat;

#ifdef M16B
    	signed short int ypixel;
   	unsigned short *yptr; 

	unsigned short*mbuffer=(unsigned short*)surface->pixels;
#else
    	signed  int ypixel;
   	unsigned  *yptr; 

	unsigned *mbuffer=(unsigned*)surface->pixels;
#endif
 //  	unsigned  *yptr; 

//	unsigned *mbuffer=(unsigned*)surface->pixels;

	#define VIRTUAL_WIDTH surface->w

	if ((surface->clip_rect.w==0) || (surface->clip_rect.h==0)) {
		return;
	}


	const Sint16 charWidthLocal = 7*xscale;
	const Sint16 charHeightLocal = 8*yscale;

	Sint16 left, right, top, bottom;
	Sint16 x1, y1, x2, y2;

	left = surface->clip_rect.x;
	x2 = x + charWidthLocal;
	if (x2<left) {
		return;
	} 
	right = surface->clip_rect.x + surface->clip_rect.w - 1;
	x1 = x;
	if (x1>right) {
		return;
	} 
	top = surface->clip_rect.y;
	y2 = y + charHeightLocal;
	if (y2<top) {
		return;
	} 
	bottom = surface->clip_rect.y + surface->clip_rect.h - 1;
	y1 = y;
	if (y1>bottom) {
		return;
	} 


        strlen = 1;

	int surfw=strlen * 7 * xscale;
	int surfh=8 * yscale;

#ifdef M16B	
        linesurf =(unsigned char *)malloc(sizeof(unsigned short)*surfw*surfh );
    	yptr = (unsigned short *)&linesurf[0];

#else
        linesurf =(unsigned char *)malloc(sizeof(unsigned )*surfw*surfh );
    	yptr = (unsigned *)&linesurf[0];

#endif
 //       linesurf =(unsigned char *)malloc(sizeof(unsigned )*surfw*surfh );
 //   	yptr = (unsigned *)&linesurf[0];


	for(ypixel = 0; ypixel<8; ypixel++) {

        	//for(col=0; col<strlen; col++) {

            		b = font_array[(string^0x80)*8 + ypixel];

            		for(bit=0; bit<7; bit++, yptr++) {              
				*yptr = (b & (1<<(7-bit))) ? fg : bg;
                		for(xrepeat = 1; xrepeat < xscale; xrepeat++, yptr++)
                    			yptr[1] = *yptr;
                        }
        	//}

        	for(yrepeat = 1; yrepeat < yscale; yrepeat++) 
            		for(xrepeat = 0; xrepeat<surfw; xrepeat++, yptr++)
                		*yptr = yptr[-surfw];
           
    	}


#ifdef M16B
    	yptr = (unsigned short*)&linesurf[0];
#else
    	yptr = (unsigned *)&linesurf[0];
#endif
//    	yptr = (unsigned *)&linesurf[0];

    	for(yrepeat = (y<0?0:y); yrepeat < y+ surfh; yrepeat++) 
        	for(xrepeat = x; xrepeat< x+surfw; xrepeat++,yptr++)
             		if(*yptr!=0 && (xrepeat+yrepeat*VIRTUAL_WIDTH) < surface->w*surface->h )mbuffer[xrepeat+yrepeat*VIRTUAL_WIDTH] = *yptr;

	free(linesurf);

}



typedef struct nk_retro_Font nk_retro_Font;
typedef struct nk_retro_event nk_retro_event;

NK_API void nk_retro_init(void);
NK_API void nk_retro_handle_event(int *evt,int poll);
NK_API void nk_retro_render(struct nk_color clear);
NK_API void nk_retro_shutdown(void);

NK_API nk_retro_Font* nk_retrofont_create(const char *name, int size);
NK_API void nk_retrofont_del(nk_retro_Font *font);
NK_API void nk_retro_set_font(nk_retro_Font *font);
NK_API struct nk_retro_event* nk_retro_event_ptr(void);
#endif
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef NK_RETRO_SOFT_IMPLEMENTATION

#ifndef _RSDL_gfxPrimitives_h
#define _RSDL_gfxPrimitives_h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

	/* ----- Versioning */

#define RSDL_GFXPRIMITIVES_MAJOR	2
#define RSDL_GFXPRIMITIVES_MINOR	0
#define RSDL_GFXPRIMITIVES_MICRO	25


	/* ---- Function Prototypes */

#ifdef _MSC_VER
#  if defined(DLL_EXPORT) && !defined(LIBRSDL_GFX_DLL_IMPORT)
#    define RSDL_GFXPRIMITIVES_SCOPE __declspec(dllexport)
#  else
#    ifdef LIBRSDL_GFX_DLL_IMPORT
#      define RSDL_GFXPRIMITIVES_SCOPE __declspec(dllimport)
#    endif
#  endif
#endif
#ifndef RSDL_GFXPRIMITIVES_SCOPE
#  define RSDL_GFXPRIMITIVES_SCOPE extern
#endif

	/* Note: all ___Color routines expect the color to be in format 0xRRGGBBAA */

	/* Pixel */

	RSDL_GFXPRIMITIVES_SCOPE int pixelColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int pixelRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Horizontal line */

	RSDL_GFXPRIMITIVES_SCOPE int hlineColor(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int hlineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Vertical line */

	RSDL_GFXPRIMITIVES_SCOPE int vlineColor(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int vlineRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Rectangle */

	RSDL_GFXPRIMITIVES_SCOPE int rectangleColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int rectangleRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Rounded-Corner Rectangle */

	RSDL_GFXPRIMITIVES_SCOPE int roundedRectangleColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int roundedRectangleRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled rectangle (Box) */

	RSDL_GFXPRIMITIVES_SCOPE int boxColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int boxRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2,
		Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Rounded-Corner Filled rectangle (Box) */

	RSDL_GFXPRIMITIVES_SCOPE int roundedBoxColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int roundedBoxRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2,
		Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Line */

	RSDL_GFXPRIMITIVES_SCOPE int lineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int lineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA Line */

	RSDL_GFXPRIMITIVES_SCOPE int aalineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aalineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Thick Line */
	RSDL_GFXPRIMITIVES_SCOPE int thickLineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, 
		Uint8 width, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int thickLineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, 
		Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Circle */

	RSDL_GFXPRIMITIVES_SCOPE int circleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int circleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Arc */

	RSDL_GFXPRIMITIVES_SCOPE int arcColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int arcRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, 
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA Circle */

	RSDL_GFXPRIMITIVES_SCOPE int aacircleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aacircleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Circle */

	RSDL_GFXPRIMITIVES_SCOPE int filledCircleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledCircleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Ellipse */

	RSDL_GFXPRIMITIVES_SCOPE int ellipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int ellipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA Ellipse */

	RSDL_GFXPRIMITIVES_SCOPE int aaellipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aaellipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Ellipse */

	RSDL_GFXPRIMITIVES_SCOPE int filledEllipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledEllipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Pie */

	RSDL_GFXPRIMITIVES_SCOPE int pieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int pieRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Pie */

	RSDL_GFXPRIMITIVES_SCOPE int filledPieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledPieRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Trigon */

	RSDL_GFXPRIMITIVES_SCOPE int trigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int trigonRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA-Trigon */

	RSDL_GFXPRIMITIVES_SCOPE int aatrigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aatrigonRGBA(RSDL_Surface * dst,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Trigon */

	RSDL_GFXPRIMITIVES_SCOPE int filledTrigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledTrigonRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Polygon */

	RSDL_GFXPRIMITIVES_SCOPE int polygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int polygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy,
		int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* AA-Polygon */

	RSDL_GFXPRIMITIVES_SCOPE int aapolygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int aapolygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy,
		int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Filled Polygon */

	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonRGBA(RSDL_Surface * dst, const Sint16 * vx,
		const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* (Note: These MT versions are required for multi-threaded operation.) */

	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonColorMT(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color, int **polyInts, int *polyAllocated);
	RSDL_GFXPRIMITIVES_SCOPE int filledPolygonRGBAMT(RSDL_Surface * dst, const Sint16 * vx,
		const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
		int **polyInts, int *polyAllocated);

	/* Characters/Strings */

	RSDL_GFXPRIMITIVES_SCOPE void gfxPrimitivesSetFont(const void *fontdata, Uint32 cw, Uint32 ch);
	RSDL_GFXPRIMITIVES_SCOPE void gfxPrimitivesSetFontRotation(Uint32 rotation);
	RSDL_GFXPRIMITIVES_SCOPE int characterColor(RSDL_Surface * dst, Sint16 x, Sint16 y, char c, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int characterRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, char c, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	RSDL_GFXPRIMITIVES_SCOPE int stringColor(RSDL_Surface * dst, Sint16 x, Sint16 y, const char *s, Uint32 color);
	RSDL_GFXPRIMITIVES_SCOPE int stringRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, const char *s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif				/* _RSDL_gfxPrimitives_h */

const unsigned int GFX_ALPHA_ADJUST_ARRAY[256] = {
	0,  /* 0 */
	15,  /* 1 */
	22,  /* 2 */
	27,  /* 3 */
	31,  /* 4 */
	35,  /* 5 */
	39,  /* 6 */
	42,  /* 7 */
	45,  /* 8 */
	47,  /* 9 */
	50,  /* 10 */
	52,  /* 11 */
	55,  /* 12 */
	57,  /* 13 */
	59,  /* 14 */
	61,  /* 15 */
	63,  /* 16 */
	65,  /* 17 */
	67,  /* 18 */
	69,  /* 19 */
	71,  /* 20 */
	73,  /* 21 */
	74,  /* 22 */
	76,  /* 23 */
	78,  /* 24 */
	79,  /* 25 */
	81,  /* 26 */
	82,  /* 27 */
	84,  /* 28 */
	85,  /* 29 */
	87,  /* 30 */
	88,  /* 31 */
	90,  /* 32 */
	91,  /* 33 */
	93,  /* 34 */
	94,  /* 35 */
	95,  /* 36 */
	97,  /* 37 */
	98,  /* 38 */
	99,  /* 39 */
	100,  /* 40 */
	102,  /* 41 */
	103,  /* 42 */
	104,  /* 43 */
	105,  /* 44 */
	107,  /* 45 */
	108,  /* 46 */
	109,  /* 47 */
	110,  /* 48 */
	111,  /* 49 */
	112,  /* 50 */
	114,  /* 51 */
	115,  /* 52 */
	116,  /* 53 */
	117,  /* 54 */
	118,  /* 55 */
	119,  /* 56 */
	120,  /* 57 */
	121,  /* 58 */
	122,  /* 59 */
	123,  /* 60 */
	124,  /* 61 */
	125,  /* 62 */
	126,  /* 63 */
	127,  /* 64 */
	128,  /* 65 */
	129,  /* 66 */
	130,  /* 67 */
	131,  /* 68 */
	132,  /* 69 */
	133,  /* 70 */
	134,  /* 71 */
	135,  /* 72 */
	136,  /* 73 */
	137,  /* 74 */
	138,  /* 75 */
	139,  /* 76 */
	140,  /* 77 */
	141,  /* 78 */
	141,  /* 79 */
	142,  /* 80 */
	143,  /* 81 */
	144,  /* 82 */
	145,  /* 83 */
	146,  /* 84 */
	147,  /* 85 */
	148,  /* 86 */
	148,  /* 87 */
	149,  /* 88 */
	150,  /* 89 */
	151,  /* 90 */
	152,  /* 91 */
	153,  /* 92 */
	153,  /* 93 */
	154,  /* 94 */
	155,  /* 95 */
	156,  /* 96 */
	157,  /* 97 */
	158,  /* 98 */
	158,  /* 99 */
	159,  /* 100 */
	160,  /* 101 */
	161,  /* 102 */
	162,  /* 103 */
	162,  /* 104 */
	163,  /* 105 */
	164,  /* 106 */
	165,  /* 107 */
	165,  /* 108 */
	166,  /* 109 */
	167,  /* 110 */
	168,  /* 111 */
	168,  /* 112 */
	169,  /* 113 */
	170,  /* 114 */
	171,  /* 115 */
	171,  /* 116 */
	172,  /* 117 */
	173,  /* 118 */
	174,  /* 119 */
	174,  /* 120 */
	175,  /* 121 */
	176,  /* 122 */
	177,  /* 123 */
	177,  /* 124 */
	178,  /* 125 */
	179,  /* 126 */
	179,  /* 127 */
	180,  /* 128 */
	181,  /* 129 */
	182,  /* 130 */
	182,  /* 131 */
	183,  /* 132 */
	184,  /* 133 */
	184,  /* 134 */
	185,  /* 135 */
	186,  /* 136 */
	186,  /* 137 */
	187,  /* 138 */
	188,  /* 139 */
	188,  /* 140 */
	189,  /* 141 */
	190,  /* 142 */
	190,  /* 143 */
	191,  /* 144 */
	192,  /* 145 */
	192,  /* 146 */
	193,  /* 147 */
	194,  /* 148 */
	194,  /* 149 */
	195,  /* 150 */
	196,  /* 151 */
	196,  /* 152 */
	197,  /* 153 */
	198,  /* 154 */
	198,  /* 155 */
	199,  /* 156 */
	200,  /* 157 */
	200,  /* 158 */
	201,  /* 159 */
	201,  /* 160 */
	202,  /* 161 */
	203,  /* 162 */
	203,  /* 163 */
	204,  /* 164 */
	205,  /* 165 */
	205,  /* 166 */
	206,  /* 167 */
	206,  /* 168 */
	207,  /* 169 */
	208,  /* 170 */
	208,  /* 171 */
	209,  /* 172 */
	210,  /* 173 */
	210,  /* 174 */
	211,  /* 175 */
	211,  /* 176 */
	212,  /* 177 */
	213,  /* 178 */
	213,  /* 179 */
	214,  /* 180 */
	214,  /* 181 */
	215,  /* 182 */
	216,  /* 183 */
	216,  /* 184 */
	217,  /* 185 */
	217,  /* 186 */
	218,  /* 187 */
	218,  /* 188 */
	219,  /* 189 */
	220,  /* 190 */
	220,  /* 191 */
	221,  /* 192 */
	221,  /* 193 */
	222,  /* 194 */
	222,  /* 195 */
	223,  /* 196 */
	224,  /* 197 */
	224,  /* 198 */
	225,  /* 199 */
	225,  /* 200 */
	226,  /* 201 */
	226,  /* 202 */
	227,  /* 203 */
	228,  /* 204 */
	228,  /* 205 */
	229,  /* 206 */
	229,  /* 207 */
	230,  /* 208 */
	230,  /* 209 */
	231,  /* 210 */
	231,  /* 211 */
	232,  /* 212 */
	233,  /* 213 */
	233,  /* 214 */
	234,  /* 215 */
	234,  /* 216 */
	235,  /* 217 */
	235,  /* 218 */
	236,  /* 219 */
	236,  /* 220 */
	237,  /* 221 */
	237,  /* 222 */
	238,  /* 223 */
	238,  /* 224 */
	239,  /* 225 */
	240,  /* 226 */
	240,  /* 227 */
	241,  /* 228 */
	241,  /* 229 */
	242,  /* 230 */
	242,  /* 231 */
	243,  /* 232 */
	243,  /* 233 */
	244,  /* 234 */
	244,  /* 235 */
	245,  /* 236 */
	245,  /* 237 */
	246,  /* 238 */
	246,  /* 239 */
	247,  /* 240 */
	247,  /* 241 */
	248,  /* 242 */
	248,  /* 243 */
	249,  /* 244 */
	249,  /* 245 */
	250,  /* 246 */
	250,  /* 247 */
	251,  /* 248 */
	251,  /* 249 */
	252,  /* 250 */
	252,  /* 251 */
	253,  /* 252 */
	253,  /* 253 */
	254,  /* 254 */
	255   /* 255 */
};

/* -===================- */

#define DEFAULT_ALPHA_PIXEL_ROUTINE
#undef EXPERIMENTAL_ALPHA_PIXEL_ROUTINE
#define ALPHA_PIXEL_ADDITIVE_BLEND

/* ---- Structures */

/*!
\brief The structure passed to the internal Bresenham iterator.
*/
typedef struct {
	Sint16 x, y;
	int dx, dy, s1, s2, swapdir, error;
	Uint32 count;
} RSDL_gfxBresenhamIterator;

/*!
\brief The structure passed to the internal Murphy iterator.
*/
typedef struct {
	Uint32 color;
	RSDL_Surface *dst;
	int u, v;		/* delta x , delta y */
	int ku, kt, kv, kd;	/* loop constants */
	int oct2;
	int quad4;
	Sint16 last1x, last1y, last2x, last2y, first1x, first1y, first2x, first2y, tempx, tempy;
} RSDL_gfxMurphyIterator;

/* ----- Defines for pixel clipping tests */

#define clip_xmin(surface) surface->clip_rect.x
#define clip_xmax(surface) surface->clip_rect.x+surface->clip_rect.w-1
#define clip_ymin(surface) surface->clip_rect.y
#define clip_ymax(surface) surface->clip_rect.y+surface->clip_rect.h-1

/*!
\brief Internal pixel drawing - fast, no blending, no locking, clipping.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param color The color value of the pixel to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int fastPixelColorNolock(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color)
{
	int bpp;
	Uint8 *p;

	/*
	* Honor clipping setup at pixel level 
	*/
	if ((x >= clip_xmin(dst)) && (x <= clip_xmax(dst)) && (y >= clip_ymin(dst)) && (y <= clip_ymax(dst))) {

		/*
		* Get destination format 
		*/
		bpp = dst->format->BytesPerPixel;
		p = (Uint8 *) dst->pixels + y * dst->pitch + x * bpp;
		switch (bpp) {
		case 1:
			*p = color;
			break;
		case 2:
			*(Uint16 *) p = color;
			break;
		case 3:
			if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
				p[0] = (color >> 16) & 0xff;
				p[1] = (color >> 8) & 0xff;
				p[2] = color & 0xff;
			} else {
				p[0] = color & 0xff;
				p[1] = (color >> 8) & 0xff;
				p[2] = (color >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *) p = color;
			break;
		}			/* switch */


	}

	return (0);
}

/*!
\brief Internal pixel drawing - fast, no blending, no locking, no clipping.

Function is faster but dangerous since no clipping check is done.
Code needs to make sure we stay in surface bounds before calling.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param color The color value of the pixel to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int fastPixelColorNolockNoclip(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color)
{
	int bpp;
	Uint8 *p;

	/*
	* Get destination format 
	*/
	bpp = dst->format->BytesPerPixel;
	p = (Uint8 *) dst->pixels + y * dst->pitch + x * bpp;
	switch (bpp) {
	case 1:
		*p = color;
		break;
	case 2:
		*(Uint16 *) p = color;
		break;
	case 3:
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			p[0] = (color >> 16) & 0xff;
			p[1] = (color >> 8) & 0xff;
			p[2] = color & 0xff;
		} else {
			p[0] = color & 0xff;
			p[1] = (color >> 8) & 0xff;
			p[2] = (color >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *) p = color;
		break;
	}				/* switch */

	return (0);
}

/*!
\brief Internal pixel drawing - fast, no blending, locking, clipping.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param color The color value of the pixel to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int fastPixelColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color)
{
	int result;

	result = fastPixelColorNolock(dst, x, y, color);

	return (result);
}

/*!
\brief Internal pixel drawing - fast, no blending, locking, RGB input.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param r The red value of the pixel to draw. 
\param g The green value of the pixel to draw. 
\param b The blue value of the pixel to draw. 
\param a The alpha value of the pixel to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int fastPixelRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 color;

	/*
	* Setup color 
	*/
	color = RSDL_MapRGBA(dst->format, r, g, b, a);

	/*
	* Draw 
	*/
	return (fastPixelColor(dst, x, y, color));
}

/*!
\brief Internal pixel drawing - fast, no blending, no locking RGB input.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param r The red value of the pixel to draw. 
\param g The green value of the pixel to draw. 
\param b The blue value of the pixel to draw. 
\param a The alpha value of the pixel to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int fastPixelRGBANolock(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 color;

	/*
	* Setup color 
	*/
	color = RSDL_MapRGBA(dst->format, r, g, b, a);

	/*
	* Draw 
	*/
	return (fastPixelColorNolock(dst, x, y, color));
}

/*!
\brief Internal pixel drawing function with alpha blending where input color in in destination format.

Contains two alternative 32 bit alpha blending routines which can be enabled at the source
level with the defines DEFAULT_ALPHA_PIXEL_ROUTINE or EXPERIMENTAL_ALPHA_PIXEL_ROUTINE.
Only the bits up to the surface depth are significant in the color value.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param color The color value of the pixel to draw. 
\param alpha The blend factor to apply while drawing.

\returns Returns 0 on success, -1 on failure.
*/
int _putPixelAlpha(RSDL_Surface *dst, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha)
{
	RSDL_PixelFormat *format;
	Uint32 Rmask, Gmask, Bmask, Amask;
	Uint32 Rshift, Gshift, Bshift, Ashift;
	Uint32 sR, sG, sB;
	Uint32 dR, dG, dB, dA;

	if (dst == NULL)
	{
		return (-1);
	}

	if (x >= clip_xmin(dst) && x <= clip_xmax(dst) && 
		y >= clip_ymin(dst) && y <= clip_ymax(dst)) 
	{

		format = dst->format;

		switch (format->BytesPerPixel) {
		case 1:
			{		/* Assuming 8-bpp */
				Uint8 *pixel = (Uint8 *) dst->pixels + y * dst->pitch + x;
				if (alpha == 255) {
					*pixel = color;
				} else {
					Uint8 R, G, B;
					RSDL_Palette *palette = format->palette;
					RSDL_Color *colors = palette->colors;
					RSDL_Color dColor = colors[*pixel];
					RSDL_Color sColor = colors[color];
					dR = dColor.r;
					dG = dColor.g;
					dB = dColor.b;
					sR = sColor.r;
					sG = sColor.g;
					sB = sColor.b;

					R = dR + ((sR - dR) * alpha >> 8);
					G = dG + ((sG - dG) * alpha >> 8);
					B = dB + ((sB - dB) * alpha >> 8);

					*pixel = RSDL_MapRGB(format, R, G, B);
				}
			}
			break;

		case 2:
			{		/* Probably 15-bpp or 16-bpp */
				Uint16 *pixel = (Uint16 *) dst->pixels + y * dst->pitch / 2 + x;
				if (alpha == 255) {
					*pixel = color;
				} else {
					Uint16 R, G, B, A;
					Uint16 dc = *pixel;

					Rmask = format->Rmask;
					Gmask = format->Gmask;
					Bmask = format->Bmask;
					Amask = format->Amask;

					dR = (dc & Rmask);
					dG = (dc & Gmask);
					dB = (dc & Bmask);

					R = (dR + (((color & Rmask) - dR) * alpha >> 8)) & Rmask;
					G = (dG + (((color & Gmask) - dG) * alpha >> 8)) & Gmask;
					B = (dB + (((color & Bmask) - dB) * alpha >> 8)) & Bmask;
					*pixel = R | G | B;
					if (Amask!=0) {
						dA = (dc & Amask);
						A = (dA + (((color & Amask) - dA) * alpha >> 8)) & Amask;
						*pixel |= A;
					}
				}
			}
			break;

		case 3: 
			{		/* Slow 24-bpp mode, usually not used */
				Uint8 R, G, B;
				Uint8 Rshift8, Gshift8, Bshift8;
				Uint8 *pixel = (Uint8 *) dst->pixels + y * dst->pitch + x * 3;

				Rshift = format->Rshift;
				Gshift = format->Gshift;
				Bshift = format->Bshift;

				Rshift8 = Rshift >> 3;
				Gshift8 = Gshift >> 3;
				Bshift8 = Bshift >> 3;

				sR = (color >> Rshift) & 0xFF;
				sG = (color >> Gshift) & 0xFF;
				sB = (color >> Bshift) & 0xFF;

				if (alpha == 255) {
					*(pixel + Rshift8) = sR;
					*(pixel + Gshift8) = sG;
					*(pixel + Bshift8) = sB;
				} else {
					dR = *((pixel) + Rshift8);
					dG = *((pixel) + Gshift8);
					dB = *((pixel) + Bshift8);

					R = dR + ((sR - dR) * alpha >> 8);
					G = dG + ((sG - dG) * alpha >> 8);
					B = dB + ((sB - dB) * alpha >> 8);

					*((pixel) + Rshift8) = R;
					*((pixel) + Gshift8) = G;
					*((pixel) + Bshift8) = B;
				}
			}
			break;

#ifdef DEFAULT_ALPHA_PIXEL_ROUTINE

		case 4:
			{		/* Probably :-) 32-bpp */
				Uint32 R, G, B, A;
				Uint32 *pixel = (Uint32 *) dst->pixels + y * dst->pitch / 4 + x;
				if (alpha == 255) {
					*pixel = color;
				} else {
					Uint32 dc = *pixel;

					Rmask = format->Rmask;
					Gmask = format->Gmask;
					Bmask = format->Bmask;
					Amask = format->Amask;

					Rshift = format->Rshift;
					Gshift = format->Gshift;
					Bshift = format->Bshift;
					Ashift = format->Ashift;

					dR = (dc & Rmask) >> Rshift;
					dG = (dc & Gmask) >> Gshift;
					dB = (dc & Bmask) >> Bshift;


					R = ((dR + ((((color & Rmask) >> Rshift) - dR) * alpha >> 8)) << Rshift) & Rmask;
					G = ((dG + ((((color & Gmask) >> Gshift) - dG) * alpha >> 8)) << Gshift) & Gmask;
					B = ((dB + ((((color & Bmask) >> Bshift) - dB) * alpha >> 8)) << Bshift) & Bmask;
					*pixel = R | G | B;
					if (Amask!=0) {
						dA = (dc & Amask) >> Ashift;

#ifdef ALPHA_PIXEL_ADDITIVE_BLEND
						A = (dA | GFX_ALPHA_ADJUST_ARRAY[alpha & 255]) << Ashift; // make destination less transparent...
#else
						A = ((dA + ((((color & Amask) >> Ashift) - dA) * alpha >> 8)) << Ashift) & Amask;
#endif
						*pixel |= A;
					}
				}
			}
			break;
#endif

#ifdef EXPERIMENTAL_ALPHA_PIXEL_ROUTINE

		case 4:{		/* Probably :-) 32-bpp */
			if (alpha == 255) {
				*((Uint32 *) dst->pixels + y * dst->pitch / 4 + x) = color;
			} else {
				Uint32 *pixel = (Uint32 *) dst->pixels + y * dst->pitch / 4 + x;
				Uint32 dR, dG, dB, dA;
				Uint32 dc = *pixel;

				Uint32 surfaceAlpha, preMultR, preMultG, preMultB;
				Uint32 aTmp;

				Rmask = format->Rmask;
				Gmask = format->Gmask;
				Bmask = format->Bmask;
				Amask = format->Amask;

				dR = (color & Rmask);
				dG = (color & Gmask);
				dB = (color & Bmask);
				dA = (color & Amask);

				Rshift = format->Rshift;
				Gshift = format->Gshift;
				Bshift = format->Bshift;
				Ashift = format->Ashift;

				preMultR = (alpha * (dR >> Rshift));
				preMultG = (alpha * (dG >> Gshift));
				preMultB = (alpha * (dB >> Bshift));

				surfaceAlpha = ((dc & Amask) >> Ashift);
				aTmp = (255 - alpha);
				if (A = 255 - ((aTmp * (255 - surfaceAlpha)) >> 8 )) {
					aTmp *= surfaceAlpha;
					R = (preMultR + ((aTmp * ((dc & Rmask) >> Rshift)) >> 8)) / A << Rshift & Rmask;
					G = (preMultG + ((aTmp * ((dc & Gmask) >> Gshift)) >> 8)) / A << Gshift & Gmask;
					B = (preMultB + ((aTmp * ((dc & Bmask) >> Bshift)) >> 8)) / A << Bshift & Bmask;
				}
				*pixel = R | G | B | (A << Ashift & Amask);

			}
			   }
			   break;
#endif
		}
	}

	return (0);
}

/*!
\brief Pixel draw with blending enabled if a<255.

\param dst The surface to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.
\param color The color value of the pixel to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int pixelColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color)
{
	Uint8 alpha;
	Uint32 mcolor;
	int result = 0;

	/*
	* Setup color 
	*/
	alpha = color & 0x000000ff;
	mcolor =
		RSDL_MapRGBA(dst->format, (color & 0xff000000) >> 24,
		(color & 0x00ff0000) >> 16, (color & 0x0000ff00) >> 8, alpha);

	/*
	* Draw 
	*/
	result = _putPixelAlpha(dst, x, y, mcolor, alpha);

	return (result);
}

/*!
\brief Pixel draw with blending enabled if a<255 - no surface locking.

\param dst The surface to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.
\param color The color value of the pixel to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int pixelColorNolock(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color)
{
	Uint8 alpha;
	Uint32 mcolor;
	int result = 0;

	/*
	* Setup color 
	*/
	alpha = color & 0x000000ff;
	mcolor =
		RSDL_MapRGBA(dst->format, (color & 0xff000000) >> 24,
		(color & 0x00ff0000) >> 16, (color & 0x0000ff00) >> 8, alpha);

	/*
	* Draw 
	*/
	result = _putPixelAlpha(dst, x, y, mcolor, alpha);

	return (result);
}


/*!
\brief Internal function to draw filled rectangle with alpha blending.

Assumes color is in destination format.

\param dst The surface to draw on.
\param x1 X coordinate of the first corner (upper left) of the rectangle.
\param y1 Y coordinate of the first corner (upper left) of the rectangle.
\param x2 X coordinate of the second corner (lower right) of the rectangle.
\param y2 Y coordinate of the second corner (lower right) of the rectangle.
\param color The color value of the rectangle to draw (0xRRGGBBAA). 
\param alpha Alpha blending amount for pixels.

\returns Returns 0 on success, -1 on failure.
*/
int _filledRectAlpha(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, Uint8 alpha)
{
	RSDL_PixelFormat *format;
	Uint32 Rmask, Gmask, Bmask, Amask;
	Uint32 Rshift, Gshift, Bshift, Ashift;
	Uint32 sR, sG, sB, sA;
	Uint32 dR, dG, dB, dA;
	Sint16 x, y;

	if (dst == NULL) {
		return (-1);
	}

	format = dst->format;
	switch (format->BytesPerPixel) {
	case 1:
		{			/* Assuming 8-bpp */
			Uint8 *row, *pixel;
			Uint8 R, G, B;
			RSDL_Color *colors = format->palette->colors;
			RSDL_Color dColor;
			RSDL_Color sColor = colors[color];
			sR = sColor.r;
			sG = sColor.g;
			sB = sColor.b;

			for (y = y1; y <= y2; y++) {
				row = (Uint8 *) dst->pixels + y * dst->pitch;
				for (x = x1; x <= x2; x++) {
					if (alpha == 255) {
						*(row + x) = color;
					} else {
						pixel = row + x;

						dColor = colors[*pixel];
						dR = dColor.r;
						dG = dColor.g;
						dB = dColor.b;

						R = dR + ((sR - dR) * alpha >> 8);
						G = dG + ((sG - dG) * alpha >> 8);
						B = dB + ((sB - dB) * alpha >> 8);

						*pixel = RSDL_MapRGB(format, R, G, B);
					}
				}
			}
		}
		break;

	case 2:
		{			/* Probably 15-bpp or 16-bpp */
			Uint16 *row, *pixel;
			Uint16 R, G, B, A;
			Uint16 dc;
			Rmask = format->Rmask;
			Gmask = format->Gmask;
			Bmask = format->Bmask;
			Amask = format->Amask;

			sR = (color & Rmask); 
			sG = (color & Gmask);
			sB = (color & Bmask);
			sA = (color & Amask);

			for (y = y1; y <= y2; y++) {
				row = (Uint16 *) dst->pixels + y * dst->pitch / 2;
				for (x = x1; x <= x2; x++) {
					if (alpha == 255) {
						*(row + x) = color;
					} else {
						pixel = row + x;
						dc = *pixel;

						dR = (dc & Rmask);
						dG = (dc & Gmask);
						dB = (dc & Bmask);

						R = (dR + ((sR - dR) * alpha >> 8)) & Rmask;
						G = (dG + ((sG - dG) * alpha >> 8)) & Gmask;
						B = (dB + ((sB - dB) * alpha >> 8)) & Bmask;
						*pixel = R | G | B;
						if (Amask!=0) {
							dA = (dc & Amask);
							A = (dA + ((sA - dA) * alpha >> 8)) & Amask;
							*pixel |= A;
						} 
					}
				}
			}
		}
		break;

	case 3:
		{			/* Slow 24-bpp mode, usually not used */
			Uint8 *row, *pixel;
			Uint8 R, G, B;
			Uint8 Rshift8, Gshift8, Bshift8;

			Rshift = format->Rshift;
			Gshift = format->Gshift;
			Bshift = format->Bshift;

			Rshift8 = Rshift >> 3;
			Gshift8 = Gshift >> 3;
			Bshift8 = Bshift >> 3;

			sR = (color >> Rshift) & 0xff;
			sG = (color >> Gshift) & 0xff;
			sB = (color >> Bshift) & 0xff;

			for (y = y1; y <= y2; y++) {
				row = (Uint8 *) dst->pixels + y * dst->pitch;
				for (x = x1; x <= x2; x++) {
					pixel = row + x * 3;

					if (alpha == 255) {
						*(pixel + Rshift8) = sR;
						*(pixel + Gshift8) = sG;
						*(pixel + Bshift8) = sB;
					} else {
						dR = *((pixel) + Rshift8);
						dG = *((pixel) + Gshift8);
						dB = *((pixel) + Bshift8);

						R = dR + ((sR - dR) * alpha >> 8);
						G = dG + ((sG - dG) * alpha >> 8);
						B = dB + ((sB - dB) * alpha >> 8);

						*((pixel) + Rshift8) = R;
						*((pixel) + Gshift8) = G;
						*((pixel) + Bshift8) = B;
					}
				}
			}
		}
		break;

#ifdef DEFAULT_ALPHA_PIXEL_ROUTINE
	case 4:
		{			/* Probably :-) 32-bpp */
			Uint32 *row, *pixel;
			Uint32 R, G, B, A;
			Uint32 dc;
			Rmask = format->Rmask;
			Gmask = format->Gmask;
			Bmask = format->Bmask;
			Amask = format->Amask;

			Rshift = format->Rshift;
			Gshift = format->Gshift;
			Bshift = format->Bshift;
			Ashift = format->Ashift;

			sR = (color & Rmask) >> Rshift;
			sG = (color & Gmask) >> Gshift;
			sB = (color & Bmask) >> Bshift;
			sA = (color & Amask) >> Ashift;

			for (y = y1; y <= y2; y++) {
				row = (Uint32 *) dst->pixels + y * dst->pitch / 4;
				for (x = x1; x <= x2; x++) {
					if (alpha == 255) {
						*(row + x) = color;
					} else {
						pixel = row + x;
						dc = *pixel;

						dR = (dc & Rmask) >> Rshift;
						dG = (dc & Gmask) >> Gshift;
						dB = (dc & Bmask) >> Bshift;

						R = ((dR + ((sR - dR) * alpha >> 8)) << Rshift) & Rmask;
						G = ((dG + ((sG - dG) * alpha >> 8)) << Gshift) & Gmask;
						B = ((dB + ((sB - dB) * alpha >> 8)) << Bshift) & Bmask;
						*pixel = R | G | B;
						if (Amask!=0) {
							dA = (dc & Amask) >> Ashift;
#ifdef ALPHA_PIXEL_ADDITIVE_BLEND
							A = (dA | GFX_ALPHA_ADJUST_ARRAY[sA & 255]) << Ashift; // make destination less transparent...
#else
							A = ((dA + ((sA - dA) * alpha >> 8)) << Ashift) & Amask;
#endif
							*pixel |= A;
						}
					}
				}
			}
		}
		break;
#endif

#ifdef EXPERIMENTAL_ALPHA_PIXEL_ROUTINE
	case 4:{			/* Probably :-) 32-bpp */
		Uint32 *row, *pixel;
		Uint32 dR, dG, dB, dA;
		Uint32 dc;
		Uint32 surfaceAlpha, preMultR, preMultG, preMultB;
		Uint32 aTmp;

		Rmask = format->Rmask;
		Gmask = format->Gmask;
		Bmask = format->Bmask;
		Amask = format->Amask;

		dR = (color & Rmask);
		dG = (color & Gmask);
		dB = (color & Bmask);
		dA = (color & Amask);

		Rshift = format->Rshift;
		Gshift = format->Gshift;
		Bshift = format->Bshift;
		Ashift = format->Ashift;

		preMultR = (alpha * (dR >> Rshift));
		preMultG = (alpha * (dG >> Gshift));
		preMultB = (alpha * (dB >> Bshift));

		for (y = y1; y <= y2; y++) {
			row = (Uint32 *) dst->pixels + y * dst->pitch / 4;
			for (x = x1; x <= x2; x++) {
				if (alpha == 255) {
					*(row + x) = color;
				} else {
					pixel = row + x;
					dc = *pixel;

					surfaceAlpha = ((dc & Amask) >> Ashift);
					aTmp = (255 - alpha);
					if (A = 255 - ((aTmp * (255 - surfaceAlpha)) >> 8 )) {
						aTmp *= surfaceAlpha;
						R = (preMultR + ((aTmp * ((dc & Rmask) >> Rshift)) >> 8)) / A << Rshift & Rmask;
						G = (preMultG + ((aTmp * ((dc & Gmask) >> Gshift)) >> 8)) / A << Gshift & Gmask;
						B = (preMultB + ((aTmp * ((dc & Bmask) >> Bshift)) >> 8)) / A << Bshift & Bmask;
					}
					*pixel = R | G | B | (A << Ashift & Amask);
				}
			}
		}
		   }
		   break;
#endif

	}

	return (0);
}

/*!
\brief Draw filled rectangle of RGBA color with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first corner (upper left) of the rectangle.
\param y1 Y coordinate of the first corner (upper left) of the rectangle.
\param x2 X coordinate of the second corner (lower right) of the rectangle.
\param y2 Y coordinate of the second corner (lower right) of the rectangle.
\param color The color value of the rectangle to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int filledRectAlpha(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
	Uint8 alpha;
	Uint32 mcolor;
	int result = 0;

	/*
	* Setup color 
	*/
	alpha = color & 0x000000ff;
	mcolor =
		RSDL_MapRGBA(dst->format, (color & 0xff000000) >> 24,
		(color & 0x00ff0000) >> 16, (color & 0x0000ff00) >> 8, alpha);

	/*
	* Draw 
	*/
	result = _filledRectAlpha(dst, x1, y1, x2, y2, mcolor, alpha);

	return (result);
}

/*!
\brief Internal function to draw horizontal line of RGBA color with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param color The color value of the line to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int _HLineAlpha(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color)
{
	return (filledRectAlpha(dst, x1, y, x2, y, color));
}

/*!
\brief Internal function to draw vertical line of RGBA color with alpha blending.

\param dst The surface to draw on.
\param x X coordinate of the points of the line.
\param y1 Y coordinate of the first point (top) of the line.
\param y2 Y coordinate of the second point (bottom) of the line.
\param color The color value of the line to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int _VLineAlpha(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color)
{
	return (filledRectAlpha(dst, x, y1, x, y2, color));
}

/*!
\brief Pixel draw with blending enabled and using alpha weight on color.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param color The color value of the pixel to draw (0xRRGGBBAA). 
\param weight The weight multiplied into the alpha value of the pixel.

\returns Returns 0 on success, -1 on failure.
*/
int pixelColorWeight(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color, Uint32 weight)
{
	Uint32 a;

	/*
	* Get alpha 
	*/
	a = (color & (Uint32) 0x000000ff);

	/*
	* Modify Alpha by weight 
	*/
	a = ((a * weight) >> 8);

	return (pixelColor(dst, x, y, (color & (Uint32) 0xffffff00) | (Uint32) a));
}

/*!
\brief Pixel draw with blending enabled and using alpha weight on color - no locking.

\param dst The surface to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param color The color value of the pixel to draw (0xRRGGBBAA). 
\param weight The weight multiplied into the alpha value of the pixel.

\returns Returns 0 on success, -1 on failure.
*/
int pixelColorWeightNolock(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color, Uint32 weight)
{
	Uint32 a;

	/*
	* Get alpha 
	*/
	a = (color & (Uint32) 0x000000ff);

	/*
	* Modify Alpha by weight 
	*/
	a = ((a * weight) >> 8);

	return (pixelColorNolock(dst, x, y, (color & (Uint32) 0xffffff00) | (Uint32) a));
}

/*!
\brief Pixel draw with blending enabled if a<255.

\param dst The surface to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.
\param r The red color value of the pixel to draw. 
\param g The green color value of the pixel to draw.
\param b The blue color value of the pixel to draw.
\param a The alpha value of the pixel to draw.

\returns Returns 0 on success, -1 on failure.
*/
int pixelRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 color;

	/*
	* Check Alpha 
	*/
	if (a == 255) {
		/*
		* No alpha blending required 
		*/
		/*
		* Setup color 
		*/
		color = RSDL_MapRGBA(dst->format, r, g, b, a);
		/*
		* Draw 
		*/
		return (fastPixelColor(dst, x, y, color));
	} else {
		/*
		* Alpha blending required 
		*/
		/*
		* Draw 
		*/
		return (pixelColor(dst, x, y, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
	}
}


/*!
\brief Draw horizontal line without blending;

Just stores the color value (including the alpha component) without blending.
Only the same number of bits of the destination surface are transfered
from the input color value.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param color The color value of the line to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int hlineColorStore(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color)
{
	Sint16 left, right, top, bottom;
	Uint8 *pixel, *pixellast;
	int dx;
	int pixx, pixy;
	Sint16 w;
	Sint16 xtmp;
	int result = -1;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Swap x1, x2 if required to ensure x1<=x2
	*/
	if (x1 > x2) {
		xtmp = x1;
		x1 = x2;
		x2 = xtmp;
	}

	/*
	* Get clipping boundary and
	* check visibility of hline 
	*/
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	}
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	}
	top = dst->clip_rect.y;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if ((y<top) || (y>bottom)) {
		return (0);
	}

	/*
	* Clip x 
	*/
	if (x1 < left) {
		x1 = left;
	}
	if (x2 > right) {
		x2 = right;
	}

	/*
	* Calculate width 
	*/
	w = x2 - x1;

	/*
	* More variable setup 
	*/
	dx = w;
	pixx = dst->format->BytesPerPixel;
	pixy = dst->pitch;
	pixel = ((Uint8 *) dst->pixels) + pixx * (int) x1 + pixy * (int) y;

	/*
	* Draw 
	*/
	switch (dst->format->BytesPerPixel) {
	case 1:
		memset(pixel, color, dx+1);
		break;
	case 2:
		pixellast = pixel + dx + dx;
		for (; pixel <= pixellast; pixel += pixx) {
			*(Uint16 *) pixel = color;
		}
		break;
	case 3:
		pixellast = pixel + dx + dx + dx;
		for (; pixel <= pixellast; pixel += pixx) {
			if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
				pixel[0] = (color >> 16) & 0xff;
				pixel[1] = (color >> 8) & 0xff;
				pixel[2] = color & 0xff;
			} else {
				pixel[0] = color & 0xff;
				pixel[1] = (color >> 8) & 0xff;
				pixel[2] = (color >> 16) & 0xff;
			}
		}
		break;
	default:		/* case 4 */
		dx = dx + dx;
		pixellast = pixel + dx + dx;
		for (; pixel <= pixellast; pixel += pixx) {
			*(Uint32 *) pixel = color;
		}
		break;
	}

	/*
	* Set result code 
	*/
	result = 0;

	return (result);
}

/*!
\brief Draw horizontal line without blending

Just stores the color value (including the alpha component) without blending.
Function should only be used for 32 bit target surfaces.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int hlineRGBAStore(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (hlineColorStore(dst, x1, x2, y, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/*!
\brief Draw horizontal line with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param color The color value of the line to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int hlineColor(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color)
{
	Sint16 left, right, top, bottom;
	Uint8 *pixel, *pixellast;
	int dx;
	int pixx, pixy;
	Sint16 xtmp;
	int result = -1;
	Uint8 *colorptr;
	Uint8 color3[3];

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Swap x1, x2 if required to ensure x1<=x2
	*/
	if (x1 > x2) {
		xtmp = x1;
		x1 = x2;
		x2 = xtmp;
	}

	/*
	* Get clipping boundary and
	* check visibility of hline 
	*/
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	}
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	}
	top = dst->clip_rect.y;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if ((y<top) || (y>bottom)) {
		return (0);
	}

	/*
	* Clip x 
	*/
	if (x1 < left) {
		x1 = left;
	}
	if (x2 > right) {
		x2 = right;
	}

	/*
	* Calculate width difference
	*/
	dx = x2 - x1;

	/*
	* Alpha check 
	*/
	if ((color & 255) == 255) {

		/*
		* No alpha-blending required 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}

		/*
		* More variable setup 
		*/
		pixx = dst->format->BytesPerPixel;
		pixy = dst->pitch;
		pixel = ((Uint8 *) dst->pixels) + pixx * (int) x1 + pixy * (int) y;

		/*
		* Draw 
		*/
		switch (dst->format->BytesPerPixel) {
		case 1:
			memset(pixel, color, dx + 1);
			break;
		case 2:
			pixellast = pixel + dx + dx;
			for (; pixel <= pixellast; pixel += pixx) {
				*(Uint16 *) pixel = color;
			}
			break;
		case 3:
			pixellast = pixel + dx + dx + dx;
			if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
				color3[0] = (color >> 16) & 0xff;
				color3[1] = (color >> 8) & 0xff;
				color3[2] = color & 0xff;
			} else {
				color3[0] = color & 0xff;
				color3[1] = (color >> 8) & 0xff;
				color3[2] = (color >> 16) & 0xff;
			}
			for (; pixel <= pixellast; pixel += pixx) {
				memcpy(pixel, color3, 3);
			}
			break;
		default:		/* case 4 */
			dx = dx + dx;
			pixellast = pixel + dx + dx;
			for (; pixel <= pixellast; pixel += pixx) {
				*(Uint32 *) pixel = color;
			}
			break;
		}

		/*
		* Set result code 
		*/
		result = 0;

	} else {

		/*
		* Alpha blending blit 
		*/
		result = _HLineAlpha(dst, x1, x1 + dx, y, color);
	}

	return (result);
}

/*!
\brief Draw horizontal line with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int hlineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (hlineColor(dst, x1, x2, y, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/*!
\brief Draw vertical line with blending.

\param dst The surface to draw on.
\param x X coordinate of the points of the line.
\param y1 Y coordinate of the first point (i.e. top) of the line.
\param y2 Y coordinate of the second point (i.e. bottom) of the line.
\param color The color value of the line to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int vlineColor(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color)
{
	Sint16 left, right, top, bottom;
	Uint8 *pixel, *pixellast;
	int dy;
	int pixx, pixy;
	Sint16 h;
	Sint16 ytmp;
	int result = -1;
	Uint8 *colorptr;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Swap y1, y2 if required to ensure y1<=y2
	*/
	if (y1 > y2) {
		ytmp = y1;
		y1 = y2;
		y2 = ytmp;
	}

	/*
	* Get clipping boundary and
	* check visibility of vline 
	*/
	left = dst->clip_rect.x;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if ((x<left) || (x>right)) {
		return (0);
	}    
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	}
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	}

	/*
	* Clip x 
	*/
	if (y1 < top) {
		y1 = top;
	}
	if (y2 > bottom) {
		y2 = bottom;
	}

	/*
	* Calculate height
	*/
	h = y2 - y1;

	/*
	* Alpha check 
	*/
	if ((color & 255) == 255) {

		/*
		* No alpha-blending required 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}

		/*
		* More variable setup 
		*/
		dy = h;
		pixx = dst->format->BytesPerPixel;
		pixy = dst->pitch;
		pixel = ((Uint8 *) dst->pixels) + pixx * (int) x + pixy * (int) y1;
		pixellast = pixel + pixy * dy;

		/*
		* Draw 
		*/
		switch (dst->format->BytesPerPixel) {
		case 1:
			for (; pixel <= pixellast; pixel += pixy) {
				*(Uint8 *) pixel = color;
			}
			break;
		case 2:
			for (; pixel <= pixellast; pixel += pixy) {
				*(Uint16 *) pixel = color;
			}
			break;
		case 3:
			for (; pixel <= pixellast; pixel += pixy) {
				if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
					pixel[0] = (color >> 16) & 0xff;
					pixel[1] = (color >> 8) & 0xff;
					pixel[2] = color & 0xff;
				} else {
					pixel[0] = color & 0xff;
					pixel[1] = (color >> 8) & 0xff;
					pixel[2] = (color >> 16) & 0xff;
				}
			}
			break;
		default:		/* case 4 */
			for (; pixel <= pixellast; pixel += pixy) {
				*(Uint32 *) pixel = color;
			}
			break;
		}

		/*
		* Set result code 
		*/
		result = 0;

	} else {

		/*
		* Alpha blending blit 
		*/

		result = _VLineAlpha(dst, x, y1, y1 + h, color);

	}

	return (result);
}

/*!
\brief Draw vertical line with blending.

\param dst The surface to draw on.
\param x X coordinate of the points of the line.
\param y1 Y coordinate of the first point (i.e. top) of the line.
\param y2 Y coordinate of the second point (i.e. bottom) of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int vlineRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (vlineColor(dst, x, y1, y2, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/*!
\brief Draw rectangle with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the rectangle.
\param y1 Y coordinate of the first point (i.e. top right) of the rectangle.
\param x2 X coordinate of the second point (i.e. bottom left) of the rectangle.
\param y2 Y coordinate of the second point (i.e. bottom left) of the rectangle.
\param color The color value of the rectangle to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int rectangleColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
	int result;
	Sint16 tmp;

	/* Check destination surface */
	if (dst == NULL)
	{
		return -1;
	}

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return 0;
	}

	/*
	* Test for special cases of straight lines or single point 
	*/
	if (x1 == x2) {
		if (y1 == y2) {
			return (pixelColor(dst, x1, y1, color));
		} else {
			return (vlineColor(dst, x1, y1, y2, color));
		}
	} else {
		if (y1 == y2) {
			return (hlineColor(dst, x1, x2, y1, color));
		}
	}

	/*
	* Swap x1, x2 if required 
	*/
	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	/*
	* Swap y1, y2 if required 
	*/
	if (y1 > y2) {
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	/*
	* Draw rectangle 
	*/
	result = 0;
	result |= hlineColor(dst, x1, x2, y1, color);
	result |= hlineColor(dst, x1, x2, y2, color);
	y1 += 1;
	y2 -= 1;
	if (y1 <= y2) {
		result |= vlineColor(dst, x1, y1, y2, color);
		result |= vlineColor(dst, x2, y1, y2, color);
	}

	return (result);

}

/*!
\brief Draw rectangle with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the rectangle.
\param y1 Y coordinate of the first point (i.e. top right) of the rectangle.
\param x2 X coordinate of the second point (i.e. bottom left) of the rectangle.
\param y2 Y coordinate of the second point (i.e. bottom left) of the rectangle.
\param r The red value of the rectangle to draw. 
\param g The green value of the rectangle to draw. 
\param b The blue value of the rectangle to draw. 
\param a The alpha value of the rectangle to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int rectangleRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (rectangleColor
		(dst, x1, y1, x2, y2, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/*!
\brief Draw rounded-corner rectangle with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the rectangle.
\param y1 Y coordinate of the first point (i.e. top right) of the rectangle.
\param x2 X coordinate of the second point (i.e. bottom left) of the rectangle.
\param y2 Y coordinate of the second point (i.e. bottom left) of the rectangle.
\param rad The radius of the corner arc.
\param color The color value of the rectangle to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int roundedRectangleColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color)
{
	int result;
	Sint16 w, h, tmp;
	Sint16 xx1, xx2, yy1, yy2;

	/* 
	* Check destination surface 
	*/
	if (dst == NULL)
	{
		return -1;
	}

	/*
	* Check radius vor valid range
	*/
	if (rad < 0) {
		return -1;
	}

	/*
	* Special case - no rounding
	*/
	if (rad == 0) {
		return rectangleColor(dst, x1, y1, x2, y2, color);
	}

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return 0;
	}

	/*
	* Test for special cases of straight lines or single point 
	*/
	if (x1 == x2) {
		if (y1 == y2) {
			return (pixelColor(dst, x1, y1, color));
		} else {
			return (vlineColor(dst, x1, y1, y2, color));
		}
	} else {
		if (y1 == y2) {
			return (hlineColor(dst, x1, x2, y1, color));
		}
	}

	/*
	* Swap x1, x2 if required 
	*/
	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	/*
	* Swap y1, y2 if required 
	*/
	if (y1 > y2) {
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	/*
	* Calculate width&height 
	*/
	w = x2 - x1;
	h = y2 - y1;

	/*
	* Maybe adjust radius
	*/
	if ((rad * 2) > w)  
	{
		rad = w / 2;
	}
	if ((rad * 2) > h)
	{
		rad = h / 2;
	}

	/*
	* Draw corners
	*/
	result = 0;
	xx1 = x1 + rad;
	xx2 = x2 - rad;
	yy1 = y1 + rad;
	yy2 = y2 - rad;
	result |= arcColor(dst, xx1, yy1, rad, 180, 270, color);
	result |= arcColor(dst, xx2, yy1, rad, 270, 360, color);
	result |= arcColor(dst, xx1, yy2, rad,  90, 180, color);
	result |= arcColor(dst, xx2, yy2, rad,   0,  90, color);

	/*
	* Draw lines
	*/
	if (xx1 <= xx2) {
		result |= hlineColor(dst, xx1, xx2, y1, color);
		result |= hlineColor(dst, xx1, xx2, y2, color);
	}
	if (yy1 <= yy2) {
		result |= vlineColor(dst, x1, yy1, yy2, color);
		result |= vlineColor(dst, x2, yy1, yy2, color);
	}

	return result;
}

/*!
\brief Draw rounded-corner rectangle with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the rectangle.
\param y1 Y coordinate of the first point (i.e. top right) of the rectangle.
\param x2 X coordinate of the second point (i.e. bottom left) of the rectangle.
\param y2 Y coordinate of the second point (i.e. bottom left) of the rectangle.
\param rad The radius of the corner arc.
\param r The red value of the rectangle to draw. 
\param g The green value of the rectangle to draw. 
\param b The blue value of the rectangle to draw. 
\param a The alpha value of the rectangle to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int roundedRectangleRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (roundedRectangleColor
		(dst, x1, y1, x2, y2, rad, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/*!
\brief Draw rounded-corner box (filled rectangle) with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the box.
\param y1 Y coordinate of the first point (i.e. top right) of the box.
\param x2 X coordinate of the second point (i.e. bottom left) of the box.
\param y2 Y coordinate of the second point (i.e. bottom left) of the box.
\param rad The radius of the corner arcs of the box.
\param color The color value of the box to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int roundedBoxColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color)
{
	int result;
	Sint16 w, h, tmp;
	Sint16 xx1, xx2, yy1, yy2;

	/* 
	* Check destination surface 
	*/
	if (dst == NULL)
	{
		return -1;
	}

	/*
	* Check radius vor valid range
	*/
	if (rad < 0) {
		return -1;
	}

	/*
	* Special case - no rounding
	*/
	if (rad == 0) {
		return rectangleColor(dst, x1, y1, x2, y2, color);
	}

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return 0;
	}

	/*
	* Test for special cases of straight lines or single point 
	*/
	if (x1 == x2) {
		if (y1 == y2) {
			return (pixelColor(dst, x1, y1, color));
		} else {
			return (vlineColor(dst, x1, y1, y2, color));
		}
	} else {
		if (y1 == y2) {
			return (hlineColor(dst, x1, x2, y1, color));
		}
	}

	/*
	* Swap x1, x2 if required 
	*/
	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	/*
	* Swap y1, y2 if required 
	*/
	if (y1 > y2) {
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	/*
	* Calculate width&height 
	*/
	w = x2 - x1;
	h = y2 - y1;

	/*
	* Maybe adjust radius
	*/
	if ((rad * 2) > w)  
	{
		rad = w / 2;
	}
	if ((rad * 2) > h)
	{
		rad = h / 2;
	}

	/*
	* Draw corners
	*/
	result = 0;
	xx1 = x1 + rad;
	xx2 = x2 - rad;
	yy1 = y1 + rad;
	yy2 = y2 - rad;
	result |= filledPieColor(dst, xx1, yy1, rad, 180, 270, color);
	result |= filledPieColor(dst, xx2, yy1, rad, 270, 360, color);
	result |= filledPieColor(dst, xx1, yy2, rad,  90, 180, color);
	result |= filledPieColor(dst, xx2, yy2, rad,   0,  90, color);

	/*
	* Draw body
	*/
	xx1++;
	xx2--;
	yy1++;
	yy2--;
	if (xx1 <= xx2) {
		result |= boxColor(dst, xx1, y1, xx2, y2, color);
	}
	if (yy1 <= yy2) {
		result |= boxColor(dst, x1, yy1, xx1-1, yy2, color);
		result |= boxColor(dst, xx2+1, yy1, x2, yy2, color);
	}

	return result;
}

/*!
\brief Draw rounded-corner box (filled rectangle) with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the box.
\param y1 Y coordinate of the first point (i.e. top right) of the box.
\param x2 X coordinate of the second point (i.e. bottom left) of the box.
\param y2 Y coordinate of the second point (i.e. bottom left) of the box.
\param rad The radius of the corner arcs of the box.
\param r The red value of the box to draw. 
\param g The green value of the box to draw. 
\param b The blue value of the box to draw. 
\param a The alpha value of the box to draw. 

\returns Returns 0 on success, -1 on failure.
*/
int roundedBoxRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2,
	Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (roundedBoxColor
		(dst, x1, y1, x2, y2, rad, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* --------- Clipping routines for line */

/* Clipping based heavily on code from                       */
/* http://www.ncsa.uiuc.edu/Vis/Graphics/src/clipCohSuth.c   */

#define CLIP_LEFT_EDGE   0x1
#define CLIP_RIGHT_EDGE  0x2
#define CLIP_BOTTOM_EDGE 0x4
#define CLIP_TOP_EDGE    0x8
#define CLIP_INSIDE(a)   (!a)
#define CLIP_REJECT(a,b) (a&b)
#define CLIP_ACCEPT(a,b) (!(a|b))

/*!
\brief Internal clip-encoding routine.

Calculates a segement-based clipping encoding for a point against a rectangle.

\param x X coordinate of point.
\param y Y coordinate of point.
\param left X coordinate of left edge of the rectangle.
\param top Y coordinate of top edge of the rectangle.
\param right X coordinate of right edge of the rectangle.
\param bottom Y coordinate of bottom edge of the rectangle.
*/
static int _clipEncode(Sint16 x, Sint16 y, Sint16 left, Sint16 top, Sint16 right, Sint16 bottom)
{
	int code = 0;

	if (x < left) {
		code |= CLIP_LEFT_EDGE;
	} else if (x > right) {
		code |= CLIP_RIGHT_EDGE;
	}
	if (y < top) {
		code |= CLIP_TOP_EDGE;
	} else if (y > bottom) {
		code |= CLIP_BOTTOM_EDGE;
	}
	return code;
}

/*!
\brief Clip line to a the clipping rectangle of a surface.

\param dst Target surface to draw on.
\param x1 Pointer to X coordinate of first point of line.
\param y1 Pointer to Y coordinate of first point of line.
\param x2 Pointer to X coordinate of second point of line.
\param y2 Pointer to Y coordinate of second point of line.
*/
static int _clipLine(RSDL_Surface * dst, Sint16 * x1, Sint16 * y1, Sint16 * x2, Sint16 * y2)
{
	Sint16 left, right, top, bottom;
	int code1, code2;
	int draw = 0;
	Sint16 swaptmp;
	float m;

	/*
	* Get clipping boundary 
	*/
	left = dst->clip_rect.x;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	top = dst->clip_rect.y;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;

	while (1) {
		code1 = _clipEncode(*x1, *y1, left, top, right, bottom);
		code2 = _clipEncode(*x2, *y2, left, top, right, bottom);
		if (CLIP_ACCEPT(code1, code2)) {
			draw = 1;
			break;
		} else if (CLIP_REJECT(code1, code2))
			break;
		else {
			if (CLIP_INSIDE(code1)) {
				swaptmp = *x2;
				*x2 = *x1;
				*x1 = swaptmp;
				swaptmp = *y2;
				*y2 = *y1;
				*y1 = swaptmp;
				swaptmp = code2;
				code2 = code1;
				code1 = swaptmp;
			}
			if (*x2 != *x1) {
				m = (float)(*y2 - *y1) / (float)(*x2 - *x1);
			} else {
				m = 1.0f;
			}
			if (code1 & CLIP_LEFT_EDGE) {
				*y1 += (Sint16) ((left - *x1) * m);
				*x1 = left;
			} else if (code1 & CLIP_RIGHT_EDGE) {
				*y1 += (Sint16) ((right - *x1) * m);
				*x1 = right;
			} else if (code1 & CLIP_BOTTOM_EDGE) {
				if (*x2 != *x1) {
					*x1 += (Sint16) ((bottom - *y1) / m);
				}
				*y1 = bottom;
			} else if (code1 & CLIP_TOP_EDGE) {
				if (*x2 != *x1) {
					*x1 += (Sint16) ((top - *y1) / m);
				}
				*y1 = top;
			}
		}
	}

	return draw;
}

/*!
\brief Draw box (filled rectangle) with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the box.
\param y1 Y coordinate of the first point (i.e. top right) of the box.
\param x2 X coordinate of the second point (i.e. bottom left) of the box.
\param y2 Y coordinate of the second point (i.e. bottom left) of the box.
\param color The color value of the box to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int boxColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
	Sint16 left, right, top, bottom;
	Uint8 *pixel, *pixellast;
	int x, dx;
	int dy;
	int pixx, pixy;
	Sint16 w, h, tmp;
	int result;
	Uint8 *colorptr;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Order coordinates to ensure that
	* x1<=x2 and y1<=y2 
	*/
	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2) {
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	/* 
	* Get clipping boundary and 
	* check visibility 
	*/
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	}
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	}
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	}
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	}

	/* Clip all points */
	if (x1<left) { 
		x1=left; 
	} else if (x1>right) {
		x1=right;
	}
	if (x2<left) { 
		x2=left; 
	} else if (x2>right) {
		x2=right;
	}
	if (y1<top) { 
		y1=top; 
	} else if (y1>bottom) {
		y1=bottom;
	}
	if (y2<top) { 
		y2=top; 
	} else if (y2>bottom) {
		y2=bottom;
	}

	/*
	* Test for special cases of straight line or single point 
	*/
	if (x1 == x2) {
		if (y1 == y2) {
			return (pixelColor(dst, x1, y1, color));
		} else { 
			return (vlineColor(dst, x1, y1, y2, color));
		}
	}
	if (y1 == y2) {
		return (hlineColor(dst, x1, x2, y1, color));
	}

	/*
	* Calculate width&height 
	*/
	w = x2 - x1;
	h = y2 - y1;

	/*
	* Alpha check 
	*/
	if ((color & 255) == 255) {

		/*
		* No alpha-blending required 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}

		/*
		* More variable setup 
		*/
		dx = w;
		dy = h;
		pixx = dst->format->BytesPerPixel;
		pixy = dst->pitch;
		pixel = ((Uint8 *) dst->pixels) + pixx * (int) x1 + pixy * (int) y1;
		pixellast = pixel + pixx * dx + pixy * dy;
		dx++;

		/*
		* Draw 
		*/
		switch (dst->format->BytesPerPixel) {
		case 1:
			for (; pixel <= pixellast; pixel += pixy) {
				memset(pixel, (Uint8) color, dx);
			}
			break;
		case 2:
			pixy -= (pixx * dx);
			for (; pixel <= pixellast; pixel += pixy) {
				for (x = 0; x < dx; x++) {
					*(Uint16*) pixel = color;
					pixel += pixx;
				}
			}
			break;
		case 3:
			pixy -= (pixx * dx);
			for (; pixel <= pixellast; pixel += pixy) {
				for (x = 0; x < dx; x++) {
					if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
						pixel[0] = (color >> 16) & 0xff;
						pixel[1] = (color >> 8) & 0xff;
						pixel[2] = color & 0xff;
					} else {
						pixel[0] = color & 0xff;
						pixel[1] = (color >> 8) & 0xff;
						pixel[2] = (color >> 16) & 0xff;
					}
					pixel += pixx;
				}
			}
			break;
		default:		/* case 4 */
			pixy -= (pixx * dx);
			for (; pixel <= pixellast; pixel += pixy) {
				for (x = 0; x < dx; x++) {
					*(Uint32 *) pixel = color;
					pixel += pixx;
				}
			}
			break;
		}

		result = 0;

	} else {

		result = filledRectAlpha(dst, x1, y1, x1 + w, y1 + h, color);

	}

	return (result);
}

/*!
\brief Draw box (filled rectangle) with blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the box.
\param y1 Y coordinate of the first point (i.e. top right) of the box.
\param x2 X coordinate of the second point (i.e. bottom left) of the box.
\param y2 Y coordinate of the second point (i.e. bottom left) of the box.
\param r The red value of the box to draw. 
\param g The green value of the box to draw. 
\param b The blue value of the box to draw. 
\param a The alpha value of the box to draw.

\returns Returns 0 on success, -1 on failure.
*/
int boxRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (boxColor(dst, x1, y1, x2, y2, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- Line */

/* Non-alpha line drawing code adapted from routine          */
/* by Pete Shinners, pete@shinners.org                       */
/* Originally from pygame, http://pygame.seul.org            */

#define ABS(a) (((a)<0) ? -(a) : (a))

/*!
\brief Draw line with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param color The color value of the line to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int lineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
	int pixx, pixy;
	int x, y;
	int dx, dy;
	int ax, ay;
	int sx, sy;
	int swaptmp;
	Uint8 *pixel;
	Uint8 *colorptr;

	/*
	* Clip line and test if we have to draw 
	*/
	if (!(_clipLine(dst, &x1, &y1, &x2, &y2))) {
		return (0);
	}

	/*
	* Test for special cases of straight lines or single point 
	*/
	if (x1 == x2) {
		if (y1 < y2) {
			return (vlineColor(dst, x1, y1, y2, color));
		} else if (y1 > y2) {
			return (vlineColor(dst, x1, y2, y1, color));
		} else {
			return (pixelColor(dst, x1, y1, color));
		}
	}
	if (y1 == y2) {
		if (x1 < x2) {
			return (hlineColor(dst, x1, x2, y1, color));
		} else if (x1 > x2) {
			return (hlineColor(dst, x2, x1, y1, color));
		}
	}

	/*
	* Variable setup 
	*/
	dx = x2 - x1;
	dy = y2 - y1;
	sx = (dx >= 0) ? 1 : -1;
	sy = (dy >= 0) ? 1 : -1;

	/*
	* Check for alpha blending 
	*/
	if ((color & 255) == 255) {

		/*
		* No alpha blending - use fast pixel routines 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}

		/*
		* More variable setup 
		*/
		dx = sx * dx + 1;
		dy = sy * dy + 1;
		pixx = dst->format->BytesPerPixel;
		pixy = dst->pitch;
		pixel = ((Uint8 *) dst->pixels) + pixx * (int) x1 + pixy * (int) y1;
		pixx *= sx;
		pixy *= sy;
		if (dx < dy) {
			swaptmp = dx;
			dx = dy;
			dy = swaptmp;
			swaptmp = pixx;
			pixx = pixy;
			pixy = swaptmp;
		}

		/*
		* Draw 
		*/
		x = 0;
		y = 0;
		switch (dst->format->BytesPerPixel) {
		case 1:
			for (; x < dx; x++, pixel += pixx) {
				*pixel = color;
				y += dy;
				if (y >= dx) {
					y -= dx;
					pixel += pixy;
				}
			}
			break;
		case 2:
			for (; x < dx; x++, pixel += pixx) {
				*(Uint16 *) pixel = color;
				y += dy;
				if (y >= dx) {
					y -= dx;
					pixel += pixy;
				}
			}
			break;
		case 3:
			for (; x < dx; x++, pixel += pixx) {
				if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
					pixel[0] = (color >> 16) & 0xff;
					pixel[1] = (color >> 8) & 0xff;
					pixel[2] = color & 0xff;
				} else {
					pixel[0] = color & 0xff;
					pixel[1] = (color >> 8) & 0xff;
					pixel[2] = (color >> 16) & 0xff;
				}
				y += dy;
				if (y >= dx) {
					y -= dx;
					pixel += pixy;
				}
			}
			break;
		default:		/* case 4 */
			for (; x < dx; x++, pixel += pixx) {
				*(Uint32 *) pixel = color;
				y += dy;
				if (y >= dx) {
					y -= dx;
					pixel += pixy;
				}
			}
			break;
		}

	} else {

		/*
		* Alpha blending required - use single-pixel blits 
		*/

		ax = ABS(dx) << 1;
		ay = ABS(dy) << 1;
		x = x1;
		y = y1;
		if (ax > ay) {
			int d = ay - (ax >> 1);

			while (x != x2) {
				pixelColorNolock (dst, x, y, color);
				if (d > 0 || (d == 0 && sx == 1)) {
					y += sy;
					d -= ax;
				}
				x += sx;
				d += ay;
			}
		} else {
			int d = ax - (ay >> 1);

			while (y != y2) {
				pixelColorNolock (dst, x, y, color);
				if (d > 0 || ((d == 0) && (sy == 1))) {
					x += sx;
					d -= ay;
				}
				y += sy;
				d += ax;
			}
		}
		pixelColorNolock (dst, x, y, color);

	}

	return (0);
}

/*!
\brief Draw line with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw.

\returns Returns 0 on success, -1 on failure.
*/
int lineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (lineColor(dst, x1, y1, x2, y2, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* AA Line */

#define AAlevels 256
#define AAbits 8

/*!
\brief Internal function to draw anti-aliased line with alpha blending and endpoint control.

This implementation of the Wu antialiasing code is based on Mike Abrash's
DDJ article which was reprinted as Chapter 42 of his Graphics Programming
Black Book, but has been optimized to work with SDL and utilizes 32-bit
fixed-point arithmetic by A. Schiffler. The endpoint control allows the
supression to draw the last pixel useful for rendering continous aa-lines
with alpha<255.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the aa-line.
\param y1 Y coordinate of the first point of the aa-line.
\param x2 X coordinate of the second point of the aa-line.
\param y2 Y coordinate of the second point of the aa-line.
\param color The color value of the aa-line to draw (0xRRGGBBAA).
\param draw_endpoint Flag indicating if the endpoint should be drawn; draw if non-zero.

\returns Returns 0 on success, -1 on failure.
*/
int _aalineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, int draw_endpoint)
{
	Sint32 xx0, yy0, xx1, yy1;
	int result;
	Uint32 intshift, erracc, erradj;
	Uint32 erracctmp, wgt/*, wgtcompmask*/;
	int dx, dy, tmp, xdir, y0p1, x0pxdir;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Clip line and test if we have to draw 
	*/
	if (!(_clipLine(dst, &x1, &y1, &x2, &y2))) {
		return (0);
	}

	/*
	* Keep on working with 32bit numbers 
	*/
	xx0 = x1;
	yy0 = y1;
	xx1 = x2;
	yy1 = y2;

	/*
	* Reorder points if required 
	*/
	if (yy0 > yy1) {
		tmp = yy0;
		yy0 = yy1;
		yy1 = tmp;
		tmp = xx0;
		xx0 = xx1;
		xx1 = tmp;
	}

	/*
	* Calculate distance 
	*/
	dx = xx1 - xx0;
	dy = yy1 - yy0;

	/*
	* Check for special cases 
	*/
	if (dx == 0) {
		/*
		* Vertical line 
		*/
		if (draw_endpoint)
		{
			return (vlineColor(dst, x1, y1, y2, color));
		} else {
			if (dy>0) {
				return (vlineColor(dst, x1, yy0, yy0+dy, color));
			} else {
				return (pixelColor(dst, x1, y1, color));
			}
		}
	} else if (dy == 0) {
		/*
		* Horizontal line 
		*/
		if (draw_endpoint)
		{
			return (hlineColor(dst, x1, x2, y1, color));
		} else {
			if (dx!=0) {
				return (hlineColor(dst, xx0, xx0+dx, y1, color));
			} else {
				return (pixelColor(dst, x1, y1, color));
			}
		}
	} else if ((dx == dy) && (draw_endpoint)) {
		/*
		* Diagonal line (with endpoint)
		*/
		return (lineColor(dst, x1, y1, x2, y2, color));
	}

	/*
	* Adjust for negative dx and set xdir 
	*/
	if (dx >= 0) {
		xdir = 1;
	} else {
		xdir = -1;
		dx = (-dx);
	}

	/*
	* Line is not horizontal, vertical or diagonal (with endpoint)
	*/
	result = 0;

	/*
	* Zero accumulator 
	*/
	erracc = 0;

	/*
	* # of bits by which to shift erracc to get intensity level 
	*/
	intshift = 32 - AAbits;

	/*
	* Mask used to flip all bits in an intensity weighting 
	*/
	/*wgtcompmask = AAlevels - 1;*/

	/*
	* Draw the initial pixel in the foreground color 
	*/
	result |= pixelColorNolock(dst, x1, y1, color);

	/*
	* x-major or y-major? 
	*/
	if (dy > dx) {

		/*
		* y-major.  Calculate 16-bit fixed point fractional part of a pixel that
		* X advances every time Y advances 1 pixel, truncating the result so that
		* we won't overrun the endpoint along the X axis 
		*/
		/*
		* Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy; 
		*/
		erradj = ((dx << 16) / dy) << 16;

		/*
		* draw all pixels other than the first and last 
		*/
		x0pxdir = xx0 + xdir;
		while (--dy) {
			erracctmp = erracc;
			erracc += erradj;
			if (erracc <= erracctmp) {
				/*
				* rollover in error accumulator, x coord advances 
				*/
				xx0 = x0pxdir;
				x0pxdir += xdir;
			}
			yy0++;		/* y-major so always advance Y */

			/*
			* the AAbits most significant bits of erracc give us the intensity
			* weighting for this pixel, and the complement of the weighting for
			* the paired pixel. 
			*/
			wgt = (erracc >> intshift) & 255;
			result |= pixelColorWeightNolock (dst, xx0, yy0, color, 255 - wgt);
			result |= pixelColorWeightNolock (dst, x0pxdir, yy0, color, wgt);
		}

	} else {

		/*
		* x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
		* that Y advances each time X advances 1 pixel, truncating the result so
		* that we won't overrun the endpoint along the X axis. 
		*/
		/*
		* Not-so-portable version: erradj = ((Uint64)dy << 32) / (Uint64)dx; 
		*/
		erradj = ((dy << 16) / dx) << 16;

		/*
		* draw all pixels other than the first and last 
		*/
		y0p1 = yy0 + 1;
		while (--dx) {

			erracctmp = erracc;
			erracc += erradj;
			if (erracc <= erracctmp) {
				/*
				* Accumulator turned over, advance y 
				*/
				yy0 = y0p1;
				y0p1++;
			}
			xx0 += xdir;	/* x-major so always advance X */
			/*
			* the AAbits most significant bits of erracc give us the intensity
			* weighting for this pixel, and the complement of the weighting for
			* the paired pixel. 
			*/
			wgt = (erracc >> intshift) & 255;
			result |= pixelColorWeightNolock (dst, xx0, yy0, color, 255 - wgt);
			result |= pixelColorWeightNolock (dst, xx0, y0p1, color, wgt);
		}
	}

	/*
	* Do we have to draw the endpoint 
	*/
	if (draw_endpoint) {
		/*
		* Draw final pixel, always exactly intersected by the line and doesn't
		* need to be weighted. 
		*/
		result |= pixelColorNolock (dst, x2, y2, color);
	}

	return (result);
}

/*!
\brief Ddraw anti-aliased line with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the aa-line.
\param y1 Y coordinate of the first point of the aa-line.
\param x2 X coordinate of the second point of the aa-line.
\param y2 Y coordinate of the second point of the aa-line.
\param color The color value of the aa-line to draw (0xRRGGBBAA).

\returns Returns 0 on success, -1 on failure.
*/
int aalineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
	return (_aalineColor(dst, x1, y1, x2, y2, color, 1));
}

/*!
\brief Draw anti-aliased line with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the aa-line.
\param y1 Y coordinate of the first point of the aa-line.
\param x2 X coordinate of the second point of the aa-line.
\param y2 Y coordinate of the second point of the aa-line.
\param r The red value of the aa-line to draw. 
\param g The green value of the aa-line to draw. 
\param b The blue value of the aa-line to draw. 
\param a The alpha value of the aa-line to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aalineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return (_aalineColor
		(dst, x1, y1, x2, y2, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, 1));
}


/* ----- Circle */

/*!
\brief Draw circle with blending.

Note: Circle drawing routine is based on an algorithms from the sge library, 
but modified by A. Schiffler for multiple pixel-draw removal and other 
minor speedup changes.

\param dst The surface to draw on.
\param x X coordinate of the center of the circle.
\param y Y coordinate of the center of the circle.
\param rad Radius in pixels of the circle.
\param color The color value of the circle to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int circleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
{
	Sint16 left, right, top, bottom;
	int result;
	Sint16 x1, y1, x2, y2;
	Sint16 cx = 0;
	Sint16 cy = rad;
	Sint16 df = 1 - rad;
	Sint16 d_e = 3;
	Sint16 d_se = -2 * rad + 5;
	Sint16 xpcx, xmcx, xpcy, xmcy;
	Sint16 ypcy, ymcy, ypcx, ymcx;
	Uint8 *colorptr;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radius 
	*/
	if (rad < 0) {
		return (-1);
	}

	/*
	* Special case for rad=0 - draw a point 
	*/
	if (rad == 0) {
		return (pixelColor(dst, x, y, color));
	}

	/*
	* Get circle and clipping boundary and 
	* test if bounding box of circle is visible 
	*/
	x2 = x + rad;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rad;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + rad;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - rad;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	} 

	/*
	* Draw circle 
	*/
	result = 0;

	/*
	* Alpha Check 
	*/
	if ((color & 255) == 255) {

		/*
		* No Alpha - direct memory writes 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}

		/*
		* Draw 
		*/
		do {
			ypcy = y + cy;
			ymcy = y - cy;
			if (cx > 0) {
				xpcx = x + cx;
				xmcx = x - cx;
				result |= fastPixelColorNolock(dst, xmcx, ypcy, color);
				result |= fastPixelColorNolock(dst, xpcx, ypcy, color);
				result |= fastPixelColorNolock(dst, xmcx, ymcy, color);
				result |= fastPixelColorNolock(dst, xpcx, ymcy, color);
			} else {
				result |= fastPixelColorNolock(dst, x, ymcy, color);
				result |= fastPixelColorNolock(dst, x, ypcy, color);
			}
			xpcy = x + cy;
			xmcy = x - cy;
			if ((cx > 0) && (cx != cy)) {
				ypcx = y + cx;
				ymcx = y - cx;
				result |= fastPixelColorNolock(dst, xmcy, ypcx, color);
				result |= fastPixelColorNolock(dst, xpcy, ypcx, color);
				result |= fastPixelColorNolock(dst, xmcy, ymcx, color);
				result |= fastPixelColorNolock(dst, xpcy, ymcx, color);
			} else if (cx == 0) {
				result |= fastPixelColorNolock(dst, xmcy, y, color);
				result |= fastPixelColorNolock(dst, xpcy, y, color);
			}
			/*
			* Update 
			*/
			if (df < 0) {
				df += d_e;
				d_e += 2;
				d_se += 2;
			} else {
				df += d_se;
				d_e += 2;
				d_se += 4;
				cy--;
			}
			cx++;
		} while (cx <= cy);

	} else {

		/*
		* Using Alpha - blended pixel blits 
		*/

		do {
			/*
			* Draw 
			*/
			ypcy = y + cy;
			ymcy = y - cy;
			if (cx > 0) {
				xpcx = x + cx;
				xmcx = x - cx;
				result |= pixelColorNolock (dst, xmcx, ypcy, color);
				result |= pixelColorNolock (dst, xpcx, ypcy, color);
				result |= pixelColorNolock (dst, xmcx, ymcy, color);
				result |= pixelColorNolock (dst, xpcx, ymcy, color);
			} else {
				result |= pixelColorNolock (dst, x, ymcy, color);
				result |= pixelColorNolock (dst, x, ypcy, color);
			}
			xpcy = x + cy;
			xmcy = x - cy;
			if ((cx > 0) && (cx != cy)) {
				ypcx = y + cx;
				ymcx = y - cx;
				result |= pixelColorNolock (dst, xmcy, ypcx, color);
				result |= pixelColorNolock (dst, xpcy, ypcx, color);
				result |= pixelColorNolock (dst, xmcy, ymcx, color);
				result |= pixelColorNolock (dst, xpcy, ymcx, color);
			} else if (cx == 0) {
				result |= pixelColorNolock (dst, xmcy, y, color);
				result |= pixelColorNolock (dst, xpcy, y, color);
			}
			/*
			* Update 
			*/
			if (df < 0) {
				df += d_e;
				d_e += 2;
				d_se += 2;
			} else {
				df += d_se;
				d_e += 2;
				d_se += 4;
				cy--;
			}
			cx++;
		} while (cx <= cy);

	}				/* Alpha check */

	return (result);
}

/*!
\brief Draw circle with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the circle.
\param y Y coordinate of the center of the circle.
\param rad Radius in pixels of the circle.
\param r The red value of the circle to draw. 
\param g The green value of the circle to draw. 
\param b The blue value of the circle to draw. 
\param a The alpha value of the circle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int circleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (circleColor(dst, x, y, rad, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- Arc */

/*!
\brief Arc with blending.

Note Arc drawing is based on circle algorithm by A. Schiffler and 
written by D. Raber. Calculates which octants arc goes through and 
renders pixels accordingly.

\param dst The surface to draw on.
\param x X coordinate of the center of the arc.
\param y Y coordinate of the center of the arc.
\param rad Radius in pixels of the arc.
\param start Starting radius in degrees of the arc. 0 degrees is down, increasing counterclockwise.
\param end Ending radius in degrees of the arc. 0 degrees is down, increasing counterclockwise.
\param color The color value of the arc to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int arcColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color)
{
	Sint16 left, right, top, bottom;
	int result;
	Sint16 x1, y1, x2, y2;
	Sint16 cx = 0;
	Sint16 cy = rad;
	Sint16 df = 1 - rad;
	Sint16 d_e = 3;
	Sint16 d_se = -2 * rad + 5;
	Sint16 xpcx, xmcx, xpcy, xmcy;
	Sint16 ypcy, ymcy, ypcx, ymcx;
	Uint8 *colorptr;
	Uint8 drawoct;
	int startoct, endoct, oct, stopval_start = 0, stopval_end = 0;
	double dstart, dend, temp = 0.;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radius 
	*/
	if (rad < 0) {
		return (-1);
	}

	/*
	* Special case for rad=0 - draw a point 
	*/
	if (rad == 0) {
		return (pixelColor(dst, x, y, color));
	}

	/*
	* Get arc's circle and clipping boundary and 
	* test if bounding box of circle is visible 
	*/
	x2 = x + rad;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rad;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + rad;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - rad;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	}  

	/*
	* Octant labeling
	*      
	*  \ 5 | 6 /
	*   \  |  /
	*  4 \ | / 7
	*     \|/
	*------+------ +x
	*     /|\
	*  3 / | \ 0
	*   /  |  \
	*  / 2 | 1 \
	*      +y
	*
	* Initially reset bitmask to 0x00000000
	* the set whether or not to keep drawing a given octant.
	* For example: 0x00111100 means we're drawing in octants 2-5
	*/
	drawoct = 0; 

	/*
	* Fixup angles
	*/
	start %= 360;
	end %= 360;
	/* 0 <= start & end < 360; note that sometimes start > end - if so, arc goes back through 0. */
	while (start < 0) start += 360;
	while (end < 0) end += 360;
	start %= 360;
	end %= 360;

	/* now, we find which octants we're drawing in. */
	startoct = start / 45;
	endoct = end / 45;
	oct = startoct - 1; /* we increment as first step in loop */

	/* stopval_start, stopval_end; */
	/* what values of cx to stop at. */
	do {
		oct = (oct + 1) % 8;

		if (oct == startoct) {
			/* need to compute stopval_start for this octant.  Look at picture above if this is unclear */
			dstart = (double)start;
			switch (oct) 
			{
			case 0:
			case 3:
				temp = sin(dstart * M_PI / 180.);
				break;
			case 1:
			case 6:
				temp = cos(dstart * M_PI / 180.);
				break;
			case 2:
			case 5:
				temp = -cos(dstart * M_PI / 180.);
				break;
			case 4:
			case 7:
				temp = -sin(dstart * M_PI / 180.);
				break;
			}
			temp *= rad;
			stopval_start = (int)temp; /* always round down. */
			/* This isn't arbitrary, but requires graph paper to explain well. */
			/* The basic idea is that we're always changing drawoct after we draw, so we */
			/* stop immediately after we render the last sensible pixel at x = ((int)temp). */

			/* and whether to draw in this octant initially */
			if (oct % 2) drawoct |= (1 << oct); /* this is basically like saying drawoct[oct] = true, if drawoct were a bool array */
			else		 drawoct &= 255 - (1 << oct); /* this is basically like saying drawoct[oct] = false */
		}
		if (oct == endoct) {
			/* need to compute stopval_end for this octant */
			dend = (double)end;
			switch (oct)
			{
			case 0:
			case 3:
				temp = sin(dend * M_PI / 180);
				break;
			case 1:
			case 6:
				temp = cos(dend * M_PI / 180);
				break;
			case 2:
			case 5:
				temp = -cos(dend * M_PI / 180);
				break;
			case 4:
			case 7:
				temp = -sin(dend * M_PI / 180);
				break;
			}
			temp *= rad;
			stopval_end = (int)temp;

			/* and whether to draw in this octant initially */
			if (startoct == endoct)	{
				/* note:      we start drawing, stop, then start again in this case */
				/* otherwise: we only draw in this octant, so initialize it to false, it will get set back to true */
				if (start > end) {
					/* unfortunately, if we're in the same octant and need to draw over the whole circle, */
					/* we need to set the rest to true, because the while loop will end at the bottom. */
					drawoct = 255;
				} else {
					drawoct &= 255 - (1 << oct);
				}
			} 
			else if (oct % 2) drawoct &= 255 - (1 << oct);
			else			  drawoct |= (1 << oct);
		} else if (oct != startoct) { /* already verified that it's != endoct */
			drawoct |= (1 << oct); /* draw this entire segment */
		}
	} while (oct != endoct);

	/* so now we have what octants to draw and when to draw them.  all that's left is the actual raster code. */

	/*
	* Draw arc 
	*/
	result = 0;

	/*
	* Alpha Check 
	*/
	if ((color & 255) == 255) {

		/*
		* No Alpha - direct memory writes 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}

		/*
		* Draw 
		*/
		do {
			ypcy = y + cy;
			ymcy = y - cy;
			if (cx > 0) {
				xpcx = x + cx;
				xmcx = x - cx;
				/* always check if we're drawing a certain octant before adding a pixel to that octant. */
				if (drawoct & 4)  result |= fastPixelColorNolock(dst, xmcx, ypcy, color); /* drawoct & 4 = 22; drawoct[2] */
				if (drawoct & 2)  result |= fastPixelColorNolock(dst, xpcx, ypcy, color);
				if (drawoct & 32) result |= fastPixelColorNolock(dst, xmcx, ymcy, color);
				if (drawoct & 64) result |= fastPixelColorNolock(dst, xpcx, ymcy, color);
			} else {
				if (drawoct & 6)  result |= fastPixelColorNolock(dst, x, ypcy, color); /* 4 + 2; drawoct[2] || drawoct[1] */
				if (drawoct & 96) result |= fastPixelColorNolock(dst, x, ymcy, color); /* 32 + 64 */
			}

			xpcy = x + cy;
			xmcy = x - cy;
			if (cx > 0 && cx != cy) {
				ypcx = y + cx;
				ymcx = y - cx;
				if (drawoct & 8)   result |= fastPixelColorNolock(dst, xmcy, ypcx, color);
				if (drawoct & 1)   result |= fastPixelColorNolock(dst, xpcy, ypcx, color);
				if (drawoct & 16)  result |= fastPixelColorNolock(dst, xmcy, ymcx, color);
				if (drawoct & 128) result |= fastPixelColorNolock(dst, xpcy, ymcx, color);
			} else if (cx == 0) {
				if (drawoct & 24)  result |= fastPixelColorNolock(dst, xmcy, y, color); /* 8 + 16 */
				if (drawoct & 129) result |= fastPixelColorNolock(dst, xpcy, y, color); /* 1 + 128 */
			}

			/*
			* Update whether we're drawing an octant
			*/
			if (stopval_start == cx) {
				/* works like an on-off switch because start & end may be in the same octant. */
				if (drawoct & (1 << startoct)) drawoct &= 255 - (1 << startoct);		
				else drawoct |= (1 << startoct);
			}
			if (stopval_end == cx) {
				if (drawoct & (1 << endoct)) drawoct &= 255 - (1 << endoct);
				else drawoct |= (1 << endoct);
			}

			/*
			* Update pixels
			*/
			if (df < 0) {
				df += d_e;
				d_e += 2;
				d_se += 2;
			} else {
				df += d_se;
				d_e += 2;
				d_se += 4;
				cy--;
			}
			cx++;
		} while (cx <= cy);

	} else {

		/*
		* Using Alpha - blended pixel blits 
		*/

		do {
			ypcy = y + cy;
			ymcy = y - cy;
			if (cx > 0) {
				xpcx = x + cx;
				xmcx = x - cx;

				/* always check if we're drawing a certain octant before adding a pixel to that octant. */
				if (drawoct & 4)  result |= pixelColorNolock(dst, xmcx, ypcy, color);
				if (drawoct & 2)  result |= pixelColorNolock(dst, xpcx, ypcy, color);
				if (drawoct & 32) result |= pixelColorNolock(dst, xmcx, ymcy, color);
				if (drawoct & 64) result |= pixelColorNolock(dst, xpcx, ymcy, color);
			} else {
				if (drawoct & 96) result |= pixelColorNolock(dst, x, ymcy, color);
				if (drawoct & 6)  result |= pixelColorNolock(dst, x, ypcy, color);
			}

			xpcy = x + cy;
			xmcy = x - cy;
			if (cx > 0 && cx != cy) {
				ypcx = y + cx;
				ymcx = y - cx;
				if (drawoct & 8)   result |= pixelColorNolock(dst, xmcy, ypcx, color);
				if (drawoct & 1)   result |= pixelColorNolock(dst, xpcy, ypcx, color);
				if (drawoct & 16)  result |= pixelColorNolock(dst, xmcy, ymcx, color);
				if (drawoct & 128) result |= pixelColorNolock(dst, xpcy, ymcx, color);
			} else if (cx == 0) {
				if (drawoct & 24)  result |= pixelColorNolock(dst, xmcy, y, color);
				if (drawoct & 129) result |= pixelColorNolock(dst, xpcy, y, color);
			}

			/*
			* Update whether we're drawing an octant
			*/
			if (stopval_start == cx) {
				/* works like an on-off switch. */
				/* This is just in case start & end are in the same octant. */
				if (drawoct & (1 << startoct)) drawoct &= 255 - (1 << startoct);		
				else						   drawoct |= (1 << startoct);
			}
			if (stopval_end == cx) {
				if (drawoct & (1 << endoct)) drawoct &= 255 - (1 << endoct);
				else						 drawoct |= (1 << endoct);
			}

			/*
			* Update pixels
			*/
			if (df < 0) {
				df += d_e;
				d_e += 2;
				d_se += 2;
			} else {
				df += d_se;
				d_e += 2;
				d_se += 4;
				cy--;
			}
			cx++;
		} while (cx <= cy);

	}				/* Alpha check */

	return (result);
}

/*!
\brief Arc with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the arc.
\param y Y coordinate of the center of the arc.
\param rad Radius in pixels of the arc.
\param start Starting radius in degrees of the arc. 0 degrees is down, increasing counterclockwise.
\param end Ending radius in degrees of the arc. 0 degrees is down, increasing counterclockwise.
\param r The red value of the arc to draw. 
\param g The green value of the arc to draw. 
\param b The blue value of the arc to draw. 
\param a The alpha value of the arc to draw.

\returns Returns 0 on success, -1 on failure.
*/
int arcRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (arcColor(dst, x, y, rad, start, end, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- AA Circle */


/*!
\brief Draw anti-aliased circle with blending.

Note: The AA-circle routine is based on AA-ellipse with identical radii.

\param dst The surface to draw on.
\param x X coordinate of the center of the aa-circle.
\param y Y coordinate of the center of the aa-circle.
\param rad Radius in pixels of the aa-circle.
\param color The color value of the aa-circle to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int aacircleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
{
	return (aaellipseColor(dst, x, y, rad, rad, color));
}

/*!
\brief Draw anti-aliased circle with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the aa-circle.
\param y Y coordinate of the center of the aa-circle.
\param rad Radius in pixels of the aa-circle.
\param r The red value of the aa-circle to draw. 
\param g The green value of the aa-circle to draw. 
\param b The blue value of the aa-circle to draw. 
\param a The alpha value of the aa-circle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aacircleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (aaellipseColor
		(dst, x, y, rad, rad, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- Filled Circle */

/*!
\brief Draw filled circle with blending.

Note: Based on algorithms from sge library with modifications by A. Schiffler for
multiple-hline draw removal and other minor speedup changes.

\param dst The surface to draw on.
\param x X coordinate of the center of the filled circle.
\param y Y coordinate of the center of the filled circle.
\param rad Radius in pixels of the filled circle.
\param color The color value of the filled circle to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int filledCircleColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
{
	Sint16 left, right, top, bottom;
	int result;
	Sint16 x1, y1, x2, y2;
	Sint16 cx = 0;
	Sint16 cy = rad;
	Sint16 ocx = (Sint16) 0xffff;
	Sint16 ocy = (Sint16) 0xffff;
	Sint16 df = 1 - rad;
	Sint16 d_e = 3;
	Sint16 d_se = -2 * rad + 5;
	Sint16 xpcx, xmcx, xpcy, xmcy;
	Sint16 ypcy, ymcy, ypcx, ymcx;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radius 
	*/
	if (rad < 0) {
		return (-1);
	}

	/*
	* Special case for rad=0 - draw a point 
	*/
	if (rad == 0) {
		return (pixelColor(dst, x, y, color));
	}

	/*
	* Get circle and clipping boundary and 
	* test if bounding box of circle is visible 
	*/
	x2 = x + rad;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rad;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + rad;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - rad;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	} 

	/*
	* Draw 
	*/
	result = 0;
	do {
		xpcx = x + cx;
		xmcx = x - cx;
		xpcy = x + cy;
		xmcy = x - cy;
		if (ocy != cy) {
			if (cy > 0) {
				ypcy = y + cy;
				ymcy = y - cy;
				result |= hlineColor(dst, xmcx, xpcx, ypcy, color);
				result |= hlineColor(dst, xmcx, xpcx, ymcy, color);
			} else {
				result |= hlineColor(dst, xmcx, xpcx, y, color);
			}
			ocy = cy;
		}
		if (ocx != cx) {
			if (cx != cy) {
				if (cx > 0) {
					ypcx = y + cx;
					ymcx = y - cx;
					result |= hlineColor(dst, xmcy, xpcy, ymcx, color);
					result |= hlineColor(dst, xmcy, xpcy, ypcx, color);
				} else {
					result |= hlineColor(dst, xmcy, xpcy, y, color);
				}
			}
			ocx = cx;
		}
		/*
		* Update 
		*/
		if (df < 0) {
			df += d_e;
			d_e += 2;
			d_se += 2;
		} else {
			df += d_se;
			d_e += 2;
			d_se += 4;
			cy--;
		}
		cx++;
	} while (cx <= cy);

	return (result);
}

/*!
\brief Draw filled circle with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the filled circle.
\param y Y coordinate of the center of the filled circle.
\param rad Radius in pixels of the filled circle.
\param r The red value of the filled circle to draw. 
\param g The green value of the filled circle to draw. 
\param b The blue value of the filled circle to draw. 
\param a The alpha value of the filled circle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledCircleRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (filledCircleColor
		(dst, x, y, rad, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- Ellipse */

/*!
\brief Draw ellipse with blending.

Note: Based on algorithms from sge library with modifications by A. Schiffler for
multiple-pixel draw removal and other minor speedup changes.

\param dst The surface to draw on.
\param x X coordinate of the center of the ellipse.
\param y Y coordinate of the center of the ellipse.
\param rx Horizontal radius in pixels of the ellipse.
\param ry Vertical radius in pixels of the ellipse.
\param color The color value of the ellipse to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int ellipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
{
	Sint16 left, right, top, bottom;
	int result;
	Sint16 x1, y1, x2, y2;
	int ix, iy;
	int h, i, j, k;
	int oh, oi, oj, ok;
	int xmh, xph, ypk, ymk;
	int xmi, xpi, ymj, ypj;
	int xmj, xpj, ymi, ypi;
	int xmk, xpk, ymh, yph;
	Uint8 *colorptr;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radii 
	*/
	if ((rx < 0) || (ry < 0)) {
		return (-1);
	}

	/*
	* Special case for rx=0 - draw a vline 
	*/
	if (rx == 0) {
		return (vlineColor(dst, x, y - ry, y + ry, color));
	}
	/*
	* Special case for ry=0 - draw a hline 
	*/
	if (ry == 0) {
		return (hlineColor(dst, x - rx, x + rx, y, color));
	}

	/*
	* Get circle and clipping boundary and 
	* test if bounding box of circle is visible 
	*/
	x2 = x + rx;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rx;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + ry;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - ry;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	} 

	/*
	* Init vars 
	*/
	oh = oi = oj = ok = 0xFFFF;

	/*
	* Draw 
	*/
	result = 0;

	/*
	* Check alpha 
	*/
	if ((color & 255) == 255) {

		/*
		* No Alpha - direct memory writes 
		*/

		/*
		* Setup color 
		*/
		colorptr = (Uint8 *) & color;
		if (RSDL_BYTEORDER == RSDL_BIG_ENDIAN) {
			color = RSDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
		} else {
			color = RSDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
		}


		if (rx > ry) {
			ix = 0;
			iy = rx * 64;

			do {
				h = (ix + 32) >> 6;
				i = (iy + 32) >> 6;
				j = (h * ry) / rx;
				k = (i * ry) / rx;

				if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j)) {
					xph = x + h;
					xmh = x - h;
					if (k > 0) {
						ypk = y + k;
						ymk = y - k;
						result |= fastPixelColorNolock(dst, xmh, ypk, color);
						result |= fastPixelColorNolock(dst, xph, ypk, color);
						result |= fastPixelColorNolock(dst, xmh, ymk, color);
						result |= fastPixelColorNolock(dst, xph, ymk, color);
					} else {
						result |= fastPixelColorNolock(dst, xmh, y, color);
						result |= fastPixelColorNolock(dst, xph, y, color);
					}
					ok = k;
					xpi = x + i;
					xmi = x - i;
					if (j > 0) {
						ypj = y + j;
						ymj = y - j;
						result |= fastPixelColorNolock(dst, xmi, ypj, color);
						result |= fastPixelColorNolock(dst, xpi, ypj, color);
						result |= fastPixelColorNolock(dst, xmi, ymj, color);
						result |= fastPixelColorNolock(dst, xpi, ymj, color);
					} else {
						result |= fastPixelColorNolock(dst, xmi, y, color);
						result |= fastPixelColorNolock(dst, xpi, y, color);
					}
					oj = j;
				}

				ix = ix + iy / rx;
				iy = iy - ix / rx;

			} while (i > h);
		} else {
			ix = 0;
			iy = ry * 64;

			do {
				h = (ix + 32) >> 6;
				i = (iy + 32) >> 6;
				j = (h * rx) / ry;
				k = (i * rx) / ry;

				if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h))) {
					xmj = x - j;
					xpj = x + j;
					if (i > 0) {
						ypi = y + i;
						ymi = y - i;
						result |= fastPixelColorNolock(dst, xmj, ypi, color);
						result |= fastPixelColorNolock(dst, xpj, ypi, color);
						result |= fastPixelColorNolock(dst, xmj, ymi, color);
						result |= fastPixelColorNolock(dst, xpj, ymi, color);
					} else {
						result |= fastPixelColorNolock(dst, xmj, y, color);
						result |= fastPixelColorNolock(dst, xpj, y, color);
					}
					oi = i;
					xmk = x - k;
					xpk = x + k;
					if (h > 0) {
						yph = y + h;
						ymh = y - h;
						result |= fastPixelColorNolock(dst, xmk, yph, color);
						result |= fastPixelColorNolock(dst, xpk, yph, color);
						result |= fastPixelColorNolock(dst, xmk, ymh, color);
						result |= fastPixelColorNolock(dst, xpk, ymh, color);
					} else {
						result |= fastPixelColorNolock(dst, xmk, y, color);
						result |= fastPixelColorNolock(dst, xpk, y, color);
					}
					oh = h;
				}

				ix = ix + iy / ry;
				iy = iy - ix / ry;

			} while (i > h);
		}

	} else {

		if (rx > ry) {
			ix = 0;
			iy = rx * 64;

			do {
				h = (ix + 32) >> 6;
				i = (iy + 32) >> 6;
				j = (h * ry) / rx;
				k = (i * ry) / rx;

				if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j)) {
					xph = x + h;
					xmh = x - h;
					if (k > 0) {
						ypk = y + k;
						ymk = y - k;
						result |= pixelColorNolock (dst, xmh, ypk, color);
						result |= pixelColorNolock (dst, xph, ypk, color);
						result |= pixelColorNolock (dst, xmh, ymk, color);
						result |= pixelColorNolock (dst, xph, ymk, color);
					} else {
						result |= pixelColorNolock (dst, xmh, y, color);
						result |= pixelColorNolock (dst, xph, y, color);
					}
					ok = k;
					xpi = x + i;
					xmi = x - i;
					if (j > 0) {
						ypj = y + j;
						ymj = y - j;
						result |= pixelColorNolock (dst, xmi, ypj, color);
						result |= pixelColorNolock (dst, xpi, ypj, color);
						result |= pixelColorNolock (dst, xmi, ymj, color);
						result |= pixelColor(dst, xpi, ymj, color);
					} else {
						result |= pixelColorNolock (dst, xmi, y, color);
						result |= pixelColorNolock (dst, xpi, y, color);
					}
					oj = j;
				}

				ix = ix + iy / rx;
				iy = iy - ix / rx;

			} while (i > h);
		} else {
			ix = 0;
			iy = ry * 64;

			do {
				h = (ix + 32) >> 6;
				i = (iy + 32) >> 6;
				j = (h * rx) / ry;
				k = (i * rx) / ry;

				if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h))) {
					xmj = x - j;
					xpj = x + j;
					if (i > 0) {
						ypi = y + i;
						ymi = y - i;
						result |= pixelColorNolock (dst, xmj, ypi, color);
						result |= pixelColorNolock (dst, xpj, ypi, color);
						result |= pixelColorNolock (dst, xmj, ymi, color);
						result |= pixelColorNolock (dst, xpj, ymi, color);
					} else {
						result |= pixelColorNolock (dst, xmj, y, color);
						result |= pixelColorNolock (dst, xpj, y, color);
					}
					oi = i;
					xmk = x - k;
					xpk = x + k;
					if (h > 0) {
						yph = y + h;
						ymh = y - h;
						result |= pixelColorNolock (dst, xmk, yph, color);
						result |= pixelColorNolock (dst, xpk, yph, color);
						result |= pixelColorNolock (dst, xmk, ymh, color);
						result |= pixelColorNolock (dst, xpk, ymh, color);
					} else {
						result |= pixelColorNolock (dst, xmk, y, color);
						result |= pixelColorNolock (dst, xpk, y, color);
					}
					oh = h;
				}

				ix = ix + iy / ry;
				iy = iy - ix / ry;

			} while (i > h);
		}

	}				/* Alpha check */

	return (result);
}

/*!
\brief Draw ellipse with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the ellipse.
\param y Y coordinate of the center of the ellipse.
\param rx Horizontal radius in pixels of the ellipse.
\param ry Vertical radius in pixels of the ellipse.
\param r The red value of the ellipse to draw. 
\param g The green value of the ellipse to draw. 
\param b The blue value of the ellipse to draw. 
\param a The alpha value of the ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int ellipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (ellipseColor(dst, x, y, rx, ry, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- AA Ellipse */

/* Windows targets do not have lrint, so provide a local inline version */
#if defined(_MSC_VER)
/* Detect 64bit and use intrinsic version */
#define lrint(d) nuklear_lrint(d)
#ifdef _M_X64
#include <emmintrin.h>
static __forceinline long 
	lrint(double d) 
{
	float f = (float)d;
	return _mm_cvtss_si32(_mm_load_ss(&f));
}
#elif defined(_M_IX86)
__inline long int
	lrint (double flt)
{	
	int intgr;
	_asm
	{
		fld flt
			fistp intgr
	};
	return intgr;
}
#elif defined(_M_ARM)
#include <armintr.h>
#pragma warning(push)
#pragma warning(disable: 4716)
__declspec(naked) long int
	lrint (double flt)
{
	__emit(0xEC410B10); /* fmdrr  d0, r0, r1 */
	__emit(0xEEBD0B40); /* ftosid s0, d0 */
	__emit(0xEE100A10); /* fmrs   r0, s0 */
	__emit(0xE12FFF1E); /* bx     lr */
}
#pragma warning(pop)
#else
#error lrint needed for MSVC on non X86/AMD64/ARM targets.
#endif
#endif

/*!
\brief Draw anti-aliased ellipse with blending.

Note: Based on code from Anders Lindstroem, which is based on code from sge library, 
which is based on code from TwinLib.

\param dst The surface to draw on.
\param x X coordinate of the center of the aa-ellipse.
\param y Y coordinate of the center of the aa-ellipse.
\param rx Horizontal radius in pixels of the aa-ellipse.
\param ry Vertical radius in pixels of the aa-ellipse.
\param color The color value of the aa-ellipse to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int aaellipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
{
	Sint16 left, right, top, bottom;
	Sint16 x1,y1,x2,y2;
	int i;
	int a2, b2, ds, dt, dxt, t, s, d;
	Sint16 xp, yp, xs, ys, dyt, od, xx, yy, xc2, yc2;
	float cp;
	double sab;
	Uint8 weight, iweight;
	int result;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radii 
	*/
	if ((rx < 0) || (ry < 0)) {
		return (-1);
	}

	/*
	* Special case for rx=0 - draw a vline 
	*/
	if (rx == 0) {
		return (vlineColor(dst, x, y - ry, y + ry, color));
	}
	/*
	* Special case for ry=0 - draw an hline 
	*/
	if (ry == 0) {
		return (hlineColor(dst, x - rx, x + rx, y, color));
	}

	/*
	* Get circle and clipping boundary and 
	* test if bounding box of circle is visible 
	*/
	x2 = x + rx;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rx;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + ry;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - ry;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	} 

	/* Variable setup */
	a2 = rx * rx;
	b2 = ry * ry;

	ds = 2 * a2;
	dt = 2 * b2;

	xc2 = 2 * x;
	yc2 = 2 * y;

	sab = sqrt((double)(a2 + b2));
	od = (Sint16)lrint(sab*0.01) + 1; /* introduce some overdraw */
	dxt = (Sint16)lrint((double)a2 / sab) + od;

	t = 0;
	s = -2 * a2 * ry;
	d = 0;

	xp = x;
	yp = y - ry;

	/* Draw */
	result = 0;

	/* "End points" */
	result |= pixelColorNolock(dst, xp, yp, color);
	result |= pixelColorNolock(dst, xc2 - xp, yp, color);
	result |= pixelColorNolock(dst, xp, yc2 - yp, color);
	result |= pixelColorNolock(dst, xc2 - xp, yc2 - yp, color);

	for (i = 1; i <= dxt; i++) {
		xp--;
		d += t - b2;

		if (d >= 0)
			ys = yp - 1;
		else if ((d - s - a2) > 0) {
			if ((2 * d - s - a2) >= 0)
				ys = yp + 1;
			else {
				ys = yp;
				yp++;
				d -= s + a2;
				s += ds;
			}
		} else {
			yp++;
			ys = yp + 1;
			d -= s + a2;
			s += ds;
		}

		t -= dt;

		/* Calculate alpha */
		if (s != 0) {
			cp = (float) abs(d) / (float) abs(s);
			if (cp > 1.0) {
				cp = 1.0;
			}
		} else {
			cp = 1.0;
		}

		/* Calculate weights */
		weight = (Uint8) (cp * 255);
		iweight = 255 - weight;

		/* Upper half */
		xx = xc2 - xp;
		result |= pixelColorWeightNolock(dst, xp, yp, color, iweight);
		result |= pixelColorWeightNolock(dst, xx, yp, color, iweight);

		result |= pixelColorWeightNolock(dst, xp, ys, color, weight);
		result |= pixelColorWeightNolock(dst, xx, ys, color, weight);

		/* Lower half */
		yy = yc2 - yp;
		result |= pixelColorWeightNolock(dst, xp, yy, color, iweight);
		result |= pixelColorWeightNolock(dst, xx, yy, color, iweight);

		yy = yc2 - ys;
		result |= pixelColorWeightNolock(dst, xp, yy, color, weight);
		result |= pixelColorWeightNolock(dst, xx, yy, color, weight);
	}

	/* Replaces original approximation code dyt = abs(yp - yc); */
	dyt = (Sint16)lrint((double)b2 / sab ) + od;    

	for (i = 1; i <= dyt; i++) {
		yp++;
		d -= s + a2;

		if (d <= 0)
			xs = xp + 1;
		else if ((d + t - b2) < 0) {
			if ((2 * d + t - b2) <= 0)
				xs = xp - 1;
			else {
				xs = xp;
				xp--;
				d += t - b2;
				t -= dt;
			}
		} else {
			xp--;
			xs = xp - 1;
			d += t - b2;
			t -= dt;
		}

		s += ds;

		/* Calculate alpha */
		if (t != 0) {
			cp = (float) abs(d) / (float) abs(t);
			if (cp > 1.0) {
				cp = 1.0;
			}
		} else {
			cp = 1.0;
		}

		/* Calculate weight */
		weight = (Uint8) (cp * 255);
		iweight = 255 - weight;

		/* Left half */
		xx = xc2 - xp;
		yy = yc2 - yp;
		result |= pixelColorWeightNolock(dst, xp, yp, color, iweight);
		result |= pixelColorWeightNolock(dst, xx, yp, color, iweight);

		result |= pixelColorWeightNolock(dst, xp, yy, color, iweight);
		result |= pixelColorWeightNolock(dst, xx, yy, color, iweight);

		/* Right half */
		xx = xc2 - xs;
		result |= pixelColorWeightNolock(dst, xs, yp, color, weight);
		result |= pixelColorWeightNolock(dst, xx, yp, color, weight);

		result |= pixelColorWeightNolock(dst, xs, yy, color, weight);
		result |= pixelColorWeightNolock(dst, xx, yy, color, weight);

	}

	return (result);
}

/*!
\brief Draw anti-aliased ellipse with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the aa-ellipse.
\param y Y coordinate of the center of the aa-ellipse.
\param rx Horizontal radius in pixels of the aa-ellipse.
\param ry Vertical radius in pixels of the aa-ellipse.
\param r The red value of the aa-ellipse to draw. 
\param g The green value of the aa-ellipse to draw. 
\param b The blue value of the aa-ellipse to draw. 
\param a The alpha value of the aa-ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aaellipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (aaellipseColor
		(dst, x, y, rx, ry, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ---- Filled Ellipse */

/* Note: */
/* Based on algorithm from sge library with multiple-hline draw removal */
/* and other speedup changes. */

/*!
\brief Draw filled ellipse with blending.

Note: Based on algorithm from sge library with multiple-hline draw removal
and other speedup changes.

\param dst The surface to draw on.
\param x X coordinate of the center of the filled ellipse.
\param y Y coordinate of the center of the filled ellipse.
\param rx Horizontal radius in pixels of the filled ellipse.
\param ry Vertical radius in pixels of the filled ellipse.
\param color The color value of the filled ellipse to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int filledEllipseColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
{
	Sint16 left, right, top, bottom;
	int result;
	Sint16 x1, y1, x2, y2;
	int ix, iy;
	int h, i, j, k;
	int oh, oi, oj, ok;
	int xmh, xph;
	int xmi, xpi;
	int xmj, xpj;
	int xmk, xpk;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radii 
	*/
	if ((rx < 0) || (ry < 0)) {
		return (-1);
	}

	/*
	* Special case for rx=0 - draw a vline 
	*/
	if (rx == 0) {
		return (vlineColor(dst, x, y - ry, y + ry, color));
	}
	/*
	* Special case for ry=0 - draw a hline 
	*/
	if (ry == 0) {
		return (hlineColor(dst, x - rx, x + rx, y, color));
	}

	/*
	* Get circle and clipping boundary and 
	* test if bounding box of circle is visible 
	*/
	x2 = x + rx;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rx;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + ry;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - ry;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	} 

	/*
	* Init vars 
	*/
	oh = oi = oj = ok = 0xFFFF;

	/*
	* Draw 
	*/
	result = 0;
	if (rx > ry) {
		ix = 0;
		iy = rx * 64;

		do {
			h = (ix + 32) >> 6;
			i = (iy + 32) >> 6;
			j = (h * ry) / rx;
			k = (i * ry) / rx;

			if ((ok != k) && (oj != k)) {
				xph = x + h;
				xmh = x - h;
				if (k > 0) {
					result |= hlineColor(dst, xmh, xph, y + k, color);
					result |= hlineColor(dst, xmh, xph, y - k, color);
				} else {
					result |= hlineColor(dst, xmh, xph, y, color);
				}
				ok = k;
			}
			if ((oj != j) && (ok != j) && (k != j)) {
				xmi = x - i;
				xpi = x + i;
				if (j > 0) {
					result |= hlineColor(dst, xmi, xpi, y + j, color);
					result |= hlineColor(dst, xmi, xpi, y - j, color);
				} else {
					result |= hlineColor(dst, xmi, xpi, y, color);
				}
				oj = j;
			}

			ix = ix + iy / rx;
			iy = iy - ix / rx;

		} while (i > h);
	} else {
		ix = 0;
		iy = ry * 64;

		do {
			h = (ix + 32) >> 6;
			i = (iy + 32) >> 6;
			j = (h * rx) / ry;
			k = (i * rx) / ry;

			if ((oi != i) && (oh != i)) {
				xmj = x - j;
				xpj = x + j;
				if (i > 0) {
					result |= hlineColor(dst, xmj, xpj, y + i, color);
					result |= hlineColor(dst, xmj, xpj, y - i, color);
				} else {
					result |= hlineColor(dst, xmj, xpj, y, color);
				}
				oi = i;
			}
			if ((oh != h) && (oi != h) && (i != h)) {
				xmk = x - k;
				xpk = x + k;
				if (h > 0) {
					result |= hlineColor(dst, xmk, xpk, y + h, color);
					result |= hlineColor(dst, xmk, xpk, y - h, color);
				} else {
					result |= hlineColor(dst, xmk, xpk, y, color);
				}
				oh = h;
			}

			ix = ix + iy / ry;
			iy = iy - ix / ry;

		} while (i > h);
	}

	return (result);
}

/*!
\brief Draw filled ellipse with blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the filled ellipse.
\param y Y coordinate of the center of the filled ellipse.
\param rx Horizontal radius in pixels of the filled ellipse.
\param ry Vertical radius in pixels of the filled ellipse.
\param r The red value of the filled ellipse to draw. 
\param g The green value of the filled ellipse to draw. 
\param b The blue value of the filled ellipse to draw. 
\param a The alpha value of the filled ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledEllipseRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (filledEllipseColor
		(dst, x, y, rx, ry, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ----- pie */

/*!
\brief Internal float (low-speed) pie-calc implementation by drawing polygons.

Note: Determines vertex array and uses polygon or filledPolygon drawing routines to render.

\param dst The surface to draw on.
\param x X coordinate of the center of the pie.
\param y Y coordinate of the center of the pie.
\param rad Radius in pixels of the pie.
\param start Starting radius in degrees of the pie.
\param end Ending radius in degrees of the pie.
\param color The color value of the pie to draw (0xRRGGBBAA). 
\param filled Flag indicating if the pie should be filled (=1) or not (=0).

\returns Returns 0 on success, -1 on failure.
*/
int _pieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color, Uint8 filled)
{
	Sint16 left, right, top, bottom;
	Sint16 x1, y1, x2, y2;
	int result;
	double angle, start_angle, end_angle;
	double deltaAngle;
	double dr;
	int numpoints, i;
	Sint16 *vx, *vy;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Sanity check radii 
	*/
	if (rad < 0) {
		return (-1);
	}

	/*
	* Fixup angles
	*/
	start = start % 360;
	end = end % 360;

	/*
	* Special case for rad=0 - draw a point 
	*/
	if (rad == 0) {
		return (pixelColor(dst, x, y, color));
	}

	/*
	* Clip against circle, not pie (not 100% optimal).
	* Get pie's circle and clipping boundary and 
	* test if bounding box of circle is visible
	*/
	x2 = x + rad;
	left = dst->clip_rect.x;
	if (x2<left) {
		return(0);
	} 
	x1 = x - rad;
	right = dst->clip_rect.x + dst->clip_rect.w - 1;
	if (x1>right) {
		return(0);
	} 
	y2 = y + rad;
	top = dst->clip_rect.y;
	if (y2<top) {
		return(0);
	} 
	y1 = y - rad;
	bottom = dst->clip_rect.y + dst->clip_rect.h - 1;
	if (y1>bottom) {
		return(0);
	} 

	/*
	* Variable setup 
	*/
	dr = (double) rad;
	deltaAngle = 3.0 / dr;
	start_angle = (double) start *(2.0 * M_PI / 360.0);
	end_angle = (double) end *(2.0 * M_PI / 360.0);
	if (start > end) {
		end_angle += (2.0 * M_PI);
	}

	/* We will always have at least 2 points */
	numpoints = 2;

	/* Count points (rather than calculating it) */
	angle = start_angle;
	while (angle < end_angle) {
		angle += deltaAngle;
		numpoints++;
	}

	/* Allocate combined vertex array */
	vx = vy = (Sint16 *) malloc(2 * sizeof(Uint16) * numpoints);
	if (vx == NULL) {
		return (-1);
	}

	/* Update point to start of vy */
	vy += numpoints;

	/* Center */
	vx[0] = x;
	vy[0] = y;

	/* First vertex */
	angle = start_angle;
	vx[1] = x + (int) (dr * cos(angle));
	vy[1] = y + (int) (dr * sin(angle));

	if (numpoints<3)
	{
		result = lineColor(dst, vx[0], vy[0], vx[1], vy[1], color);
	}
	else
	{
		/* Calculate other vertices */
		i = 2;
		angle = start_angle;
		while (angle < end_angle) {
			angle += deltaAngle;
			if (angle>end_angle)
			{
				angle = end_angle;
			}
			vx[i] = x + (int) (dr * cos(angle));
			vy[i] = y + (int) (dr * sin(angle));
			i++;
		}

		/* Draw */
		if (filled) {
			result = filledPolygonColor(dst, vx, vy, numpoints, color);
		} else {
			result = polygonColor(dst, vx, vy, numpoints, color);
		}
	}

	/* Free combined vertex array */
	free(vx);

	return (result);
}

/*!
\brief Draw pie (outline) with alpha blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the pie.
\param y Y coordinate of the center of the pie.
\param rad Radius in pixels of the pie.
\param start Starting radius in degrees of the pie.
\param end Ending radius in degrees of the pie.
\param color The color value of the pie to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int pieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, 
	Sint16 start, Sint16 end, Uint32 color) 
{
	return (_pieColor(dst, x, y, rad, start, end, color, 0));

}

/*!
\brief Draw pie (outline) with alpha blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the pie.
\param y Y coordinate of the center of the pie.
\param rad Radius in pixels of the pie.
\param start Starting radius in degrees of the pie.
\param end Ending radius in degrees of the pie.
\param r The red value of the pie to draw. 
\param g The green value of the pie to draw. 
\param b The blue value of the pie to draw. 
\param a The alpha value of the pie to draw.

\returns Returns 0 on success, -1 on failure.
*/
int pieRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
	Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return (_pieColor(dst, x, y, rad, start, end,
		((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, 0));

}

/*!
\brief Draw filled pie with alpha blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the filled pie.
\param y Y coordinate of the center of the filled pie.
\param rad Radius in pixels of the filled pie.
\param start Starting radius in degrees of the filled pie.
\param end Ending radius in degrees of the filled pie.
\param color The color value of the filled pie to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int filledPieColor(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color)
{
	return (_pieColor(dst, x, y, rad, start, end, color, 1));
}

/*!
\brief Draw filled pie with alpha blending.

\param dst The surface to draw on.
\param x X coordinate of the center of the filled pie.
\param y Y coordinate of the center of the filled pie.
\param rad Radius in pixels of the filled pie.
\param start Starting radius in degrees of the filled pie.
\param end Ending radius in degrees of the filled pie.
\param r The red value of the filled pie to draw. 
\param g The green value of the filled pie to draw. 
\param b The blue value of the filled pie to draw. 
\param a The alpha value of the filled pie to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledPieRGBA(RSDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
	Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return (_pieColor(dst, x, y, rad, start, end,
		((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, 1));
}

/* ------ Trigon */

/*!
\brief Draw trigon (triangle outline) with alpha blending.

Note: Creates vertex array and uses polygon routine to render.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the trigon.
\param y1 Y coordinate of the first point of the trigon.
\param x2 X coordinate of the second point of the trigon.
\param y2 Y coordinate of the second point of the trigon.
\param x3 X coordinate of the third point of the trigon.
\param y3 Y coordinate of the third point of the trigon.
\param color The color value of the trigon to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int trigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
{
	Sint16 vx[3]; 
	Sint16 vy[3];

	vx[0]=x1;
	vx[1]=x2;
	vx[2]=x3;
	vy[0]=y1;
	vy[1]=y2;
	vy[2]=y3;

	return(polygonColor(dst,vx,vy,3,color));
}

/*!
\brief Draw trigon (triangle outline) with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the trigon.
\param y1 Y coordinate of the first point of the trigon.
\param x2 X coordinate of the second point of the trigon.
\param y2 Y coordinate of the second point of the trigon.
\param x3 X coordinate of the third point of the trigon.
\param y3 Y coordinate of the third point of the trigon.
\param r The red value of the trigon to draw. 
\param g The green value of the trigon to draw. 
\param b The blue value of the trigon to draw. 
\param a The alpha value of the trigon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int trigonRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Sint16 vx[3]; 
	Sint16 vy[3];

	vx[0]=x1;
	vx[1]=x2;
	vx[2]=x3;
	vy[0]=y1;
	vy[1]=y2;
	vy[2]=y3;

	return(polygonRGBA(dst,vx,vy,3,r,g,b,a));
}				 

/* ------ AA-Trigon */

/*!
\brief Draw anti-aliased trigon (triangle outline) with alpha blending.

Note: Creates vertex array and uses aapolygon routine to render.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the aa-trigon.
\param y1 Y coordinate of the first point of the aa-trigon.
\param x2 X coordinate of the second point of the aa-trigon.
\param y2 Y coordinate of the second point of the aa-trigon.
\param x3 X coordinate of the third point of the aa-trigon.
\param y3 Y coordinate of the third point of the aa-trigon.
\param color The color value of the aa-trigon to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int aatrigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
{
	Sint16 vx[3]; 
	Sint16 vy[3];

	vx[0]=x1;
	vx[1]=x2;
	vx[2]=x3;
	vy[0]=y1;
	vy[1]=y2;
	vy[2]=y3;

	return(aapolygonColor(dst,vx,vy,3,color));
}

/*!
\brief Draw anti-aliased trigon (triangle outline) with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the aa-trigon.
\param y1 Y coordinate of the first point of the aa-trigon.
\param x2 X coordinate of the second point of the aa-trigon.
\param y2 Y coordinate of the second point of the aa-trigon.
\param x3 X coordinate of the third point of the aa-trigon.
\param y3 Y coordinate of the third point of the aa-trigon.
\param r The red value of the aa-trigon to draw. 
\param g The green value of the aa-trigon to draw. 
\param b The blue value of the aa-trigon to draw. 
\param a The alpha value of the aa-trigon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aatrigonRGBA(RSDL_Surface * dst,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Sint16 vx[3]; 
	Sint16 vy[3];

	vx[0]=x1;
	vx[1]=x2;
	vx[2]=x3;
	vy[0]=y1;
	vy[1]=y2;
	vy[2]=y3;

	return(aapolygonRGBA(dst,vx,vy,3,r,g,b,a));
}				   

/* ------ Filled Trigon */

/*!
\brief Draw filled trigon (triangle) with alpha blending.

Note: Creates vertex array and uses aapolygon routine to render.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the filled trigon.
\param y1 Y coordinate of the first point of the filled trigon.
\param x2 X coordinate of the second point of the filled trigon.
\param y2 Y coordinate of the second point of the filled trigon.
\param x3 X coordinate of the third point of the filled trigon.
\param y3 Y coordinate of the third point of the filled trigon.
\param color The color value of the filled trigon to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int filledTrigonColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
{
	Sint16 vx[3]; 
	Sint16 vy[3];

	vx[0]=x1;
	vx[1]=x2;
	vx[2]=x3;
	vy[0]=y1;
	vy[1]=y2;
	vy[2]=y3;

	return(filledPolygonColor(dst,vx,vy,3,color));
}

/*!
\brief Draw filled trigon (triangle) with alpha blending.

Note: Creates vertex array and uses aapolygon routine to render.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the filled trigon.
\param y1 Y coordinate of the first point of the filled trigon.
\param x2 X coordinate of the second point of the filled trigon.
\param y2 Y coordinate of the second point of the filled trigon.
\param x3 X coordinate of the third point of the filled trigon.
\param y3 Y coordinate of the third point of the filled trigon.
\param r The red value of the filled trigon to draw. 
\param g The green value of the filled trigon to draw. 
\param b The blue value of the filled trigon to draw. 
\param a The alpha value of the filled trigon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledTrigonRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Sint16 vx[3]; 
	Sint16 vy[3];

	vx[0]=x1;
	vx[1]=x2;
	vx[2]=x3;
	vy[0]=y1;
	vy[1]=y2;
	vy[2]=y3;

	return(filledPolygonRGBA(dst,vx,vy,3,r,g,b,a));
}

/* ---- Polygon */

/*!
\brief Draw polygon with alpha blending.

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the polygon.
\param vy Vertex array containing Y coordinates of the points of the polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param color The color value of the polygon to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int polygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
{
	int result;
	int i;
	const Sint16 *x1, *y1, *x2, *y2;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Vertex array NULL check 
	*/
	if (vx == NULL) {
		return (-1);
	}
	if (vy == NULL) {
		return (-1);
	}

	/*
	* Sanity check 
	*/
	if (n < 3) {
		return (-1);
	}

	/*
	* Pointer setup 
	*/
	x1 = x2 = vx;
	y1 = y2 = vy;
	x2++;
	y2++;

	/*
	* Draw 
	*/
	result = 0;
	for (i = 1; i < n; i++) {
		result |= lineColor(dst, *x1, *y1, *x2, *y2, color);
		x1 = x2;
		y1 = y2;
		x2++;
		y2++;
	}
	result |= lineColor(dst, *x1, *y1, *vx, *vy, color);

	return (result);
}

/*!
\brief Draw polygon with alpha blending.

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the polygon.
\param vy Vertex array containing Y coordinates of the points of the polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the polygon to draw. 
\param g The green value of the polygon to draw. 
\param b The blue value of the polygon to draw. 
\param a The alpha value of the polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int polygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (polygonColor(dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ---- AA-Polygon */

/*!
\brief Draw anti-aliased polygon with alpha blending.

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the aa-polygon.
\param vy Vertex array containing Y coordinates of the points of the aa-polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param color The color value of the aa-polygon to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int aapolygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
{
	int result;
	int i;
	const Sint16 *x1, *y1, *x2, *y2;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Vertex array NULL check 
	*/
	if (vx == NULL) {
		return (-1);
	}
	if (vy == NULL) {
		return (-1);
	}

	/*
	* Sanity check 
	*/
	if (n < 3) {
		return (-1);
	}

	/*
	* Pointer setup 
	*/
	x1 = x2 = vx;
	y1 = y2 = vy;
	x2++;
	y2++;

	/*
	* Draw 
	*/
	result = 0;
	for (i = 1; i < n; i++) {
		result |= _aalineColor(dst, *x1, *y1, *x2, *y2, color, 0);
		x1 = x2;
		y1 = y2;
		x2++;
		y2++;
	}
	result |= _aalineColor(dst, *x1, *y1, *vx, *vy, color, 0);

	return (result);
}

/*!
\brief Draw anti-aliased polygon with alpha blending.

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the aa-polygon.
\param vy Vertex array containing Y coordinates of the points of the aa-polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the aa-polygon to draw. 
\param g The green value of the aa-polygon to draw. 
\param b The blue value of the aa-polygon to draw. 
\param a The alpha value of the aa-polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aapolygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (aapolygonColor(dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

/* ---- Filled Polygon */

/*!
\brief Internal helper qsort callback functions used in filled polygon drawing.

\param a The surface to draw on.
\param b Vertex array containing X coordinates of the points of the polygon.

\returns Returns 0 if a==b, a negative number if a<b or a positive number if a>b.
*/
int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *) a) - (*(const int *) b);
}

/*!
\brief Global vertex array to use if optional parameters are not given in filledPolygonMT calls.

Note: Used for non-multithreaded (default) operation of filledPolygonMT.
*/
static int *gfxPrimitivesPolyIntsGlobal = NULL;

/*!
\brief Flag indicating if global vertex array was already allocated.

Note: Used for non-multithreaded (default) operation of filledPolygonMT.
*/
static int gfxPrimitivesPolyAllocatedGlobal = 0;

/*!
\brief Draw filled polygon with alpha blending (multi-threaded capable).

Note: The last two parameters are optional; but are required for multithreaded operation.  

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param color The color value of the filled polygon to draw (0xRRGGBBAA). 
\param polyInts Preallocated, temporary vertex array used for sorting vertices. Required for multithreaded operation; set to NULL otherwise.
\param polyAllocated Flag indicating if temporary vertex array was allocated. Required for multithreaded operation; set to NULL otherwise.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonColorMT(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color, int **polyInts, int *polyAllocated)
{
	int result;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = NULL;
	int *gfxPrimitivesPolyIntsNew = NULL;
	int gfxPrimitivesPolyAllocated = 0;

	/*
	* Check visibility of clipping rectangle
	*/
	if ((dst->clip_rect.w==0) || (dst->clip_rect.h==0)) {
		return(0);
	}

	/*
	* Vertex array NULL check 
	*/
	if (vx == NULL) {
		return (-1);
	}
	if (vy == NULL) {
		return (-1);
	}

	/*
	* Sanity check number of edges
	*/
	if (n < 3) {
		return -1;
	}

	/*
	* Map polygon cache  
	*/
	if ((polyInts==NULL) || (polyAllocated==NULL)) {
		/* Use global cache */
		gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
		gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
	} else {
		/* Use local cache */
		gfxPrimitivesPolyInts = *polyInts;
		gfxPrimitivesPolyAllocated = *polyAllocated;
	}

	/*
	* Allocate temp array, only grow array 
	*/
	if (!gfxPrimitivesPolyAllocated) {
		gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
		gfxPrimitivesPolyAllocated = n;
	} else {
		if (gfxPrimitivesPolyAllocated < n) {
			gfxPrimitivesPolyIntsNew = (int *) realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
			if (!gfxPrimitivesPolyIntsNew) {
				if (!gfxPrimitivesPolyInts) {
					free(gfxPrimitivesPolyInts);
					gfxPrimitivesPolyInts = NULL;
				}
				gfxPrimitivesPolyAllocated = 0;
			} else {
				gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsNew;
				gfxPrimitivesPolyAllocated = n;
			}
		}
	}

	/*
	* Check temp array
	*/
	if (gfxPrimitivesPolyInts==NULL) {        
		gfxPrimitivesPolyAllocated = 0;
	}

	/*
	* Update cache variables
	*/
	if ((polyInts==NULL) || (polyAllocated==NULL)) { 
		gfxPrimitivesPolyIntsGlobal =  gfxPrimitivesPolyInts;
		gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
	} else {
		*polyInts = gfxPrimitivesPolyInts;
		*polyAllocated = gfxPrimitivesPolyAllocated;
	}

	/*
	* Check temp array again
	*/
	if (gfxPrimitivesPolyInts==NULL) {        
		return(-1);
	}

	/*
	* Determine Y maxima 
	*/
	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < n); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		} else if (vy[i] > maxy) {
			maxy = vy[i];
		}
	}

	/*
	* Draw, scanning y 
	*/
	result = 0;
	for (y = miny; (y <= maxy); y++) {
		ints = 0;
		for (i = 0; (i < n); i++) {
			if (!i) {
				ind1 = n - 1;
				ind2 = 0;
			} else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			} else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			} else {
				continue;
			}
			if ( ((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)) ) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			} 	    
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i+1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			result |= hlineColor(dst, xa, xb, y, color);
		}
	}

	return (result);
}

/*!
\brief Draw filled polygon with alpha blending (multi-threaded capable).

Note: The last two parameters are optional; but are required for multithreaded operation.  

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw. 
\param g The green value of the filled polygon to draw. 
\param b The blue value of the filed polygon to draw. 
\param a The alpha value of the filled polygon to draw.
\param polyInts Preallocated, temporary vertex array used for sorting vertices. Required for multithreaded operation; set to NULL otherwise.
\param polyAllocated Flag indicating if temporary vertex array was allocated. Required for multithreaded operation; set to NULL otherwise.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonRGBAMT(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int **polyInts, int *polyAllocated)
{
	/*
	* Draw 
	*/
	return (filledPolygonColorMT(dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, polyInts, polyAllocated));
}

/*!
\brief Draw filled polygon with alpha blending.

Note: Standard filledPolygon function is calling multithreaded version with NULL parameters
to use the global vertex cache.

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param color The color value of the filled polygon to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonColor(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
{
	/*
	* Draw 
	*/
	return (filledPolygonColorMT(dst, vx, vy, n, color, NULL, NULL));
}

/*!
\brief Draw filled polygon with alpha blending.

\param dst The surface to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw. 
\param g The green value of the filled polygon to draw. 
\param b The blue value of the filed polygon to draw. 
\param a The alpha value of the filled polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonRGBA(RSDL_Surface * dst, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw 
	*/
	return (filledPolygonColorMT(dst, vx, vy, n, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a, NULL, NULL));
}


/*!
\brief Internal function to initialize the Bresenham line iterator.

Example of use:
RSDL_gfxBresenhamIterator b;
_bresenhamInitialize (&b, x1, y1, x2, y2);
do { 
plot(b.x, b.y); 
} while (_bresenhamIterate(&b)==0); 

\param b Pointer to struct for bresenham line drawing state.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.

\returns Returns 0 on success, -1 on failure.
*/
int _bresenhamInitialize(RSDL_gfxBresenhamIterator *b, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2)
{
	int temp;

	if (b==NULL) {
		return(-1);
	}

	b->x = x1;
	b->y = y1;

	/* dx = abs(x2-x1), s1 = sign(x2-x1) */
	if ((b->dx = x2 - x1) != 0) {
		if (b->dx < 0) {
			b->dx = -b->dx;
			b->s1 = -1;
		} else {
			b->s1 = 1;
		}
	} else {
		b->s1 = 0;	
	}

	/* dy = abs(y2-y1), s2 = sign(y2-y1)    */
	if ((b->dy = y2 - y1) != 0) {
		if (b->dy < 0) {
			b->dy = -b->dy;
			b->s2 = -1;
		} else {
			b->s2 = 1;
		}
	} else {
		b->s2 = 0;	
	}

	if (b->dy > b->dx) {
		temp = b->dx;
		b->dx = b->dy;
		b->dy = temp;
		b->swapdir = 1;
	} else {
		b->swapdir = 0;
	}

	b->count = (b->dx<0) ? 0 : (unsigned int)b->dx;
	b->dy <<= 1;
	b->error = b->dy - b->dx;
	b->dx <<= 1;	

	return(0);
}


/*!
\brief Internal function to move Bresenham line iterator to the next position.

Maybe updates the x and y coordinates of the iterator struct.

\param b Pointer to struct for bresenham line drawing state.

\returns Returns 0 on success, 1 if last point was reached, 2 if moving past end-of-line, -1 on failure.
*/
int _bresenhamIterate(RSDL_gfxBresenhamIterator *b)
{	
	if (b==NULL) {
		return (-1);
	}

	/* last point check */
	if (b->count==0) {
		return (2);
	}

	while (b->error >= 0) {
		if (b->swapdir) {
			b->x += b->s1;
		} else  {
			b->y += b->s2;
		}

		b->error -= b->dx;
	}

	if (b->swapdir) {
		b->y += b->s2;
	} else {
		b->x += b->s1;
	}

	b->error += b->dy;	
	b->count--;		

	/* count==0 indicates "end-of-line" */
	return ((b->count) ? 0 : 1);
}


/*!
\brief Internal function to to draw parallel lines with Murphy algorithm.

\param m Pointer to struct for murphy iterator.
\param x X coordinate of point.
\param y Y coordinate of point.
\param d1 Direction square/diagonal.
*/
void _murphyParaline(RSDL_gfxMurphyIterator *m, Sint16 x, Sint16 y, int d1)
{
	int p;
	d1 = -d1;

	for (p = 0; p <= m->u; p++) {

		pixelColorNolock(m->dst, x, y, m->color);

		if (d1 <= m->kt) {
			if (m->oct2 == 0) {
				x++;
			} else {
				if (m->quad4 == 0) {
					y++;
				} else {
					y--;
				}
			}
			d1 += m->kv;
		} else {	
			x++;
			if (m->quad4 == 0) {
				y++;
			} else {
				y--;
			}
			d1 += m->kd;
		}
	}

	m->tempx = x;
	m->tempy = y;
}

/*!
\brief Internal function to to draw one iteration of the Murphy algorithm.

\param m Pointer to struct for murphy iterator.
\param miter Iteration count.
\param ml1bx X coordinate of a point.
\param ml1by Y coordinate of a point.
\param ml2bx X coordinate of a point.
\param ml2by Y coordinate of a point.
\param ml1x X coordinate of a point.
\param ml1y Y coordinate of a point.
\param ml2x X coordinate of a point.
\param ml2y Y coordinate of a point.

*/
void _murphyIteration(RSDL_gfxMurphyIterator *m, Uint8 miter, 
	Uint16 ml1bx, Uint16 ml1by, Uint16 ml2bx, Uint16 ml2by, 
	Uint16 ml1x, Uint16 ml1y, Uint16 ml2x, Uint16 ml2y)
{
	int atemp1, atemp2;
	int ftmp1, ftmp2;
	Uint16 m1x, m1y, m2x, m2y;	
	Uint16 fix, fiy, lax, lay, curx, cury;
	Uint16 px[4], py[4];
	RSDL_gfxBresenhamIterator b;

	if (miter > 1) {
		if (m->first1x != -32768) {
			fix = (m->first1x + m->first2x) / 2;
			fiy = (m->first1y + m->first2y) / 2;
			lax = (m->last1x + m->last2x) / 2;
			lay = (m->last1y + m->last2y) / 2;
			curx = (ml1x + ml2x) / 2;
			cury = (ml1y + ml2y) / 2;

			atemp1 = (fix - curx);
			atemp2 = (fiy - cury);
			ftmp1 = atemp1 * atemp1 + atemp2 * atemp2;
			atemp1 = (lax - curx);
			atemp2 = (lay - cury);
			ftmp2 = atemp1 * atemp1 + atemp2 * atemp2;

			if (ftmp1 <= ftmp2) {
				m1x = m->first1x;
				m1y = m->first1y;
				m2x = m->first2x;
				m2y = m->first2y;
			} else {
				m1x = m->last1x;
				m1y = m->last1y;
				m2x = m->last2x;
				m2y = m->last2y;
			}

			atemp1 = (m2x - ml2x);
			atemp2 = (m2y - ml2y);
			ftmp1 = atemp1 * atemp1 + atemp2 * atemp2;
			atemp1 = (m2x - ml2bx);
			atemp2 = (m2y - ml2by);
			ftmp2 = atemp1 * atemp1 + atemp2 * atemp2;

			if (ftmp2 >= ftmp1) {
				ftmp1 = ml2bx;
				ftmp2 = ml2by;
				ml2bx = ml2x;
				ml2by = ml2y;
				ml2x = ftmp1;
				ml2y = ftmp2;
				ftmp1 = ml1bx;
				ftmp2 = ml1by;
				ml1bx = ml1x;
				ml1by = ml1y;
				ml1x = ftmp1;
				ml1y = ftmp2;
			}

			_bresenhamInitialize(&b, m2x, m2y, m1x, m1y);
			do {
				pixelColorNolock(m->dst, b.x, b.y, m->color);
			} while (_bresenhamIterate(&b)==0);

			_bresenhamInitialize(&b, m1x, m1y, ml1bx, ml1by);
			do {
				pixelColorNolock(m->dst, b.x, b.y, m->color);
			} while (_bresenhamIterate(&b)==0);

			_bresenhamInitialize(&b, ml1bx, ml1by, ml2bx, ml2by);
			do {
				pixelColorNolock(m->dst, b.x, b.y, m->color);
			} while (_bresenhamIterate(&b)==0);

			_bresenhamInitialize(&b, ml2bx, ml2by, m2x, m2y);
			do {
				pixelColorNolock(m->dst, b.x, b.y, m->color);
			} while (_bresenhamIterate(&b)==0);

			px[0] = m1x;
			px[1] = m2x;
			px[2] = ml1bx;
			px[3] = ml2bx;
			py[0] = m1y;
			py[1] = m2y;
			py[2] = ml1by;
			py[3] = ml2by;			
			polygonColor(m->dst, (Sint16 *)px,(Sint16 *)py, 4, m->color);						
		}
	}

	m->last1x = ml1x;
	m->last1y = ml1y;
	m->last2x = ml2x;
	m->last2y = ml2y;
	m->first1x = ml1bx;
	m->first1y = ml1by;
	m->first2x = ml2bx;
	m->first2y = ml2by;
}


#define HYPOT(x,y) sqrt((double)(x)*(double)(x)+(double)(y)*(double)(y)) 

/*!
\brief Internal function to to draw wide lines with Murphy algorithm.

Draws lines parallel to ideal line.

\param m Pointer to struct for murphy iterator.
\param x1 X coordinate of first point.
\param y1 Y coordinate of first point.
\param x2 X coordinate of second point.
\param y2 Y coordinate of second point.
\param width Width of line.
\param miter Iteration count.

*/
void _murphyWideline(RSDL_gfxMurphyIterator *m, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint8 miter)
{	
	float offset = (float)width / 2.f;

	Sint16 temp;
	Sint16 ptx, pty, ml1x = x1, ml1y = y1, ml2x = x2, ml2y = y2, ml1bx = x1, ml1by = y1, ml2bx = x2, ml2by = y2;

	int d0, d1;		/* difference terms d0=perpendicular to line, d1=along line */

	int q;			/* pel counter,q=perpendicular to line */
	int tmp;

	int dd;			/* distance along line */
	int tk;			/* thickness threshold */
	double ang;		/* angle for initial point calculation */
	double sang, cang;

	/* Initialisation */
	m->u = x2 - x1;	/* delta x */
	m->v = y2 - y1;	/* delta y */

	if (m->u < 0) {	/* swap to make sure we are in quadrants 1 or 4 */
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;		
		m->u *= -1;
		m->v *= -1;
	}

	if (m->v < 0) {	/* swap to 1st quadrant and flag */
		m->v *= -1;
		m->quad4 = 1;
	} else {
		m->quad4 = 0;
	}

	if (m->v > m->u) {	/* swap things if in 2 octant */
		tmp = m->u;
		m->u = m->v;
		m->v = tmp;
		m->oct2 = 1;
	} else {
		m->oct2 = 0;
	}

	m->ku = m->u + m->u;	/* change in l for square shift */
	m->kv = m->v + m->v;	/* change in d for square shift */
	m->kd = m->kv - m->ku;	/* change in d for diagonal shift */
	m->kt = m->u - m->kv;	/* diag/square decision threshold */

	d0 = 0;
	d1 = 0;
	dd = 0;

	ang = atan((double) m->v / (double) m->u);	/* calc new initial point - offset both sides of ideal */	
	sang = sin(ang);
	cang = cos(ang);

	if (m->oct2 == 0) {
		ptx = x1 + (Sint16)lrint(offset * sang);
		if (m->quad4 == 0) {
			pty = y1 - (Sint16)lrint(offset * cang);
		} else {
			pty = y1 + (Sint16)lrint(offset * cang);
		}
	} else {
		ptx = x1 - (Sint16)lrint(offset * cang);
		if (m->quad4 == 0) {
			pty = y1 + (Sint16)lrint(offset * sang);
		} else {
			pty = y1 - (Sint16)lrint(offset * sang);
		}
	}

	/* used here for constant thickness line */
	tk = (int) (4. * HYPOT(ptx - x1, pty - y1) * HYPOT(m->u, m->v));

	if (miter == 0) {
		m->first1x = -32768;
		m->first1y = -32768;
		m->first2x = -32768;
		m->first2y = -32768;
		m->last1x = -32768;
		m->last1y = -32768;
		m->last2x = -32768;
		m->last2y = -32768;
	}

	for (q = 0; dd <= tk; q++) {	/* outer loop, stepping perpendicular to line */

		_murphyParaline(m, ptx, pty, d1);	/* call to inner loop - right edge */
		if (q == 0) {
			ml1x = ptx;
			ml1y = pty;
			ml1bx = m->tempx;
			ml1by = m->tempy;
		} else {
			ml2x = ptx;
			ml2y = pty;
			ml2bx = m->tempx;
			ml2by = m->tempy;
		}
		if (d0 < m->kt) {	/* square move */
			if (m->oct2 == 0) {
				if (m->quad4 == 0) {
					pty++;
				} else {
					pty--;
				}
			} else {
				ptx++;
			}
		} else {	/* diagonal move */
			dd += m->kv;
			d0 -= m->ku;
			if (d1 < m->kt) {	/* normal diagonal */
				if (m->oct2 == 0) {
					ptx--;
					if (m->quad4 == 0) {
						pty++;
					} else {
						pty--;
					}
				} else {
					ptx++;
					if (m->quad4 == 0) {
						pty--;
					} else {
						pty++;
					}
				}
				d1 += m->kv;
			} else {	/* double square move, extra parallel line */
				if (m->oct2 == 0) {
					ptx--;
				} else {
					if (m->quad4 == 0) {
						pty--;
					} else {
						pty++;
					}
				}
				d1 += m->kd;
				if (dd > tk) {
					_murphyIteration(m, miter, ml1bx, ml1by, ml2bx, ml2by, ml1x, ml1y, ml2x, ml2y);
					return;	/* breakout on the extra line */
				}
				_murphyParaline(m, ptx, pty, d1);
				if (m->oct2 == 0) {
					if (m->quad4 == 0) {
						pty++;
					} else {

						pty--;
					}
				} else {
					ptx++;
				}
			}
		}
		dd += m->ku;
		d0 += m->kv;
	}

	_murphyIteration(m, miter, ml1bx, ml1by, ml2bx, ml2by, ml1x, ml1y, ml2x, ml2y);
}


/*!
\brief Draw a thick line with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param width Width of the line in pixels. Must be >0.
\param color The color value of the line to draw (0xRRGGBBAA). 

\returns Returns 0 on success, -1 on failure.
*/
int thickLineColor(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint32 color)
{	
	int wh;
	RSDL_gfxMurphyIterator m;

	if (dst == NULL) return -1;
	if (width < 1) return -1;

	/* Special case: thick "point" */
	if ((x1 == x2) && (y1 == y2)) {
		wh = width / 2;
		return boxColor(dst, x1 - wh, y1 - wh, x2 + width, y2 + width, color);		
	}

	m.dst = dst;
	m.color = color;

	_murphyWideline(&m, x1, y1, x2, y2, width, 0);
	_murphyWideline(&m, x1, y1, x2, y2, width, 1);

	return(0);
}

/*!
\brief Draw a thick line with alpha blending.

\param dst The surface to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param width Width of the line in pixels. Must be >0.
\param r The red value of the character to draw. 
\param g The green value of the character to draw. 
\param b The blue value of the character to draw. 
\param a The alpha value of the character to draw.

\returns Returns 0 on success, -1 on failure.
*/	
int thickLineRGBA(RSDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return (thickLineColor(dst, x1, y1, x2, y2, width, 
		((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}

#ifdef NK_RETRO_OLD_EVENTS
extern retro_input_poll_t input_poll_cb;
extern retro_input_state_t input_state_cb;

struct nk_retro_event {

	char Key_Sate[512];
	char old_Key_Sate[512];
	int LSHIFTON;
	int MOUSE_EMULATED; /* 1 = joypad act as mouse in GUI */
	int MOUSE_PAS; /* 4 = default */
	int MOUSE_RELATIVE; /* 0 = absolute */
	int gmx;
	int gmy; /* mouse */
	int mouse_wu;
	int mouse_wd;
	int slowdown;
	int showpointer;
};

static struct nk_retro_event revent;
#endif

struct nk_retro_Font {
    int width;
    int height;
    struct nk_user_font handle;
};

static struct nk_retro {
    RSDL_Surface *screen_surface;
	nk_retro_Font *rsdlfont;
    struct nk_context ctx;
} retro;


static void
nk_retro_scissor(RSDL_Surface *surface, float x, float y, float w, float h)
{
    static RSDL_Rect RSDL_clip_rect;
    RSDL_clip_rect.x = (Sint16)x;
    RSDL_clip_rect.y = (Sint16)y;
    RSDL_clip_rect.w = (Uint16)(w  + 1);
    RSDL_clip_rect.h = (Uint16)h;
    RSDL_SetClipRect(surface, &RSDL_clip_rect);
}

static void
nk_retro_stroke_line(RSDL_Surface *surface, short x0, short y0, short x1,
    short y1, unsigned int line_thickness, struct nk_color col)
{
    thickLineRGBA(surface, x0, y0, x1, y1, line_thickness, col.r, col.g, col.b, col.a);
}

static void
nk_retro_stroke_rect(RSDL_Surface *surface, short x, short y, unsigned short w,
    unsigned short h, unsigned short r, unsigned short line_thickness, struct nk_color col)
{
    /* Note: thickness is not used by default */
    if (r == 0) {
        rectangleRGBA(surface, x, y, x + w, y + h, col.r, col.g, col.b, col.a); 
    } else {
        roundedRectangleRGBA(surface, x, y, x + w, y + h, r, col.r, col.g, col.b, col.a);
    }
}

static void
nk_retro_fill_rect(RSDL_Surface *surface, short x, short y, unsigned short w,
    unsigned short h, unsigned short r, struct nk_color col)
{
    if (r == 0) {
       boxRGBA(surface, x, y, x + w, y + h, col.r, col.g, col.b, col.a); 
    } else {
        roundedBoxRGBA(surface, x, y, x + w, y + h, r, col.r, col.g, col.b, col.a);
    }
}

static void 
nk_retro_fill_triangle(RSDL_Surface *surface, short x0, short y0, short x1, short y1, short x2, short y2, struct nk_color col)
{
    filledTrigonRGBA(surface, x0, y0, x1, y1, x2, y2, col.r, col.g, col.b, col.a);
}

static void
nk_retro_stroke_triangle(RSDL_Surface *surface, short x0, short y0, short x1,
    short y1, short x2, short y2, unsigned short line_thickness, struct nk_color col)
{
    /* Note: thickness is not used by default */
    aatrigonRGBA(surface, x0, y0, x1, y1, x2, y2, col.r, col.g, col.b, col.a); 
}

static void
nk_retro_fill_polygon(RSDL_Surface *surface, const struct nk_vec2i *pnts, int count, struct nk_color col)
{
    Sint16 p_x[NK_RSDL_MAX_POINTS];
    Sint16 p_y[NK_RSDL_MAX_POINTS];
    int i;
    for (i = 0; (i < count) && (i < NK_RSDL_MAX_POINTS); ++i) {
        p_x[i] = pnts[i].x;
        p_y[i] = pnts[i].y;
    }
    filledPolygonRGBA (surface, (Sint16 *)p_x, (Sint16 *)p_y, count, col.r, col.g, col.b, col.a);
}

static void
nk_retro_stroke_polygon(RSDL_Surface *surface, const struct nk_vec2i *pnts, int count,
    unsigned short line_thickness, struct nk_color col)
{
    /* Note: thickness is not used by default */
    Sint16 p_x[NK_RSDL_MAX_POINTS];
    Sint16 p_y[NK_RSDL_MAX_POINTS];
    int i;
    for (i = 0; (i < count) && (i < NK_RSDL_MAX_POINTS); ++i) {
        p_x[i] = pnts[i].x;
        p_y[i] = pnts[i].y;
    }
    aapolygonRGBA(surface, (Sint16 *)p_x, (Sint16 *)p_y, count, col.r, col.g, col.b, col.a); 
}

static void
nk_retro_stroke_polyline(RSDL_Surface *surface, const struct nk_vec2i *pnts,
    int count, unsigned short line_thickness, struct nk_color col)
{
    int x0, y0, x1, y1;
    if (count == 1) {
        x0 = pnts[0].x;
        y0 = pnts[0].y;
        x1 = x0;
        y1 = y0;
        thickLineRGBA(surface, x0, y0, x1, y1, (Uint8)line_thickness, col.r, col.g, col.b, col.a);
    } else if (count >= 2) {
        int i;
        for (i = 0; i < (count - 1); i++) {
            x0 = pnts[i].x;
            y0 = pnts[i].y;
            x1 = pnts[i + 1].x;
            y1 = pnts[i + 1].y;
            thickLineRGBA(surface, x0, y0, x1, y1, (Uint8)line_thickness, col.r, col.g, col.b, col.a);
        }
    }
}

static void
nk_retro_fill_circle(RSDL_Surface *surface, short x, short y, unsigned short w,
    unsigned short h, struct nk_color col)
{
    filledEllipseRGBA(surface,  x + w /2, y + h /2, w / 2, h / 2, col.r, col.g, col.b, col.a); 
}

static void
nk_retro_stroke_circle(RSDL_Surface *surface, short x, short y, unsigned short w,
    unsigned short h, unsigned short line_thickness, struct nk_color col)
{
    /* Note: thickness is not used by default */
    aaellipseRGBA (surface,  x + w /2, y + h /2, w / 2, h / 2, col.r, col.g, col.b, col.a); 
}

static void
nk_retro_stroke_curve(RSDL_Surface *surface, struct nk_vec2i p1,
    struct nk_vec2i p2, struct nk_vec2i p3, struct nk_vec2i p4, unsigned int num_segments,
    unsigned short line_thickness, struct nk_color col)
{
    unsigned int i_step;
    float t_step;
    struct nk_vec2i last = p1;

    num_segments = MAX(num_segments, 1);
    t_step = 1.0f/(float)num_segments;
    for (i_step = 1; i_step <= num_segments; ++i_step) {
        float t = t_step * (float)i_step;
        float u = 1.0f - t;
        float w1 = u*u*u;
        float w2 = 3*u*u*t;
        float w3 = 3*u*t*t;
        float w4 = t * t *t;
        float x = w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x;
        float y = w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y;
        nk_retro_stroke_line(surface, last.x, last.y, (short)x, (short)y, line_thickness, col);
        last.x = (short)x; last.y = (short)y;
    }
}

/*static*/ void
nk_retro_draw_text(RSDL_Surface *surface, short x, short y, unsigned short w, unsigned short h,
    const char *text, int len, nk_retro_Font *font, struct nk_color cbg, struct nk_color cfg)
{
    int i;
    nk_retro_fill_rect(surface, x, y, len * font->width, font->height, 0, cbg);

    for (i = 0; i < len; i++) {
        /* characterRGBA(surface, x, y, text[i], cfg.r, cfg.g, cfg.b, cfg.a); */
#ifdef M16B

	Retro_Draw_char(surface,  x,  y,  text[i],  1, 1,cfg.r<<8|cfg.g<<3|cfg.b>>3,0);

#if 0
	Retro_Draw_char(surface,  x,  y,  text[i],  1, 1,/*cfg.a<<8|*/cfg.r<<8|cfg.g<<3|cfg.b>>3, /*cbg.a<<24|*/cbg.r<<8|cbg.g<<3|cbg.b>>3);
#endif
#else
	Retro_Draw_char(surface,  x,  y,  text[i],  1, 1,cfg.a<<24|cfg.r<<16|cfg.g<<8|cfg.b, cbg.a<<24|cbg.r<<16|cbg.g<<8|cbg.b);
#endif
        x += font->width;
    }

/* FIXME TODO */
/* Retro_Draw_string(surface,  x,  y,  text, len, 1, 1,cfg.a<<24|cfg.r<<16|cfg.g<<8|cfg.b, cbg.a<<24|cbg.r<<16|cbg.g<<8|cbg.b); */

}
static void
interpolate_color(struct nk_color c1, struct nk_color c2, struct nk_color *result, float fraction)
{
    float r = c1.r + (c2.r - c1.r) * fraction;
    float g = c1.g + (c2.g - c1.g) * fraction;
    float b = c1.b + (c2.b - c1.b) * fraction;
    float a = c1.a + (c2.a - c1.a) * fraction;

    result->r = (nk_byte)NK_CLAMP(0, r, 255);
    result->g = (nk_byte)NK_CLAMP(0, g, 255);
    result->b = (nk_byte)NK_CLAMP(0, b, 255);
    result->a = (nk_byte)NK_CLAMP(0, a, 255);
}

static void
nk_retro_fill_rect_multi_color(RSDL_Surface *surface, short x, short y, unsigned short w, unsigned short h,
    struct nk_color left, struct nk_color top,  struct nk_color right, struct nk_color bottom)
{
    struct nk_color X1, X2, Y;
    float fraction_x, fraction_y;
    int i,j;

    for (j = 0; j < h; j++) {
        fraction_y = ((float)j) / h;
        for (i = 0; i < w; i++) {
            fraction_x = ((float)i) / w;
            interpolate_color(left, top, &X1, fraction_x);
            interpolate_color(right, bottom, &X2, fraction_x);
            interpolate_color(X1, X2, &Y, fraction_y);
            pixelRGBA(surface, x + i, y + j, Y.r, Y.g, Y.b, Y.a); 
        }
    }
}

static void
nk_retro_clear(RSDL_Surface *surface, struct nk_color col)
{
    nk_retro_fill_rect(surface, 0, 0, surface->w, surface->h, 0, col);
}

static void
nk_retro_blit(RSDL_Surface *surface)
{
}

NK_API void
nk_retro_render(struct nk_color clear)
{
    const struct nk_command *cmd;

    RSDL_Surface *screen_surface = retro.screen_surface;
    nk_retro_clear(screen_surface, clear);

    nk_foreach(cmd, &retro.ctx)
    {
        switch (cmd->type) {
        case NK_COMMAND_NOP: break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
            nk_retro_scissor(screen_surface, s->x, s->y, s->w, s->h);
        } break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line *l = (const struct nk_command_line *)cmd;
            nk_retro_stroke_line(screen_surface, l->begin.x, l->begin.y, l->end.x,
                l->end.y, l->line_thickness, l->color);
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
            nk_retro_stroke_rect(screen_surface, r->x, r->y, r->w, r->h,
                (unsigned short)r->rounding, r->line_thickness, r->color);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
            nk_retro_fill_rect(screen_surface, r->x, r->y, r->w, r->h,
                (unsigned short)r->rounding, r->color);
        } break;
        case NK_COMMAND_CIRCLE: {
            const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
            nk_retro_stroke_circle(screen_surface, c->x, c->y, c->w, c->h, c->line_thickness, c->color);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
            nk_retro_fill_circle(screen_surface, c->x, c->y, c->w, c->h, c->color);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
            nk_retro_stroke_triangle(screen_surface, t->a.x, t->a.y, t->b.x, t->b.y,
                t->c.x, t->c.y, t->line_thickness, t->color);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
            nk_retro_fill_triangle(screen_surface, t->a.x, t->a.y, t->b.x, t->b.y, t->c.x, t->c.y, t->color);
        } break;
        case NK_COMMAND_POLYGON: {
            const struct nk_command_polygon *p =(const struct nk_command_polygon*)cmd;
            nk_retro_stroke_polygon(screen_surface, p->points, p->point_count, p->line_thickness,p->color);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
            nk_retro_fill_polygon(screen_surface, p->points, p->point_count, p->color);
        } break;
        case NK_COMMAND_POLYLINE: {
            const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
            nk_retro_stroke_polyline(screen_surface, p->points, p->point_count, p->line_thickness, p->color);
        } break;
        case NK_COMMAND_TEXT: {
            const struct nk_command_text *t = (const struct nk_command_text*)cmd;
            nk_retro_draw_text(screen_surface, t->x, t->y, t->w, t->h,
                (const char*)t->string, t->length,
                (nk_retro_Font*)t->font->userdata.ptr,
                t->background, t->foreground);
        } break;
        case NK_COMMAND_CURVE: {
            const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
            nk_retro_stroke_curve(screen_surface, q->begin, q->ctrl[0], q->ctrl[1],
                q->end, 22, q->line_thickness, q->color);
        } break;
        case NK_COMMAND_RECT_MULTI_COLOR: {
            const struct nk_command_rect_multi_color *r = (const struct nk_command_rect_multi_color *)cmd;
            nk_retro_fill_rect_multi_color(screen_surface, r->x, r->y, r->w, r->h, r->left, r->top, r->right, r->bottom);
        } break;
        case NK_COMMAND_IMAGE:
        case NK_COMMAND_ARC:
        case NK_COMMAND_ARC_FILLED:
        default: break;
        }
    }
    nk_retro_blit(retro.screen_surface);
    nk_clear(&retro.ctx);

#ifdef NK_RETRO_OLD_EVENTS
    if(revent.showpointer==1)draw_cross(retro.screen_surface,revent.gmx,revent.gmy);
#endif
}

static void
nk_retro_clipbard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    /* Not supported in SDL 1.2. Use platform specific code.  */
}

static void
nk_retro_clipbard_copy(nk_handle usr, const char *text, int len)
{
    /* Not supported in SDL 1.2. Use platform specific code.  */
}

nk_retro_Font*
nk_retrofont_create(const char *name, int size)
{

   nk_retro_Font *font = (nk_retro_Font*)calloc(1, sizeof(nk_retro_Font));
    font->width = 8; /* Default in  the RSDL_gfx library */
    font->height = 8; /* Default in  the RSDL_gfx library */
    if (!font)
        return NULL;
   /* font->handle */

    return font;
}
void
nk_retrofont_del(nk_retro_Font *font)
{
    if(!font) return;

    free(font);
}

static float
nk_retro_get_text_width(nk_handle handle, float height, const char *text, int len)
{
    nk_retro_Font *font = (nk_retro_Font*)handle.ptr;
 
    if(!font || !text)
        return 0;
    return (float)(len * font->width);
}

#ifdef NK_RETRO_OLD_EVENTS
static void retro_init_event()
{
	revent.MOUSE_EMULATED=-1;
	revent.MOUSE_PAS=4;
	revent.MOUSE_RELATIVE=10;
	revent.gmx=(retro.screen_surface->w/2)-1;
	revent.gmy=(retro.screen_surface->h/2)-1;
	revent.mouse_wu=0;
	revent.mouse_wd=0;
	revent.slowdown=0;
	memset(revent.Key_Sate,0,512);
	memset(revent.old_Key_Sate ,0, sizeof(revent.old_Key_Sate));
	revent.LSHIFTON=-1;
	revent.showpointer=1;
}

NK_API struct nk_retro_event* 
nk_retro_event_ptr()
{
	nk_retro_event* event=&revent;
	return event;
}
#endif

NK_API void
nk_retro_init(void)
{
    struct nk_user_font *font=&retro.rsdlfont->handle;

    font->userdata = nk_handle_ptr(retro.rsdlfont);
    font->height = (float)retro.rsdlfont->height;
    font->width = nk_retro_get_text_width;

    nk_init_default(&retro.ctx, font);
    retro.ctx.clip.copy = nk_retro_clipbard_copy;
    retro.ctx.clip.paste = nk_retro_clipbard_paste;
    retro.ctx.clip.userdata = nk_handle_ptr(0);

#ifdef NK_RETRO_OLD_EVENTS
    retro_init_event();
#endif
}

NK_API void
nk_retro_set_font(nk_retro_Font *xfont)
{
    struct nk_user_font *font = &xfont->handle;
    font->userdata = nk_handle_ptr(xfont);
    font->height = (float)xfont->height;
    font->width = nk_retro_get_text_width;
    nk_style_set_font(&retro.ctx, font);
}

#ifdef NK_RETRO_OLD_EVENTS
static void retro_key(int key,int down)
{
	struct nk_context *ctx = &retro.ctx;
	if(key<512);
	else return;

        if (key == RETROK_RSHIFT || key == RETROK_LSHIFT) nk_input_key(ctx, NK_KEY_SHIFT, down);
        else if (key == RETROK_DELETE)    nk_input_key(ctx, NK_KEY_DEL, down);
        else if (key == RETROK_RETURN)    nk_input_key(ctx, NK_KEY_ENTER, down);
        else if (key == RETROK_TAB)       nk_input_key(ctx, NK_KEY_TAB, down);
        else if (key == RETROK_LEFT)      nk_input_key(ctx, NK_KEY_LEFT, down);
        else if (key == RETROK_RIGHT)     nk_input_key(ctx, NK_KEY_RIGHT, down);
        else if (key == RETROK_UP)        nk_input_key(ctx, NK_KEY_UP, down);
        else if (key == RETROK_DOWN)      nk_input_key(ctx, NK_KEY_DOWN, down);
        else if (key == RETROK_BACKSPACE) nk_input_key(ctx, NK_KEY_BACKSPACE, down);
        else if (key == RETROK_HOME)      nk_input_key(ctx, NK_KEY_TEXT_START, down);
        else if (key == RETROK_END)       nk_input_key(ctx, NK_KEY_TEXT_END, down);
        else if (key == RETROK_SPACE && !down) nk_input_char(ctx, ' ');

        else if (key >= RETROK_0 && key <= RETROK_9 && down) {
                    nk_rune rune = '0' + key - RETROK_0;
                    nk_input_unicode(ctx, rune);
        }
        else if (key >= RETROK_PERIOD && down) {
                    nk_input_unicode(ctx, '.');
        }
	else if (key >= RETROK_a && key <= RETROK_z && down) {
                    nk_rune rune = 'a' + key - RETROK_a;
                    rune = ((1 == revent.LSHIFTON) ? (nk_rune)nk_to_upper((int)rune):rune);
                    nk_input_unicode(ctx, rune);
        }
}


static void mousebut(int but,int down,int x,int y){

	struct nk_context *ctx = &retro.ctx;

 	if(but==1)nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
 	else if(but==2)nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
 	else if(but==3)nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
	/* else if(but==4)nk_input_scroll(ctx,(float)down); */
	/* printf("but:%d %s (%d,%d)\n",but,down==1?"pressed":"released",x,y); */
}

static void Process_key()
{
	int i;

	for(i=0;i<320;i++)
        	revent.Key_Sate[i]=input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,i) ? 0x80: 0;
   
	if(memcmp( revent.Key_Sate,revent.old_Key_Sate , sizeof(revent.Key_Sate) ) )
	 	for(i=0;i<320;i++)
			if(revent.Key_Sate[i] && revent.Key_Sate[i]!=revent.old_Key_Sate[i]  )
        	{
	
				if(i==RETROK_RSHIFT){
					revent.LSHIFTON=-revent.LSHIFTON;
					printf("Modifier shift pressed %d \n",revent.LSHIFTON); 
					continue;
				}
/*
				if(i==RETROK_F12){
					//play_tape();
					continue;
				}

				if(i==RETROK_RCTRL){
					//CTRLON=-CTRLON;
					printf("Modifier crtl pressed %d \n",CTRLON); 
					continue;
				}
				if(i==RETROK_RSHIFT){
					//SHITFON=-SHITFON;
					printf("Modifier shift pressed %d \n",SHIFTON); 
					continue;
				}

				if(i==RETROK_LALT){
					//KBMOD=-KBMOD;
					printf("Modifier alt pressed %d \n",KBMOD); 
					continue;
				}
//printf("press: %d \n",i);
*/
				retro_key(i,1);
	
        	}	
        	else if ( !revent.Key_Sate[i] && revent.Key_Sate[i]!=revent.old_Key_Sate[i]  )
        	{
				if(i==RETROK_LSHIFT){
					revent.LSHIFTON=-revent.LSHIFTON;
					printf("Modifier shift released %d \n",revent.LSHIFTON); 
					continue;
				}
/*
				if(i==RETROK_F12){
      				//kbd_buf_feed("|tape\nrun\"\n^");
					continue;
				}

				if(i==RETROK_RCTRL){
					CTRLON=-CTRLON;
					printf("Modifier crtl released %d \n",CTRLON); 
					continue;
				}
				if(i==RETROK_RSHIFT){
					SHIFTON=-SHIFTON;
					printf("Modifier shift released %d \n",SHIFTON); 
					continue;
				}

				if(i==RETROK_LALT){
					KBMOD=-KBMOD;
					printf("Modifier alt released %d \n",KBMOD); 
					continue;
				}
//printf("release: %d \n",i);
*/
				retro_key(i,0);
	
        	}	

	memcpy(revent.old_Key_Sate,revent.Key_Sate , sizeof(revent.Key_Sate) );

}

NK_API void
nk_retro_handle_event(int *evt,int poll)
{
   struct nk_context *ctx = &retro.ctx;

   if(poll)input_poll_cb();

   static int lmx=0,lmy=0;
   static int mmbL=0,mmbR=0,mmbM=0;
   static int mbt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   int mouse_l,mouse_m,mouse_r;

   int16_t mouse_x=0,mouse_y=0;

   Process_key();

   int i=2;//TOGGLE: real mouse/ joypad emulate mouse 
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
      mbt[i]=1;
   else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      mbt[i]=0;
      revent.MOUSE_EMULATED=-revent.MOUSE_EMULATED;
   }

   revent.mouse_wu = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELUP);
   revent.mouse_wd = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELDOWN);
   if(revent.mouse_wu || revent.mouse_wd)mousebut(4,revent.mouse_wd?-1:1,0,0);

   if(revent.MOUSE_EMULATED==1){

      if(revent.slowdown>0)return /*1*/;

      mouse_l=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
      mouse_r=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
      mouse_m=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y);


   }
   else {
   		mouse_l    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
   		mouse_r    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);
   		mouse_m    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE); 
   }

	//relative
	if(revent.MOUSE_RELATIVE){

   		if(revent.MOUSE_EMULATED==1){

      		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))mouse_x += revent.MOUSE_PAS;
      		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))mouse_x -= revent.MOUSE_PAS;
      		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))mouse_y += revent.MOUSE_PAS;
      		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))mouse_y -= revent.MOUSE_PAS;

   		}
   		else {

   			mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
   			mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);

   		}

   		revent.gmx+=mouse_x;
   		revent.gmy+=mouse_y;
   		if(revent.gmx<0)revent.gmx=0;
   		if(revent.gmx>(int)(retro.screen_surface->w-1))revent.gmx=retro.screen_surface->w-1;
   		if(revent.gmy<0)revent.gmy=0;
   		if(revent.gmy>(int)(retro.screen_surface->h-1))revent.gmy=retro.screen_surface->h-1;

	}
	else{

		//absolute
		//FIXME FULLSCREEN no pointer
		int p_x = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
 		int p_y = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

 		if(p_x!=0 && p_y!=0){
			int px=(int)((p_x+0x7fff)*retro.screen_surface->w/0xffff);
			int py=(int)((p_y+0x7fff)*retro.screen_surface->h/0xffff);
			//printf("px:%d py:%d (%d,%d)\n",p_x,p_y,px,py);
			revent.gmx=px;
			revent.gmy=py;

#if defined(__ANDROID__) || defined(ANDROID)
  			//mouse_l=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
 			//mouse_r=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);

			if(holdleft==0){
				starthold=GetTicks();
				holdleft=1;	
			}
			else if(holdleft==1){
	
				timehold=GetTicks()-starthold;
	
				if(timehold>250){
					mouse_l=input_state_cb(0, RETRO_DEVICE_POINTER, 	0,RETRO_DEVICE_ID_POINTER_PRESSED);
				}
			}
	
			//mouse_l=input_state_cb(0, RETRO_DEVICE_POINTER, 0,RETRO_DEVICE_ID_POINTER_PRESSED);

			//FIXME: mouse left button in scale widget.
#endif

 		}

	}

    if(mmbL==0 && mouse_l){

		mmbL=1;		
		mousebut(1,1,revent.gmx,revent.gmy);
    }
    else if(mmbL==1 && !mouse_l) {
#if defined(__ANDROID__) || defined(ANDROID)
		holdleft=0;
#endif
		mmbL=0;
		mousebut(1,0,revent.gmx,revent.gmy);
    }

    if(mmbR==0 && mouse_r){

      	mmbR=1;
		mousebut(2,1,revent.gmx,revent.gmy);		
   	}
   	else if(mmbR==1 && !mouse_r) {

      	mmbR=0;
		mousebut(2,0,revent.gmx,revent.gmy);
   	}

   	if(mmbM==0 && mouse_m){

      	mmbM=1;
		mousebut(3,1,revent.gmx,revent.gmy);		
   	}
   	else if(mmbM==1 && !mouse_m) {

      	mmbM=0;
		mousebut(3,0,revent.gmx,revent.gmy);
   	}

	if(revent.gmx!=lmx || lmy!=revent.gmy){
		nk_input_motion(ctx, revent.gmx, revent.gmy);
		//printf("mx:%d my:%d \n",gmx,gmy);
	}
	lmx=revent.gmx;lmy=revent.gmy;

}
#endif

NK_API void
nk_retro_shutdown(void)
{
    nk_free(&retro.ctx);
}

#endif
