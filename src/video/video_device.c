/*! ============================================================================
 * @file video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-03-16
 *  Organization: http://www.ds0101.net
 */

#include "video_device.h"

static VideoDeviceEntry* g_video_device_entries[] = {
#if MIL_VIDEO_DRIVER_DUMMY
	&g_video_dummy,
#endif
#if MIL_VIDEO_DRIVER_QVFB
	&g_video_qvfb,
#endif
    NULL
};
VideoDevice* g_current_video;

/* It's just support single device. */
VideoDevice* CreateVideoDevice(const char* driver_name)
{
	VideoDevice* video = NULL;
	int i = 0;
	PixelFormat vformat;

	/* Check to make sure we don't overwrite 'g_current_video' */
	if ( NULL != g_current_video ) {
        _C(g_current_video)->videoQuit(g_current_video);
	}

	/* Select the proper video driver */
	if ( NULL != driver_name ) {
		for ( i = 0; NULL != g_video_device_entries[i]; ++i ) {
			if ( MIL_strcasecmp(g_video_device_entries[i]->name, driver_name) == 0) {
				if ( NULL != g_video_device_entries[i]->create ) {
					video = g_video_device_entries[i]->create();
					break;
				}
			}
		}
	} else {
		for ( i = 0; NULL != g_video_device_entries[i]; ++i ) {
			if ( NULL != g_video_device_entries[i]->create ) {
				video = g_video_device_entries[i]->create();
				if ( NULL != video ) { /* Find the creatable device in default */
					break;
				}
			}
		}
	}
	if ( video == NULL ) {
        MIL_SetError("No available video device\n");
        return (NULL);
	}
	return (g_current_video = video);
}

CONSTRUCTOR(VideoDevice)
{
    memset(&self->vinfo, 0, sizeof(self->vinfo));
    memset(&self->gl_config, 0, sizeof(self->gl_config));
    _m(name) = NULL;
    _m(display_format_alpha_pixel) = NULL;
    _m(screen) = _m(shadow) = _m(visible) = NULL;
    _m(physpal) = NULL;
    _m(gammacols) = NULL;
    _m(offset_x) = _m(offset_y) = 0;
    _m(handles_any_size) = 0;
    _m(input_grab) = MIL_GRAB_OFF;
    return self;
}

DESTRUCTOR(VideoDevice)
{
}

int VideoDevice_X_videoInit(_Self(VideoDevice), PixelFormat *vformat)
{
    printf("VideoDevice::videoInit vformat=%p\n", vformat);
    return -1;
}

MIL_Rect** VideoDevice_X_listModes(_Self(VideoDevice), PixelFormat *format, Uint32 flags)
{
    return NULL;
}

Surface* VideoDevice_X_setVideoMode(_Self(VideoDevice), Surface* current,
			int width, int height, int bpp, Uint32 flags)
{
    return NULL;
}

int VideoDevice_X_toggleFullScreen(_Self(VideoDevice), int on)
{
    return -1;
}

void VideoDevice_X_updateMouse(_Self(VideoDevice))
{
}

MIL_Overlay* VideoDevice_X_createYUVOverlay(_Self(VideoDevice), int width, int height,
                                 Uint32 format, Surface *display)
{
    return NULL;
}

int VideoDevice_X_setColors(_Self(VideoDevice), int firstcolor, int ncolors,
		 MIL_Color *colors)
{
    return -1;
}

void VideoDevice_X_updateRects(_Self(VideoDevice), int numrects, MIL_Rect *rects)
{
}

void VideoDevice_X_videoQuit(_Self(VideoDevice))
{
}

int VideoDevice_X_allocHWSurface(_Self(VideoDevice), Surface *surface)
{
    return -1;
}

int VideoDevice_X_checkHWBlit(_Self(VideoDevice), Surface *src, Surface *dst)
{
    return -1;
}

int VideoDevice_X_fillHWRect(_Self(VideoDevice), Surface *dst, MIL_Rect *rect, Uint32 color)
{
    return -1;
}

int VideoDevice_X_setHWColorKey(_Self(VideoDevice), Surface *surface, Uint32 key)
{
    return -1;
}
int VideoDevice_X_setHWAlpha(_Self(VideoDevice), Surface *surface, Uint8 value)
{
    return -1;
}

int VideoDevice_X_lockHWSurface(_Self(VideoDevice), Surface *surface)
{
    return 0;
}

void VideoDevice_X_unlockHWSurface(_Self(VideoDevice), Surface *surface)
{
}

int VideoDevice_X_flipHWSurface(_Self(VideoDevice), Surface *surface)
{
    return -1;
}

void VideoDevice_X_freeHWSurface(_Self(VideoDevice), Surface *surface)
{
}

int VideoDevice_X_setGamma(_Self(VideoDevice), float red, float green, float blue)
{
    return -1;
}

int VideoDevice_X_getGamma(_Self(VideoDevice), float *red, float *green, float *blue)
{
    return -1;
}

int VideoDevice_X_setGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int VideoDevice_X_getGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int VideoDevice_X_GL_LoadLibrary(_Self(VideoDevice), const char *path)
{
    return -1;
}

void* VideoDevice_X_GL_GetProcAddress(_Self(VideoDevice), const char *proc)
{
    return NULL;
}

int VideoDevice_X_GL_GetAttribute(_Self(VideoDevice), MIL_GLattr attrib, int* value)
{
    return -1;
}

int VideoDevice_X_GL_MakeCurrent(_Self(VideoDevice))
{
    return -1;
}

void VideoDevice_X_GL_SwapBuffers(_Self(VideoDevice))
{
}

VIRTUAL_METHOD_MAP_BEGIN(VideoDevice, NonBase)

    NON_DESTRUCTOR
    METHOD_MAP(VideoDevice, videoInit)
    METHOD_MAP(VideoDevice, listModes)
    METHOD_MAP(VideoDevice, setVideoMode)
    METHOD_MAP(VideoDevice, toggleFullScreen)
    METHOD_MAP(VideoDevice, updateMouse)
    METHOD_MAP(VideoDevice, createYUVOverlay)
    METHOD_MAP(VideoDevice, setColors)
    METHOD_MAP(VideoDevice, updateRects)
    METHOD_MAP(VideoDevice, videoQuit)
    METHOD_MAP(VideoDevice, allocHWSurface)
    METHOD_MAP(VideoDevice, checkHWBlit)
    METHOD_MAP(VideoDevice, fillHWRect)
    METHOD_MAP(VideoDevice, setHWColorKey)
    METHOD_MAP(VideoDevice, setHWAlpha)
    METHOD_MAP(VideoDevice, lockHWSurface)
    METHOD_MAP(VideoDevice, unlockHWSurface)
    METHOD_MAP(VideoDevice, flipHWSurface)
    METHOD_MAP(VideoDevice, freeHWSurface)
    METHOD_MAP(VideoDevice, setGamma)
    METHOD_MAP(VideoDevice, getGamma)
    METHOD_MAP(VideoDevice, setGammaRamp)
    METHOD_MAP(VideoDevice, getGammaRamp)
    METHOD_MAP(VideoDevice, GL_LoadLibrary)
    METHOD_MAP(VideoDevice, GL_GetProcAddress)
    METHOD_MAP(VideoDevice, GL_GetAttribute)
    METHOD_MAP(VideoDevice, GL_MakeCurrent)
    METHOD_MAP(VideoDevice, GL_SwapBuffers)

VIRTUAL_METHOD_MAP_END

METHOD_MAP_PLACEHOLDER(VideoDevice)

