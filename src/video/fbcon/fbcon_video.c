/*! ============================================================================
 * @file qvfb_video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-20
 *  Organization: http://www.ds0101.net
 */
#include "MIL_config.h"

#if MIL_VIDEO_DRIVER_FBCON

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

/* TODO: which headfile. */
#if 0
#ifndef HAVE_GETPAGESIZE
#include <asm/page.h>		/* For definition of PAGE_SIZE */
#endif
#endif

#include <linux/vt.h>

#include "surface.h"
#include "pixel_format.h"
#include "fbcon_video.h"

#if defined(i386) && defined(FB_TYPE_VGA_PLANES)
#define VGA16_FBCON_SUPPORT
#include <sys/io.h>		/* For ioperm() */
#ifndef FB_AUX_VGA_PLANES_VGA4
#define FB_AUX_VGA_PLANES_VGA4	0
#endif
#endif

#define FBCON_VIDEO_DRIVER_NAME "fbcon"

/* A list of video resolutions that we query for (sorted largest to smallest) */
static const MIL_Rect checkres[] = {
	{  0, 0, 1600, 1200 },		/* 16 bpp: 0x11E, or 286 */
	{  0, 0, 1408, 1056 },		/* 16 bpp: 0x19A, or 410 */
	{  0, 0, 1280, 1024 },		/* 16 bpp: 0x11A, or 282 */
	{  0, 0, 1152,  864 },		/* 16 bpp: 0x192, or 402 */
	{  0, 0, 1024,  768 },		/* 16 bpp: 0x117, or 279 */
	{  0, 0,  960,  720 },		/* 16 bpp: 0x18A, or 394 */
	{  0, 0,  800,  600 },		/* 16 bpp: 0x114, or 276 */
	{  0, 0,  768,  576 },		/* 16 bpp: 0x182, or 386 */
	{  0, 0,  720,  576 },		/* PAL */
	{  0, 0,  720,  480 },		/* NTSC */
	{  0, 0,  640,  480 },		/* 16 bpp: 0x111, or 273 */
	{  0, 0,  640,  400 },		/*  8 bpp: 0x100, or 256 */
	{  0, 0,  512,  384 },
	{  0, 0,  320,  240 },
	{  0, 0,  320,  200 }
};
static const struct {
	int xres;
	int yres;
	int pixclock;
	int left;
	int right;
	int upper;
	int lower;
	int hslen;
	int vslen;
	int sync;
	int vmode;
} vesa_timings[] = {
#ifdef USE_VESA_TIMINGS	/* Only tested on Matrox Millenium I */
	{  640,  400, 39771,  48, 16, 39,  8,  96, 2, 2, 0 },	/* 70 Hz */
	{  640,  480, 39683,  48, 16, 33, 10,  96, 2, 0, 0 },	/* 60 Hz */
	{  768,  576, 26101, 144, 16, 28,  6, 112, 4, 0, 0 },	/* 60 Hz */
	{  800,  600, 24038, 144, 24, 28,  8, 112, 6, 0, 0 },	/* 60 Hz */
	{  960,  720, 17686, 144, 24, 28,  8, 112, 4, 0, 0 },	/* 60 Hz */
	{ 1024,  768, 15386, 160, 32, 30,  4, 128, 4, 0, 0 },	/* 60 Hz */
	{ 1152,  864, 12286, 192, 32, 30,  4, 128, 4, 0, 0 },	/* 60 Hz */
	{ 1280, 1024,  9369, 224, 32, 32,  4, 136, 4, 0, 0 },	/* 60 Hz */
	{ 1408, 1056,  8214, 256, 40, 32,  5, 144, 5, 0, 0 },	/* 60 Hz */
	{ 1600, 1200,/*?*/0, 272, 48, 32,  5, 152, 5, 0, 0 },	/* 60 Hz */
#else
	/* You can generate these timings from your XF86Config file using
	   the 'modeline2fb' perl script included with the fbset package.
	   These timings were generated for Matrox Millenium I, 15" monitor.
	*/
	{  320,  200, 79440,  16, 16, 20,  4,  48, 1, 0, 2 },	/* 70 Hz */
	{  320,  240, 63492,  16, 16, 16,  4,  48, 2, 0, 2 },	/* 72 Hz */
	{  512,  384, 49603,  48, 16, 16,  1,  64, 3, 0, 0 },	/* 78 Hz */
	{  640,  400, 31746,  96, 32, 41,  1,  64, 3, 2, 0 },	/* 85 Hz */
	{  640,  480, 31746, 120, 16, 16,  1,  64, 3, 0, 0 },	/* 75 Hz */
	{  768,  576, 26101, 144, 16, 28,  6, 112, 4, 0, 0 },	/* 60 Hz */
	{  800,  600, 20000,  64, 56, 23, 37, 120, 6, 3, 0 },	/* 72 Hz */
	{  960,  720, 17686, 144, 24, 28,  8, 112, 4, 0, 0 },	/* 60 Hz */
	{ 1024,  768, 13333, 144, 24, 29,  3, 136, 6, 0, 0 },	/* 70 Hz */
	{ 1152,  864, 12286, 192, 32, 30,  4, 128, 4, 0, 0 },	/* 60 Hz */
	{ 1280, 1024,  9369, 224, 32, 32,  4, 136, 4, 0, 0 },	/* 60 Hz */
	{ 1408, 1056,  8214, 256, 40, 32,  5, 144, 5, 0, 0 },	/* 60 Hz */
	{ 1600, 1200,/*?*/0, 272, 48, 32,  5, 152, 5, 0, 0 }	/* 60 Hz */
#endif
};
enum {
	FBCON_ROTATE_NONE = 0,
	FBCON_ROTATE_CCW = 90,
	FBCON_ROTATE_UD = 180,
	FBCON_ROTATE_CW = 270
};

VideoDevice* CreateFBconVideoDevice(void)
{
    VideoDevice* video = (VideoDevice*)New(FBconVideoDevice);
    if (NULL != video) {
        PixelFormat* vformat = (PixelFormat*)New(PixelFormat);
        if (0 != _c(video)->videoInit(video, vformat)) {
            Delete(vformat);
            Delete(video);
            return NULL;
        }
        Delete(vformat);
    }
    return video;
}

/* Shadow buffer functions */
static FB_bitBlit FB_blit16;
static FB_bitBlit FB_blit16blocked;

VideoDeviceEntry g_video_fbcon = {
    FBCON_VIDEO_DRIVER_NAME,
    CreateFBconVideoDevice
};

static void FB_WaitVBL(_Self(VideoDevice))
{
#ifdef FBIOWAITRETRACE /* Heheh, self didn't make it into the main kernel */
	ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOWAITRETRACE, 0);
#endif
	return;
}

static void FB_WaitIdle(_Self(VideoDevice))
{
	return;
}

static int FB_FlipHWSurface(_Self(VideoDevice), Surface *surface)
{
	if ( ((FBconVideoDevice*)self)->hw_data->switched_away ) {
		return -2; /* no hardware access */
	}

	/* Wait for vertical retrace and then flip display */
	((FBconVideoDevice*)self)->hw_data->cache_vinfo.yoffset = 
        ((FBconVideoDevice*)self)->hw_data->flip_page * surface->h;
	if ( FB_IsSurfaceBusy(((FBconVideoDevice*)self)->screen) ) {
		FB_WaitBusySurfaces(self);
	}
	((FBconVideoDevice*)self)->hw_data->wait_vbl(self);
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPAN_DISPLAY, 
                &((FBconVideoDevice*)self)->hw_data->cache_vinfo) < 0 ) {
		MIL_SetError("ioctl(FBIOPAN_DISPLAY) failed");
		return(-1);
	}
	((FBconVideoDevice*)self)->hw_data->flip_page = 
        !((FBconVideoDevice*)self)->hw_data->flip_page;

	surface->pixels = 
        ((FBconVideoDevice*)self)->hw_data->flip_address[((FBconVideoDevice*)self)->hw_data->flip_page];
	return(0);
}
CONSTRUCTOR(FBconVideoDevice)
{
    printf("FBconVideoDevice %p constructed...\n", self);
    _m(hw_data) = (MIL_PrivateVideoData *)MIL_malloc(sizeof (*_m(hw_data)));
    ((VideoDevice*)self)->name = FBCON_VIDEO_DRIVER_NAME;
	if (NULL == _m(hw_data)) {
		/*MIL_OutOfMemory();*/
		if ( self ) {
			MIL_free(self);
		}
		return(NULL);
	}
	MIL_memset(_m(hw_data), 0, (sizeof *_m(hw_data)));
	_m(hw_data)->wait_vbl = FB_WaitVBL;
	_m(hw_data)->wait_idle = FB_WaitIdle;
	_m(hw_data)->mouse_fd = -1;
	_m(hw_data)->keyboard_fd = -1;

    return self;
}

DESTRUCTOR(FBconVideoDevice)
{
    printf("FBconVideoDevice %p destructed...\n", self);
    MIL_free(((FBconVideoDevice*)self)->hw_data);
}

static int MIL_getpagesize(void)
{
#ifdef HAVE_GETPAGESIZE
	return getpagesize();
#elif defined(PAGE_SIZE)
	return PAGE_SIZE;
#else
#error Can not determine system page size.
	return 4096;  /* self is what it USED to be in Linux... */
#endif
}


/* Small wrapper for mmap() so we can play nicely with no-mmu hosts
 * (non-mmu hosts disallow the MAP_SHARED flag) */

static void *do_mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
	void *ret;
	ret = mmap(start, length, prot, flags, fd, offset);
	if ( ret == (char *)-1 && (flags & MAP_SHARED) ) {
		ret = mmap(start, length, prot,
		           (flags & ~MAP_SHARED) | MAP_PRIVATE, fd, offset);
	}
	return ret;
}

#define FB_MODES_DB	"/etc/fb.modes"

static int read_fbmodes_line(FILE*f, char* line, int length)
{
	int blank;
	char* c;
	int i;
	
	blank=0;
	/* find a relevant line */
	do
	{
		if (!fgets(line,length,f))
			return 0;
		c=line;
		while(((*c=='\t')||(*c==' '))&&(*c!=0))
			c++;
		
		if ((*c=='\n')||(*c=='#')||(*c==0))
			blank=1;
		else
			blank=0;
	}
	while(blank);
	/* remove whitespace at the begining of the string */
	i=0;
	do
	{
		line[i]=c[i];
		i++;
	}
	while(c[i]!=0);
	return 1;
}

static int read_fbmodes_mode(FILE *f, struct fb_var_screeninfo *vinfo)
{
	char line[1024];
	char option[256];

	/* Find a "geometry" */
	do {
		if (read_fbmodes_line(f, line, sizeof(line))==0)
			return 0;
		if (MIL_strncmp(line,"geometry",8)==0)
			break;
	}
	while(1);

	MIL_sscanf(line, "geometry %d %d %d %d %d", &vinfo->xres, &vinfo->yres, 
			&vinfo->xres_virtual, &vinfo->yres_virtual, &vinfo->bits_per_pixel);
	if (read_fbmodes_line(f, line, sizeof(line))==0)
		return 0;
			
	MIL_sscanf(line, "timings %d %d %d %d %d %d %d", &vinfo->pixclock, 
			&vinfo->left_margin, &vinfo->right_margin, &vinfo->upper_margin, 
			&vinfo->lower_margin, &vinfo->hsync_len, &vinfo->vsync_len);
		
	vinfo->sync=0;
	vinfo->vmode=FB_VMODE_NONINTERLACED;
				
	/* Parse misc options */
	do {
		if (read_fbmodes_line(f, line, sizeof(line))==0)
			return 0;

		if (MIL_strncmp(line,"hsync",5)==0) {
			MIL_sscanf(line,"hsync %s",option);
			if (MIL_strncmp(option,"high",4)==0)
				vinfo->sync |= FB_SYNC_HOR_HIGH_ACT;
		}
		else if (MIL_strncmp(line,"vsync",5)==0) {
			MIL_sscanf(line,"vsync %s",option);
			if (MIL_strncmp(option,"high",4)==0)
				vinfo->sync |= FB_SYNC_VERT_HIGH_ACT;
		}
		else if (MIL_strncmp(line,"csync",5)==0) {
			MIL_sscanf(line,"csync %s",option);
			if (MIL_strncmp(option,"high",4)==0)
				vinfo->sync |= FB_SYNC_COMP_HIGH_ACT;
		}
		else if (MIL_strncmp(line,"extsync",5)==0) {
			MIL_sscanf(line,"extsync %s",option);
			if (MIL_strncmp(option,"true",4)==0)
				vinfo->sync |= FB_SYNC_EXT;
		}
		else if (MIL_strncmp(line,"laced",5)==0) {
			MIL_sscanf(line,"laced %s",option);
			if (MIL_strncmp(option,"true",4)==0)
				vinfo->vmode |= FB_VMODE_INTERLACED;
		}
		else if (MIL_strncmp(line,"double",6)==0) {
			MIL_sscanf(line,"double %s",option);
			if (MIL_strncmp(option,"true",4)==0)
				vinfo->vmode |= FB_VMODE_DOUBLE;
		}
	}
	while(MIL_strncmp(line,"endmode",7)!=0);

	return 1;
}

static int FB_available(void)
{
	int console = -1;
	/* Added check for /fb/0 (devfs) */
	/* but - use environment variable first... if it fails, still check defaults */
	int idx = 0;
	const char *MIL_fbdevs[4] = { NULL, "/dev/fb0", "/dev/fb/0", NULL };

	MIL_fbdevs[0] = MIL_getenv("MIL_FBDEV");
	if( !MIL_fbdevs[0] )
		idx++;
	for( ; MIL_fbdevs[idx]; idx++ )
	{
		console = open(MIL_fbdevs[idx], O_RDWR, 0);
		if ( console >= 0 ) {
			close(console);
			break;
		}
	}
	return(console >= 0);
}

static int FB_CheckMode(_Self(VideoDevice), struct fb_var_screeninfo *vinfo,
                        int index, unsigned int *w, unsigned int *h)
{
	int mode_okay;

	mode_okay = 0;
	vinfo->bits_per_pixel = (index+1)*8;
	vinfo->xres = *w;
	vinfo->xres_virtual = *w;
	vinfo->yres = *h;
	vinfo->yres_virtual = *h;
	vinfo->activate = FB_ACTIVATE_TEST;
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUT_VSCREENINFO, 
                vinfo) == 0 ) {
#ifdef FBCON_DEBUG
		fprintf(stderr, "Checked mode %dx%d at %d bpp, got mode %dx%d at %d bpp\n", *w, *h, (index+1)*8, vinfo->xres, vinfo->yres, vinfo->bits_per_pixel);
#endif
		if ( (((vinfo->bits_per_pixel+7)/8)-1) == index ) {
			*w = vinfo->xres;
			*h = vinfo->yres;
			mode_okay = 1;
		}
	}
	return mode_okay;
}

static int FB_AddMode(_Self(VideoDevice), int index, unsigned int w, unsigned int h, int check_timings)
{
	MIL_Rect *mode;
	int i;
	int next_mode;

	/* Check to see if we already have self mode */
	if ( ((FBconVideoDevice*)self)->hw_data->MIL_nummodes[index] > 0 ) {
		mode = 
            ((FBconVideoDevice*)self)->hw_data->MIL_modelist[index][((FBconVideoDevice*)self)->hw_data->MIL_nummodes[index]-1];
		if ( (mode->w == w) && (mode->h == h) ) {
#ifdef FBCON_DEBUG
			fprintf(stderr, "We already have mode %dx%d at %d bytes per pixel\n", w, h, index+1);
#endif
			return(0);
		}
	}

	/* Only allow a mode if we have a valid timing for it */
	if ( check_timings ) {
		int found_timing = 0;
		for ( i=0; i<(sizeof(vesa_timings)/sizeof(vesa_timings[0])); ++i ) {
			if ( (w == vesa_timings[i].xres) &&
			     (h == vesa_timings[i].yres) && vesa_timings[i].pixclock ) {
				found_timing = 1;
				break;
			}
		}
		if ( !found_timing ) {
#ifdef FBCON_DEBUG
			fprintf(stderr, "No valid timing line for mode %dx%d\n", w, h);
#endif
			return(0);
		}
	}

	/* Set up the new video mode rectangle */
	mode = (MIL_Rect *)MIL_malloc(sizeof *mode);
	if ( mode == NULL ) {
		/*MIL_OutOfMemory();*/
		return(-1);
	}
	mode->x = 0;
	mode->y = 0;
	mode->w = w;
	mode->h = h;
#ifdef FBCON_DEBUG
	fprintf(stderr, "Adding mode %dx%d at %d bytes per pixel\n", w, h, index+1);
#endif

	/* Allocate the new list of modes, and fill in the new mode */
	next_mode = ((FBconVideoDevice*)self)->hw_data->MIL_nummodes[index];
	((FBconVideoDevice*)self)->hw_data->MIL_modelist[index] = (MIL_Rect **)
	       MIL_realloc(((FBconVideoDevice*)self)->hw_data->MIL_modelist[index], (1+next_mode+1)*sizeof(MIL_Rect *));
	if ( ((FBconVideoDevice*)self)->hw_data->MIL_modelist[index] == NULL ) {
		/*MIL_OutOfMemory();*/
		((FBconVideoDevice*)self)->hw_data->MIL_nummodes[index] = 0;
		MIL_free(mode);
		return(-1);
	}
	((FBconVideoDevice*)self)->hw_data->MIL_modelist[index][next_mode] = mode;
	((FBconVideoDevice*)self)->hw_data->MIL_modelist[index][next_mode+1] = NULL;
	((FBconVideoDevice*)self)->hw_data->MIL_nummodes[index]++;

	return(0);
}

static int cmpmodes(const void *va, const void *vb)
{
    const MIL_Rect *a = *(const MIL_Rect**)va;
    const MIL_Rect *b = *(const MIL_Rect**)vb;
    if ( a->h == b->h )
        return b->w - a->w;
    else
        return b->h - a->h;
}

static void FB_SortModes(_Self(VideoDevice))
{
	int i;
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		if ( ((FBconVideoDevice*)self)->hw_data->MIL_nummodes[i] > 0 ) {
			MIL_qsort(((FBconVideoDevice*)self)->hw_data->MIL_modelist[i], ((FBconVideoDevice*)self)->hw_data->MIL_nummodes[i], sizeof *((FBconVideoDevice*)self)->hw_data->MIL_modelist[i], cmpmodes);
		}
	}
}

static void FB_SavePalette(_Self(VideoDevice), struct fb_fix_screeninfo *finfo,
                                  struct fb_var_screeninfo *vinfo);
static void FB_RestorePalette(_Self(VideoDevice));
#ifdef VGA16_FBCON_SUPPORT
static void FB_VGA16Update(_Self(VideoDevice), int numrects, MIL_Rect *rects);
#endif

#ifdef VGA16_FBCON_SUPPORT
static Surface *FB_SetVGA16Mode(_Self(VideoDevice), Surface *cur,
				int width, int height, int bpp, Uint32 flags)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
    Surface* current = (Surface*)cur;

    /* TODO: add self invoke at last. */
#if 0
	/* Set the terminal into graphics mode */
	if ( FB_EnterGraphicsMode(self) < 0 ) {
		return(NULL);
	}
#endif

	/* Restore the original palette */
	FB_RestorePalette(self);

	/* Set the video mode and get the final screen format */
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGET_VSCREENINFO, &vinfo) < 0 ) {
		MIL_SetError("Couldn't get console screen info");
		return(NULL);
	}
	((FBconVideoDevice*)self)->hw_data->cache_vinfo = vinfo;
#ifdef FBCON_DEBUG
	fprintf(stderr, "Printing actual vinfo:\n");
	print_vinfo(&vinfo);
#endif
	if ( ! _vc5(current, reallocFormat, bpp, 0, 0, 0, 0) ) {
		return(NULL);
	}
	current->format->palette->ncolors = 16;

	/* Get the fixed information about the console hardware.
	   This is necessary since finfo.line_length changes.
	 */
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGET_FSCREENINFO, &finfo) < 0 ) {
		MIL_SetError("Couldn't get console hardware info");
		return(NULL);
	}
#ifdef FBCON_DEBUG
	fprintf(stderr, "Printing actual finfo:\n");
	print_finfo(&finfo);
#endif

	/* Save hardware palette, if needed */
	FB_SavePalette(self, &finfo, &vinfo);

	/* Set up the new mode framebuffer */
	current->flags = MIL_FULLSCREEN;
	current->w = vinfo.xres;
	current->h = vinfo.yres;
	current->pitch = current->w;
	current->pixels = MIL_malloc(current->h*current->pitch);

	/* Set the update rectangle function */
	_c(self)->updateRects = FB_VGA16Update;

	/* We're done */
	return(cur);
}
#endif /* VGA16_FBCON_SUPPORT */

static int FBconVideoDevice_X_videoInit(_Self(VideoDevice), PixelFormat *format)
{
	const int pagesize = MIL_getpagesize();
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	int i, j;
	int current_index;
	unsigned int current_w;
	unsigned int current_h;
	const char *MIL_fbdev;
	const char *rotation;
	FILE *modesdb;
    PixelFormat* vformat = (PixelFormat*)format;

	/* Initialize the library */
	MIL_fbdev = MIL_getenv("MIL_FBDEV");
	if ( MIL_fbdev == NULL ) {
		MIL_fbdev = "/dev/fb0";
	}
	((FBconVideoDevice*)self)->hw_data->console_fd = open(MIL_fbdev, O_RDWR, 0);
	if ( ((FBconVideoDevice*)self)->hw_data->console_fd < 0 ) {
		MIL_SetError("Unable to open %s", MIL_fbdev);
		return(-1);
	}

#if !MIL_THREADS_DISABLED
	/* Create the hardware surface lock mutex */
	((FBconVideoDevice*)self)->hw_data->hw_lock = MIL_CreateMutex();
	if ( ((FBconVideoDevice*)self)->hw_data->hw_lock == NULL ) {
		MIL_SetError("Unable to create lock mutex");
		_c(self)->videoQuit(self);
		return(-1);
	}
#endif

	/* Get the type of video hardware */
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGET_FSCREENINFO, &finfo) < 0 ) {
		MIL_SetError("Couldn't get console hardware info");
		_c(self)->videoQuit(self);
		return(-1);
	}
	switch (finfo.type) {
		case FB_TYPE_PACKED_PIXELS:
			/* Supported, no worries.. */
			break;
#ifdef VGA16_FBCON_SUPPORT
		case FB_TYPE_VGA_PLANES:
			/* VGA16 is supported, but that's it */
			if ( finfo.type_aux == FB_AUX_VGA_PLANES_VGA4 ) {
				if ( ioperm(0x3b4, 0x3df - 0x3b4 + 1, 1) < 0 ) {
					MIL_SetError("No I/O port permissions");
					_c(self)->videoQuit(self);
					return(-1);
				}
				_c(self)->setVideoMode = FB_SetVGA16Mode;
				break;
			}
			/* Fall through to unsupported case */
#endif /* VGA16_FBCON_SUPPORT */
		default:
			MIL_SetError("Unsupported console hardware");
			_c(self)->videoQuit(self);
			return(-1);
	}
	switch (finfo.visual) {
		case FB_VISUAL_TRUECOLOR:
		case FB_VISUAL_PSEUDOCOLOR:
		case FB_VISUAL_STATIC_PSEUDOCOLOR:
		case FB_VISUAL_DIRECTCOLOR:
			break;
		default:
			MIL_SetError("Unsupported console hardware");
			_c(self)->videoQuit(self);
			return(-1);
	}

	/* Check if the user wants to disable hardware acceleration */
	{ const char *fb_accel;
		fb_accel = MIL_getenv("MIL_FBACCEL");
		if ( fb_accel ) {
			finfo.accel = MIL_atoi(fb_accel);
		}
	}

	/* Memory map the device, compensating for buggy PPC mmap() */
	((FBconVideoDevice*)self)->hw_data->mapped_offset = (((long)finfo.smem_start) -
	                (((long)finfo.smem_start)&~(pagesize-1)));
	((FBconVideoDevice*)self)->hw_data->mapped_memlen = finfo.smem_len+((FBconVideoDevice*)self)->hw_data->mapped_offset;
	((FBconVideoDevice*)self)->hw_data->mapped_mem = do_mmap(NULL, ((FBconVideoDevice*)self)->hw_data->mapped_memlen,
	                  PROT_READ|PROT_WRITE, MAP_SHARED, ((FBconVideoDevice*)self)->hw_data->console_fd, 0);
	if ( ((FBconVideoDevice*)self)->hw_data->mapped_mem == (char *)-1 ) {
		MIL_SetError("Unable to memory map the video hardware");
		((FBconVideoDevice*)self)->hw_data->mapped_mem = NULL;
		_c(self)->videoQuit(self);
		return(-1);
	}

	/* Determine the current screen depth */
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGET_VSCREENINFO, &vinfo) < 0 ) {
		MIL_SetError("Couldn't get console pixel format");
		_c(self)->videoQuit(self);
		return(-1);
	}
	vformat->BitsPerPixel = vinfo.bits_per_pixel;
	if ( vformat->BitsPerPixel < 8 ) {
		/* Assuming VGA16, we handle self via a shadow framebuffer */
		vformat->BitsPerPixel = 8;
	}
	for ( i=0; i<vinfo.red.length; ++i ) {
		vformat->Rmask <<= 1;
		vformat->Rmask |= (0x00000001<<vinfo.red.offset);
	}
	for ( i=0; i<vinfo.green.length; ++i ) {
		vformat->Gmask <<= 1;
		vformat->Gmask |= (0x00000001<<vinfo.green.offset);
	}
	for ( i=0; i<vinfo.blue.length; ++i ) {
		vformat->Bmask <<= 1;
		vformat->Bmask |= (0x00000001<<vinfo.blue.offset);
	}
	((FBconVideoDevice*)self)->hw_data->saved_vinfo = vinfo;

	/* Save hardware palette, if needed */
	FB_SavePalette(self, &finfo, &vinfo);

	/* If the I/O registers are available, memory map them so we
	   can take advantage of any supported hardware acceleration.
	 */
	vinfo.accel_flags = 0;	/* Temporarily reserve registers */
	ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUT_VSCREENINFO, &vinfo);
	if ( finfo.accel && finfo.mmio_len ) {
		((FBconVideoDevice*)self)->hw_data->mapped_iolen = finfo.mmio_len;
		((FBconVideoDevice*)self)->hw_data->mapped_io = do_mmap(NULL, ((FBconVideoDevice*)self)->hw_data->mapped_iolen, PROT_READ|PROT_WRITE,
		                 MAP_SHARED, ((FBconVideoDevice*)self)->hw_data->console_fd, ((FBconVideoDevice*)self)->hw_data->mapped_memlen);
		if ( ((FBconVideoDevice*)self)->hw_data->mapped_io == (char *)-1 ) {
			/* Hmm, failed to memory map I/O registers */
			((FBconVideoDevice*)self)->hw_data->mapped_io = NULL;
		}
	}

	((FBconVideoDevice*)self)->hw_data->rotate = FBCON_ROTATE_NONE;
	rotation = MIL_getenv("MIL_VIDEO_FBCON_ROTATION");
	if (rotation != NULL) {
		if (MIL_strlen(rotation) == 0) {
			((FBconVideoDevice*)self)->hw_data->shadow_fb = 0;
			((FBconVideoDevice*)self)->hw_data->rotate = FBCON_ROTATE_NONE;
#ifdef FBCON_DEBUG
			printf("Not rotating, no shadow\n");
#endif
		} else if (!MIL_strcmp(rotation, "NONE")) {
			((FBconVideoDevice*)self)->hw_data->shadow_fb = 1;
			((FBconVideoDevice*)self)->hw_data->rotate = FBCON_ROTATE_NONE;
#ifdef FBCON_DEBUG
			printf("Not rotating, but still using shadow\n");
#endif
		} else if (!MIL_strcmp(rotation, "CW")) {
			((FBconVideoDevice*)self)->hw_data->shadow_fb = 1;
			((FBconVideoDevice*)self)->hw_data->rotate = FBCON_ROTATE_CW;
#ifdef FBCON_DEBUG
			printf("Rotating screen clockwise\n");
#endif
		} else if (!MIL_strcmp(rotation, "CCW")) {
			((FBconVideoDevice*)self)->hw_data->shadow_fb = 1;
			((FBconVideoDevice*)self)->hw_data->rotate = FBCON_ROTATE_CCW;
#ifdef FBCON_DEBUG
			printf("Rotating screen counter clockwise\n");
#endif
		} else if (!MIL_strcmp(rotation, "UD")) {
			((FBconVideoDevice*)self)->hw_data->shadow_fb = 1;
			((FBconVideoDevice*)self)->hw_data->rotate = FBCON_ROTATE_UD;
#ifdef FBCON_DEBUG
			printf("Rotating screen upside down\n");
#endif
		} else {
			MIL_SetError("\"%s\" is not a valid value for "
				 "MIL_VIDEO_FBCON_ROTATION", rotation);
			return(-1);
		}
	}

	if (((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_CW || ((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_CCW) {
		current_w = vinfo.yres;
		current_h = vinfo.xres;
	} else {
		current_w = vinfo.xres;
		current_h = vinfo.yres;
	}

	/* Query for the list of available video modes */
	current_index = ((vinfo.bits_per_pixel+7)/8)-1;
	modesdb = fopen(FB_MODES_DB, "r");
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		((FBconVideoDevice*)self)->hw_data->MIL_nummodes[i] = 0;
		((FBconVideoDevice*)self)->hw_data->MIL_modelist[i] = NULL;
	}
	if ( MIL_getenv("MIL_FB_BROKEN_MODES") != NULL ) {
		FB_AddMode(self, current_index, current_w, current_h, 0);
	} else if(modesdb) {
		while ( read_fbmodes_mode(modesdb, &vinfo) ) {
			for ( i=0; i<NUM_MODELISTS; ++i ) {
				unsigned int w, h;

				if (((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_CW || ((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_CCW) {
					w = vinfo.yres;
					h = vinfo.xres;
				} else {
					w = vinfo.xres;
					h = vinfo.yres;
				}
				/* See if we are querying for the current mode */
				if ( i == current_index ) {
					if ( (current_w > w) || (current_h > h) ) {
						/* Only check once */
						FB_AddMode(self, i, current_w, current_h, 0);
						current_index = -1;
					}
				}
				if ( FB_CheckMode(self, &vinfo, i, &w, &h) ) {
					FB_AddMode(self, i, w, h, 0);
				}
			}
		}
		fclose(modesdb);
		FB_SortModes(self);
	} else {
		for ( i=0; i<NUM_MODELISTS; ++i ) {
			for ( j=0; j<(sizeof(checkres)/sizeof(checkres[0])); ++j ) {
				unsigned int w, h;

				if (((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_CW || ((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_CCW) {
					w = checkres[j].h;
					h = checkres[j].w;
				} else {
					w = checkres[j].w;
					h = checkres[j].h;
				}
				/* See if we are querying for the current mode */
				if ( i == current_index ) {
					if ( (current_w > w) || (current_h > h) ) {
						/* Only check once */
						FB_AddMode(self, i, current_w, current_h, 0);
						current_index = -1;
					}
				}
				if ( FB_CheckMode(self, &vinfo, i, &w, &h) ) {
					FB_AddMode(self, i, w, h, 1);
				}
			}
		}
	}

	self->vinfo.current_w = current_w;
	self->vinfo.current_h = current_h;
	self->vinfo.wm_available = 0;
	self->vinfo.hw_available = !((FBconVideoDevice*)self)->hw_data->shadow_fb;
	self->vinfo.video_mem = ((FBconVideoDevice*)self)->hw_data->shadow_fb ? 0 : finfo.smem_len/1024;
	/* Fill in our hardware acceleration capabilities */
	if ( ((FBconVideoDevice*)self)->hw_data->mapped_io ) {
		switch (finfo.accel) {
		    case FB_ACCEL_MATROX_MGA2064W:
		    case FB_ACCEL_MATROX_MGA1064SG:
		    case FB_ACCEL_MATROX_MGA2164W:
		    case FB_ACCEL_MATROX_MGA2164W_AGP:
		    case FB_ACCEL_MATROX_MGAG100:
		    /*case FB_ACCEL_MATROX_MGAG200: G200 acceleration broken! */
		    case FB_ACCEL_MATROX_MGAG400:
#ifdef FBACCEL_DEBUG
			printf("Matrox hardware accelerator!\n");
#endif
            /* TODO: will supportted. */
#if 0
			FB_MatroxAccel(self, finfo.accel);
#endif
			break;
		    case FB_ACCEL_3DFX_BANSHEE:
#ifdef FBACCEL_DEBUG
			printf("3DFX hardware accelerator!\n");
#endif
            /* TODO: will supportted. */
#if 0
			FB_3DfxAccel(self, finfo.accel);
#endif
			break;
		    case FB_ACCEL_NV3:
		    case FB_ACCEL_NV4:
#ifdef FBACCEL_DEBUG
			printf("NVidia hardware accelerator!\n");
#endif
            /* TODO: will supportted. */
#if 0
			FB_RivaAccel(self, finfo.accel);
#endif
			break;
		    default:
#ifdef FBACCEL_DEBUG
			printf("Unknown hardware accelerator.\n");
#endif
			break;
		}
	}

	if (((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		((FBconVideoDevice*)self)->hw_data->shadow_mem = (char *)MIL_malloc(((FBconVideoDevice*)self)->hw_data->mapped_memlen);
		if (((FBconVideoDevice*)self)->hw_data->shadow_mem == NULL) {
			MIL_SetError("No memory for shadow");
			return (-1);
		} 
	}

	/* Enable mouse and keyboard support */
#if 0
	if ( FB_OpenKeyboard(self) < 0 ) {
		_c(self)->videoQuit(self);
		return(-1);
	}
	if ( FB_OpenMouse(self) < 0 ) {
		const char *sdl_nomouse;

		sdl_nomouse = MIL_getenv("MIL_NOMOUSE");
		if ( ! sdl_nomouse ) {
			MIL_SetError("Unable to open mouse");
			_c(self)->videoQuit(self);
			return(-1);
		}
	}
#endif

	/* We're done! */
	return(0);
}

static MIL_Rect** FBconVideoDevice_X_listModes(_Self(VideoDevice), 
        PixelFormat *format, Uint32 flags)
{
	return(((FBconVideoDevice*)self)->hw_data->MIL_modelist[((_vc0(format, getBitsPerPixel)+7)/8)-1]);
}

/* Various screen update functions available */
#if 0
static void FB_DirectUpdate(_Self(VideoDevice), int numrects, MIL_Rect *rects)
{
	int width = ((FBconVideoDevice*)self)->hw_data->cache_vinfo.xres;
	int height = ((FBconVideoDevice*)self)->hw_data->cache_vinfo.yres;
	int bytes_per_pixel = (((FBconVideoDevice*)self)->hw_data->cache_vinfo.bits_per_pixel + 7) / 8;
	int i;

	if (!((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		/* The application is already updating the visible video memory */
		return;
	}

	if (((FBconVideoDevice*)self)->hw_data->cache_vinfo.bits_per_pixel != 16) {
		MIL_SetError("Shadow copy only implemented for 16 bpp");
		return;
	}

	for (i = 0; i < numrects; i++) {
		int x1, y1, x2, y2;
		int scr_x1, scr_y1, scr_x2, scr_y2;
		int sha_x1, sha_y1;
		int shadow_right_delta;  /* Address change when moving right in dest */
		int shadow_down_delta;   /* Address change when moving down in dest */
		char *src_start;
		char *dst_start;

		x1 = rects[i].x; 
		y1 = rects[i].y;
		x2 = x1 + rects[i].w; 
		y2 = y1 + rects[i].h;

		if (x1 < 0) {
			x1 = 0;
		} else if (x1 > width) {
			x1 = width;
		}
		if (x2 < 0) {
			x2 = 0;
		} else if (x2 > width) {
			x2 = width;
		}
		if (y1 < 0) {
			y1 = 0;
		} else if (y1 > height) {
			y1 = height;
		}
		if (y2 < 0) {
			y2 = 0;
		} else if (y2 > height) {
			y2 = height;
		}
		if (x2 <= x1 || y2 <= y1) {
			continue;
		}

		switch (((FBconVideoDevice*)self)->hw_data->rotate) {
			case FBCON_ROTATE_NONE:
				sha_x1 = scr_x1 = x1;
				sha_y1 = scr_y1 = y1;
				scr_x2 = x2;
				scr_y2 = y2;
				shadow_right_delta = 1;
				shadow_down_delta = width;
				break;
			case FBCON_ROTATE_CCW:
				scr_x1 = y1;
				scr_y1 = width - x2;
				scr_x2 = y2;
				scr_y2 = width - x1;
				sha_x1 = x2 - 1;
				sha_y1 = y1;
				shadow_right_delta = width;
				shadow_down_delta = -1;
				break;
			case FBCON_ROTATE_UD:
				scr_x1 = width - x2;
				scr_y1 = height - y2;
				scr_x2 = width - x1;
				scr_y2 = height - y1;
				sha_x1 = x2 - 1;
				sha_y1 = y2 - 1;
				shadow_right_delta = -1;
				shadow_down_delta = -width;
				break;
			case FBCON_ROTATE_CW:
				scr_x1 = height - y2;
				scr_y1 = x1;
				scr_x2 = height - y1;
				scr_y2 = x2;
				sha_x1 = x1;
				sha_y1 = y2 - 1;
				shadow_right_delta = -width;
				shadow_down_delta = 1;
				break;
			default:
				MIL_SetError("Unknown rotation");
				return;
		}

		src_start = ((FBconVideoDevice*)self)->hw_data->shadow_mem +
			(sha_y1 * width + sha_x1) * bytes_per_pixel;
		dst_start = ((FBconVideoDevice*)self)->hw_data->mapped_mem + 
           ((FBconVideoDevice*)self)->hw_data->mapped_offset + scr_y1 * 
           ((FBconVideoDevice*)self)->hw_data->physlinebytes + 
			scr_x1 * bytes_per_pixel;

		((FBconVideoDevice*)self)->hw_data->blitFunc((Uint8 *) src_start,
				shadow_right_delta, 
				shadow_down_delta, 
				(Uint8 *) dst_start,
				((FBconVideoDevice*)self)->hw_data->physlinebytes,
				scr_x2 - scr_x1,
				scr_y2 - scr_y1);
	}
}
#endif

#ifdef FBCON_DEBUG
static void print_vinfo(struct fb_var_screeninfo *vinfo)
{
	fprintf(stderr, "Printing vinfo:\n");
	fprintf(stderr, "\txres: %d\n", vinfo->xres);
	fprintf(stderr, "\tyres: %d\n", vinfo->yres);
	fprintf(stderr, "\txres_virtual: %d\n", vinfo->xres_virtual);
	fprintf(stderr, "\tyres_virtual: %d\n", vinfo->yres_virtual);
	fprintf(stderr, "\txoffset: %d\n", vinfo->xoffset);
	fprintf(stderr, "\tyoffset: %d\n", vinfo->yoffset);
	fprintf(stderr, "\tbits_per_pixel: %d\n", vinfo->bits_per_pixel);
	fprintf(stderr, "\tgrayscale: %d\n", vinfo->grayscale);
	fprintf(stderr, "\tnonstd: %d\n", vinfo->nonstd);
	fprintf(stderr, "\tactivate: %d\n", vinfo->activate);
	fprintf(stderr, "\theight: %d\n", vinfo->height);
	fprintf(stderr, "\twidth: %d\n", vinfo->width);
	fprintf(stderr, "\taccel_flags: %d\n", vinfo->accel_flags);
	fprintf(stderr, "\tpixclock: %d\n", vinfo->pixclock);
	fprintf(stderr, "\tleft_margin: %d\n", vinfo->left_margin);
	fprintf(stderr, "\tright_margin: %d\n", vinfo->right_margin);
	fprintf(stderr, "\tupper_margin: %d\n", vinfo->upper_margin);
	fprintf(stderr, "\tlower_margin: %d\n", vinfo->lower_margin);
	fprintf(stderr, "\thsync_len: %d\n", vinfo->hsync_len);
	fprintf(stderr, "\tvsync_len: %d\n", vinfo->vsync_len);
	fprintf(stderr, "\tsync: %d\n", vinfo->sync);
	fprintf(stderr, "\tvmode: %d\n", vinfo->vmode);
	fprintf(stderr, "\tred: %d/%d\n", vinfo->red.length, vinfo->red.offset);
	fprintf(stderr, "\tgreen: %d/%d\n", vinfo->green.length, vinfo->green.offset);
	fprintf(stderr, "\tblue: %d/%d\n", vinfo->blue.length, vinfo->blue.offset);
	fprintf(stderr, "\talpha: %d/%d\n", vinfo->transp.length, vinfo->transp.offset);
}
static void print_finfo(struct fb_fix_screeninfo *finfo)
{
	fprintf(stderr, "Printing finfo:\n");
	fprintf(stderr, "\tsmem_start = %p\n", (char *)finfo->smem_start);
	fprintf(stderr, "\tsmem_len = %d\n", finfo->smem_len);
	fprintf(stderr, "\ttype = %d\n", finfo->type);
	fprintf(stderr, "\ttype_aux = %d\n", finfo->type_aux);
	fprintf(stderr, "\tvisual = %d\n", finfo->visual);
	fprintf(stderr, "\txpanstep = %d\n", finfo->xpanstep);
	fprintf(stderr, "\typanstep = %d\n", finfo->ypanstep);
	fprintf(stderr, "\tywrapstep = %d\n", finfo->ywrapstep);
	fprintf(stderr, "\tline_length = %d\n", finfo->line_length);
	fprintf(stderr, "\tmmio_start = %p\n", (char *)finfo->mmio_start);
	fprintf(stderr, "\tmmio_len = %d\n", finfo->mmio_len);
	fprintf(stderr, "\taccel = %d\n", finfo->accel);
}
#endif

static int choose_fbmodes_mode(struct fb_var_screeninfo *vinfo)
{
	int matched;
	FILE *modesdb;
	struct fb_var_screeninfo cinfo;

	matched = 0;
	modesdb = fopen(FB_MODES_DB, "r");
	if ( modesdb ) {
		/* Parse the mode definition file */
		while ( read_fbmodes_mode(modesdb, &cinfo) ) {
			if ( (vinfo->xres == cinfo.xres && vinfo->yres == cinfo.yres) &&
			     (!matched || (vinfo->bits_per_pixel == cinfo.bits_per_pixel)) ) {
				vinfo->pixclock = cinfo.pixclock;
				vinfo->left_margin = cinfo.left_margin;
				vinfo->right_margin = cinfo.right_margin;
				vinfo->upper_margin = cinfo.upper_margin;
				vinfo->lower_margin = cinfo.lower_margin;
				vinfo->hsync_len = cinfo.hsync_len;
				vinfo->vsync_len = cinfo.vsync_len;
				if ( matched ) {
					break;
				}
				matched = 1;
			}
		}
		fclose(modesdb);
	}
	return(matched);
}

static int choose_vesa_mode(struct fb_var_screeninfo *vinfo)
{
	int matched;
	int i;

	/* Check for VESA timings */
	matched = 0;
	for ( i=0; i<(sizeof(vesa_timings)/sizeof(vesa_timings[0])); ++i ) {
		if ( (vinfo->xres == vesa_timings[i].xres) &&
		     (vinfo->yres == vesa_timings[i].yres) ) {
#ifdef FBCON_DEBUG
			fprintf(stderr, "Using VESA timings for %dx%d\n",
						vinfo->xres, vinfo->yres);
#endif
			if ( vesa_timings[i].pixclock ) {
				vinfo->pixclock = vesa_timings[i].pixclock;
			}
			vinfo->left_margin = vesa_timings[i].left;
			vinfo->right_margin = vesa_timings[i].right;
			vinfo->upper_margin = vesa_timings[i].upper;
			vinfo->lower_margin = vesa_timings[i].lower;
			vinfo->hsync_len = vesa_timings[i].hslen;
			vinfo->vsync_len = vesa_timings[i].vslen;
			vinfo->sync = vesa_timings[i].sync;
			vinfo->vmode = vesa_timings[i].vmode;
			matched = 1;
			break;
		}
	}
	return(matched);
}

static void FB_FreeHWSurfaces(_Self(VideoDevice))
{
	vidmem_bucket *bucket, *freeable;

	bucket = ((FBconVideoDevice*)self)->hw_data->surfaces.next;
	while ( bucket ) {
		freeable = bucket;
		bucket = bucket->next;
		MIL_free(freeable);
	}
	((FBconVideoDevice*)self)->hw_data->surfaces.next = NULL;
}

static int FB_InitHWSurfaces(_Self(VideoDevice), Surface *screen, char *base, int size);
static Surface* FBconVideoDevice_X_setVideoMode(_Self(VideoDevice), 
        Surface *cur, int width, int height, int bpp, Uint32 flags)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	int i;
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	char *surfaces_mem;
	int surfaces_len;
    Surface* current = (Surface*)cur;

#if 0
	/* Set the terminal into graphics mode */
	if ( FB_EnterGraphicsMode(self) < 0 ) {
		return(NULL);
	}
#endif

	/* Restore the original palette */
	FB_RestorePalette(self);

	/* Set the video mode and get the final screen format */
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGET_VSCREENINFO, &vinfo) < 0 ) {
		MIL_SetError("Couldn't get console screen info");
		return(NULL);
	}
#ifdef FBCON_DEBUG
	fprintf(stderr, "Printing original vinfo:\n");
	print_vinfo(&vinfo);
#endif
	/* Do not use double buffering with shadow buffer */
	if (((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		flags &= ~MIL_DOUBLEBUF;
	}

	if ( (vinfo.xres != width) || (vinfo.yres != height) ||
	     (vinfo.bits_per_pixel != bpp) || (flags & MIL_DOUBLEBUF) ) {
		vinfo.activate = FB_ACTIVATE_NOW;
		vinfo.accel_flags = 0;
		vinfo.bits_per_pixel = bpp;
		vinfo.xres = width;
		vinfo.xres_virtual = width;
		vinfo.yres = height;
		if ( flags & MIL_DOUBLEBUF ) {
			vinfo.yres_virtual = height*2;
		} else {
			vinfo.yres_virtual = height;
		}
		vinfo.xoffset = 0;
		vinfo.yoffset = 0;
		vinfo.red.length = vinfo.red.offset = 0;
		vinfo.green.length = vinfo.green.offset = 0;
		vinfo.blue.length = vinfo.blue.offset = 0;
		vinfo.transp.length = vinfo.transp.offset = 0;
		if ( ! choose_fbmodes_mode(&vinfo) ) {
			choose_vesa_mode(&vinfo);
		}
#ifdef FBCON_DEBUG
		fprintf(stderr, "Printing wanted vinfo:\n");
		print_vinfo(&vinfo);
#endif
		if ( !((FBconVideoDevice*)self)->hw_data->shadow_fb &&
				ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUT_VSCREENINFO, &vinfo) < 0 ) {
			vinfo.yres_virtual = height;
			if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUT_VSCREENINFO, &vinfo) < 0 ) {
				MIL_SetError("Couldn't set console screen info");
				return(NULL);
			}
		}
	} else {
		int maxheight;

		/* Figure out how much video memory is available */
		if ( flags & MIL_DOUBLEBUF ) {
			maxheight = height*2;
		} else {
			maxheight = height;
		}
		if ( vinfo.yres_virtual > maxheight ) {
			vinfo.yres_virtual = maxheight;
		}
	}
	((FBconVideoDevice*)self)->hw_data->cache_vinfo = vinfo;
#ifdef FBCON_DEBUG
	fprintf(stderr, "Printing actual vinfo:\n");
	print_vinfo(&vinfo);
#endif
	Rmask = 0;
	for ( i=0; i<vinfo.red.length; ++i ) {
		Rmask <<= 1;
		Rmask |= (0x00000001<<vinfo.red.offset);
	}
	Gmask = 0;
	for ( i=0; i<vinfo.green.length; ++i ) {
		Gmask <<= 1;
		Gmask |= (0x00000001<<vinfo.green.offset);
	}
	Bmask = 0;
	for ( i=0; i<vinfo.blue.length; ++i ) {
		Bmask <<= 1;
		Bmask |= (0x00000001<<vinfo.blue.offset);
	} 
	if ( ! _vc5(current, reallocFormat, vinfo.bits_per_pixel,
	                                  Rmask, Gmask, Bmask, 0) ) {
		return(NULL);
	}

	/* Get the fixed information about the console hardware.
	   This is necessary since finfo.line_length changes.
	 */
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGET_FSCREENINFO, &finfo) < 0 ) {
		MIL_SetError("Couldn't get console hardware info");
		return(NULL);
	}

	/* Save hardware palette, if needed */
	FB_SavePalette(self, &finfo, &vinfo);

	if (((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		if (vinfo.bits_per_pixel == 16) {
			((FBconVideoDevice*)self)->hw_data->blitFunc = (((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_NONE ||
					((FBconVideoDevice*)self)->hw_data->rotate == FBCON_ROTATE_UD) ?
				FB_blit16 : FB_blit16blocked;
		} else {
#ifdef FBCON_DEBUG
			fprintf(stderr, "Init vinfo:\n");
			print_vinfo(&vinfo);
#endif
			MIL_SetError("Using software buffer, but no blitter "
					"function is available for %d bpp.",
					vinfo.bits_per_pixel);
			return(NULL);
		}
	}

	/* Set up the new mode framebuffer */
	current->flags &= MIL_FULLSCREEN;
	if (((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		current->flags |= MIL_SWSURFACE;
	} else {
		current->flags |= MIL_HWSURFACE;
	}
	current->w = vinfo.xres;
	current->h = vinfo.yres;
	if (((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		current->pitch = current->w * ((vinfo.bits_per_pixel + 7) / 8);
		current->pixels = ((FBconVideoDevice*)self)->hw_data->shadow_mem;
		((FBconVideoDevice*)self)->hw_data->physlinebytes = finfo.line_length;
	} else {
		current->pitch = finfo.line_length;
		current->pixels = ((FBconVideoDevice*)self)->hw_data->mapped_mem+((FBconVideoDevice*)self)->hw_data->mapped_offset;
	}

	/* Set up the information for hardware surfaces */
	surfaces_mem = (char *)current->pixels +
		vinfo.yres_virtual*current->pitch;
	surfaces_len = (((FBconVideoDevice*)self)->hw_data->shadow_fb) ?
		0 : (((FBconVideoDevice*)self)->hw_data->mapped_memlen-(surfaces_mem-((FBconVideoDevice*)self)->hw_data->mapped_mem));

	FB_FreeHWSurfaces(self);
	FB_InitHWSurfaces(self, cur, surfaces_mem, surfaces_len);

	/* Let the application know we have a hardware palette */
	switch (finfo.visual) {
		case FB_VISUAL_PSEUDOCOLOR:
		current->flags |= MIL_HWPALETTE;
		break;
		default:
		break;
	}

	/* Update for double-buffering, if we can */
	if ( flags & MIL_DOUBLEBUF ) {
		if ( vinfo.yres_virtual == (height*2) ) {
			current->flags |= MIL_DOUBLEBUF;
			((FBconVideoDevice*)self)->hw_data->flip_page = 0;
			((FBconVideoDevice*)self)->hw_data->flip_address[0] = (char *)current->pixels;
			((FBconVideoDevice*)self)->hw_data->flip_address[1] = (char *)current->pixels+
				current->h*current->pitch;
			((FBconVideoDevice*)self)->screen = current;
			FB_FlipHWSurface(self, current);
			((FBconVideoDevice*)self)->screen = NULL;
		}
	}

	/* Set the update rectangle function */
/* 	_c(self)->updateRects = FB_DirectUpdate;
 */

	/* We're done */
	return(cur);
}

#ifdef FBCON_DEBUG
void FB_DumpHWSurfaces(_Self(VideoDevice))
{
	vidmem_bucket *bucket;

	printf("Memory left: %d (%d total)\n", ((FBconVideoDevice*)self)->hw_data->surfaces_memleft, ((FBconVideoDevice*)self)->hw_data->surfaces_memtotal);
	printf("\n");
	printf("         Base  Size\n");
	for ( bucket=&(((FBconVideoDevice*)self)->hw_data->surfaces); bucket; bucket=bucket->next ) {
		printf("Bucket:  %p, %d (%s)\n", bucket->base, bucket->size, bucket->used ? "used" : "free");
		if ( bucket->prev ) {
			if ( bucket->base != bucket->prev->base+bucket->prev->size ) {
				printf("Warning, corrupt bucket list! (prev)\n");
			}
		} else {
			if ( bucket != &(((FBconVideoDevice*)self)->hw_data->surfaces) ) {
				printf("Warning, corrupt bucket list! (!prev)\n");
			}
		}
		if ( bucket->next ) {
			if ( bucket->next->base != bucket->base+bucket->size ) {
				printf("Warning, corrupt bucket list! (next)\n");
			}
		}
	}
	printf("\n");
}
#endif

static int FB_InitHWSurfaces(_Self(VideoDevice), Surface *screen, char *base, int size)
{
	vidmem_bucket *bucket;

	((FBconVideoDevice*)self)->hw_data->surfaces_memtotal = size;
	((FBconVideoDevice*)self)->hw_data->surfaces_memleft = size;

	if ( ((FBconVideoDevice*)self)->hw_data->surfaces_memleft > 0 ) {
		bucket = (vidmem_bucket *)MIL_malloc(sizeof(*bucket));
		if ( bucket == NULL ) {
			/*MIL_OutOfMemory();*/
			return(-1);
		}
		bucket->prev = &(((FBconVideoDevice*)self)->hw_data->surfaces);
		bucket->used = 0;
		bucket->dirty = 0;
		bucket->base = base;
		bucket->size = size;
		bucket->next = NULL;
	} else {
		bucket = NULL;
	}

	((FBconVideoDevice*)self)->hw_data->surfaces.prev = NULL;
	((FBconVideoDevice*)self)->hw_data->surfaces.used = 1;
	((FBconVideoDevice*)self)->hw_data->surfaces.dirty = 0;
	((FBconVideoDevice*)self)->hw_data->surfaces.base = ((Surface*)screen)->pixels;
	((FBconVideoDevice*)self)->hw_data->surfaces.size = (unsigned int)((long)base - (long)((FBconVideoDevice*)self)->hw_data->surfaces.base);
	((FBconVideoDevice*)self)->hw_data->surfaces.next = bucket;
	((Surface*)screen)->hwdata = (struct private_hwdata *)&(((FBconVideoDevice*)self)->hw_data->surfaces);
	return(0);
}


static int FBconVideoDevice_X_allocHWSurface(_Self(VideoDevice), Surface *sur)
{
	vidmem_bucket *bucket;
	int size;
	int extra;
    Surface* surface = (Surface*)sur;
    Surface* screen = ((FBconVideoDevice*)ACT_VIDEO_DEVICE)->screen;

    /* Temporarily, we only allow surfaces the same width as display.
       Some blitters require the pitch between two hardware surfaces
       to be the same.  Others have interesting alignment restrictions.
       Until someone who knows these details looks at the code...
       */
    if ( surface->pitch > _c(screen)->getPitch((Surface*)screen) ) {
        MIL_SetError("Surface requested wider than screen");
        return(-1);
    }
    surface->pitch = _c(screen)->getPitch((Surface*)screen);
    size = surface->h * surface->pitch;
#ifdef FBCON_DEBUG
    fprintf(stderr, "Allocating bucket of %d bytes\n", size);
#endif

	/* Quick check for available mem */
	if ( size > ((FBconVideoDevice*)self)->hw_data->surfaces_memleft ) {
		MIL_SetError("Not enough video memory");
		return(-1);
	}

	/* Search for an empty bucket big enough */
	for ( bucket=&((FBconVideoDevice*)self)->hw_data->surfaces; 
             bucket; bucket=bucket->next ) {
		if ( ! bucket->used && (size <= bucket->size) ) {
			break;
		}
	}
	if ( bucket == NULL ) {
		MIL_SetError("Video memory too fragmented");
		return(-1);
	}

	/* Create a new bucket for left-over memory */
	extra = (bucket->size - size);
	if ( extra ) {
		vidmem_bucket *newbucket;

#ifdef FBCON_DEBUG
	fprintf(stderr, "Adding new free bucket of %d bytes\n", extra);
#endif
		newbucket = (vidmem_bucket *)MIL_malloc(sizeof(*newbucket));
		if ( newbucket == NULL ) {
			/*MIL_OutOfMemory();*/
			return(-1);
		}
		newbucket->prev = bucket;
		newbucket->used = 0;
		newbucket->base = bucket->base+size;
		newbucket->size = extra;
		newbucket->next = bucket->next;
		if ( bucket->next ) {
			bucket->next->prev = newbucket;
		}
		bucket->next = newbucket;
	}

	/* Set the current bucket values and return it! */
	bucket->used = 1;
	bucket->size = size;
	bucket->dirty = 0;
#ifdef FBCON_DEBUG
	fprintf(stderr, "Allocated %d bytes at %p\n", bucket->size, bucket->base);
#endif
	((FBconVideoDevice*)self)->hw_data->surfaces_memleft -= size;
	surface->flags |= MIL_HWSURFACE;
	surface->pixels = bucket->base;
	surface->hwdata = (struct private_hwdata *)bucket;
	return(0);
}
static void FBconVideoDevice_X_freeHWSurface(_Self(VideoDevice), Surface *sur)
{
	vidmem_bucket *bucket, *freeable;
    Surface* surface = (Surface*)sur;

	/* Look for the bucket in the current list */
	for ( bucket=&(((FBconVideoDevice*)self)->hw_data->surfaces); 
            bucket; bucket=bucket->next ) {
		if ( bucket == (vidmem_bucket *)surface->hwdata ) {
			break;
		}
	}
	if ( bucket && bucket->used ) {
		/* Add the memory back to the total */
#ifdef DGA_DEBUG
	printf("Freeing bucket of %d bytes\n", bucket->size);
#endif
		((FBconVideoDevice*)self)->hw_data->surfaces_memleft += bucket->size;

		/* Can we merge the space with surrounding buckets? */
		bucket->used = 0;
		if ( bucket->next && ! bucket->next->used ) {
#ifdef DGA_DEBUG
	printf("Merging with next bucket, for %d total bytes\n", bucket->size+bucket->next->size);
#endif
			freeable = bucket->next;
			bucket->size += bucket->next->size;
			bucket->next = bucket->next->next;
			if ( bucket->next ) {
				bucket->next->prev = bucket;
			}
			MIL_free(freeable);
		}
		if ( bucket->prev && ! bucket->prev->used ) {
#ifdef DGA_DEBUG
	printf("Merging with previous bucket, for %d total bytes\n", bucket->prev->size+bucket->size);
#endif
			freeable = bucket;
			bucket->prev->size += bucket->size;
			bucket->prev->next = bucket->next;
			if ( bucket->next ) {
				bucket->next->prev = bucket->prev;
			}
			MIL_free(freeable);
		}
	}
	surface->pixels = NULL;
	surface->hwdata = NULL;
}

static int FBconVideoDevice_X_lockHWSurface(_Self(VideoDevice), Surface *sur)
{
    Surface* surface = (Surface*)sur;
	if ( ((FBconVideoDevice*)self)->hw_data->switched_away ) {
		return -2; /* no hardware access */
	}
	if ( surface == ((FBconVideoDevice*)self)->screen ) {
        MIL_mutex* mutex = ((FBconVideoDevice*)self)->hw_data->hw_lock;
		_c(mutex)->lock(mutex);
		if ( FB_IsSurfaceBusy(surface) ) {
			FB_WaitBusySurfaces(self);
		}
	} else {
		if ( FB_IsSurfaceBusy(surface) ) {
			FB_WaitBusySurfaces(self);
		}
	}
	return(0);
}

static void FBconVideoDevice_X_unlockHWSurface(_Self(VideoDevice), Surface* surface)
{
	if ( (Surface*)surface == ((FBconVideoDevice*)self)->screen ) {
        MIL_mutex* mutex = ((FBconVideoDevice*)self)->hw_data->hw_lock;
		_c(mutex)->unlock(mutex);
	}
}

static int FBconVideoDevice_X_flipHWSurface(_Self(VideoDevice), Surface *sur)
{
    Surface* surface = (Surface*)sur;

	if ( ((FBconVideoDevice*)self)->hw_data->switched_away ) {
		return -2; /* no hardware access */
	}

	/* Wait for vertical retrace and then flip display */
	((FBconVideoDevice*)self)->hw_data->cache_vinfo.yoffset = 
        ((FBconVideoDevice*)self)->hw_data->flip_page * surface->h;
	if ( FB_IsSurfaceBusy(((FBconVideoDevice*)self)->screen) ) {
		FB_WaitBusySurfaces(self);
	}
	((FBconVideoDevice*)self)->hw_data->wait_vbl(self);
	if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPAN_DISPLAY, &(((FBconVideoDevice*)self)->hw_data->cache_vinfo)) < 0 ) {
		MIL_SetError("ioctl(FBIOPAN_DISPLAY) failed");
		return(-1);
	}
	((FBconVideoDevice*)self)->hw_data->flip_page = !((FBconVideoDevice*)self)->hw_data->flip_page;

	surface->pixels = ((FBconVideoDevice*)self)->hw_data->flip_address[((FBconVideoDevice*)self)->hw_data->flip_page];
	return(0);
}

static void FB_blit16(Uint8 *byte_src_pos, int src_right_delta, int src_down_delta,
		Uint8 *byte_dst_pos, int dst_linebytes, int width, int height)
{
	int w;
	Uint16 *src_pos = (Uint16 *)byte_src_pos;
	Uint16 *dst_pos = (Uint16 *)byte_dst_pos;

	while (height) {
		Uint16 *src = src_pos;
		Uint16 *dst = dst_pos;
		for (w = width; w != 0; w--) {
			*dst = *src;
			src += src_right_delta;
			dst++;
		}
		dst_pos = (Uint16 *)((Uint8 *)dst_pos + dst_linebytes);
		src_pos += src_down_delta;
		height--;
	}
}

#define BLOCKSIZE_W 32
#define BLOCKSIZE_H 32

static void FB_blit16blocked(Uint8 *byte_src_pos, int src_right_delta, int src_down_delta, 
		Uint8 *byte_dst_pos, int dst_linebytes, int width, int height)
{
	int w;
	Uint16 *src_pos = (Uint16 *)byte_src_pos;
	Uint16 *dst_pos = (Uint16 *)byte_dst_pos;

	while (height > 0) {
		Uint16 *src = src_pos;
		Uint16 *dst = dst_pos;
		for (w = width; w > 0; w -= BLOCKSIZE_W) {
			FB_blit16((Uint8 *)src,
					src_right_delta,
					src_down_delta,
					(Uint8 *)dst,
					dst_linebytes,
					MIL_min(w, BLOCKSIZE_W),
					MIL_min(height, BLOCKSIZE_H));
			src += src_right_delta * BLOCKSIZE_W;
			dst += BLOCKSIZE_W;
		}
		dst_pos = (Uint16 *)((Uint8 *)dst_pos + dst_linebytes * BLOCKSIZE_H);
		src_pos += src_down_delta * BLOCKSIZE_H;
		height -= BLOCKSIZE_H;
	}
}

static void FBconVideoDevice_X_updateRects(_Self(VideoDevice), int numrects, 
        MIL_Rect *rects)
{
	int width = ((FBconVideoDevice*)self)->hw_data->cache_vinfo.xres;
	int height = ((FBconVideoDevice*)self)->hw_data->cache_vinfo.yres;
	int bytes_per_pixel = (((FBconVideoDevice*)self)->hw_data->cache_vinfo.bits_per_pixel + 7) / 8;
	int i;

	if (!((FBconVideoDevice*)self)->hw_data->shadow_fb) {
		/* The application is already updating the visible video memory */
		return;
	}

	if (((FBconVideoDevice*)self)->hw_data->cache_vinfo.bits_per_pixel != 16) {
		MIL_SetError("Shadow copy only implemented for 16 bpp");
		return;
	}

	for (i = 0; i < numrects; i++) {
		int x1, y1, x2, y2;
		int scr_x1, scr_y1, scr_x2, scr_y2;
		int sha_x1, sha_y1;
		int shadow_right_delta;  /* Address change when moving right in dest */
		int shadow_down_delta;   /* Address change when moving down in dest */
		char *src_start;
		char *dst_start;

		x1 = rects[i].x; 
		y1 = rects[i].y;
		x2 = x1 + rects[i].w; 
		y2 = y1 + rects[i].h;

		if (x1 < 0) {
			x1 = 0;
		} else if (x1 > width) {
			x1 = width;
		}
		if (x2 < 0) {
			x2 = 0;
		} else if (x2 > width) {
			x2 = width;
		}
		if (y1 < 0) {
			y1 = 0;
		} else if (y1 > height) {
			y1 = height;
		}
		if (y2 < 0) {
			y2 = 0;
		} else if (y2 > height) {
			y2 = height;
		}
		if (x2 <= x1 || y2 <= y1) {
			continue;
		}

		switch (((FBconVideoDevice*)self)->hw_data->rotate) {
			case FBCON_ROTATE_NONE:
				sha_x1 = scr_x1 = x1;
				sha_y1 = scr_y1 = y1;
				scr_x2 = x2;
				scr_y2 = y2;
				shadow_right_delta = 1;
				shadow_down_delta = width;
				break;
			case FBCON_ROTATE_CCW:
				scr_x1 = y1;
				scr_y1 = width - x2;
				scr_x2 = y2;
				scr_y2 = width - x1;
				sha_x1 = x2 - 1;
				sha_y1 = y1;
				shadow_right_delta = width;
				shadow_down_delta = -1;
				break;
			case FBCON_ROTATE_UD:
				scr_x1 = width - x2;
				scr_y1 = height - y2;
				scr_x2 = width - x1;
				scr_y2 = height - y1;
				sha_x1 = x2 - 1;
				sha_y1 = y2 - 1;
				shadow_right_delta = -1;
				shadow_down_delta = -width;
				break;
			case FBCON_ROTATE_CW:
				scr_x1 = height - y2;
				scr_y1 = x1;
				scr_x2 = height - y1;
				scr_y2 = x2;
				sha_x1 = x1;
				sha_y1 = y2 - 1;
				shadow_right_delta = -width;
				shadow_down_delta = 1;
				break;
			default:
				MIL_SetError("Unknown rotation");
				return;
		}

		src_start = ((FBconVideoDevice*)self)->hw_data->shadow_mem +
			(sha_y1 * width + sha_x1) * bytes_per_pixel;
		dst_start = ((FBconVideoDevice*)self)->hw_data->mapped_mem + ((FBconVideoDevice*)self)->hw_data->mapped_offset + scr_y1 * ((FBconVideoDevice*)self)->hw_data->physlinebytes + 
			scr_x1 * bytes_per_pixel;

		((FBconVideoDevice*)self)->hw_data->blitFunc((Uint8 *) src_start,
				shadow_right_delta, 
				shadow_down_delta, 
				(Uint8 *) dst_start,
				((FBconVideoDevice*)self)->hw_data->physlinebytes,
				scr_x2 - scr_x1,
				scr_y2 - scr_y1);
	}
}

#ifdef VGA16_FBCON_SUPPORT
/* Code adapted with thanks from the XFree86 VGA16 driver! :) */
#define writeGr(index, value) \
outb(index, 0x3CE);           \
outb(value, 0x3CF);
#define writeSeq(index, value) \
outb(index, 0x3C4);            \
outb(value, 0x3C5);

static void FB_VGA16Update(_Self(VideoDevice), int numrects, MIL_Rect *rects)
{
    Surface *screen = NULL;
    int width, height, FBPitch, left, i, j, SRCPitch, phase;
    register Uint32 m;
    Uint8  s1, s2, s3, s4;
    Uint32 *src, *srcPtr;
    Uint8  *dst, *dstPtr;

    if ( ((FBconVideoDevice*)self)->hw_data->switched_away ) {
        return; /* no hardware access */
    }

    screen = (Surface*)(((FBconVideoDevice*)self)->screen);
    FBPitch = screen->w >> 3;
    SRCPitch = screen->pitch >> 2;

    writeGr(0x03, 0x00);
    writeGr(0x05, 0x00);
    writeGr(0x01, 0x00);
    writeGr(0x08, 0xFF);

    while(numrects--) {
	left = rects->x & ~7;
        width = (rects->w + 7) >> 3;
        height = rects->h;
        src = (Uint32*)screen->pixels + (rects->y * SRCPitch) + (left >> 2); 
        dst = (Uint8*)((FBconVideoDevice*)self)->hw_data->mapped_mem + (rects->y * FBPitch) + (left >> 3);

	if((phase = (long)dst & 3L)) {
	    phase = 4 - phase;
	    if(phase > width) phase = width;
	    width -= phase;
	}

        while(height--) {
	    writeSeq(0x02, 1 << 0);
	    dstPtr = dst;
	    srcPtr = src;
	    i = width;
	    j = phase;
	    while(j--) {
		m = (srcPtr[1] & 0x01010101) | ((srcPtr[0] & 0x01010101) << 4);
 		*dstPtr++ = (m >> 24) | (m >> 15) | (m >> 6) | (m << 3);
		srcPtr += 2;
	    }
	    while(i >= 4) {
		m = (srcPtr[1] & 0x01010101) | ((srcPtr[0] & 0x01010101) << 4);
 		s1 = (m >> 24) | (m >> 15) | (m >> 6) | (m << 3);
		m = (srcPtr[3] & 0x01010101) | ((srcPtr[2] & 0x01010101) << 4);
 		s2 = (m >> 24) | (m >> 15) | (m >> 6) | (m << 3);
		m = (srcPtr[5] & 0x01010101) | ((srcPtr[4] & 0x01010101) << 4);
 		s3 = (m >> 24) | (m >> 15) | (m >> 6) | (m << 3);
		m = (srcPtr[7] & 0x01010101) | ((srcPtr[6] & 0x01010101) << 4);
 		s4 = (m >> 24) | (m >> 15) | (m >> 6) | (m << 3);
		*((Uint32*)dstPtr) = s1 | (s2 << 8) | (s3 << 16) | (s4 << 24);
		srcPtr += 8;
		dstPtr += 4;
		i -= 4;
	    }
	    while(i--) {
		m = (srcPtr[1] & 0x01010101) | ((srcPtr[0] & 0x01010101) << 4);
 		*dstPtr++ = (m >> 24) | (m >> 15) | (m >> 6) | (m << 3);
		srcPtr += 2;
	    }

	    writeSeq(0x02, 1 << 1);
	    dstPtr = dst;
	    srcPtr = src;
	    i = width;
	    j = phase;
	    while(j--) {
		m = (srcPtr[1] & 0x02020202) | ((srcPtr[0] & 0x02020202) << 4);
 		*dstPtr++ = (m >> 25) | (m >> 16) | (m >> 7) | (m << 2);
		srcPtr += 2;
	    }
	    while(i >= 4) {
		m = (srcPtr[1] & 0x02020202) | ((srcPtr[0] & 0x02020202) << 4);
 		s1 = (m >> 25) | (m >> 16) | (m >> 7) | (m << 2);
		m = (srcPtr[3] & 0x02020202) | ((srcPtr[2] & 0x02020202) << 4);
 		s2 = (m >> 25) | (m >> 16) | (m >> 7) | (m << 2);
		m = (srcPtr[5] & 0x02020202) | ((srcPtr[4] & 0x02020202) << 4);
 		s3 = (m >> 25) | (m >> 16) | (m >> 7) | (m << 2);
		m = (srcPtr[7] & 0x02020202) | ((srcPtr[6] & 0x02020202) << 4);
 		s4 = (m >> 25) | (m >> 16) | (m >> 7) | (m << 2);
		*((Uint32*)dstPtr) = s1 | (s2 << 8) | (s3 << 16) | (s4 << 24);
		srcPtr += 8;
		dstPtr += 4;
		i -= 4;
	    }
	    while(i--) {
		m = (srcPtr[1] & 0x02020202) | ((srcPtr[0] & 0x02020202) << 4);
 		*dstPtr++ = (m >> 25) | (m >> 16) | (m >> 7) | (m << 2);
		srcPtr += 2;
	    }

	    writeSeq(0x02, 1 << 2);
	    dstPtr = dst;
	    srcPtr = src;
	    i = width;
	    j = phase;
	    while(j--) {
		m = (srcPtr[1] & 0x04040404) | ((srcPtr[0] & 0x04040404) << 4);
 		*dstPtr++ = (m >> 26) | (m >> 17) | (m >> 8) | (m << 1);
		srcPtr += 2;
	    }
	    while(i >= 4) {
		m = (srcPtr[1] & 0x04040404) | ((srcPtr[0] & 0x04040404) << 4);
 		s1 = (m >> 26) | (m >> 17) | (m >> 8) | (m << 1);
		m = (srcPtr[3] & 0x04040404) | ((srcPtr[2] & 0x04040404) << 4);
 		s2 = (m >> 26) | (m >> 17) | (m >> 8) | (m << 1);
		m = (srcPtr[5] & 0x04040404) | ((srcPtr[4] & 0x04040404) << 4);
 		s3 = (m >> 26) | (m >> 17) | (m >> 8) | (m << 1);
		m = (srcPtr[7] & 0x04040404) | ((srcPtr[6] & 0x04040404) << 4);
 		s4 = (m >> 26) | (m >> 17) | (m >> 8) | (m << 1);
		*((Uint32*)dstPtr) = s1 | (s2 << 8) | (s3 << 16) | (s4 << 24);
		srcPtr += 8;
		dstPtr += 4;
		i -= 4;
	    }
	    while(i--) {
		m = (srcPtr[1] & 0x04040404) | ((srcPtr[0] & 0x04040404) << 4);
 		*dstPtr++ = (m >> 26) | (m >> 17) | (m >> 8) | (m << 1);
		srcPtr += 2;
	    }
	    
	    writeSeq(0x02, 1 << 3);
	    dstPtr = dst;
	    srcPtr = src;
	    i = width;
	    j = phase;
	    while(j--) {
		m = (srcPtr[1] & 0x08080808) | ((srcPtr[0] & 0x08080808) << 4);
 		*dstPtr++ = (m >> 27) | (m >> 18) | (m >> 9) | m;
		srcPtr += 2;
	    }
	    while(i >= 4) {
		m = (srcPtr[1] & 0x08080808) | ((srcPtr[0] & 0x08080808) << 4);
 		s1 = (m >> 27) | (m >> 18) | (m >> 9) | m;
		m = (srcPtr[3] & 0x08080808) | ((srcPtr[2] & 0x08080808) << 4);
 		s2 = (m >> 27) | (m >> 18) | (m >> 9) | m;
		m = (srcPtr[5] & 0x08080808) | ((srcPtr[4] & 0x08080808) << 4);
 		s3 = (m >> 27) | (m >> 18) | (m >> 9) | m;
		m = (srcPtr[7] & 0x08080808) | ((srcPtr[6] & 0x08080808) << 4);
 		s4 = (m >> 27) | (m >> 18) | (m >> 9) | m;
		*((Uint32*)dstPtr) = s1 | (s2 << 8) | (s3 << 16) | (s4 << 24);
		srcPtr += 8;
		dstPtr += 4;
		i -= 4;
	    }
	    while(i--) {
		m = (srcPtr[1] & 0x08080808) | ((srcPtr[0] & 0x08080808) << 4);
 		*dstPtr++ = (m >> 27) | (m >> 18) | (m >> 9) | m;
		srcPtr += 2;
	    }

            dst += FBPitch;
            src += SRCPitch;
        }
        rects++;
    }
}
#endif /* VGA16_FBCON_SUPPORT */

void FB_SavePaletteTo(_Self(VideoDevice), int palette_len, __u16 *area)
{
	struct fb_cmap cmap;

	cmap.start = 0;
	cmap.len = palette_len;
	cmap.red = &area[0*palette_len];
	cmap.green = &area[1*palette_len];
	cmap.blue = &area[2*palette_len];
	cmap.transp = NULL;
	ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGETCMAP, &cmap);
}

void FB_RestorePaletteFrom(_Self(VideoDevice), int palette_len, __u16 *area)
{
	struct fb_cmap cmap;

	cmap.start = 0;
	cmap.len = palette_len;
	cmap.red = &area[0*palette_len];
	cmap.green = &area[1*palette_len];
	cmap.blue = &area[2*palette_len];
	cmap.transp = NULL;
	ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUTCMAP, &cmap);
}

static void FB_SavePalette(_Self(VideoDevice), struct fb_fix_screeninfo *finfo,
                                  struct fb_var_screeninfo *vinfo)
{
	int i;

	/* Save hardware palette, if needed */
	if ( finfo->visual == FB_VISUAL_PSEUDOCOLOR ) {
		((FBconVideoDevice*)self)->hw_data->saved_cmaplen = 1<<vinfo->bits_per_pixel;
		((FBconVideoDevice*)self)->hw_data->saved_cmap=(__u16 *)MIL_malloc(3*((FBconVideoDevice*)self)->hw_data->saved_cmaplen*sizeof(*((FBconVideoDevice*)self)->hw_data->saved_cmap));
		if ( ((FBconVideoDevice*)self)->hw_data->saved_cmap != NULL ) {
			FB_SavePaletteTo(self, ((FBconVideoDevice*)self)->hw_data->saved_cmaplen, ((FBconVideoDevice*)self)->hw_data->saved_cmap);
		}
	}

	/* Added support for FB_VISUAL_DIRECTCOLOR.
	   With self mode pixel information is passed through the palette...
	   Neat fading and gamma correction effects can be had by simply
	   fooling around with the palette instead of changing the pixel
	   values themselves... Very neat!

	   Adam Meyerowitz 1/19/2000
	   ameyerow@optonline.com
	*/
	if ( finfo->visual == FB_VISUAL_DIRECTCOLOR ) {
		__u16 new_entries[3*256];

		/* Save the colormap */
		((FBconVideoDevice*)self)->hw_data->saved_cmaplen = 256;
		((FBconVideoDevice*)self)->hw_data->saved_cmap=(__u16 *)MIL_malloc(3*((FBconVideoDevice*)self)->hw_data->saved_cmaplen*sizeof(*((FBconVideoDevice*)self)->hw_data->saved_cmap));
		if ( ((FBconVideoDevice*)self)->hw_data->saved_cmap != NULL ) {
			FB_SavePaletteTo(self, ((FBconVideoDevice*)self)->hw_data->saved_cmaplen, ((FBconVideoDevice*)self)->hw_data->saved_cmap);
		}

		/* Allocate new identity colormap */
		for ( i=0; i<256; ++i ) {
	      		new_entries[(0*256)+i] =
			new_entries[(1*256)+i] =
			new_entries[(2*256)+i] = (i<<8)|i;
		}
		FB_RestorePaletteFrom(self, 256, new_entries);
	}
}

static void FB_RestorePalette(_Self(VideoDevice))
{
	/* Restore the original palette */
	if ( ((FBconVideoDevice*)self)->hw_data->saved_cmap ) {
		FB_RestorePaletteFrom(self, ((FBconVideoDevice*)self)->hw_data->saved_cmaplen, ((FBconVideoDevice*)self)->hw_data->saved_cmap);
		MIL_free(((FBconVideoDevice*)self)->hw_data->saved_cmap);
		((FBconVideoDevice*)self)->hw_data->saved_cmap = NULL;
	}
}

static int FBconVideoDevice_X_setColors(_Self(VideoDevice), int firstcolor, 
        int ncolors, MIL_Color *colors)
{
	int i;
	__u16 r[256];
	__u16 g[256];
	__u16 b[256];
	struct fb_cmap cmap;

	/* Set up the colormap */
	for (i = 0; i < ncolors; i++) {
		r[i] = colors[i].r << 8;
		g[i] = colors[i].g << 8;
		b[i] = colors[i].b << 8;
	}
	cmap.start = firstcolor;
	cmap.len = ncolors;
	cmap.red = r;
	cmap.green = g;
	cmap.blue = b;
	cmap.transp = NULL;

	if( (ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUTCMAP, &cmap) < 0) ||
	    !(((FBconVideoDevice*)self)->screen->flags & MIL_HWPALETTE) ) {
	        colors = ((FBconVideoDevice*)self)->screen->format->palette->colors;
		ncolors = ((FBconVideoDevice*)self)->screen->format->palette->ncolors;
		cmap.start = 0;
		cmap.len = ncolors;
		MIL_memset(r, 0, sizeof(r));
		MIL_memset(g, 0, sizeof(g));
		MIL_memset(b, 0, sizeof(b));
		if ( ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOGETCMAP, &cmap) == 0 ) {
			for ( i=ncolors-1; i>=0; --i ) {
				colors[i].r = (r[i]>>8);
				colors[i].g = (g[i]>>8);
				colors[i].b = (b[i]>>8);
			}
		}
		return(0);
	}
	return(1);
}

int FB_InGraphicsMode(_SELF)
{
    FBconVideoDevice* video = (FBconVideoDevice*)self;
	return((video->hw_data->keyboard_fd >= 0) && (video->hw_data->saved_kbd_mode >= 0));
}

/* Note:  If we are terminated, self could be called in the middle of
   another MIL video routine -- notably UpdateRects.
*/
static void FBconVideoDevice_X_videoQuit(_Self(VideoDevice))
{
	int i, j;

	if ( ((FBconVideoDevice*)self)->screen ) {
		/* Clear screen and tell MIL not to free the pixels */
		if ( ((FBconVideoDevice*)self)->screen->pixels && FB_InGraphicsMode(self) ) {
#if defined(__powerpc__) || defined(__ia64__)	/* SIGBUS when using MIL_memset() ?? */
			Uint8 *rowp = (Uint8 *)(((FBconVideoDevice*)self)->screen->pixels);
			int left = ((FBconVideoDevice*)self)->screen->pitch * 
                ((FBconVideoDevice*)self)->screen->h;
			while ( left-- ) { *rowp++ = 0; }
#else
			MIL_memset(((FBconVideoDevice*)self)->screen->pixels, 0,
                    ((FBconVideoDevice*)self)->screen->h * 
                    ((FBconVideoDevice*)self)->screen->pitch);
#endif
		}
		/* This test fails when using the VGA16 shadow memory */
		if ( ((char *)((FBconVideoDevice*)self)->screen->pixels >= 
                    ((FBconVideoDevice*)self)->hw_data->mapped_mem) &&
		     ((char *)((FBconVideoDevice*)self)->screen->pixels < 
              (((FBconVideoDevice*)self)->hw_data->mapped_mem+((FBconVideoDevice*)self)->hw_data->mapped_memlen)) ) {
			((FBconVideoDevice*)self)->screen->pixels = NULL;
		}
	}

	/* Clear the lock mutex */
	if ( ((FBconVideoDevice*)self)->hw_data->hw_lock ) {
		MIL_DestroyMutex(((FBconVideoDevice*)self)->hw_data->hw_lock);
		((FBconVideoDevice*)self)->hw_data->hw_lock = NULL;
	}

	/* Clean up defined video modes */
	for ( i=0; i<NUM_MODELISTS; ++i ) {
		if ( ((FBconVideoDevice*)self)->hw_data->MIL_modelist[i] != NULL ) {
			for ( j=0; ((FBconVideoDevice*)self)->hw_data->MIL_modelist[i][j]; ++j ) {
				MIL_free(((FBconVideoDevice*)self)->hw_data->MIL_modelist[i][j]);
			}
			MIL_free(((FBconVideoDevice*)self)->hw_data->MIL_modelist[i]);
			((FBconVideoDevice*)self)->hw_data->MIL_modelist[i] = NULL;
		}
	}

	/* Clean up the memory bucket list */
	FB_FreeHWSurfaces(self);

	/* Close console and input file descriptors */
	if ( ((FBconVideoDevice*)self)->hw_data->console_fd > 0 ) {
		/* Unmap the video framebuffer and I/O registers */
		if ( ((FBconVideoDevice*)self)->hw_data->mapped_mem ) {
			munmap(((FBconVideoDevice*)self)->hw_data->mapped_mem, ((FBconVideoDevice*)self)->hw_data->mapped_memlen);
			((FBconVideoDevice*)self)->hw_data->mapped_mem = NULL;
		}
		if ( ((FBconVideoDevice*)self)->hw_data->mapped_io ) {
			munmap(((FBconVideoDevice*)self)->hw_data->mapped_io, ((FBconVideoDevice*)self)->hw_data->mapped_iolen);
			((FBconVideoDevice*)self)->hw_data->mapped_io = NULL;
		}

		/* Restore the original video mode and palette */
		if ( FB_InGraphicsMode(self) ) {
			FB_RestorePalette(self);
			ioctl(((FBconVideoDevice*)self)->hw_data->console_fd, FBIOPUT_VSCREENINFO, &(((FBconVideoDevice*)self)->hw_data->saved_vinfo));
		}

		/* We're all done with the framebuffer */
		close(((FBconVideoDevice*)self)->hw_data->console_fd);
		((FBconVideoDevice*)self)->hw_data->console_fd = -1;
	}
/* TODO: ... */
#if 0
	FB_CloseMouse(self);
	FB_CloseKeyboard(self);
#endif
}


int FBconVideoDevice_X_toggleFullScreen(_Self(VideoDevice), int on)
{
    return -1;
}

void FBconVideoDevice_X_updateMouse(_Self(VideoDevice))
{
}

MIL_Overlay* FBconVideoDevice_X_createYUVOverlay(_Self(VideoDevice), int width, int height,
                                 Uint32 format, Surface *display)
{
    return NULL;
}

int FBconVideoDevice_X_checkHWBlit(_Self(VideoDevice), Surface *src, Surface *dst)
{
    return -1;
}

int FBconVideoDevice_X_fillHWRect(_Self(VideoDevice), Surface *dst, MIL_Rect *rect, Uint32 color)
{
    return -1;
}

int FBconVideoDevice_X_setHWColorKey(_Self(VideoDevice), Surface *surface, Uint32 key)
{
    return -1;
}
int FBconVideoDevice_X_setHWAlpha(_Self(VideoDevice), Surface *surface, Uint8 value)
{
}

int FBconVideoDevice_X_setGamma(_Self(VideoDevice), float red, float green, float blue)
{
    return -1;
}

int FBconVideoDevice_X_getGamma(_Self(VideoDevice), float *red, float *green, float *blue)
{
    return -1;
}

int FBconVideoDevice_X_setGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int FBconVideoDevice_X_getGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int FBconVideoDevice_X_GL_LoadLibrary(_Self(VideoDevice), const char *path)
{
    return -1;
}

void* FBconVideoDevice_X_GL_GetProcAddress(_Self(VideoDevice), const char *proc)
{
    return NULL;
}

int FBconVideoDevice_X_GL_GetAttribute(_Self(VideoDevice), MIL_GLattr attrib, int* value)
{
    return -1;
}

int FBconVideoDevice_X_GL_MakeCurrent(_Self(VideoDevice))
{
    return -1;
}

void FBconVideoDevice_X_GL_SwapBuffers(_Self(VideoDevice))
{
}

METHOD_MAP_BEGIN(FBconVideoDevice, VideoDevice)

    CONSTRUCTOR_MAP(FBconVideoDevice)
    DESTRUCTOR_MAP(FBconVideoDevice)
    METHOD_MAP(FBconVideoDevice, videoInit)
    METHOD_MAP(FBconVideoDevice, listModes)
    METHOD_MAP(FBconVideoDevice, setVideoMode)
    METHOD_MAP(FBconVideoDevice, toggleFullScreen)
    METHOD_MAP(FBconVideoDevice, updateMouse)
    METHOD_MAP(FBconVideoDevice, createYUVOverlay)
    METHOD_MAP(FBconVideoDevice, setColors)
    METHOD_MAP(FBconVideoDevice, updateRects)
    METHOD_MAP(FBconVideoDevice, videoQuit)
    METHOD_MAP(FBconVideoDevice, allocHWSurface)
    METHOD_MAP(FBconVideoDevice, checkHWBlit)
    METHOD_MAP(FBconVideoDevice, fillHWRect)
    METHOD_MAP(FBconVideoDevice, setHWColorKey)
    METHOD_MAP(FBconVideoDevice, setHWAlpha)
    METHOD_MAP(FBconVideoDevice, lockHWSurface)
    METHOD_MAP(FBconVideoDevice, unlockHWSurface)
    METHOD_MAP(FBconVideoDevice, flipHWSurface)
    METHOD_MAP(FBconVideoDevice, freeHWSurface)
    METHOD_MAP(FBconVideoDevice, setGamma)
    METHOD_MAP(FBconVideoDevice, getGamma)
    METHOD_MAP(FBconVideoDevice, setGammaRamp)
    METHOD_MAP(FBconVideoDevice, getGammaRamp)
    METHOD_MAP(FBconVideoDevice, GL_LoadLibrary)
    METHOD_MAP(FBconVideoDevice, GL_GetProcAddress)
    METHOD_MAP(FBconVideoDevice, GL_GetAttribute)
    METHOD_MAP(FBconVideoDevice, GL_MakeCurrent)
    METHOD_MAP(FBconVideoDevice, GL_SwapBuffers)

METHOD_MAP_END




#endif
