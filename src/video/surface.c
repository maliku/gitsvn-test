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
#include "pixel_format.h"
#include "pixels.h"
#include "RLEaccel.h"
#include "MIL_gdi.h"

/*
 * Get the current information about the video hardware
 */
const MIL_VideoInfo *MIL_GetVideoInfo(void)
{
	const MIL_VideoInfo *info;

	info = NULL;
	if ( ACT_VIDEO_DEVICE ) {
		info = &ACT_VIDEO_DEVICE->vinfo;
	}
	return(info);
}

CONSTRUCTOR(Surface)
{
    _m(flags) = 0;
    _m(format) = NULL;
    _m(w) = _m(h) = _m(pitch) = 0;
    _m(pixels) = NULL;
    _m(offset) = 0;
    _m(hwdata) = NULL;
    _m(locked) = 0;
    _m(map) = 0;
    _m(format_version) = 0;
    _m(refcount) = 0;
    memset(&_m(clip_rect), 0, sizeof(_m(clip_rect)));
    VIRTUAL_METHOD_VERIFY_ONCE_BEGIN
        VirtualMethodVerify(self, lock);
        VirtualMethodVerify(self, unlock);
        VirtualMethodVerify(self, setColorKey);
        VirtualMethodVerify(self, setAlpha);
        VirtualMethodVerify(self, setClipRect);
        VirtualMethodVerify(self, getClipRect);
        VirtualMethodVerify(self, blit);
        VirtualMethodVerify(self, fillRect);
        VirtualMethodVerify(self, saveBMP);
        VirtualMethodVerify(self, stretchBlit);
        VirtualMethodVerify(self, displayFormat);
        VirtualMethodVerify(self, displayFormatAlpha);
        VirtualMethodVerify(self, convert);
        VirtualMethodVerify(self, reallocFormat);
        VirtualMethodVerify(self, formatChanged);
        VirtualMethodVerify(self, getWidth);
        VirtualMethodVerify(self, getHeight);
        VirtualMethodVerify(self, getPitch);
        VirtualMethodVerify(self, getFlags);
        VirtualMethodVerify(self, getBitsPerPixel);
        VirtualMethodVerify(self, calculatePitch);
        VirtualMethodVerify(self, mapSurface);
        VirtualMethodVerify(self, RLE);
        VirtualMethodVerify(self, UnRLE);
    VIRTUAL_METHOD_VERIFY_ONCE_END

    return self;
}

DESTRUCTOR(Surface)
{
    Surface* surface = (Surface*)self;
    // TODO: merge here.
	/* Free anything that's not NULL, and not the screen surface */
//	if ((surface == NULL) ||
//	    (ACT_VIDEO_DEVICE &&
//	    ((surface == MIL_ShadowSurface)||(surface == ACT_VIDEO_DEVICE->screen)))) {
//		return;
//	}
	if ( --surface->refcount > 0 ) {
		return;
	}
	while ( surface->locked > 0 ) {
		_vc0(surface, unlock);
	}
	if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
	        _vc1(surface, UnRLE, 0);
	}
	if ( surface->format ) {
		Delete(surface->format);
		surface->format = NULL;
	}
	if ( surface->map != NULL ) {
		MIL_FreeBlitMap(surface->map);
		surface->map = NULL;
	}
	if ( surface->hwdata ) {
		VideoDevice *video = ACT_VIDEO_DEVICE;
		VideoDevice *this  = ACT_VIDEO_DEVICE;
		_vc1(video, freeHWSurface, surface);
	}
	if ( surface->pixels &&
	     ((surface->flags & MIL_PREALLOC) != MIL_PREALLOC) ) {
		MIL_free(surface->pixels);
	}
#ifdef CHECK_LEAKS
	--surfaces_allocated;
#endif
}

int Surface_X_lock(_SELF)
{
    Surface* surface = (Surface*)self;
    /* TODO: May be the thread lock should insert here. */
	if ( ! surface->locked ) {
		/* Perform the lock */
		if ( surface->flags & (MIL_HWSURFACE|MIL_ASYNCBLIT) ) {
			VideoDevice *video = ACT_VIDEO_DEVICE;
			VideoDevice *this  = ACT_VIDEO_DEVICE;
			if ( _vc1(video, lockHWSurface, surface) < 0 ) {
				return(-1);
			}
		}
        else {
            /* TODO: Add thread safe lock. */
        }
		if ( surface->flags & MIL_RLEACCEL ) {
			_vc1(surface, UnRLE, 1);
			surface->flags |= MIL_RLEACCEL;	/* save accel'd state */
		}
		/* This needs to be done here in case pixels changes value */
		surface->pixels = (Uint8 *)surface->pixels + surface->offset;
	}

	/* Increment the surface lock count, for recursive locks */
	++surface->locked;

	/* Ready to go.. */
	return (0);
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
		VideoDevice *video = ACT_VIDEO_DEVICE;
		VideoDevice *this  = ACT_VIDEO_DEVICE;
		_vc1(video, unlockHWSurface, surface);
	} else {
		/* Update RLE encoded surface with new data */
		if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
		        surface->flags &= ~MIL_RLEACCEL; /* stop lying */
                _vc0(surface, RLE);
		}
	}
}

int  Surface_X_setColorKey(_SELF, Uint32 flag, Uint32 key)
{
    Surface* surface = (Surface*)self;
	/* Sanity check the flag as it gets passed in */
	if ( flag & MIL_SRCCOLORKEY ) {
		if ( flag & (MIL_RLEACCEL|MIL_RLEACCELOK) ) {
			flag = (MIL_SRCCOLORKEY | MIL_RLEACCELOK);
		} else {
			flag = MIL_SRCCOLORKEY;
		}
	} else {
		flag = 0;
	}

	/* Optimize away operations that don't change anything */
	if ( (flag == (surface->flags & (MIL_SRCCOLORKEY|MIL_RLEACCELOK))) &&
	     (key == surface->format->colorkey) ) {
		return(0);
	}

	/* UnRLE surfaces before we change the colorkey */
	if ( surface->flags & MIL_RLEACCEL ) {
	        _vc1(surface, UnRLE, 1);
	}

	if ( flag ) {
		VideoDevice *video = ACT_VIDEO_DEVICE;
		VideoDevice *this  = ACT_VIDEO_DEVICE;


		surface->flags |= MIL_SRCCOLORKEY;
		surface->format->colorkey = key;
		if ( (surface->flags & MIL_HWACCEL) == MIL_HWACCEL ) {
			if ((_vc2(video, setHWColorKey, surface, key) < 0) ) {
				surface->flags &= ~MIL_HWACCEL;
			}
		}
		if ( flag & MIL_RLEACCELOK ) {
			surface->flags |= MIL_RLEACCELOK;
		} else {
			surface->flags &= ~MIL_RLEACCELOK;
		}
	} else {
		surface->flags &= ~(MIL_SRCCOLORKEY|MIL_RLEACCELOK);
		surface->format->colorkey = 0;
	}
	MIL_InvalidateMap(surface->map);
	return(0);

}

int  Surface_X_setAlpha(_SELF, Uint32 flag, Uint8 value)
{
    Surface* surface = (Surface*)self;
    Uint32 oldflags = surface->flags;
	Uint32 oldalpha = surface->format->alpha;

	/* Sanity check the flag as it gets passed in */
	if ( flag & MIL_SRCALPHA ) {
		if ( flag & (MIL_RLEACCEL|MIL_RLEACCELOK) ) {
			flag = (MIL_SRCALPHA | MIL_RLEACCELOK);
		} else {
			flag = MIL_SRCALPHA;
		}
	} else {
		flag = 0;
	}

	/* Optimize away operations that don't change anything */
	if ( (flag == (surface->flags & (MIL_SRCALPHA|MIL_RLEACCELOK))) &&
	     (!flag || value == oldalpha) ) {
		return(0);
	}

	if(!(flag & MIL_RLEACCELOK) && (surface->flags & MIL_RLEACCEL))
		_vc1(surface, UnRLE, 1);

	if ( flag ) {
		VideoDevice* video = ACT_VIDEO_DEVICE;
		VideoDevice* this  = ACT_VIDEO_DEVICE;

		surface->flags |= MIL_SRCALPHA;
		surface->format->alpha = value;
		if ( (surface->flags & MIL_HWACCEL) == MIL_HWACCEL ) {
			if ( (_vc2(video, setHWAlpha, surface, value) < 0) ) {
				surface->flags &= ~MIL_HWACCEL;
			}
		}
		if ( flag & MIL_RLEACCELOK ) {
		        surface->flags |= MIL_RLEACCELOK;
		} else {
		        surface->flags &= ~MIL_RLEACCELOK;
		}
	} else {
		surface->flags &= ~MIL_SRCALPHA;
		surface->format->alpha = MIL_ALPHA_OPAQUE;
	}
	/*
	 * The representation for software surfaces is independent of
	 * per-surface alpha, so no need to invalidate the blit mapping
	 * if just the alpha value was changed. (If either is 255, we still
	 * need to invalidate.)
	 */
	if((surface->flags & MIL_HWACCEL) == MIL_HWACCEL
	   || oldflags != surface->flags
	   || (((oldalpha + 1) ^ (value + 1)) & 0x100))
		MIL_InvalidateMap(surface->map);
	return(0);
}

MIL_Bool Surface_X_setClipRect(_SELF, const MIL_Rect *rect)
{
	MIL_Rect full_rect;
    Surface* surface = (Surface*)self;

	/* Don't do anything if there's no surface to act on */
	if ( ! surface ) {
		return MIL_FALSE;
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
	return MIL_IntersectRect(rect, &full_rect, &surface->clip_rect);
}

void Surface_X_getClipRect(_SELF, MIL_Rect *rect)
{	
    Surface* surface = (Surface*)self;
    if ( surface && rect ) {
        *rect = surface->clip_rect;
    }
}

int  Surface_X_blit(_SELF, const MIL_Rect *srcrect, Surface *dst, const MIL_Point* dstpoint)
{
    MIL_Rect dstrect = {0, 0, 0, 0};
	int srcx, srcy, w, h;
    Surface* src = (Surface*)self;
	/* Make sure the surfaces aren't locked */
	if ( ! src || ! dst ) {
		MIL_SetError("MIL_UpperBlit: passed a NULL surface");
		return(-1);
	}
	if ( src->locked || dst->locked ) {
		MIL_SetError("Surfaces must not be locked during blit");
		return(-1);
	}

	/* If the destination rectangle is NULL, use the entire dest surface */
	if ( NULL != dstpoint ) {
        dstrect.x = dstpoint->x;
        dstrect.y = dstpoint->y;
    }

	/* clip the source rectangle to the source surface */
	if(srcrect) {
        int maxw, maxh;

		srcx = srcrect->x;
		w = srcrect->w;
		if(srcx < 0) {
            w += srcx;
            dstrect.x -= srcx;
			srcx = 0;
		}
		maxw = src->w - srcx;
		if(maxw < w)
			w = maxw;

		srcy = srcrect->y;
		h = srcrect->h;
		if(srcy < 0) {
            h += srcy;
            dstrect.y -= srcy;
			srcy = 0;
		}
		maxh = src->h - srcy;
		if(maxh < h)
			h = maxh;
	    
    } else {
        srcx = srcy = 0;
		w = src->w;
		h = src->h;
	}

	/* clip the destination rectangle against the clip rectangle */
	{
        MIL_Rect *clip = &dst->clip_rect;
        int dx, dy;

		dx = clip->x - dstrect.x;
		if(dx > 0) {
			w -= dx;
			dstrect.x += dx;
			srcx += dx;
		}
		dx = dstrect.x + w - clip->x - clip->w;
		if(dx > 0)
			w -= dx;

		dy = clip->y - dstrect.y;
        printf("clip->y = %d, dstrect->y = %d.\n", clip->y, dstrect.y);
		if(dy > 0) {
			h -= dy;
			dstrect.y += dy;
			srcy += dy;
		}
		dy = dstrect.y + h - clip->y - clip->h;
		if(dy > 0)
			h -= dy;
	}

	if(w > 0 && h > 0) {
        MIL_Rect sr;
        sr.x = srcx;
		sr.y = srcy;
		sr.w = dstrect.w = w;
		sr.h = dstrect.h = h;
        printf("src.x = %d, y = %d, w = %d, h = %d.\n", sr.x, sr.y, sr.w, sr.h);
        printf("dst.x = %d, y = %d, w = %d, h = %d.\n", dstrect.x, dstrect.y, dstrect.w, dstrect.h);
		return MIL_LowerBlit(src, &sr, dst, &dstrect);
	}
//	dstrect.w = dstrect.h = 0;
	return 0;
}

static int MIL_FillRect1(Surface *dst, MIL_Rect *dstrect, Uint32 color)
{
	/* FIXME: We have to worry about packing order.. *sigh* */
	MIL_SetError("1-bpp rect fill not yet implemented");
	return -1;
}

static int MIL_FillRect4(Surface *dst, MIL_Rect *dstrect, Uint32 color)
{
	/* FIXME: We have to worry about packing order.. *sigh* */
	MIL_SetError("4-bpp rect fill not yet implemented");
	return -1;
}

int  Surface_X_fillRect(_SELF, MIL_Rect *dstrect, Uint32 color)
{
	VideoDevice *video = ACT_VIDEO_DEVICE;
	VideoDevice *this  = ACT_VIDEO_DEVICE;
    Surface* dst = (Surface*)self;
	int x, y;
	Uint8 *row;

	/* This function doesn't work on surfaces < 8 bpp */
	if ( dst->format->BitsPerPixel < 8 ) {
		switch(dst->format->BitsPerPixel) {
		    case 1:
			return MIL_FillRect1(dst, dstrect, color);
			break;
		    case 4:
			return MIL_FillRect4(dst, dstrect, color);
			break;
		    default:
			MIL_SetError("Fill rect on unsupported surface format");
			return(-1);
			break;
		}
	}

	/* If 'dstrect' == NULL, then fill the whole surface */
	if ( dstrect ) {
		/* Perform clipping */
		if ( !MIL_IntersectRect(dstrect, &dst->clip_rect, dstrect) ) {
			return(0);
		}
	} else {
		dstrect = &dst->clip_rect;
	}

	/* Check for hardware acceleration */
	if ( ((dst->flags & MIL_HWSURFACE) == MIL_HWSURFACE) &&
					video->vinfo.blit_fill ) {
		MIL_Rect hw_rect;
		if ( dst == ACT_VIDEO_DEVICE->screen ) {
			hw_rect = *dstrect;
			hw_rect.x += ACT_VIDEO_DEVICE->offset_x;
			hw_rect.y += ACT_VIDEO_DEVICE->offset_y;
			dstrect = &hw_rect;
		}
		return(_vc3(video, fillHWRect, dst, dstrect, color));
	}

	/* Perform software fill */
	if ( _vc0(dst, lock) != 0 ) {
		return(-1);
	}
	row = (Uint8 *)dst->pixels+dstrect->y*dst->pitch+
			dstrect->x*dst->format->BytesPerPixel;
	if ( dst->format->palette || (color == 0) ) {
		x = dstrect->w*dst->format->BytesPerPixel;
		if ( !color && !((uintptr_t)row&3) && !(x&3) && !(dst->pitch&3) ) {
			int n = x >> 2;
			for ( y=dstrect->h; y; --y ) {
				MIL_memset4(row, 0, n);
				row += dst->pitch;
			}
		} else {
#ifdef __powerpc__
			/*
			 * MIL_memset() on PPC (both glibc and codewarrior) uses
			 * the dcbz (Data Cache Block Zero) instruction, which
			 * causes an alignment exception if the destination is
			 * uncachable, so only use it on software surfaces
			 */
			if((dst->flags & MIL_HWSURFACE) == MIL_HWSURFACE) {
				if(dstrect->w >= 8) {
					/*
					 * 64-bit stores are probably most
					 * efficient to uncached video memory
					 */
					double fill;
					MIL_memset(&fill, color, (sizeof fill));
					for(y = dstrect->h; y; y--) {
						Uint8 *d = row;
						unsigned n = x;
						unsigned nn;
						Uint8 c = color;
						double f = fill;
						while((unsigned long)d
						      & (sizeof(double) - 1)) {
							*d++ = c;
							n--;
						}
						nn = n / (sizeof(double) * 4);
						while(nn) {
							((double *)d)[0] = f;
							((double *)d)[1] = f;
							((double *)d)[2] = f;
							((double *)d)[3] = f;
							d += 4*sizeof(double);
							nn--;
						}
						n &= ~(sizeof(double) * 4 - 1);
						nn = n / sizeof(double);
						while(nn) {
							*(double *)d = f;
							d += sizeof(double);
							nn--;
						}
						n &= ~(sizeof(double) - 1);
						while(n) {
							*d++ = c;
							n--;
						}
						row += dst->pitch;
					}
				} else {
					/* narrow boxes */
					for(y = dstrect->h; y; y--) {
						Uint8 *d = row;
						Uint8 c = color;
						int n = x;
						while(n) {
							*d++ = c;
							n--;
						}
						row += dst->pitch;
					}
				}
			} else
#endif /* __powerpc__ */
			{
				for(y = dstrect->h; y; y--) {
					MIL_memset(row, color, x);
					row += dst->pitch;
				}
			}
		}
	} else {
		switch (dst->format->BytesPerPixel) {
		    case 2:
			for ( y=dstrect->h; y; --y ) {
				Uint16 *pixels = (Uint16 *)row;
				Uint16 c = (Uint16)color;
				Uint32 cc = (Uint32)c << 16 | c;
				int n = dstrect->w;
				if((uintptr_t)pixels & 3) {
					*pixels++ = c;
					n--;
				}
				if(n >> 1)
					MIL_memset4(pixels, cc, n >> 1);
				if(n & 1)
					pixels[n - 1] = c;
				row += dst->pitch;
			}
			break;

		    case 3:
			#if MIL_BYTEORDER == MIL_BIG_ENDIAN
				color <<= 8;
			#endif
			for ( y=dstrect->h; y; --y ) {
				Uint8 *pixels = row;
				for ( x=dstrect->w; x; --x ) {
					MIL_memcpy(pixels, &color, 3);
					pixels += 3;
				}
				row += dst->pitch;
			}
			break;

		    case 4:
			for(y = dstrect->h; y; --y) {
				MIL_memset4(row, color, dstrect->w);
				row += dst->pitch;
			}
			break;
		}
	}
	_vc0(dst, unlock);

	/* We're done! */
	return(0);
}

int  Surface_X_saveBMP(_SELF, const char *file)
{}

int  Surface_X_stretchBlit(_SELF, const MIL_Rect *srcrect, Surface *dst, const MIL_Rect* dstrect)
{
    return MIL_SoftStretch((Surface*)self, srcrect, dst, dstrect);
}

Surface* Surface_X_displayFormat(_SELF)
{
	Uint32 flags;
    Surface* surface = (Surface*)self;

	if ( ! ACT_VIDEO_DEVICE->visible ) {
		MIL_SetError("No video mode has been set");
		return(NULL);
	}
	/* Set the flags appropriate for copying to display surface */
	if (((ACT_VIDEO_DEVICE->visible->flags & MIL_HWSURFACE) == MIL_HWSURFACE) 
            && ACT_VIDEO_DEVICE->vinfo.blit_hw)
		flags = MIL_HWSURFACE;
	else 
		flags = MIL_SWSURFACE;
#ifdef AUTORLE_DISPLAYFORMAT
	flags |= (surface->flags & (MIL_SRCCOLORKEY | MIL_SRCALPHA));
	flags |= MIL_RLEACCELOK;
#else
	flags |= surface->flags & (MIL_SRCCOLORKEY | MIL_SRCALPHA | MIL_RLEACCELOK);
#endif
	return(_vc2(surface, convert, ACT_VIDEO_DEVICE->visible->format, flags));

}

Surface* Surface_X_displayFormatAlpha(_SELF)
{
	PixelFormat* vf = NULL;
	PixelFormat* format = NULL;
	Surface* converted = NULL;
    Surface* surface = (Surface*)self;
	Uint32 flags;
	/* default to ARGB8888 */
	Uint32 amask = 0xff000000;
	Uint32 rmask = 0x00ff0000;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x000000ff;

	if ( ! ACT_VIDEO_DEVICE->visible ) {
		MIL_SetError("No video mode has been set");
		return(NULL);
	}
	vf = ACT_VIDEO_DEVICE->visible->format;

	switch(vf->BytesPerPixel) {
	    case 2:
		/* For XGY5[56]5, use, AXGY8888, where {X, Y} = {R, B}.
		   For anything else (like ARGB4444) it doesn't matter
		   since we have no special code for it anyway */
		if ( (vf->Rmask == 0x1f) &&
		     (vf->Bmask == 0xf800 || vf->Bmask == 0x7c00)) {
			rmask = 0xff;
			bmask = 0xff0000;
		}
		break;

	    case 3:
	    case 4:
		/* Keep the video format, as long as the high 8 bits are
		   unused or alpha */
		if ( (vf->Rmask == 0xff) && (vf->Bmask == 0xff0000) ) {
			rmask = 0xff;
			bmask = 0xff0000;
		}
		break;

	    default:
		/* We have no other optimised formats right now. When/if a new
		   optimised alpha format is written, add the converter here */
		break;
	}
	format = (PixelFormat*)MIL_AllocFormat(32, rmask, gmask, bmask, amask);
	flags = ACT_VIDEO_DEVICE->visible->flags & MIL_HWSURFACE;
	flags |= surface->flags & (MIL_SRCALPHA | MIL_RLEACCELOK);
	converted = _vc2(surface, convert, format, flags);
	Delete(format);
	return(converted);
}

Surface* Surface_X_convert(_SELF, PixelFormat *format, Uint32 flags)
{
	Surface* convert = NULL;
    Surface* surface = (Surface*)self;
	Uint32 colorkey = 0;
	Uint8 alpha = 0;
	Uint32 surface_flags;
	MIL_Rect bounds;

	/* Check for empty destination palette! (results in empty image) */
	if ( format->palette != NULL ) {
		int i;
		for ( i=0; i<format->palette->ncolors; ++i ) {
			if ( (format->palette->colors[i].r != 0) ||
			     (format->palette->colors[i].g != 0) ||
			     (format->palette->colors[i].b != 0) )
				break;
		}
		if ( i == format->palette->ncolors ) {
			MIL_SetError("Empty destination palette");
			return(NULL);
		}
	}

	/* Only create hw surfaces with alpha channel if hw alpha blits
	   are supported */
	if(format->Amask != 0 && (flags & MIL_HWSURFACE)) {
		const MIL_VideoInfo *vi = MIL_GetVideoInfo();
		if(!vi || !vi->blit_hw_A)
			flags &= ~MIL_HWSURFACE;
	}

	/* Create a new surface with the desired format */
	convert = CreateRGBSurface(flags,
				surface->w, surface->h, format->BitsPerPixel,
		format->Rmask, format->Gmask, format->Bmask, format->Amask);
	if ( convert == NULL ) {
		return(NULL);
	}

	/* Copy the palette if any */
	if ( format->palette && convert->format->palette ) {
		MIL_memcpy(convert->format->palette->colors,
				format->palette->colors,
				format->palette->ncolors*sizeof(MIL_Color));
		convert->format->palette->ncolors = format->palette->ncolors;
	}

	/* Save the original surface color key and alpha */
	surface_flags = surface->flags;
	if ( (surface_flags & MIL_SRCCOLORKEY) == MIL_SRCCOLORKEY ) {
		/* Convert colourkeyed surfaces to RGBA if requested */
		if((flags & MIL_SRCCOLORKEY) != MIL_SRCCOLORKEY
		   && format->Amask) {
			surface_flags &= ~MIL_SRCCOLORKEY;
		} else {
			colorkey = surface->format->colorkey;
			_vc2(surface, setColorKey, 0, 0);
		}
	}
	if ( (surface_flags & MIL_SRCALPHA) == MIL_SRCALPHA ) {
		/* Copy over the alpha channel to RGBA if requested */
		if ( format->Amask ) {
			surface->flags &= ~MIL_SRCALPHA;
		} else {
			alpha = surface->format->alpha;
			_vc2(surface, setAlpha, 0, 0);
		}
	}

	/* Copy over the image data */
	bounds.x = 0;
	bounds.y = 0;
	bounds.w = surface->w;
	bounds.h = surface->h;
	MIL_LowerBlit(surface, &bounds, convert, &bounds);

	/* Clean up the original surface, and update converted surface */
	if ( convert != NULL ) {
		_vc1(convert, setClipRect, &surface->clip_rect);
	}
	if ( (surface_flags & MIL_SRCCOLORKEY) == MIL_SRCCOLORKEY ) {
		Uint32 cflags = surface_flags&(MIL_SRCCOLORKEY|MIL_RLEACCELOK);
		if ( convert != NULL ) {
			Uint8 keyR, keyG, keyB;

			_vc4(surface->format, getRGB, colorkey, &keyR, &keyG, &keyB);
            _vc2(convert, setColorKey, cflags|(flags&MIL_RLEACCELOK),
                    _vc3(convert->format, mapRGB, keyR, keyG, keyB));
        }
		_vc2(surface, setColorKey, cflags, colorkey);
	}
	if ( (surface_flags & MIL_SRCALPHA) == MIL_SRCALPHA ) {
		Uint32 aflags = surface_flags&(MIL_SRCALPHA|MIL_RLEACCELOK);
		if ( convert != NULL ) {
		        _vc2(convert, setAlpha, aflags|(flags&MIL_RLEACCELOK),
				alpha);
		}
		if ( format->Amask ) {
			surface->flags |= MIL_SRCALPHA;
		} else {
			_vc2(surface, setAlpha, aflags, alpha);
		}
	}

	/* We're ready to go! */
	return(convert);
}

PixelFormat* Surface_X_reallocFormat(_SELF, int bpp,
        Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
    Surface* surface = (Surface*)self;
	if ( surface->format ) {
		Delete(surface->format);
		_vc0(surface, formatChanged);
	}
	surface->format = (PixelFormat*)MIL_AllocFormat(bpp, Rmask, Gmask, Bmask, Amask);
	return surface->format;
}

/*
 * Change any previous mappings from/to the new surface format
 */
void Surface_X_formatChanged(_SELF)
{
	static int format_version = 0;
    Surface* surface = (Surface*)self;
	++format_version;
	if ( format_version < 0 ) { /* It wrapped... */
		format_version = 1;
	}
	surface->format_version = format_version;
	MIL_InvalidateMap(surface->map);
}

/* 
 * Calculate the pad-aligned scanline width of a surface
 */
Uint32 Surface_X_calculatePitch(_SELF)
{
	Uint32 pitch;
    Surface* surface = (Surface*)self;

	/* Surface should be 4-byte aligned for speed */
	pitch = surface->w * surface->format->BytesPerPixel;
	switch (surface->format->BitsPerPixel) {
		case 1:
			pitch = (pitch + 7) / 8;
			break;
		case 4:
			pitch = (pitch + 1) / 2;
			break;
		default:
			break;
	}
	return (pitch = (pitch + 3) & ~3);	/* 4-byte aligning */
}

int Surface_X_mapSurface(_SELF, Surface *dst)
{
    Surface* src = (Surface*)self;
	PixelFormat *srcfmt;
	PixelFormat *dstfmt;
	MIL_BlitMap *map;

	/* Clear out any previous mapping */
	map = src->map;
	if ( (src->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
		_vc1(src, UnRLE, 1);
	}
	MIL_InvalidateMap(map);

	/* Figure out what kind of mapping we're doing */
	map->identity = 0;
	srcfmt = src->format;
	dstfmt = dst->format;
	switch (srcfmt->BytesPerPixel) {
	    case 1:
		switch (dstfmt->BytesPerPixel) {
		    case 1:
			/* Palette --> Palette */
			/* If both MIL_HWSURFACE, assume have same palette */
			if ( ((src->flags & MIL_HWSURFACE) == MIL_HWSURFACE) &&
			     ((dst->flags & MIL_HWSURFACE) == MIL_HWSURFACE) ) {
				map->identity = 1;
			} else {
				map->table = Map1to1(srcfmt->palette,
					dstfmt->palette, &map->identity);
			}
			if ( ! map->identity ) {
				if ( map->table == NULL ) {
					return(-1);
				}
			}
			if (srcfmt->BitsPerPixel!=dstfmt->BitsPerPixel)
				map->identity = 0;
			break;

		    default:
			/* Palette --> BitField */
			map->table = _vc1(srcfmt, map1toN, (MIL_PixelFormat*)dstfmt);
			if ( map->table == NULL ) {
				return(-1);
			}
			break;
		}
		break;
	default:
		switch (dstfmt->BytesPerPixel) {
		    case 1:
			/* BitField --> Palette */
			map->table = _vc2(srcfmt, mapNto1, (MIL_PixelFormat*)dstfmt, &map->identity);
			if ( ! map->identity ) {
				if ( map->table == NULL ) {
					return(-1);
				}
			}
			map->identity = 0;	/* Don't optimize to copy */
			break;
		    default:
			/* BitField --> BitField */
			if ( FORMAT_EQUAL(srcfmt, dstfmt) )
				map->identity = 1;
			break;
		}
		break;
	}

	map->dst = dst;
	map->format_version = dst->format_version;

	/* Choose your blitters wisely */
	return(MIL_CalculateBlit(src));
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

int Surface_X_RLE(_SELF)
{
	int retcode;
    Surface* surface = (Surface*)self;

	/* Clear any previous RLE conversion */
	if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
		_vc1(surface, UnRLE, 1);
	}

	/* We don't support RLE encoding of bitmaps */
	if ( surface->format->BitsPerPixel < 8 ) {
		return(-1);
	}

	/* Lock the surface if it's in hardware */
	if ( MIL_MUSTLOCK(surface) ) {
		if ( _vc0(surface, lock) < 0 ) {
			return(-1);
		}
	}

	/* Encode */
	if((surface->flags & MIL_SRCCOLORKEY) == MIL_SRCCOLORKEY) {
	    retcode = RLEColorkeySurface(surface);
	} else {
	    if((surface->flags & MIL_SRCALPHA) == MIL_SRCALPHA
	       && surface->format->Amask != 0)
		retcode = RLEAlphaSurface(surface);
	    else
		retcode = -1;	/* no RLE for per-surface alpha sans ckey */
	}

	/* Unlock the surface if it's in hardware */
	if ( MIL_MUSTLOCK(surface) ) {
		_vc0(surface, unlock);
	}

	if(retcode < 0)
	    return -1;

	/* The surface is now accelerated */
	surface->flags |= MIL_RLEACCEL;

	return(0);
}

void Surface_X_UnRLE(_SELF, int recode)
{
    Surface* surface = (Surface*)self;
    if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
        surface->flags &= ~MIL_RLEACCEL;

        if(recode && (surface->flags & MIL_PREALLOC) != MIL_PREALLOC
                && (surface->flags & MIL_HWSURFACE) != MIL_HWSURFACE) {
            if((surface->flags & MIL_SRCCOLORKEY) == MIL_SRCCOLORKEY) {
                MIL_Rect full;
                unsigned alpha_flag;

                /* re-create the original surface */
                surface->pixels = MIL_malloc(surface->h * surface->pitch);
                if ( !surface->pixels ) {
                    /* Oh crap... */
                    surface->flags |= MIL_RLEACCEL;
                    return;
                }

                /* fill it with the background colour */
                MIL_FillRect(surface, NULL, surface->format->colorkey);

                /* now render the encoded surface */
                full.x = full.y = 0;
                full.w = surface->w;
                full.h = surface->h;
                alpha_flag = surface->flags & MIL_SRCALPHA;
                surface->flags &= ~MIL_SRCALPHA; /* opaque blit */
                MIL_RLEBlit(surface, &full, surface, &full);
                surface->flags |= alpha_flag;
            } else {
                if ( !UnRLEAlpha(surface) ) {
                    /* Oh crap... */
                    surface->flags |= MIL_RLEACCEL;
                    return;
                }
            }
        }

        if ( surface->map && surface->map->sw_data->aux_data ) {
            MIL_free(surface->map->sw_data->aux_data);
            surface->map->sw_data->aux_data = NULL;
        }
    }
}

VIRTUAL_METHOD_MAP_BEGIN(Surface, NonBase)
    CONSTRUCTOR_MAP(Surface)
    DESTRUCTOR_MAP(Surface)
    METHOD_MAP(Surface, lock)
    METHOD_MAP(Surface, unlock)
    METHOD_MAP(Surface, setColorKey)
    METHOD_MAP(Surface, setAlpha)
    METHOD_MAP(Surface, setClipRect)
    METHOD_MAP(Surface, getClipRect)
    METHOD_MAP(Surface, blit)
    METHOD_MAP(Surface, fillRect)
    METHOD_MAP(Surface, saveBMP)
    METHOD_MAP(Surface, stretchBlit)
    METHOD_MAP(Surface, displayFormat)
    METHOD_MAP(Surface, displayFormatAlpha)
    METHOD_MAP(Surface, convert)
    METHOD_MAP(Surface, reallocFormat)
    METHOD_MAP(Surface, formatChanged)

    METHOD_MAP(Surface, getWidth)
    METHOD_MAP(Surface, getHeight)
    METHOD_MAP(Surface, getPitch)
    METHOD_MAP(Surface, getFlags)
    METHOD_MAP(Surface, getBitsPerPixel)
    METHOD_MAP(Surface, calculatePitch)
    METHOD_MAP(Surface, mapSurface)
    METHOD_MAP(Surface, RLE)
    METHOD_MAP(Surface, UnRLE)

METHOD_MAP(Surface, getBytesPerPixel)

VIRTUAL_METHOD_MAP_END 




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
	surface->format = (PixelFormat*)MIL_AllocFormat(depth, Rmask, Gmask, Bmask, Amask);
	if ( surface->format == NULL ) {
		MIL_free(surface);
		return(NULL);
	}
	if ( Amask ) {
		surface->flags |= MIL_SRCALPHA;
	}
	surface->w = width;
	surface->h = height;
	surface->pitch = _vc0(surface, calculatePitch);
	surface->pixels = NULL;
	surface->offset = 0;
	surface->hwdata = NULL;
	surface->locked = 0;
	surface->map = NULL;
	_vc1(surface, setClipRect, NULL);
	_vc0(surface, formatChanged);

	/* Get the pixels */
	if ( ((flags&MIL_HWSURFACE) == MIL_SWSURFACE) || 
				(_vc1(video, allocHWSurface, (Surface*)surface) < 0) ) {
		if ( surface->w && surface->h ) {
			surface->pixels = MIL_malloc(surface->h * surface->pitch);
			if ( surface->pixels == NULL ) {
				Delete(surface);
				MIL_OutOfMemory();
				return(NULL);
			}
			/* This is important for bitmaps */
			MIL_memset(surface->pixels, 0, surface->h * surface->pitch);
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

/* 
 * This function performs a fast fill of the given rectangle with 'color'
 */
int MIL_FillRect(Surface *dst, MIL_Rect *dstrect, Uint32 color)
{
	VideoDevice *video = ACT_VIDEO_DEVICE;
	VideoDevice *this  = ACT_VIDEO_DEVICE;
	int x, y;
	Uint8 *row;

    /* TODO: Will to support palette? */
#if 0
	/* This function doesn't work on surfaces < 8 bpp */
	if ( dst->format->BitsPerPixel < 8 ) {
		switch(dst->format->BitsPerPixel) {
		    case 1:
			return MIL_FillRect1(dst, dstrect, color);
			break;
		    case 4:
			return MIL_FillRect4(dst, dstrect, color);
			break;
		    default:
			MIL_SetError("Fill rect on unsupported surface format");
			return(-1);
			break;
		}
	}
#endif

	/* If 'dstrect' == NULL, then fill the whole surface */
	if ( dstrect ) {
		/* Perform clipping */
		if ( !MIL_IntersectRect(dstrect, &dst->clip_rect, dstrect) ) {
			return(0);
		}
	} else {
		dstrect = &dst->clip_rect;
	}

	/* Check for hardware acceleration */
	if ( ((dst->flags & MIL_HWSURFACE) == MIL_HWSURFACE) &&
					video->vinfo.blit_fill ) {
		MIL_Rect hw_rect;
		if ( dst == ACT_VIDEO_DEVICE->screen ) {
			hw_rect = *dstrect;
			hw_rect.x += ACT_VIDEO_DEVICE->offset_x;
			hw_rect.y += ACT_VIDEO_DEVICE->offset_y;
			dstrect = &hw_rect;
		}
		return(_vc3(video, fillHWRect, dst, dstrect, color));
	}

	/* Perform software fill */
	if ( _vc0(dst, lock) != 0 ) {
		return(-1);
	}
	row = (Uint8 *)dst->pixels+dstrect->y*dst->pitch+
			dstrect->x*dst->format->BytesPerPixel;
	if ( dst->format->palette || (color == 0) ) {
		x = dstrect->w*dst->format->BytesPerPixel;
		if ( !color && !((uintptr_t)row&3) && !(x&3) && !(dst->pitch&3) ) {
			int n = x >> 2;
			for ( y=dstrect->h; y; --y ) {
				MIL_memset4(row, 0, n);
				row += dst->pitch;
			}
		} else {
#ifdef __powerpc__
			/*
			 * MIL_memset() on PPC (both glibc and codewarrior) uses
			 * the dcbz (Data Cache Block Zero) instruction, which
			 * causes an alignment exception if the destination is
			 * uncachable, so only use it on software surfaces
			 */
			if((dst->flags & MIL_HWSURFACE) == MIL_HWSURFACE) {
				if(dstrect->w >= 8) {
					/*
					 * 64-bit stores are probably most
					 * efficient to uncached video memory
					 */
					double fill;
					MIL_memset(&fill, color, (sizeof fill));
					for(y = dstrect->h; y; y--) {
						Uint8 *d = row;
						unsigned n = x;
						unsigned nn;
						Uint8 c = color;
						double f = fill;
						while((unsigned long)d
						      & (sizeof(double) - 1)) {
							*d++ = c;
							n--;
						}
						nn = n / (sizeof(double) * 4);
						while(nn) {
							((double *)d)[0] = f;
							((double *)d)[1] = f;
							((double *)d)[2] = f;
							((double *)d)[3] = f;
							d += 4*sizeof(double);
							nn--;
						}
						n &= ~(sizeof(double) * 4 - 1);
						nn = n / sizeof(double);
						while(nn) {
							*(double *)d = f;
							d += sizeof(double);
							nn--;
						}
						n &= ~(sizeof(double) - 1);
						while(n) {
							*d++ = c;
							n--;
						}
						row += dst->pitch;
					}
				} else {
					/* narrow boxes */
					for(y = dstrect->h; y; y--) {
						Uint8 *d = row;
						Uint8 c = color;
						int n = x;
						while(n) {
							*d++ = c;
							n--;
						}
						row += dst->pitch;
					}
				}
			} else
#endif /* __powerpc__ */
			{
				for(y = dstrect->h; y; y--) {
					MIL_memset(row, color, x);
					row += dst->pitch;
				}
			}
		}
	} else {
		switch (dst->format->BytesPerPixel) {
		    case 2:
			for ( y=dstrect->h; y; --y ) {
				Uint16 *pixels = (Uint16 *)row;
				Uint16 c = (Uint16)color;
				Uint32 cc = (Uint32)c << 16 | c;
				int n = dstrect->w;
				if((uintptr_t)pixels & 3) {
					*pixels++ = c;
					n--;
				}
				if(n >> 1)
					MIL_memset4(pixels, cc, n >> 1);
				if(n & 1)
					pixels[n - 1] = c;
				row += dst->pitch;
			}
			break;

		    case 3:
			#if MIL_BYTEORDER == MIL_BIG_ENDIAN
				color <<= 8;
			#endif
			for ( y=dstrect->h; y; --y ) {
				Uint8 *pixels = row;
				for ( x=dstrect->w; x; --x ) {
					MIL_memcpy(pixels, &color, 3);
					pixels += 3;
				}
				row += dst->pitch;
			}
			break;

		    case 4:
			for(y = dstrect->h; y; --y) {
				MIL_memset4(row, color, dstrect->w);
				row += dst->pitch;
			}
			break;
		}
	}
	_vc0(dst, unlock);

	/* We're done! */
	return(0);
}

