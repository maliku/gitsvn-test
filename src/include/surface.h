/*!============================================================================
 * @file surface.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月11日 17时01分39秒 
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef  SURFACE_INC
#define  SURFACE_INC
#include "MIL_video.h"

CLASS_INHERIT_BEGIN(Surface, MIL_Surface)
    METHOD_DECLARE_PLACEHOLDER(Surface)
    
    Uint32 flags;				/* Read-only */
	MIL_PixelFormat *format;		/* Read-only */
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
	struct MIL_BlitMap *map;		/* Private */

	/* format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/* Private */

	/* Reference count -- used when freeing surface */
	int refcount;				/* Read-mostly */
CLASS_INHERIT_END

#endif   /* ----- #ifndef SURFACE_INC  ----- */

