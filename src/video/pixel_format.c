/*! ============================================================================
 * @file pixel_format.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "pixel_format.h"
#include "pixels.h"

VIRTUAL_METHOD_REGISTER_PLACEHOLDER(MIL_PixelFormat, NonBase)
METHOD_REGISTER_PLACEHOLDER(MIL_PixelFormat)

CONSTRUCTOR(PixelFormat)
{
    _m(palette) = NULL;
    _m(BitsPerPixel) = 0;
	_m(BytesPerPixel) = 0;
	_m(Rloss) = 0;
	_m(Gloss) = 0;
	_m(Bloss) = 0;
	_m(Aloss) = 0;
	_m(Rshift) = 0;
	_m(Gshift) = 0;
	_m(Bshift) = 0;
	_m(Ashift) = 0;
	_m(Rmask) = 0;
	_m(Gmask) = 0;
	_m(Bmask) = 0;
	_m(Amask) = 0;
	_m(colorkey) = 0;
	_m(alpha) = 0;

    return self;
}

DESTRUCTOR(PixelFormat)
{
    PixelFormat* format = (PixelFormat*)self;
    if ( NULL != format ) {
        if ( format->palette ) {
            if ( format->palette->colors ) {
                MIL_free(format->palette->colors);
            }
            MIL_free(format->palette);
        }
    }
}

Uint32 PixelFormat_X_mapRGB(_CSELF, Uint8 r, Uint8 g, Uint8 b)
{
    PixelFormat* format = (PixelFormat*)self;
	if ( format->palette == NULL ) {
		return (r >> format->Rloss) << format->Rshift
		       | (g >> format->Gloss) << format->Gshift
		       | (b >> format->Bloss) << format->Bshift
		       | format->Amask;
	} else {
		return MIL_FindColor(format->palette, r, g, b);
	}
}

Uint32 PixelFormat_X_mapRGBA(_CSELF, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    PixelFormat* format = (PixelFormat*)self;
	if ( format->palette == NULL ) {
	        return (r >> format->Rloss) << format->Rshift
		    | (g >> format->Gloss) << format->Gshift
		    | (b >> format->Bloss) << format->Bshift
		    | ((a >> format->Aloss) << format->Ashift & format->Amask);
	} else {
		return MIL_FindColor(format->palette, r, g, b);
	}
}

void PixelFormat_X_getRGB(_CSELF, Uint32 pixel, Uint8 *r,Uint8 *g,Uint8 *b)
{
    PixelFormat* fmt = (PixelFormat*)self;
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

void PixelFormat_X_getRGBA(_CSELF, Uint32 pixel, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
    PixelFormat* fmt = (PixelFormat*)self;
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
/* Map from Palette to Palette */
Uint8* Map1to1(MIL_Palette *src, MIL_Palette *dst, int *identical)
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

Uint8* PixelFormat_X_mapNto1(_CSELF, MIL_PixelFormat *dst_fmt, int *identical)
{
    PixelFormat* src = (PixelFormat*)self;
    PixelFormat* dst = (PixelFormat*)dst_fmt;
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

/* Map from Palette to BitField */
Uint8* PixelFormat_X_map1toN(_CSELF, MIL_PixelFormat *dfmt)
{
	Uint8 *map = NULL;
	int i;
	int  bpp;
	unsigned alpha;
    PixelFormat* src = (PixelFormat*)self;
    PixelFormat* dst = (PixelFormat*)dfmt;
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
Uint8  PixelFormat_X_getBytesPerPixel(_CSELF)
{
    return _tm(PixelFormat, BytesPerPixel);
}

Uint8  PixelFormat_X_getBitsPerPixel(_CSELF)
{
    return _tm(PixelFormat, BitsPerPixel);
}

Uint8  PixelFormat_X_getAlpha(_CSELF)
{
    return _tm(PixelFormat, alpha);
}

Uint32  PixelFormat_X_getColorKey(_CSELF)
{
    return _tm(PixelFormat, colorkey);
}

VIRTUAL_METHOD_REGBEGIN(PixelFormat, MIL_PixelFormat)
    DESTRUCTOR_REGISTER(PixelFormat)
    METHOD_REGISTER(PixelFormat, mapRGB)
    METHOD_REGISTER(PixelFormat, mapRGBA)
    METHOD_REGISTER(PixelFormat, getRGB)
    METHOD_REGISTER(PixelFormat, getRGBA)
    METHOD_REGISTER(PixelFormat, mapNto1)
    METHOD_REGISTER(PixelFormat, map1toN)
    METHOD_REGISTER(PixelFormat, getBytesPerPixel)
    METHOD_REGISTER(PixelFormat, getBitsPerPixel)
    METHOD_REGISTER(PixelFormat, getAlpha)
    METHOD_REGISTER(PixelFormat, getColorKey)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(PixelFormat)
    CONSTRUCTOR_REGISTER(PixelFormat)
METHOD_REGEND

/*
 * Allocate a pixel format structure and fill it according to the given info.
 */
MIL_PixelFormat* MIL_AllocFormat(int bpp,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	PixelFormat *format = NULL;
	Uint32 mask;

	/* Allocate an empty pixel format structure */
	format = (PixelFormat*)New(PixelFormat);
	if ( format == NULL ) {
		MIL_OutOfMemory();
		return(NULL);
	}
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
			Delete(format);
			MIL_OutOfMemory();
			return(NULL);
		}
		(format->palette)->ncolors = ncolors;
		(format->palette)->colors = (MIL_Color *)MIL_malloc(
				(format->palette)->ncolors*sizeof(MIL_Color));
		if ( (format->palette)->colors == NULL ) {
			Delete(format);
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
	return((MIL_PixelFormat*)format);
}

