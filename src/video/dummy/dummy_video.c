/*! ============================================================================
 * @file DummyVideo.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月16日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "dummy_video.h"

#define DUMMY_VIDEO_DRIVER_NAME "dummy"

VideoDevice* CreateDummyVideoDevice(void)
{
    VideoDevice* video = (VideoDevice*)New(DummyVideoDevice);
    if (NULL != video) {
        _VC(video)->videoInit(video, NULL);
    }
    return video;
}

VideoDeviceEntry g_video_dummy = {
    DUMMY_VIDEO_DRIVER_NAME,
    CreateDummyVideoDevice
};

CONSTRUCTOR(DummyVideoDevice)
{
    printf("DummyVideoDevice %p constructed...\n", self);
    ((VideoDevice*)self)->name = DUMMY_VIDEO_DRIVER_NAME;
    return self;
}

DESTRUCTOR(DummyVideoDevice)
{
    printf("DummyVideoDevice %p destructed...\n", self);
}

#if 0
int DummyVideoDevice_X_videoInit(_Self(VideoDevice), MIL_PixelFormat *vformat)
{
    printf("DummyVideoDevice::videoInit vformat=%p\n", vformat);
    return -1;
}

MIL_Rect** DummyVideoDevice_X_listModes(_Self(VideoDevice), MIL_PixelFormat *format, Uint32 flags)
{
    return NULL;
}

MIL_Surface* DummyVideoDevice_X_setVideoMode(_Self(VideoDevice), MIL_Surface* current,
			int width, int height, int bpp, Uint32 flags)
{
    return NULL;
}

int DummyVideoDevice_X_toggleFullScreen(_Self(VideoDevice), int on)
{
    return -1;
}

void DummyVideoDevice_X_updateMouse(_Self(VideoDevice))
{
}

MIL_Overlay* DummyVideoDevice_X_createYUVOverlay(_Self(VideoDevice), int width, int height,
                                 Uint32 format, MIL_Surface *display)
{
    return NULL;
}

int DummyVideoDevice_X_setColors(_Self(VideoDevice), int firstcolor, int ncolors,
		 MIL_Color *colors)
{
    return -1;
}

void DummyVideoDevice_X_updateRects(_Self(VideoDevice), int numrects, MIL_Rect *rects)
{
}

void DummyVideoDevice_X_videoQuit(_Self(VideoDevice))
{
}

int DummyVideoDevice_X_allocHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

int DummyVideoDevice_X_checkHWBlit(_Self(VideoDevice), MIL_Surface *src, MIL_Surface *dst)
{
    return -1;
}

int DummyVideoDevice_X_fillHWRect(_Self(VideoDevice), MIL_Surface *dst, MIL_Rect *rect, Uint32 color)
{
    return -1;
}

int DummyVideoDevice_X_setHWColorKey(_Self(VideoDevice), MIL_Surface *surface, Uint32 key)
{
    return -1;
}
int DummyVideoDevice_X_setHWAlpha(_Self(VideoDevice), MIL_Surface *surface, Uint8 value)
{
}

int DummyVideoDevice_X_lockHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

void DummyVideoDevice_X_unlockHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

int DummyVideoDevice_X_flipHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

void DummyVideoDevice_X_freeHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

int DummyVideoDevice_X_setGamma(_Self(VideoDevice), float red, float green, float blue)
{
    return -1;
}

int DummyVideoDevice_X_getGamma(_Self(VideoDevice), float *red, float *green, float *blue)
{
    return -1;
}

int DummyVideoDevice_X_setGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int DummyVideoDevice_X_getGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int DummyVideoDevice_X_GL_LoadLibrary(_Self(VideoDevice), const char *path)
{
    return -1;
}

void* DummyVideoDevice_X_GL_GetProcAddress(_Self(VideoDevice), const char *proc)
{
    return NULL;
}

int DummyVideoDevice_X_GL_GetAttribute(_Self(VideoDevice), MIL_GLattr attrib, int* value)
{
    return -1;
}

int DummyVideoDevice_X_GL_MakeCurrent(_Self(VideoDevice))
{
    return -1;
}

void DummyVideoDevice_X_GL_SwapBuffers(_Self(VideoDevice))
{
}
#endif

VIRTUAL_METHOD_REGBEGIN(DummyVideoDevice, VideoDevice)

    DESTRUCTOR_REGISTER(DummyVideoDevice)
/*     METHOD_REGISTER(DummyVideoDevice, videoInit)
 *     METHOD_REGISTER(DummyVideoDevice, listModes)
 *     METHOD_REGISTER(DummyVideoDevice, setVideoMode)
 *     METHOD_REGISTER(DummyVideoDevice, toggleFullScreen)
 *     METHOD_REGISTER(DummyVideoDevice, updateMouse)
 *     METHOD_REGISTER(DummyVideoDevice, createYUVOverlay)
 *     METHOD_REGISTER(DummyVideoDevice, setColors)
 *     METHOD_REGISTER(DummyVideoDevice, updateRects)
 *     METHOD_REGISTER(DummyVideoDevice, videoQuit)
 *     METHOD_REGISTER(DummyVideoDevice, allocHWSurface)
 *     METHOD_REGISTER(DummyVideoDevice, checkHWBlit)
 *     METHOD_REGISTER(DummyVideoDevice, fillHWRect)
 *     METHOD_REGISTER(DummyVideoDevice, setHWColorKey)
 *     METHOD_REGISTER(DummyVideoDevice, setHWAlpha)
 *     METHOD_REGISTER(DummyVideoDevice, lockHWSurface)
 *     METHOD_REGISTER(DummyVideoDevice, unlockHWSurface)
 *     METHOD_REGISTER(DummyVideoDevice, flipHWSurface)
 *     METHOD_REGISTER(DummyVideoDevice, freeHWSurface)
 *     METHOD_REGISTER(DummyVideoDevice, setGamma)
 *     METHOD_REGISTER(DummyVideoDevice, getGamma)
 *     METHOD_REGISTER(DummyVideoDevice, setGammaRamp)
 *     METHOD_REGISTER(DummyVideoDevice, getGammaRamp)
 *     METHOD_REGISTER(DummyVideoDevice, GL_LoadLibrary)
 *     METHOD_REGISTER(DummyVideoDevice, GL_GetProcAddress)
 *     METHOD_REGISTER(DummyVideoDevice, GL_GetAttribute)
 *     METHOD_REGISTER(DummyVideoDevice, GL_MakeCurrent)
 *     METHOD_REGISTER(DummyVideoDevice, GL_SwapBuffers)
 */

VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(DummyVideoDevice)
    CONSTRUCTOR_REGISTER(DummyVideoDevice)
METHOD_REGEND
