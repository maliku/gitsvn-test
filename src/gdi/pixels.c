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

/* General (mostly internal) pixel/color manipulation routines for MIL */

#include "MIL_endian.h"
#include "MIL_video.h"
//#include "MIL_sysvideo.h"
#include "blit.h"
#include "pixels.h"
#include "RLEaccel.h"

/* Helper functions */
/*
 * Allocate a pixel format structure and fill it according to the given info.
 */
MIL_PixelFormat *MIL_AllocFormat(int bpp,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	MIL_PixelFormat *format;
	Uint32 mask;

	/* Allocate an empty pixel format structure */
	format = MIL_malloc(sizeof(*format));
	if ( format == NULL ) {
		MIL_OutOfMemory();
		return(NULL);
	}
	MIL_memset(format, 0, sizeof(*format));
	format->alpha = MIL_ALPHA_OPAQUE;

	/* Set up the format */
	format->BitsPerPixel = bpp;
	format->BytesPerPixel = (bpp+7)/8;
	if ( Rmask || Bmask || Gmask ) { /* Packed pixels with custom mask */
		format->palette = NULL;
		format->Rshift = 0;
		format->Rloss = 8;
		if ( Rmask ) {
			for ( mask = Rmask; !(mask&0x01); mask >>= 1 )
				++format->Rshift;
			for ( ; (mask&0x01); mask >>= 1 )
				--format->Rloss;
		}
		format->Gshift = 0;
		format->Gloss = 8;
		if ( Gmask ) {
			for ( mask = Gmask; !(mask&0x01); mask >>= 1 )
				++format->Gshift;
			for ( ; (mask&0x01); mask >>= 1 )
				--format->Gloss;
		}
		format->Bshift = 0;
		format->Bloss = 8;
		if ( Bmask ) {
			for ( mask = Bmask; !(mask&0x01); mask >>= 1 )
				++format->Bshift;
			for ( ; (mask&0x01); mask >>= 1 )
				--format->Bloss;
		}
		format->Ashift = 0;
		format->Aloss = 8;
		if ( Amask ) {
			for ( mask = Amask; !(mask&0x01); mask >>= 1 )
				++format->Ashift;
			for ( ; (mask&0x01); mask >>= 1 )
				--format->Aloss;
		}
		format->Rmask = Rmask;
		format->Gmask = Gmask;
		format->Bmask = Bmask;
		format->Amask = Amask;
	} else if ( bpp > 8 ) {		/* Packed pixels with standard mask */
		/* R-G-B */
		if ( bpp > 24 )
			bpp = 24;
		format->Rloss = 8-(bpp/3);
		format->Gloss = 8-(bpp/3)-(bpp%3);
		format->Bloss = 8-(bpp/3);
		format->Rshift = ((bpp/3)+(bpp%3))+(bpp/3);
		format->Gshift = (bpp/3);
		format->Bshift = 0;
		format->Rmask = ((0xFF>>format->Rloss)<<format->Rshift);
		format->Gmask = ((0xFF>>format->Gloss)<<format->Gshift);
		format->Bmask = ((0xFF>>format->Bloss)<<format->Bshift);
	} else {
		/* Palettized formats have no mask info */
		format->Rloss = 8;
		format->Gloss = 8;
		format->Bloss = 8;
		format->Aloss = 8;
		format->Rshift = 0;
		format->Gshift = 0;
		format->Bshift = 0;
		format->Ashift = 0;
		format->Rmask = 0;
		format->Gmask = 0;
		format->Bmask = 0;
		format->Amask = 0;
	}
	if ( bpp <= 8 ) {			/* Palettized mode */
		int ncolors = 1<<bpp;
#ifdef DEBUG_PALETTE
		fprintf(stderr,"bpp=%d ncolors=%d\n",bpp,ncolors);
#endif
		format->palette = (MIL_Palette *)MIL_malloc(sizeof(MIL_Palette));
		if ( format->palette == NULL ) {
			MIL_FreeFormat(format);
			MIL_OutOfMemory();
			return(NULL);
		}
		(format->palette)->ncolors = ncolors;
		(format->palette)->colors = (MIL_Color *)MIL_malloc(
				(format->palette)->ncolors*sizeof(MIL_Color));
		if ( (format->palette)->colors == NULL ) {
			MIL_FreeFormat(format);
			MIL_OutOfMemory();
			return(NULL);
		}
		if ( Rmask || Bmask || Gmask ) {
			/* create palette according to masks */
			int i;
			int Rm=0,Gm=0,Bm=0;
			int Rw=0,Gw=0,Bw=0;
#ifdef ENABLE_PALETTE_ALPHA
			int Am=0,Aw=0;
#endif
			if(Rmask)
			{
				Rw=8-format->Rloss;
				for(i=format->Rloss;i>0;i-=Rw)
					Rm|=1<<i;
			}
#ifdef DEBUG_PALETTE
			fprintf(stderr,"Rw=%d Rm=0x%02X\n",Rw,Rm);
#endif
			if(Gmask)
			{
				Gw=8-format->Gloss;
				for(i=format->Gloss;i>0;i-=Gw)
					Gm|=1<<i;
			}
#ifdef DEBUG_PALETTE
			fprintf(stderr,"Gw=%d Gm=0x%02X\n",Gw,Gm);
#endif
			if(Bmask)
			{
				Bw=8-format->Bloss;
				for(i=format->Bloss;i>0;i-=Bw)
					Bm|=1<<i;
			}
#ifdef DEBUG_PALETTE
			fprintf(stderr,"Bw=%d Bm=0x%02X\n",Bw,Bm);
#endif
#ifdef ENABLE_PALETTE_ALPHA
			if(Amask)
			{
				Aw=8-format->Aloss;
				for(i=format->Aloss;i>0;i-=Aw)
					Am|=1<<i;
			}
# ifdef DEBUG_PALETTE
			fprintf(stderr,"Aw=%d Am=0x%02X\n",Aw,Am);
# endif
#endif
			for(i=0; i < ncolors; ++i) {
				int r,g,b;
				r=(i&Rmask)>>format->Rshift;
				r=(r<<format->Rloss)|((r*Rm)>>Rw);
				format->palette->colors[i].r=r;

				g=(i&Gmask)>>format->Gshift;
				g=(g<<format->Gloss)|((g*Gm)>>Gw);
				format->palette->colors[i].g=g;

				b=(i&Bmask)>>format->Bshift;
				b=(b<<format->Bloss)|((b*Bm)>>Bw);
				format->palette->colors[i].b=b;

#ifdef ENABLE_PALETTE_ALPHA
				a=(i&Amask)>>format->Ashift;
				a=(a<<format->Aloss)|((a*Am)>>Aw);
				format->palette->colors[i].unused=a;
#else
				format->palette->colors[i].unused=0;
#endif
			}
		} else if ( ncolors == 2 ) {
			/* Create a black and white bitmap palette */
			format->palette->colors[0].r = 0xFF;
			format->palette->colors[0].g = 0xFF;
			format->palette->colors[0].b = 0xFF;
			format->palette->colors[1].r = 0x00;
			format->palette->colors[1].g = 0x00;
			format->palette->colors[1].b = 0x00;
		} else {
			/* Create an empty palette */
			MIL_memset((format->palette)->colors, 0,
				(format->palette)->ncolors*sizeof(MIL_Color));
		}
	}
	return(format);
}
MIL_PixelFormat *MIL_ReallocFormat(MIL_Surface *surface, int bpp,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	if ( surface->format ) {
		MIL_FreeFormat(surface->format);
		MIL_FormatChanged(surface);
	}
	surface->format = MIL_AllocFormat(bpp, Rmask, Gmask, Bmask, Amask);
	return surface->format;
}

/*
 * Change any previous mappings from/to the new surface format
 */
void MIL_FormatChanged(MIL_Surface *surface)
{
	static int format_version = 0;
	++format_version;
	if ( format_version < 0 ) { /* It wrapped... */
		format_version = 1;
	}
	surface->format_version = format_version;
	MIL_InvalidateMap(surface->map);
}
/*
 * Free a previously allocated format structure
 */
void MIL_FreeFormat(MIL_PixelFormat *format)
{
	if ( format ) {
		if ( format->palette ) {
			if ( format->palette->colors ) {
				MIL_free(format->palette->colors);
			}
			MIL_free(format->palette);
		}
		MIL_free(format);
	}
}
/*
 * Calculate an 8-bit (3 red, 3 green, 2 blue) dithered palette of colors
 */
void MIL_DitherColors(MIL_Color *colors, int bpp)
{
	int i;
	if(bpp != 8)
		return;		/* only 8bpp supported right now */

	for(i = 0; i < 256; i++) {
		int r, g, b;
		/* map each bit field to the full [0, 255] interval,
		   so 0 is mapped to (0, 0, 0) and 255 to (255, 255, 255) */
		r = i & 0xe0;
		r |= r >> 3 | r >> 6;
		colors[i].r = r;
		g = (i << 3) & 0xe0;
		g |= g >> 3 | g >> 6;
		colors[i].g = g;
		b = i & 0x3;
		b |= b << 2;
		b |= b << 4;
		colors[i].b = b;
	}
}
/* 
 * Calculate the pad-aligned scanline width of a surface
 */
Uint16 MIL_CalculatePitch(MIL_Surface *surface)
{
	Uint16 pitch;

	/* Surface should be 4-byte aligned for speed */
	pitch = surface->w*surface->format->BytesPerPixel;
	switch (surface->format->BitsPerPixel) {
		case 1:
			pitch = (pitch+7)/8;
			break;
		case 4:
			pitch = (pitch+1)/2;
			break;
		default:
			break;
	}
	pitch = (pitch + 3) & ~3;	/* 4-byte aligning */
	return(pitch);
}
/*
 * Match an RGB value to a particular palette index
 */
Uint8 MIL_FindColor(MIL_Palette *pal, Uint8 r, Uint8 g, Uint8 b)
{
	/* Do colorspace distance matching */
	unsigned int smallest;
	unsigned int distance;
	int rd, gd, bd;
	int i;
	Uint8 pixel=0;
		
	smallest = ~0;
	for ( i=0; i<pal->ncolors; ++i ) {
		rd = pal->colors[i].r - r;
		gd = pal->colors[i].g - g;
		bd = pal->colors[i].b - b;
		distance = (rd*rd)+(gd*gd)+(bd*bd);
		if ( distance < smallest ) {
			pixel = i;
			if ( distance == 0 ) { /* Perfect match! */
				break;
			}
			smallest = distance;
		}
	}
	return(pixel);
}

/* Find the opaque pixel value corresponding to an RGB triple */
Uint32 MIL_MapRGB
(const MIL_PixelFormat * const format,
 const Uint8 r, const Uint8 g, const Uint8 b)
{
	if ( format->palette == NULL ) {
		return (r >> format->Rloss) << format->Rshift
		       | (g >> format->Gloss) << format->Gshift
		       | (b >> format->Bloss) << format->Bshift
		       | format->Amask;
	} else {
		return MIL_FindColor(format->palette, r, g, b);
	}
}

/* Find the pixel value corresponding to an RGBA quadruple */
Uint32 MIL_MapRGBA
(const MIL_PixelFormat * const format,
 const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	if ( format->palette == NULL ) {
	        return (r >> format->Rloss) << format->Rshift
		    | (g >> format->Gloss) << format->Gshift
		    | (b >> format->Bloss) << format->Bshift
		    | ((a >> format->Aloss) << format->Ashift & format->Amask);
	} else {
		return MIL_FindColor(format->palette, r, g, b);
	}
}

void MIL_GetRGBA(Uint32 pixel, const MIL_PixelFormat * const fmt,
		 Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
	if ( fmt->palette == NULL ) {
	        /*
		 * This makes sure that the result is mapped to the
		 * interval [0..255], and the maximum value for each
		 * component is 255. This is important to make sure
		 * that white is indeed reported as (255, 255, 255),
		 * and that opaque alpha is 255.
		 * This only works for RGB bit fields at least 4 bit
		 * wide, which is almost always the case.
		 */
	        unsigned v;
		v = (pixel & fmt->Rmask) >> fmt->Rshift;
		*r = (v << fmt->Rloss) + (v >> (8 - (fmt->Rloss << 1)));
		v = (pixel & fmt->Gmask) >> fmt->Gshift;
		*g = (v << fmt->Gloss) + (v >> (8 - (fmt->Gloss << 1)));
		v = (pixel & fmt->Bmask) >> fmt->Bshift;
		*b = (v << fmt->Bloss) + (v >> (8 - (fmt->Bloss << 1)));
		if(fmt->Amask) {
		        v = (pixel & fmt->Amask) >> fmt->Ashift;
			*a = (v << fmt->Aloss) + (v >> (8 - (fmt->Aloss << 1)));
		} else {
		        *a = MIL_ALPHA_OPAQUE;
                }
	} else {
		*r = fmt->palette->colors[pixel].r;
		*g = fmt->palette->colors[pixel].g;
		*b = fmt->palette->colors[pixel].b;
		*a = MIL_ALPHA_OPAQUE;
	}
}

void MIL_GetRGB(Uint32 pixel, const MIL_PixelFormat * const fmt,
                Uint8 *r,Uint8 *g,Uint8 *b)
{
	if ( fmt->palette == NULL ) {
	        /* the note for MIL_GetRGBA above applies here too */
	        unsigned v;
		v = (pixel & fmt->Rmask) >> fmt->Rshift;
		*r = (v << fmt->Rloss) + (v >> (8 - (fmt->Rloss << 1)));
		v = (pixel & fmt->Gmask) >> fmt->Gshift;
		*g = (v << fmt->Gloss) + (v >> (8 - (fmt->Gloss << 1)));
		v = (pixel & fmt->Bmask) >> fmt->Bshift;
		*b = (v << fmt->Bloss) + (v >> (8 - (fmt->Bloss << 1)));
	} else {
		*r = fmt->palette->colors[pixel].r;
		*g = fmt->palette->colors[pixel].g;
		*b = fmt->palette->colors[pixel].b;
	}
}

/* Apply gamma to a set of colors - this is easy. :) */
void MIL_ApplyGamma(Uint16 *gamma, MIL_Color *colors, MIL_Color *output,
							int ncolors)
{
	int i;

	for ( i=0; i<ncolors; ++i ) {
		output[i].r = gamma[0*256 + colors[i].r] >> 8;
		output[i].g = gamma[1*256 + colors[i].g] >> 8;
		output[i].b = gamma[2*256 + colors[i].b] >> 8;
	}
}

/* Map from Palette to Palette */
static Uint8 *Map1to1(MIL_Palette *src, MIL_Palette *dst, int *identical)
{
	Uint8 *map;
	int i;

	if ( identical ) {
		if ( src->ncolors <= dst->ncolors ) {
			/* If an identical palette, no need to map */
			if ( MIL_memcmp(src->colors, dst->colors, src->ncolors*
						sizeof(MIL_Color)) == 0 ) {
				*identical = 1;
				return(NULL);
			}
		}
		*identical = 0;
	}
	map = (Uint8 *)MIL_malloc(src->ncolors);
	if ( map == NULL ) {
		MIL_OutOfMemory();
		return(NULL);
	}
	for ( i=0; i<src->ncolors; ++i ) {
		map[i] = MIL_FindColor(dst,
			src->colors[i].r, src->colors[i].g, src->colors[i].b);
	}
	return(map);
}
/* Map from Palette to BitField */
static Uint8 *Map1toN(MIL_PixelFormat *src, MIL_PixelFormat *dst)
{
	Uint8 *map;
	int i;
	int  bpp;
	unsigned alpha;
	MIL_Palette *pal = src->palette;

	bpp = ((dst->BytesPerPixel == 3) ? 4 : dst->BytesPerPixel);
	map = (Uint8 *)MIL_malloc(pal->ncolors*bpp);
	if ( map == NULL ) {
		MIL_OutOfMemory();
		return(NULL);
	}

	alpha = dst->Amask ? src->alpha : 0;
	/* We memory copy to the pixel map so the endianness is preserved */
	for ( i=0; i<pal->ncolors; ++i ) {
		ASSEMBLE_RGBA(&map[i*bpp], dst->BytesPerPixel, dst,
			      pal->colors[i].r, pal->colors[i].g,
			      pal->colors[i].b, alpha);
	}
	return(map);
}
/* Map from BitField to Dithered-Palette to Palette */
static Uint8 *MapNto1(MIL_PixelFormat *src, MIL_PixelFormat *dst, int *identical)
{
	/* Generate a 256 color dither palette */
	MIL_Palette dithered;
	MIL_Color colors[256];
	MIL_Palette *pal = dst->palette;
	
	/* MIL_DitherColors does not initialize the 'unused' component of colors,
	   but Map1to1 compares it against pal, so we should initialize it. */  
	MIL_memset(colors, 0, sizeof(colors));

	dithered.ncolors = 256;
	MIL_DitherColors(colors, 8);
	dithered.colors = colors;
	return(Map1to1(&dithered, pal, identical));
}

MIL_BlitMap *MIL_AllocBlitMap(void)
{
	MIL_BlitMap *map;

	/* Allocate the empty map */
	map = (MIL_BlitMap *)MIL_malloc(sizeof(*map));
	if ( map == NULL ) {
		MIL_OutOfMemory();
		return(NULL);
	}
	MIL_memset(map, 0, sizeof(*map));

	/* Allocate the software blit data */
	map->sw_data = (struct private_swaccel *)MIL_malloc(sizeof(*map->sw_data));
	if ( map->sw_data == NULL ) {
		MIL_FreeBlitMap(map);
		MIL_OutOfMemory();
		return(NULL);
	}
	MIL_memset(map->sw_data, 0, sizeof(*map->sw_data));

	/* It's ready to go */
	return(map);
}
void MIL_InvalidateMap(MIL_BlitMap *map)
{
	if ( ! map ) {
		return;
	}
	map->dst = NULL;
	map->format_version = (unsigned int)-1;
	if ( map->table ) {
		MIL_free(map->table);
		map->table = NULL;
	}
}
int MIL_MapSurface (MIL_Surface *src, MIL_Surface *dst)
{
	MIL_PixelFormat *srcfmt;
	MIL_PixelFormat *dstfmt;
	MIL_BlitMap *map;

	/* Clear out any previous mapping */
	map = src->map;
	if ( (src->flags & MIL_RLEACCEL) == MIL_RLEACCEL ) {
		MIL_UnRLESurface(src, 1);
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
			map->table = Map1toN(srcfmt, dstfmt);
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
			map->table = MapNto1(srcfmt, dstfmt, &map->identity);
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
void MIL_FreeBlitMap(MIL_BlitMap *map)
{
	if ( map ) {
		MIL_InvalidateMap(map);
		if ( map->sw_data != NULL ) {
			MIL_free(map->sw_data);
		}
		MIL_free(map);
	}
}
