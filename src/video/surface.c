/*! ============================================================================
 * @file surface.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-11
 *  Organization: http://www.ds0101.net
 */

#include "video_device.h"
#include "surface.h"

CONSTRUCTOR(Surface)
{
    self->locked = 0;
    return self;
}

DESTRUCTOR(Surface)
{
    Surface* surface = (Surface*)self;
    // TODO: merge here.
	/* Free anything that's not NULL, and not the screen surface */
//	if ((surface == NULL) ||
//	    (ACT_VIDEO_DEVICE &&
//	    ((surface == MIL_ShadowSurface)||(surface == MIL_VideoSurface)))) {
//		return;
//	}
	if ( --surface->refcount > 0 ) {
		return;
	}
	while ( surface->locked > 0 ) {
		MIL_UnlockSurface(surface);
	}
	if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
//	        MIL_UnRLESurface(surface, 0);
	}
	if ( surface->format ) {
//		MIL_FreeFormat(surface->format);
		surface->format = NULL;
	}
	if ( surface->map != NULL ) {
//		MIL_FreeBlitMap(surface->map);
		surface->map = NULL;
	}
	if ( surface->hwdata ) {
//		MIL_VideoDevice *video = ACT_VIDEO_DEVICE;
//		MIL_VideoDevice *this  = ACT_VIDEO_DEVICE;
//		video->FreeHWSurface(this, surface);
	}
	if ( surface->pixels &&
	     ((surface->flags & MIL_PREALLOC) != MIL_PREALLOC) ) {
//		MIL_free(surface->pixels);
	}
#ifdef CHECK_LEAKS
	--surfaces_allocated;
#endif
}

void* Surface_X_lock(_SELF)
{
    Surface* surface = (Surface*)self;
	if ( ! surface->locked ) {
		/* Perform the lock */
		if ( surface->flags & (MIL_HWSURFACE|MIL_ASYNCBLIT) ) {
            // TODO: Merge here.
//			MIL_VideoDevice *video = ACT_VIDEO_DEVICE;
//			MIL_VideoDevice *this  = ACT_VIDEO_DEVICE;
//			if ( video->LockHWSurface(this, surface) < 0 ) {
//				return(NULL);
//			}
		}
		if ( surface->flags & MIL_RLEACCEL ) {
            // TODO: and here
//			MIL_UnRLESurface(surface, 1);
			surface->flags |= MIL_RLEACCEL;	/* save accel'd state */
		}
		/* This needs to be done here in case pixels changes value */
		surface->pixels = (Uint8 *)surface->pixels + surface->offset;
	}

	/* Increment the surface lock count, for recursive locks */
	++surface->locked;

	/* Ready to go.. */
	return(surface->pixels);
}

void Surface_X_unlock(_SELF)
{
    Surface* surface = (Surface*)self;
	/* Only perform an unlock if we are locked */
	if ( ! surface->locked || (--surface->locked > 0) ) {
		return;
	}

	/* Perform the unlock */
	surface->pixels = (Uint8 *)surface->pixels - surface->offset;

	/* Unlock hardware or accelerated surfaces */
	if ( surface->flags & (MIL_HWSURFACE|MIL_ASYNCBLIT) ) {
        // TODO: merge here.
//		MIL_VideoDevice *video = ACT_VIDEO_DEVICE;
//		MIL_VideoDevice *this  = ACT_VIDEO_DEVICE;
//		video->UnlockHWSurface(this, surface);
	} else {
		/* Update RLE encoded surface with new data */
		if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
		        surface->flags &= ~MIL_RLEACCEL; /* stop lying */
//			MIL_RLESurface(surface);
		}
	}
}

int  Surface_X_setColorKey(_SELF, Uint32 flag, Uint32 key)
{}

int  Surface_X_setAlpha(_SELF, Uint32 flag, Uint8 alpha)
{}

void Surface_X_setClipRect(_SELF, const MIL_Rect *rect)
{}

void Surface_X_getClipRect(_SELF, MIL_Rect *rect)
{	
    Surface* surface = (Surface*)self;
    if ( surface && rect ) {
        *rect = surface->clip_rect;
    }
}

int  Surface_X_blitSurface(_SELF, MIL_Rect *srcrect, MIL_Surface *dst, MIL_Rect *dstrect)
{
    Surface* ssrc = (Surface*)self;
    Surface* sdst = (Surface*)dst;
	/* Make sure the surfaces aren't locked */
	if ( ! ssrc || ! sdst ) {
		MIL_SetError("MIL_UpperBlit: passed a NULL surface");
		return(-1);
	}
	if ( ssrc->locked || sdst->locked ) {
		MIL_SetError("Surfaces must not be locked during blit");
		return(-1);
	}
    // TODO: compelete here.
    return 0;
}

int  Surface_X_fillRect(_SELF, MIL_Rect *dstrect, Uint32 color)
{}

int  Surface_X_saveBMP(_SELF, const char *file)
{}

MIL_Surface* Surface_X_displayFormat(_SELF)
{}

MIL_Surface* Surface_X_displayFormatAlpha(_SELF)
{}

MIL_Surface* Surface_X_convert(_SELF, MIL_PixelFormat *fmt, Uint32 flags)
{
    return NULL;
}

Uint32 Surface_X_getWidth(_CSELF)
{
    return ((const Surface*)self)->w;
}

Uint32 Surface_X_getHeight(_CSELF)
{
    return ((const Surface*)self)->h;
}

Uint32 Surface_X_getPitch(_CSELF)
{
    return ((const Surface*)self)->pitch;
}

Uint32 Surface_X_getFlags(_CSELF)
{
    return ((const Surface*)self)->flags;
}

Uint8 Surface_X_getBitsPerPixel(_CSELF)
{
    return ((const Surface*)self)->format->BitsPerPixel;
}

Uint8 Surface_X_getBytesPerPixel(_CSELF)
{
    return ((const Surface*)self)->format->BytesPerPixel;
}

VIRTUAL_METHOD_REGBEGIN(Surface, MIL_Surface)
    DESTRUCTOR_REGISTER(Surface)
METHOD_REGISTER(Surface, lock)
METHOD_REGISTER(Surface, unlock)
METHOD_REGISTER(Surface, setColorKey)
METHOD_REGISTER(Surface, setAlpha)
METHOD_REGISTER(Surface, setClipRect)
METHOD_REGISTER(Surface, getClipRect)
METHOD_REGISTER(Surface, blitSurface)
METHOD_REGISTER(Surface, fillRect)
METHOD_REGISTER(Surface, saveBMP)
METHOD_REGISTER(Surface, displayFormat)
METHOD_REGISTER(Surface, displayFormatAlpha)
METHOD_REGISTER(Surface, convert)

METHOD_REGISTER(Surface, getWidth)
METHOD_REGISTER(Surface, getHeight)
METHOD_REGISTER(Surface, getPitch)
METHOD_REGISTER(Surface, getFlags)
METHOD_REGISTER(Surface, getBitsPerPixel)
METHOD_REGISTER(Surface, getBytesPerPixel)

VIRTUAL_METHOD_REGEND 

METHOD_REGBEGIN(Surface)
    CONSTRUCTOR_REGISTER(Surface)
METHOD_REGEND

/* Public routines */
/*
 * Create an empty RGB surface of the appropriate depth
 */
Surface * CreateRGBSurface (Uint32 flags,
			int width, int height, int depth,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	VideoDevice *video = ACT_VIDEO_DEVICE;
	Surface *screen;
	Surface *surface;

	/* Make sure the size requested doesn't overflow our datatypes */
	/* Next time I write a library like MIL, I'll use int for size. :) */
	if ( width < 0 || height < 0 ) {
		MIL_SetError("Width or height is invalid.");
		return(NULL);
	}

	/* Check to see if we desire the surface in video memory */
	if ( video ) {
		screen = (Surface*)video->visible;
	} else {
		screen = NULL;
	}
	if ( screen && ((screen->flags & MIL_HWSURFACE) == MIL_HWSURFACE) ) {
		if ( (flags&(MIL_SRCCOLORKEY | MIL_SRCALPHA)) != 0 ) {
			flags |= MIL_HWSURFACE;
		}
		if ( (flags & MIL_SRCCOLORKEY) == MIL_SRCCOLORKEY ) {
			if ( ! ACT_VIDEO_DEVICE->vinfo.blit_hw_CC ) {
				flags &= ~MIL_HWSURFACE;
			}
		}
		if ( (flags & MIL_SRCALPHA) == MIL_SRCALPHA ) {
			if ( ! ACT_VIDEO_DEVICE->vinfo.blit_hw_A ) {
				flags &= ~MIL_HWSURFACE;
			}
		}
	} else {
		flags &= ~MIL_HWSURFACE;
	}

	/* Allocate the surface */
	surface = (Surface *)New(Surface);
	if ( surface == NULL ) {
		MIL_OutOfMemory();
		return(NULL);
	}
	surface->flags = MIL_SWSURFACE;
	if ( (flags & MIL_HWSURFACE) == MIL_HWSURFACE ) {
		if ((Amask) && (video->display_format_alpha_pixel))
		{
			depth = video->display_format_alpha_pixel->BitsPerPixel;
			Rmask = video->display_format_alpha_pixel->Rmask;
			Gmask = video->display_format_alpha_pixel->Gmask;
			Bmask = video->display_format_alpha_pixel->Bmask;
			Amask = video->display_format_alpha_pixel->Amask;
		}
		else
		{
			depth = screen->format->BitsPerPixel;
			Rmask = screen->format->Rmask;
			Gmask = screen->format->Gmask;
			Bmask = screen->format->Bmask;
			Amask = screen->format->Amask;
		}
	}
	surface->format = MIL_AllocFormat(depth, Rmask, Gmask, Bmask, Amask);
	if ( surface->format == NULL ) {
		MIL_free(surface);
		return(NULL);
	}
	if ( Amask ) {
		surface->flags |= MIL_SRCALPHA;
	}
	surface->w = width;
	surface->h = height;
	surface->pitch = MIL_CalculatePitch(surface);
	surface->pixels = NULL;
	surface->offset = 0;
	surface->hwdata = NULL;
	surface->locked = 0;
	surface->map = NULL;
	MIL_SetClipRect(surface, NULL);
	MIL_FormatChanged(surface);

	/* Get the pixels */
	if ( ((flags&MIL_HWSURFACE) == MIL_SWSURFACE) || 
				(_vc1(video, allocHWSurface, (MIL_Surface*)surface) < 0) ) {
		if ( surface->w && surface->h ) {
			surface->pixels = MIL_malloc(surface->h*surface->pitch);
			if ( surface->pixels == NULL ) {
				Delete(surface);
				MIL_OutOfMemory();
				return(NULL);
			}
			/* This is important for bitmaps */
			MIL_memset(surface->pixels, 0, surface->h*surface->pitch);
		}
	}

	/* Allocate an empty mapping */
	surface->map = MIL_AllocBlitMap();
	if ( surface->map == NULL ) {
		Delete(surface);
		return(NULL);
	}

	/* The surface is ready to go */
	surface->refcount = 1;
#ifdef CHECK_LEAKS
	++surfaces_allocated;
#endif
	return(surface);
}
