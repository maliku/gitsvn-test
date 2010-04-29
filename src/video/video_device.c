/*! ============================================================================
 * @file video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月16日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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

VideoDevice* CreateVideoDevice(const char* driver_name)
{
	VideoDevice* video = NULL;
	int i = 0;
	MIL_PixelFormat vformat;

	/* Check to make sure we don't overwrite 'g_current_video' */
	if ( NULL != g_current_video ) {
        _VC(g_current_video)->videoQuit(g_current_video);
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
		for ( i=0; NULL != g_video_device_entries[i]; ++i ) {
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
		return(NULL);
	}
	return (g_current_video = video);
}

int VideoDevice_X_videoInit(_Self(VideoDevice), MIL_PixelFormat *vformat)
{
    printf("VideoDevice::videoInit vformat=%p\n", vformat);
    return -1;
}

MIL_Rect** VideoDevice_X_listModes(_Self(VideoDevice), MIL_PixelFormat *format, Uint32 flags)
{
    return NULL;
}

MIL_Surface* VideoDevice_X_setVideoMode(_Self(VideoDevice), MIL_Surface* current,
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
                                 Uint32 format, MIL_Surface *display)
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

int VideoDevice_X_allocHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

int VideoDevice_X_checkHWBlit(_Self(VideoDevice), MIL_Surface *src, MIL_Surface *dst)
{
    return -1;
}

int VideoDevice_X_fillHWRect(_Self(VideoDevice), MIL_Surface *dst, MIL_Rect *rect, Uint32 color)
{
    return -1;
}

int VideoDevice_X_setHWColorKey(_Self(VideoDevice), MIL_Surface *surface, Uint32 key)
{
    return -1;
}
int VideoDevice_X_setHWAlpha(_Self(VideoDevice), MIL_Surface *surface, Uint8 value)
{
    return -1;
}

int VideoDevice_X_lockHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

void VideoDevice_X_unlockHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

int VideoDevice_X_flipHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

void VideoDevice_X_freeHWSurface(_Self(VideoDevice), MIL_Surface *surface)
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

VIRTUAL_METHOD_REGBEGIN(VideoDevice, NonBase)

    NON_DESTRUCTOR
    METHOD_REGISTER(VideoDevice, videoInit)
    METHOD_REGISTER(VideoDevice, listModes)
    METHOD_REGISTER(VideoDevice, setVideoMode)
    METHOD_REGISTER(VideoDevice, toggleFullScreen)
    METHOD_REGISTER(VideoDevice, updateMouse)
    METHOD_REGISTER(VideoDevice, createYUVOverlay)
    METHOD_REGISTER(VideoDevice, setColors)
    METHOD_REGISTER(VideoDevice, updateRects)
    METHOD_REGISTER(VideoDevice, videoQuit)
    METHOD_REGISTER(VideoDevice, allocHWSurface)
    METHOD_REGISTER(VideoDevice, checkHWBlit)
    METHOD_REGISTER(VideoDevice, fillHWRect)
    METHOD_REGISTER(VideoDevice, setHWColorKey)
    METHOD_REGISTER(VideoDevice, setHWAlpha)
    METHOD_REGISTER(VideoDevice, lockHWSurface)
    METHOD_REGISTER(VideoDevice, unlockHWSurface)
    METHOD_REGISTER(VideoDevice, flipHWSurface)
    METHOD_REGISTER(VideoDevice, freeHWSurface)
    METHOD_REGISTER(VideoDevice, setGamma)
    METHOD_REGISTER(VideoDevice, getGamma)
    METHOD_REGISTER(VideoDevice, setGammaRamp)
    METHOD_REGISTER(VideoDevice, getGammaRamp)
    METHOD_REGISTER(VideoDevice, GL_LoadLibrary)
    METHOD_REGISTER(VideoDevice, GL_GetProcAddress)
    METHOD_REGISTER(VideoDevice, GL_GetAttribute)
    METHOD_REGISTER(VideoDevice, GL_MakeCurrent)
    METHOD_REGISTER(VideoDevice, GL_SwapBuffers)

VIRTUAL_METHOD_REGEND

METHOD_REGISTER_PLACEHOLDER(VideoDevice)

