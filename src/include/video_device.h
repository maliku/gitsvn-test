/*!============================================================================
 * @file gfx.h 
 * @Synopsis abstract graphics layer.
 * @author DongKai
 * @version 1.0
 * @date 2010年03月23日 22时50分46秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  GFX_INC
#define  GFX_INC

#include "coo.h"
#include "MIL_video.h"

CLASS(VideoDevice) {
    VIRTUAL_METHOD_DECLARE_BEGIN(VideoDevice) 
	/* * * */
	/* Initialization/Query functions */

	/* Initialize the native video subsystem, filling 'vformat' with the 
	   "best" display pixel format, returning 0 or -1 if there's an error.
	 */
	int (*videoInit)(_Self(VideoDevice), MIL_PixelFormat *vformat);

	/* List the available video modes for the given pixel format, sorted
	   from largest to smallest.
	 */
	MIL_Rect** (*listModes)(_Self(VideoDevice), MIL_PixelFormat *format, Uint32 flags);

	/* Set the requested video mode, returning a surface which will be
	   set to the MIL_VideoSurface.  The width and height will already
	   be verified by ListModes(), and the video subsystem is free to
	   set the mode to a supported bit depth different from the one
	   specified -- the desired bpp will be emulated with a shadow
	   surface if necessary.  If a new mode is returned, this function
	   should take care of cleaning up the current mode.
	 */
	MIL_Surface* (*setVideoMode)(_Self(VideoDevice), MIL_Surface *current,
				int width, int height, int bpp, Uint32 flags);

	/* Toggle the fullscreen mode */
	int (*toggleFullScreen)(_Self(VideoDevice), int on);

	/* This is called after the video mode has been set, to get the
	   initial mouse state.  It should queue events as necessary to
	   properly represent the current mouse focus and position.
	 */
	void (*updateMouse)(_Self(VideoDevice));

	/* Create a YUV video surface (possibly overlay) of the given
	   format.  The hardware should be able to perform at least 2x
	   scaling on display.
	 */
	MIL_Overlay* (*createYUVOverlay)(_Self(VideoDevice), int width, int height,
	                                 Uint32 format, MIL_Surface *display);

    /* Sets the color entries { firstcolor .. (firstcolor+ncolors-1) }
	   of the physical palette to those in 'colors'. If the device is
	   using a software palette (MIL_HWPALETTE not set), then the
	   changes are reflected in the logical palette of the screen
	   as well.
	   The return value is 1 if all entries could be set properly
	   or 0 otherwise.
	*/
	int (*setColors)(_Self(VideoDevice), int firstcolor, int ncolors,
			 MIL_Color *colors);

	/* This pointer should exist in the native video subsystem and should
	   point to an appropriate update function for the current video mode
	 */
	void (*updateRects)(_Self(VideoDevice), int numrects, MIL_Rect *rects);

	/* Reverse the effects VideoInit() -- called if VideoInit() fails
	   or if the application is shutting down the video subsystem.
	*/
	void (*videoQuit)(_Self(VideoDevice));

	/* * * */
	/* Hardware acceleration functions */

	/* Allocates a surface in video memory */
	int (*allocHWSurface)(_Self(VideoDevice), MIL_Surface *surface);

	/* Sets the hardware accelerated blit function, if any, based
	   on the current flags of the surface (colorkey, alpha, etc.)
	 */
	int (*checkHWBlit)(_Self(VideoDevice), MIL_Surface *src, MIL_Surface *dst);

	/* Fills a surface rectangle with the given color */
	int (*fillHWRect)(_Self(VideoDevice), MIL_Surface *dst, MIL_Rect *rect, Uint32 color);

	/* Sets video mem colorkey and accelerated blit function */
	int (*setHWColorKey)(_Self(VideoDevice), MIL_Surface *surface, Uint32 key);

	/* Sets per surface hardware alpha value */
	int (*setHWAlpha)(_Self(VideoDevice), MIL_Surface *surface, Uint8 value);

	/* Returns a readable/writable surface */
	int (*lockHWSurface)(_Self(VideoDevice), MIL_Surface *surface);
	void (*unlockHWSurface)(_Self(VideoDevice), MIL_Surface *surface);

	/* Performs hardware flipping */
	int (*flipHWSurface)(_Self(VideoDevice), MIL_Surface *surface);

	/* Frees a previously allocated video surface */
	void (*freeHWSurface)(_Self(VideoDevice), MIL_Surface *surface);

	/* Set the gamma correction directly (emulated with gamma ramps) */
	int (*setGamma)(_Self(VideoDevice), float red, float green, float blue);

	/* Get the gamma correction directly (emulated with gamma ramps) */
	int (*getGamma)(_Self(VideoDevice), float *red, float *green, float *blue);

	/* Set the gamma ramp */
	int (*setGammaRamp)(_Self(VideoDevice), Uint16 *ramp);

	/* Get the gamma ramp */
	int (*getGammaRamp)(_Self(VideoDevice), Uint16 *ramp);

	/* * * */
	/* OpenGL support */

	/* Sets the dll to use for OpenGL and loads it */
	int (*GL_LoadLibrary)(_Self(VideoDevice), const char *path);

	/* Retrieves the address of a function in the gl library */
	void* (*GL_GetProcAddress)(_Self(VideoDevice), const char *proc);

	/* Get attribute information from the windowing system. */
	int (*GL_GetAttribute)(_Self(VideoDevice), MIL_GLattr attrib, int* value);

	/* Make the context associated with this driver current */
	int (*GL_MakeCurrent)(_Self(VideoDevice));

	/* Swap the current buffers in double buffer mode. */
	void (*GL_SwapBuffers)(_Self(VideoDevice));

    VIRTUAL_METHOD_DECLARE_END 

    METHOD_DECLARE_BEGIN(VideoDevice)
        char* (*getName)(_Self(VideoDevice), char* buff, Uint32 len);
    METHOD_DECLARE_END

    const char *name;
	/* Information about the video hardware */
	MIL_VideoInfo info;

	/* The pixel format used when MIL_CreateRGBSurface creates MIL_HWSURFACEs with alpha */
	MIL_PixelFormat* displayformatalphapixel;
	
};

STRUCT {
    const char* name;
    VideoDevice* (*create)(void);
} VideoDeviceEntry;

#if MIL_VIDEO_DRIVER_DUMMY
extern VideoDeviceEntry g_video_dummy;
#endif
#if MIL_VIDEO_DRIVER_QVFB
extern VideoDeviceEntry g_video_qvfb;
#endif

/* Video device factory. */
VideoDevice* CreateVideoDevice(const char* name);

#endif   /* ----- #ifndef GFX_INC  ----- */

