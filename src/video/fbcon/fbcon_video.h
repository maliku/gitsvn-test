/*!============================================================================
 * @file qvfb_video.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月20日 23时12分54秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  FBcon_VIDEO_INC
#define  FBcon_VIDEO_INC
#include <sys/types.h>
#include <termios.h>
#include <linux/fb.h>

#include "surface.h"
#include "video_device.h"
//#include "MIL_mouse.h"
#include "MIL_mutex.h"
//#include "../MIL_sysvideo.h"
#if MIL_INPUT_TSLIB
#include "tslib.h"
#endif

/* Hidden "self" pointer for the video functions */
#define _THIS	VideoDevice* self

typedef void FB_bitBlit(
		Uint8 *src_pos,
		int src_right_delta,	/* pixels, not bytes */
		int src_down_delta,		/* pixels, not bytes */
		Uint8 *dst_pos,
		int dst_linebytes,
		int width,
		int height);

/* This is the structure we use to keep track of video memory */
typedef struct vidmem_bucket {
	struct vidmem_bucket *prev;
	int used;
	int dirty;
	char *base;
	unsigned int size;
	struct vidmem_bucket *next;
} vidmem_bucket;

/* Private display data */
STRUCT {
	int console_fd;
	struct fb_var_screeninfo cache_vinfo;
	struct fb_var_screeninfo saved_vinfo;
	int saved_cmaplen;
	__u16 *saved_cmap;

	int current_vt;
	int saved_vt;
	int keyboard_fd;
	int saved_kbd_mode;
	struct termios saved_kbd_termios;

	int mouse_fd;
#if MIL_INPUT_TSLIB
	struct tsdev *ts_dev;
#endif

	char *mapped_mem;
	char *shadow_mem;
	int mapped_memlen;
	int mapped_offset;
	char *mapped_io;
	long mapped_iolen;
	int flip_page;
	char *flip_address[2];
	int rotate;
	int shadow_fb;				/* Tells whether a shadow is being used. */
	FB_bitBlit *blitFunc;
	int physlinebytes;			/* Length of a line in bytes in physical fb */

#define NUM_MODELISTS	4		/* 8, 16, 24, and 32 bits-per-pixel */
	int MIL_nummodes[NUM_MODELISTS];
	MIL_Rect **MIL_modelist[NUM_MODELISTS];

	vidmem_bucket surfaces;
	int surfaces_memtotal;
	int surfaces_memleft;

	MIL_mutex *hw_lock;
	int switched_away;
	struct fb_var_screeninfo screen_vinfo;
	Uint32 screen_arealen;
	Uint8 *screen_contents;
	__u16  screen_palette[3*256];

	void (*wait_vbl)(_THIS);
	void (*wait_idle)(_THIS);
}MIL_PrivateVideoData;

/* Accelerator types that are supported by the driver, but are not
   necessarily in the kernel headers on the system we compile on.
*/
#ifndef FB_ACCEL_MATROX_MGAG400
#define FB_ACCEL_MATROX_MGAG400	26	/* Matrox G400			*/
#endif
#ifndef FB_ACCEL_3DFX_BANSHEE
#define FB_ACCEL_3DFX_BANSHEE	31	/* 3Dfx Banshee			*/
#endif

CLASS_INHERIT_BEGIN(FBconVideoDevice, VideoDevice)
    METHOD_DECLARE_PLACEHOLDER(FBconVideoDevice)
    /* Private display data */
    MIL_PrivateVideoData* hw_data; 
    Surface* screen;
	int offset_x;
	int offset_y;
CLASS_INHERIT_END


/* These functions are defined in MIL_fbvideo.c */
extern void FB_SavePaletteTo(_THIS, int palette_len, __u16 *area);
extern void FB_RestorePaletteFrom(_THIS, int palette_len, __u16 *area);

/* These are utility functions for working with video surfaces */

static __inline__ void FB_AddBusySurface(Surface *surface)
{
	((vidmem_bucket *)surface->hwdata)->dirty = 1;
}

static __inline__ int FB_IsSurfaceBusy(Surface *surface)
{
	return ((vidmem_bucket *)surface->hwdata)->dirty;
}

static __inline__ void FB_WaitBusySurfaces(_THIS)
{
	vidmem_bucket *bucket;

	/* Wait for graphic operations to complete */
	wait_idle(self);

	/* Clear all surface dirty bits */
	for ( bucket=&(((FBconVideoDevice*)self)->hw_data->surfaces); bucket; 
            bucket=bucket->next ) {
		bucket->dirty = 0;
	}
}

static __inline__ void FB_dst_to_xy(_THIS, Surface *dst, int *x, int *y)
{
	*x = (long)((char *)dst->pixels - ((FBconVideoDevice*)self)->hw_data->mapped_mem) % ((FBconVideoDevice*)self)->screen->pitch;
	*y = (long)((char *)dst->pixels - ((FBconVideoDevice*)self)->hw_data->mapped_mem) / 
        ((FBconVideoDevice*)self)->screen->pitch;
	if ( dst == ((FBconVideoDevice*)self)->screen ) {
		*x += ((FBconVideoDevice*)self)->offset_x;
		*y += ((FBconVideoDevice*)self)->offset_y;
	}
}
#endif   /* ----- #ifndef FBcon_VIDEO_INC  ----- */
