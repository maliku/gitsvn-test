/*
    MIL - A embedded system graphics Middle Interface Layer.
    Copyright (C) 2009-2010 Dong Kai

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Dong Kai
    libmilui@gmail.com
*/

/** @file MIL_video.h
 *  Header file for access to the MIL raw framebuffer window
 */

#ifndef _MIL_video_h
#define _MIL_video_h

#include "MIL_stdinc.h"
//#include "MIL_error.h"
#include "MIL_rwops.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/** @name Transparency definitions
 *  These define alpha as the opacity of a surface
 */
/*@{*/
#define MIL_ALPHA_OPAQUE 255
#define MIL_ALPHA_TRANSPARENT 0
/*@}*/

/** @name Useful data types */
/*@{*/
STRUCT {
	Sint32 x, y;
	Uint32 w, h;
} MIL_Rect;

STRUCT {
	float x, y;
	float w, h;
} MIL_RectF;

STRUCT {
	Sint32 x, y;
} MIL_Point;

STRUCT {
	float x, y;
} MIL_PointF;

STRUCT {
	Sint32 w, h;
} MIL_Size;

STRUCT {
	float w, h;
} MIL_SizeF;

typedef struct MIL_Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 unused;
} MIL_Color;

typedef struct MIL_Palette {
	int       ncolors;
	MIL_Color *colors;
} MIL_Palette;
/*@}*/

/**
 * name MIL_PixelFormat
 * brief Interface for Pixel Format, which is readonly structure for user.
 */
CLASS(MIL_PixelFormat) 
{
    METHOD_DECLARE_BEGIN(MIL_PixelFormat)
        Uint32 (*mapRGB)(_CSELF, Uint8 r, Uint8 g, Uint8 b);
        Uint32 (*mapRGBA)(_CSELF, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void (*getRGB)(_CSELF, Uint32 pixel, Uint8 *r,Uint8 *g,Uint8 *b);
        void (*getRGBA)(_CSELF, Uint32 pixel, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
        Uint8* (*mapNto1)(_CSELF, MIL_PixelFormat *dst, int *identical);
        Uint8* (*map1toN)(_CSELF, MIL_PixelFormat *dst);
        Uint8  (*getBytesPerPixel)(_CSELF);
        Uint8  (*getBitsPerPixel)(_CSELF);
        Uint8  (*getAlpha)(_CSELF);
        Uint32 (*getColorKey)(_CSELF);
    METHOD_DECLARE_END
};

extern MIL_PixelFormat* MIL_AllocFormat(int bpp,
        Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

#if 0
/** This structure should be treated as read-only, except for 'pixels',
 *  which, if not NULL, contains the raw pixel data for the surface.
 */
CLASS(MIL_Surface) 
{
    METHOD_DECLARE_BEGIN(MIL_Surface)
    void* (*lock)(_SELF);
    void (*unlock)(_SELF);
    int  (*setColorKey)(_SELF, Uint32 flag, Uint32 key);
    int  (*setAlpha)(_SELF, Uint32 flag, Uint8 alpha);
    void (*setClipRect)(_SELF, const MIL_Rect *rect);
    void (*getClipRect)(_SELF, MIL_Rect *rect);
    int  (*blitSurface)(_SELF, MIL_Rect *srcrect, MIL_Surface *dst, MIL_Rect *dstrect);
    int  (*fillRect)(_SELF, MIL_Rect *dstrect, Uint32 color);
    int  (*saveBMP)(_SELF, const char *file);
    MIL_Surface* (*displayFormat)(_SELF);
    MIL_Surface* (*displayFormatAlpha)(_SELF);
    MIL_Surface* (*convert)(_SELF, MIL_PixelFormat *fmt, Uint32 flags);

    Uint32 (*getWidth)(_CSELF);
    Uint32 (*getHeight)(_CSELF);
    Uint32 (*getPitch)(_CSELF);
    Uint32 (*getFlags)(_CSELF);
    Uint8 (*getBitsPerPixel)(_CSELF);
    Uint8 (*getBytesPerPixel)(_CSELF);

    METHOD_DECLARE_END

    
};
#endif

/** @name MIL_Surface Flags
 *  These are the currently supported flags for the MIL_surface
 */
/*@{*/

/** Available for CreateRGBSurface() or SetVideoMode() */
/*@{*/
#define MIL_SWSURFACE	0x00000000	/**< MIL_Surface is in system memory */
#define MIL_HWSURFACE	0x00000001	/**< MIL_Surface is in video memory */
#define MIL_ASYNCBLIT	0x00000004	/**< Use asynchronous blits if possible */
/*@}*/

/** Available for MIL_SetVideoMode() */
/*@{*/
#define MIL_ANYFORMAT	0x10000000	/**< Allow any video depth/pixel-format */
#define MIL_HWPALETTE	0x20000000	/**< MIL_Surface has exclusive palette */
#define MIL_DOUBLEBUF	0x40000000	/**< Set up double-buffered video mode */
#define MIL_FULLSCREEN	0x80000000	/**< MIL_Surface is a full screen display */
#define MIL_OPENGL      0x00000002      /**< Create an OpenGL rendering context */
#define MIL_OPENGLBLIT	0x0000000A	/**< Create an OpenGL rendering context and use it for blitting */
#define MIL_RESIZABLE	0x00000010	/**< This video mode may be resized */
#define MIL_NOFRAME	0x00000020	/**< No window caption or edge frame */
/*@}*/

/** Used internally (read-only) */
/*@{*/
#define MIL_HWACCEL	0x00000100	/**< Blit uses hardware acceleration */
#define MIL_SRCCOLORKEY	0x00001000	/**< Blit uses a source color key */
#define MIL_RLEACCELOK	0x00002000	/**< Private flag */
#define MIL_RLEACCEL	0x00004000	/**< MIL_Surface is RLE encoded */
#define MIL_SRCALPHA	0x00010000	/**< Blit uses source alpha blending */
#define MIL_PREALLOC	0x01000000	/**< MIL_Surface uses preallocated memory */
/*@}*/

/*@}*/

/** Evaluates to true if the surface needs to be locked before access */
#define MIL_MUSTLOCK(surface)	\
  (surface->offset ||		\
  ((surface->flags & (MIL_HWSURFACE|MIL_ASYNCBLIT|MIL_RLEACCEL)) != 0))


/** Useful for determining the video hardware capabilities */
typedef struct MIL_VideoInfo {
	Uint32 hw_available :1;	/**< Flag: Can you create hardware surfaces? */
	Uint32 wm_available :1;	/**< Flag: Can you talk to a window manager? */
	Uint32 UnusedBits1  :6;
	Uint32 UnusedBits2  :1;
	Uint32 blit_hw      :1;	/**< Flag: Accelerated blits HW --> HW */
	Uint32 blit_hw_CC   :1;	/**< Flag: Accelerated blits with Colorkey */
	Uint32 blit_hw_A    :1;	/**< Flag: Accelerated blits with Alpha */
	Uint32 blit_sw      :1;	/**< Flag: Accelerated blits SW --> HW */
	Uint32 blit_sw_CC   :1;	/**< Flag: Accelerated blits with Colorkey */
	Uint32 blit_sw_A    :1;	/**< Flag: Accelerated blits with Alpha */
	Uint32 blit_fill    :1;	/**< Flag: Accelerated color fill */
	Uint32 UnusedBits3  :16;
	Uint32 video_mem;	/**< The total amount of video memory (in K) */
	MIL_PixelFormat *vfmt;	/**< Value: The format of the video surface */
	int    current_w;	/**< Value: The current video mode width */
	int    current_h;	/**< Value: The current video mode height */
} MIL_VideoInfo;


/** @name Overlay Formats
 *  The most common video overlay formats.
 *  For an explanation of these pixel formats, see:
 *	http://www.webartz.com/fourcc/indexyuv.htm
 *
 *  For information on the relationship between color spaces, see:
 *  http://www.neuro.sfc.keio.ac.jp/~aly/polygon/info/color-space-faq.html
 */
/*@{*/
#define MIL_YV12_OVERLAY  0x32315659	/**< Planar mode: Y + V + U  (3 planes) */
#define MIL_IYUV_OVERLAY  0x56555949	/**< Planar mode: Y + U + V  (3 planes) */
#define MIL_YUY2_OVERLAY  0x32595559	/**< Packed mode: Y0+U0+Y1+V0 (1 plane) */
#define MIL_UYVY_OVERLAY  0x59565955	/**< Packed mode: U0+Y0+V0+Y1 (1 plane) */
#define MIL_YVYU_OVERLAY  0x55595659	/**< Packed mode: Y0+V0+Y1+U0 (1 plane) */
/*@}*/

/** The YUV hardware video overlay */
typedef struct MIL_Overlay {
	Uint32 format;				/**< Read-only */
	int w, h;				/**< Read-only */
	int planes;				/**< Read-only */
	Uint16 *pitches;			/**< Read-only */
	Uint8 **pixels;				/**< Read-write */

	/** @name Hardware-specific surface info */
        /*@{*/
	struct private_yuvhwfuncs *hwfuncs;
	struct private_yuvhwdata *hwdata;
        /*@{*/

	/** @name Special flags */
        /*@{*/
	Uint32 hw_overlay :1;	/**< Flag: This overlay hardware accelerated? */
	Uint32 UnusedBits :31;
        /*@}*/
} MIL_Overlay;


/** Public enumeration for setting the OpenGL window attributes. */
typedef enum {
    MIL_GL_RED_SIZE,
    MIL_GL_GREEN_SIZE,
    MIL_GL_BLUE_SIZE,
    MIL_GL_ALPHA_SIZE,
    MIL_GL_BUFFER_SIZE,
    MIL_GL_DOUBLEBUFFER,
    MIL_GL_DEPTH_SIZE,
    MIL_GL_STENCIL_SIZE,
    MIL_GL_ACCUM_RED_SIZE,
    MIL_GL_ACCUM_GREEN_SIZE,
    MIL_GL_ACCUM_BLUE_SIZE,
    MIL_GL_ACCUM_ALPHA_SIZE,
    MIL_GL_STEREO,
    MIL_GL_MULTISAMPLEBUFFERS,
    MIL_GL_MULTISAMPLESAMPLES,
    MIL_GL_ACCELERATED_VISUAL,
    MIL_GL_SWAP_CONTROL
} MIL_GLattr;

/** @name flags for MIL_SetPalette() */
/*@{*/
#define MIL_LOGPAL 0x01
#define MIL_PHYSPAL 0x02
/*@}*/

typedef enum {
	MIL_GRAB_QUERY = -1,
	MIL_GRAB_OFF = 0,
	MIL_GRAB_ON = 1,
	MIL_GRAB_FULLSCREEN	/**< Used internally */
} MIL_GrabMode;
                   
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _MIL_video_h */
