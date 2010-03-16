/*!============================================================================
 * @file surface.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月07日 20时26分54秒 
 *  Organization: http://www.ds0101.net 
 */

#ifndef  SURFACE_INC
#define  SURFACE_INC

#include "coo.h"
/* #include "mutex.h"
#include "rwops.h"
 */

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Transparency definitions: These define alpha as the opacity of a surface */
#define MIL_ALPHA_OPAQUE 255
#define MIL_ALPHA_TRANSPARENT 0

/* Useful data types */
typedef struct MIL_Rect {
	Sint32 x, y;
	Uint32 w, h;
} MIL_Rect;

typedef struct MIL_Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 unused;
} MIL_Color;
typedef MIL_Color MIL_Colour;

typedef struct MIL_Palette {
	int       ncolors;
	MIL_Color *colors;
} MIL_Palette;

/* Everything in the pixel format structure is read-only */
typedef struct MIL_PixelFormat {
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
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	Uint32 Amask;

	/* RGB color key information */
	Uint32 colorkey;
	/* Alpha value information (per-surface alpha) */
	Uint8  alpha;
} MIL_PixelFormat;

/* This structure should be treated as read-only, except for 'pixels',
   which, if not NULL, contains the raw pixel data for the surface.
*/
CLASS(MIL_Surface) {
    VIRTUAL_METHOD_REGISTER_PLACEHOLDER(MIL_Surface)
    METHOD_REGISTER_PLACEHOLDER(MIL_Surface)
    
    Uint32 flags;				/* Read-only */
	MIL_PixelFormat *format;		/* Read-only */
	int w, h;				/* Read-only */
	Uint16 pitch;				/* Read-only */
	void *pixels;				/* Read-write */
	int offset;				/* Private */

	/* Hardware-specific surface info */
	struct private_hwdata *hwdata;

	/* clipping information */
	MIL_Rect clip_rect;			/* Read-only */
	Uint32 unused1;				/* for binary compatibility */

	/* Allow recursive locks */
	Uint32 locked;				/* Private */

	/* info for fast blit mapping to other surfaces */
	struct MIL_BlitMap *map;		/* Private */

	/* format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/* Private */

	/* Reference count -- used when freeing surface */
	int refcount;				/* Read-mostly */
} MIL_Surface;

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef SURFACE_INC  ----- */

