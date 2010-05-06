/*
    MIL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

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

    Sam Lantinga
    slouken@libsdl.org
*/
#include "MIL_config.h"

#include "video_device.h"
#include "blit.h"
#include "surface.h"
//#include "MIL_sysvideo.h"
#include "RLEaccel.h"
#include "pixels.h"

#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)) && MIL_ASSEMBLY_ROUTINES
#define MMX_ASMBLIT
#if (__GNUC__ > 2)  /* SSE instructions aren't in GCC 2. */
#define SSE_ASMBLIT
#endif
#endif

#if defined(MMX_ASMBLIT)
#include "MIL_cpuinfo.h"
#include "mmx.h"
#endif

/* The general purpose software blit routine */
static int MIL_SoftBlit(Surface *src, MIL_Rect *srcrect,
			Surface *dst, MIL_Rect *dstrect)
{
	int okay;
	int src_locked;
	int dst_locked;

	/* Everything is okay at the beginning...  */
	okay = 1;

	/* Lock the destination if it's in hardware */
	dst_locked = 0;
	if ( MIL_MUSTLOCK(dst) ) {
		if ( _vc0(dst, lock) < 0 ) {
			okay = 0;
		} else {
			dst_locked = 1;
		}
	}
	/* Lock the source if it's in hardware */
	src_locked = 0;
	if ( MIL_MUSTLOCK(src) ) {
		if ( _vc0(src, lock) < 0 ) {
			okay = 0;
		} else {
			src_locked = 1;
		}
	}

	/* Set up source and destination buffer pointers, and BLIT! */
	if ( okay  && srcrect->w && srcrect->h ) {
		MIL_BlitInfo info;
		MIL_loblit RunBlit;

		/* Set up the blit information */
		info.s_pixels = (Uint8 *)src->pixels +
				(Uint16)srcrect->y*src->pitch +
				(Uint16)srcrect->x*src->format->BytesPerPixel;
		info.s_width = srcrect->w;
		info.s_height = srcrect->h;
		info.s_skip=src->pitch-info.s_width*src->format->BytesPerPixel;
		info.d_pixels = (Uint8 *)dst->pixels +
				(Uint16)dstrect->y*dst->pitch +
				(Uint16)dstrect->x*dst->format->BytesPerPixel;
		info.d_width = dstrect->w;
		info.d_height = dstrect->h;
		info.d_skip=dst->pitch-info.d_width*dst->format->BytesPerPixel;
		info.aux_data = src->map->sw_data->aux_data;
		info.src = src->format;
		info.table = src->map->table;
		info.dst = dst->format;
		RunBlit = src->map->sw_data->blit;

		/* Run the actual software blit */
		RunBlit(&info);
	}

	/* We need to unlock the surfaces if they're locked */
	if ( dst_locked ) {
		_vc0(dst, unlock);
	}
	if ( src_locked ) {
		_vc0(src, unlock);
	}
	/* Blit is done! */
	return(okay ? 0 : -1);
}

#ifdef MMX_ASMBLIT
static __INLINE__ void MIL_memcpyMMX(Uint8 *to, const Uint8 *from, int len)
{
	int i;

	for(i=0; i<len/8; i++) {
		__asm__ __volatile__ (
		"	movq (%0), %%mm0\n"
		"	movq %%mm0, (%1)\n"
		: : "r" (from), "r" (to) : "memory");
		from+=8;
		to+=8;
	}
	if (len&7)
		MIL_memcpy(to, from, len&7);
}

#ifdef SSE_ASMBLIT
static __INLINE__ void MIL_memcpySSE(Uint8 *to, const Uint8 *from, int len)
{
	int i;

	__asm__ __volatile__ (
	"	prefetchnta (%0)\n"
	"	prefetchnta 64(%0)\n"
	"	prefetchnta 128(%0)\n"
	"	prefetchnta 192(%0)\n"
	: : "r" (from) );

	for(i=0; i<len/8; i++) {
		__asm__ __volatile__ (
		"	prefetchnta 256(%0)\n"
		"	movq (%0), %%mm0\n"
		"	movntq %%mm0, (%1)\n"
		: : "r" (from), "r" (to) : "memory");
		from+=8;
		to+=8;
	}
	if (len&7)
		MIL_memcpy(to, from, len&7);
}
#endif
#endif

static void MIL_BlitCopy(MIL_BlitInfo *info)
{
	Uint8 *src, *dst;
	int w, h;
	int srcskip, dstskip;

	w = info->d_width*info->dst->BytesPerPixel;
	h = info->d_height;
	src = info->s_pixels;
	dst = info->d_pixels;
	srcskip = w+info->s_skip;
	dstskip = w+info->d_skip;

#ifdef SSE_ASMBLIT
	if(MIL_HasSSE())
	{
		while ( h-- ) {
			MIL_memcpySSE(dst, src, w);
			src += srcskip;
			dst += dstskip;
		}
		__asm__ __volatile__ (
		"	emms\n"
		::);
	}
	else
#endif
#ifdef MMX_ASMBLIT
	if(MIL_HasMMX())
	{
		while ( h-- ) {
			MIL_memcpyMMX(dst, src, w);
			src += srcskip;
			dst += dstskip;
		}
		__asm__ __volatile__ (
		"	emms\n"
		::);
	}
	else
#endif
	while ( h-- ) {
		MIL_memcpy(dst, src, w);
		src += srcskip;
		dst += dstskip;
	}
}

static void MIL_BlitCopyOverlap(MIL_BlitInfo *info)
{
	Uint8 *src, *dst;
	int w, h;
	int srcskip, dstskip;

	w = info->d_width*info->dst->BytesPerPixel;
	h = info->d_height;
	src = info->s_pixels;
	dst = info->d_pixels;
	srcskip = w+info->s_skip;
	dstskip = w+info->d_skip;
	if ( dst < src ) {
		while ( h-- ) {
			MIL_memcpy(dst, src, w);
			src += srcskip;
			dst += dstskip;
		}
	} else {
		src += ((h-1) * srcskip);
		dst += ((h-1) * dstskip);
		while ( h-- ) {
			MIL_revcpy(dst, src, w);
			src -= srcskip;
			dst -= dstskip;
		}
	}
}

/* Figure out which of many blit routines to set up on a surface */
int MIL_CalculateBlit(Surface *surface)
{
	int blit_index;

	/* Clean everything out to start */
	if ( (surface->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
		_vc1(surface, UnRLE, 1);
	}
	surface->map->sw_blit = NULL;

	/* Figure out if an accelerated hardware blit is possible */
	surface->flags &= ~MIL_HWACCEL;
	if ( surface->map->identity ) {
		int hw_blit_ok;

		if ( (surface->flags & MIL_HWSURFACE) == MIL_HWSURFACE ) {
			/* We only support accelerated blitting to hardware */
			if ( surface->map->dst->flags & MIL_HWSURFACE ) {
				hw_blit_ok = ACT_VIDEO_DEVICE->vinfo.blit_hw;
			} else {
				hw_blit_ok = 0;
			}
			if (hw_blit_ok && (surface->flags & MIL_SRCCOLORKEY)) {
				hw_blit_ok = ACT_VIDEO_DEVICE->vinfo.blit_hw_CC;
			}
			if ( hw_blit_ok && (surface->flags & MIL_SRCALPHA) ) {
				hw_blit_ok = ACT_VIDEO_DEVICE->vinfo.blit_hw_A;
			}
		} else {
			/* We only support accelerated blitting to hardware */
			if ( surface->map->dst->flags & MIL_HWSURFACE ) {
				hw_blit_ok = ACT_VIDEO_DEVICE->vinfo.blit_sw;
			} else {
				hw_blit_ok = 0;
			}
			if (hw_blit_ok && (surface->flags & MIL_SRCCOLORKEY)) {
				hw_blit_ok = ACT_VIDEO_DEVICE->vinfo.blit_sw_CC;
			}
			if ( hw_blit_ok && (surface->flags & MIL_SRCALPHA) ) {
				hw_blit_ok = ACT_VIDEO_DEVICE->vinfo.blit_sw_A;
			}
		}
		if ( hw_blit_ok ) {
			VideoDevice *video = ACT_VIDEO_DEVICE;
			VideoDevice *this  = ACT_VIDEO_DEVICE;
			_vc2(video, checkHWBlit, (Surface*)surface, (Surface*)surface->map->dst);
		}
	}
	
	/* if an alpha pixel format is specified, we can accelerate alpha blits */
	if (((surface->flags & MIL_HWSURFACE) == MIL_HWSURFACE )&&(ACT_VIDEO_DEVICE->display_format_alpha_pixel)) 
	{
		if ( (surface->flags & MIL_SRCALPHA) ) 
			if ( ACT_VIDEO_DEVICE->vinfo.blit_hw_A ) {
				VideoDevice *video = ACT_VIDEO_DEVICE;
				VideoDevice *this  = ACT_VIDEO_DEVICE;
                _vc2(video, checkHWBlit, (Surface*)surface, (Surface*)surface->map->dst);
			}
	}

	/* Get the blit function index, based on surface mode */
	/* { 0 = nothing, 1 = colorkey, 2 = alpha, 3 = colorkey+alpha } */
	blit_index = 0;
	blit_index |= (!!(surface->flags & MIL_SRCCOLORKEY))      << 0;
	if ( surface->flags & MIL_SRCALPHA
	     && (surface->format->alpha != MIL_ALPHA_OPAQUE
		 || surface->format->Amask) ) {
	        blit_index |= 2;
	}

	/* Check for special "identity" case -- copy blit */
	if ( surface->map->identity && blit_index == 0 ) {
	        surface->map->sw_data->blit = MIL_BlitCopy;

		/* Handle overlapping blits on the same surface */
		if ( surface == surface->map->dst ) {
		        surface->map->sw_data->blit = MIL_BlitCopyOverlap;
		}
	} else {
		if ( surface->format->BitsPerPixel < 8 ) {
			surface->map->sw_data->blit =
			    MIL_CalculateBlit0(surface, blit_index);
		} else {
			switch ( surface->format->BytesPerPixel ) {
			    case 1:
				surface->map->sw_data->blit =
				    MIL_CalculateBlit1(surface, blit_index);
				break;
			    case 2:
			    case 3:
			    case 4:
				surface->map->sw_data->blit =
				    MIL_CalculateBlitN(surface, blit_index);
				break;
			    default:
				surface->map->sw_data->blit = NULL;
				break;
			}
		}
	}
	/* Make sure we have a blit function */
	if ( surface->map->sw_data->blit == NULL ) {
		MIL_InvalidateMap(surface->map);
		MIL_SetError("Blit combination not supported");
		return(-1);
	}

	/* Choose software blitting function */
	if(surface->flags & MIL_RLEACCELOK
	   && (surface->flags & MIL_HWACCEL) != MIL_HWACCEL) {

	        if(surface->map->identity
		   && (blit_index == 1
		       || (blit_index == 3 && !surface->format->Amask))) {
		        if ( _vc0(surface, RLE) == 0 )
			        surface->map->sw_blit = MIL_RLEBlit;
		} else if(blit_index == 2 && surface->format->Amask) {
		        if ( _vc0(surface, RLE) == 0 )
			        surface->map->sw_blit = MIL_RLEAlphaBlit;
		}
	}
	
	if ( surface->map->sw_blit == NULL ) {
		surface->map->sw_blit = MIL_SoftBlit;
	}
	return(0);
}

/* 
 * Set up a blit between two surfaces -- split into three parts:
 * The upper part, MIL_UpperBlit(), performs clipping and rectangle 
 * verification.  The lower part is a pointer to a low level
 * accelerated blitting function.
 *
 * These parts are separated out and each used internally by this 
 * library in the optimimum places.  They are exported so that if
 * you know exactly what you are doing, you can optimize your code
 * by calling the one(s) you need.
 */
int MIL_LowerBlit (Surface *src, MIL_Rect *srcrect,
				Surface *dst, MIL_Rect *dstrect)
{
	MIL_blit do_blit;
	MIL_Rect hw_srcrect;
	MIL_Rect hw_dstrect;

	/* Check to make sure the blit mapping is valid */
	if ( (src->map->dst != dst) ||
             (src->map->dst->format_version != src->map->format_version) ) {
		if ( _vc1(src, mapSurface, dst) < 0 ) {
			return(-1);
		}
	}

	/* Figure out which blitter to use */
	if ( (src->flags & MIL_HWACCEL) == MIL_HWACCEL ) {
		if ( src == ACT_VIDEO_DEVICE->screen ) {
			hw_srcrect = *srcrect;
			hw_srcrect.x += ACT_VIDEO_DEVICE->offset_x;
			hw_srcrect.y += ACT_VIDEO_DEVICE->offset_y;
			srcrect = &hw_srcrect;
		}
		if ( dst == ACT_VIDEO_DEVICE->screen ) {
			hw_dstrect = *dstrect;
			hw_dstrect.x += ACT_VIDEO_DEVICE->offset_x;
			hw_dstrect.y += ACT_VIDEO_DEVICE->offset_y;
			dstrect = &hw_dstrect;
		}
		do_blit = src->map->hw_blit;
	} else {
		do_blit = src->map->sw_blit;
	}
	return(do_blit(src, srcrect, dst, dstrect));
}
