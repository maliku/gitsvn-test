/*!============================================================================
 * @file surface.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-11 17:01:39 
 *  Organization: http://www.ds0101.net
 */

#ifndef  SURFACE_INC
#define  SURFACE_INC
#include "MIL_video.h"
#include "MIL_mutex.h"
#include "pixel_format.h"

struct _MIL_BlitMap;
typedef struct _MIL_BlitMap MIL_BlitMap;

CLASS(Surface)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(Surface)
    void* (*lock)(_SELF);
    void (*unlock)(_SELF);
    int  (*setColorKey)(_SELF, Uint32 flag, Uint32 key);
    int  (*setAlpha)(_SELF, Uint32 flag, Uint8 alpha);
    MIL_bool (*setClipRect)(_SELF, const MIL_Rect *rect);
    void (*getClipRect)(_SELF, MIL_Rect *rect);
    int  (*blitSurface)(_SELF, MIL_Rect *srcrect, Surface *dst, MIL_Rect *dstrect);
    int  (*fillRect)(_SELF, MIL_Rect *dstrect, Uint32 color);
    int  (*saveBMP)(_SELF, const char *file);
    Surface* (*displayFormat)(_SELF);
    Surface* (*displayFormatAlpha)(_SELF);
    Surface* (*convert)(_SELF, PixelFormat *fmt, Uint32 flags);
    PixelFormat* (*reallocFormat)(_SELF, int bpp,
            Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
    void (*formatChanged)(_SELF);

    Uint32 (*getWidth)(_CSELF);
    Uint32 (*getHeight)(_CSELF);
    Uint32 (*getPitch)(_CSELF);
    Uint32 (*getFlags)(_CSELF);
    Uint8 (*getBitsPerPixel)(_CSELF);
    Uint8 (*getBytesPerPixel)(_CSELF);
    Uint32 (*calculatePitch)(_SELF);
    int (*mapSurface)(_SELF, Surface *dst);

    VIRTUAL_METHOD_DECLARE_END

    METHOD_DECLARE_PLACEHOLDER(Surface)
    
    Uint32 flags;				/* Read-only */
	PixelFormat* format;		/* Read-only */
	Uint32 w, h;				/* Read-only */
	Uint32 pitch;				/* Read-only */
	void *pixels;				/* Read-write */
	int offset;				/* Private */

	/* Hardware-specific surface info */
	struct private_hwdata *hwdata;

	/* clipping information */
	MIL_Rect clip_rect;			/* Read-only */

	/* Allow recursive locks */
	Uint32 locked;				/* Private */

	/* info for fast blit mapping to other surfaces */
	MIL_BlitMap *map;		/* Private */

	/* format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/* Private */

	/* Reference count -- used when freeing surface */
	int refcount;				/* Read-mostly */
};

Surface * CreateRGBSurface (Uint32 flags,
			int width, int height, int depth,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
#endif   /* ----- #ifndef SURFACE_INC  ----- */

