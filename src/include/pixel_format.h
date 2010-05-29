/*!============================================================================
 * @file pixel_format.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  PIXEL_FORMAT_INC
#define  PIXEL_FORMAT_INC
#include "MIL_video.h"

/** Everything in the pixel format structure is read-only */
BEGIN_CLASS_INHERIT(PixelFormat, MIL_PixelFormat)
    NO_METHOD_EXPAND(PixelFormat)
    
	MIL_Palette *palette;
	Uint8  BitsPerPixel;
	Uint8  BytesPerPixel;
	Uint8  Rloss;
	Uint8  Gloss;
	Uint8  Bloss;
	Uint8  Aloss;
	Uint8  Rshift;
	Uint8  Gshift;
	Uint8  Bshift;
	Uint8  Ashift;
	/** Alpha value information (per-surface alpha) */
	Uint8  alpha;
    Uint8  align;
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	Uint32 Amask;

	/** RGB color key information */
	Uint32 colorkey;
END_CLASS_INHERIT

Uint8* Map1to1(MIL_Palette *src, MIL_Palette *dst, int *identical);
#endif   /* ----- #ifndef PIXEL_FORMAT_INC  ----- */

