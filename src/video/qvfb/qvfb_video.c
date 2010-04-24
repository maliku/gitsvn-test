/*! ============================================================================
 * @file qvfb_video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月20日
 *  Organization: http://www.ds0101.net
 */
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "surface.h"
#include "qvfb_video.h"

#define QVFB_VIDEO_DRIVER_NAME "qvfb"

VideoDevice* CreateQVFbVideoDevice(void)
{
    MIL_PixelFormat vf;
    VideoDevice* vd = (VideoDevice*)New(QVFbVideoDevice);
    if (NULL != vd) {
        if (0 != _VC(vd)->videoInit(vd, &vf)) {
            Delete(vd);
            return NULL;
        }
    }
    return vd;
}

VideoDeviceEntry g_video_qvfb = {
    QVFB_VIDEO_DRIVER_NAME,
    CreateQVFbVideoDevice
};

CONSTRUCTOR(QVFbVideoDevice)
{
    printf("QVFbVideoDevice %p constructed...\n", self);
    _m(hw_data) = (QVFbHardwareDependent*)MIL_malloc(sizeof (*_m(hw_data)));
    ((VideoDevice*)self)->name = QVFB_VIDEO_DRIVER_NAME;
    return self;
}

DESTRUCTOR(QVFbVideoDevice)
{
    printf("QVFbVideoDevice %p destructed...\n", self);
    MIL_free(((QVFbVideoDevice*)self)->hw_data);
}

int QVFbVideoDevice_X_videoInit(_Self(VideoDevice), MIL_PixelFormat *vformat)
{
    char file [MIL_MAX_PATH];
    int display = 0;
    key_t key;
    int shmid = 0;
    QVFbHardwareDependent* data = ((QVFbVideoDevice*)self)->hw_data;

//    if (GetMgEtcIntValue ("qvfb", "display", &display) < 0)
//        display = 0;

    sprintf (file, QT_VFB_MOUSE_PIPE, display);
    key = ftok (file, 'b');

    shmid = shmget (key, 0, 0);
    if (shmid != -1)
        data->shmrgn = (unsigned char *)shmat(shmid, 0, 0);

    if ((int)data->shmrgn == -1 || data->shmrgn == NULL) {
        MIL_SetError ("VIDEO>QVFb: Unable to attach to virtual FrameBuffer server.\n");
        return -1;
    }

     data->hdr = (QVFbHeader *) data->shmrgn;

    vformat->BitsPerPixel = data->hdr->depth;
    switch (vformat->BitsPerPixel) {
#ifdef _MIL_SHADOW
        case 1:
            break;
        case 4:
            break;
#endif
        case 8:
            vformat->BytesPerPixel = 1;
            data->hdr->numcols = 256;
            break;
        case 12:
            vformat->BitsPerPixel = 16;
            vformat->BytesPerPixel = 2;
            vformat->Rmask = 0x00000F00;
            vformat->Gmask = 0x000000F0;
            vformat->Bmask = 0x0000000F;
            break;
        case 16:
            vformat->BytesPerPixel = 2;
            vformat->Rmask = 0x0000F800;
            vformat->Gmask = 0x000007E0;
            vformat->Bmask = 0x0000001F;
            break;
        case 32:
            vformat->BytesPerPixel = 4;
            vformat->Amask = 0xFF000000;
            vformat->Rmask = 0x00FF0000;
            vformat->Gmask = 0x0000FF00;
            vformat->Bmask = 0x000000FF;
            break;
        default:
            MIL_SetError("VIDEO>QVFb: Not supported depth\n");
            return -1;
    }

    return 0;
}

MIL_Rect** QVFbVideoDevice_X_listModes(_Self(VideoDevice), 
        MIL_PixelFormat *format, Uint32 flags)
{
    return (MIL_Rect **) -1;
}

MIL_Surface* QVFbVideoDevice_X_setVideoMode(_Self(VideoDevice), 
        MIL_Surface *current, int width, int height, int bpp, Uint32 flags)
{
    /* Set up the mode framebuffer */
    _VC(current)->setFlags(current, MIL_HWSURFACE | MIL_FULLSCREEN);
    _VC(current)->setWidth(current, ((QVFbVideoDevice*)self)->hw_data->hdr->width);
    _VC(current)->setHeight(current, ((QVFbVideoDevice*)self)->hw_data->hdr->height);
    _VC(current)->setPitch(current, ((QVFbVideoDevice*)self)->hw_data->hdr->linestep);
    ((Surface*)current)->pixels = ((QVFbVideoDevice*)self)->hw_data->shmrgn + ((QVFbVideoDevice*)self)->hw_data->hdr->dataoffset;

    return current;
}

int QVFbVideoDevice_X_toggleFullScreen(_Self(VideoDevice), int on)
{
    return -1;
}

void QVFbVideoDevice_X_updateMouse(_Self(VideoDevice))
{
}

MIL_Overlay* QVFbVideoDevice_X_createYUVOverlay(_Self(VideoDevice), int width, int height,
                                 Uint32 format, MIL_Surface *display)
{
    return NULL;
}
int QVFbVideoDevice_X_setColors(_Self(VideoDevice), int firstcolor, 
        int ncolors, MIL_Color *colors)
{
    int i, pixel = firstcolor;

    for (i = 0; i < ncolors; i++) {
        ((QVFbVideoDevice*)self)->hw_data->hdr->clut [pixel] 
                = (0xff << 24) | ((colors[i].r & 0xff) << 16) | ((colors[i].g & 0xff) << 8) | (colors[i].b & 0xff);
        pixel ++;
    }

    return 1;
}

void QVFbVideoDevice_X_updateRects(_Self(VideoDevice), int numrects, 
        MIL_Rect *rects)
{
    int i;

    RECT bound = ((QVFbVideoDevice*)self)->hw_data->hdr->update;

    for (i = 0; i < numrects; ++i) {
        RECT rc = {rects[i].x, rects[i].y, 
                        rects[i].x + rects[i].w, rects[i].y + rects[i].h};

//        SetRect (&rc, rects[i].x, rects[i].y, 
//                        rects[i].x + rects[i].w, rects[i].y + rects[i].h);
//        if (IsRectEmpty (&bound))
            bound = rc;
//        else
//            GetBoundRect (&bound, &bound, &rc);
    }

    ((QVFbVideoDevice*)self)->hw_data->hdr->update = bound;
    ((QVFbVideoDevice*)self)->hw_data->hdr->dirty = MIL_TRUE;
}

void QVFbVideoDevice_X_videoQuit(_Self(VideoDevice))
{
    shmdt(((QVFbVideoDevice*)self)->hw_data->shmrgn);
}

int QVFbVideoDevice_X_allocHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

int QVFbVideoDevice_X_checkHWBlit(_Self(VideoDevice), MIL_Surface *src, MIL_Surface *dst)
{
    return -1;
}

int QVFbVideoDevice_X_fillHWRect(_Self(VideoDevice), MIL_Surface *dst, MIL_Rect *rect, Uint32 color)
{
    return -1;
}

int QVFbVideoDevice_X_setHWColorKey(_Self(VideoDevice), MIL_Surface *surface, Uint32 key)
{
    return -1;
}
int QVFbVideoDevice_X_setHWAlpha(_Self(VideoDevice), MIL_Surface *surface, Uint8 value)
{
}

int QVFbVideoDevice_X_lockHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

void QVFbVideoDevice_X_unlockHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

int QVFbVideoDevice_X_flipHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

void QVFbVideoDevice_X_freeHWSurface(_Self(VideoDevice), MIL_Surface *surface)
{
}

int QVFbVideoDevice_X_setGamma(_Self(VideoDevice), float red, float green, float blue)
{
    return -1;
}

int QVFbVideoDevice_X_getGamma(_Self(VideoDevice), float *red, float *green, float *blue)
{
    return -1;
}

int QVFbVideoDevice_X_setGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int QVFbVideoDevice_X_getGammaRamp(_Self(VideoDevice), Uint16 *ramp)
{
    return -1;
}

int QVFbVideoDevice_X_GL_LoadLibrary(_Self(VideoDevice), const char *path)
{
    return -1;
}

void* QVFbVideoDevice_X_GL_GetProcAddress(_Self(VideoDevice), const char *proc)
{
    return NULL;
}

int QVFbVideoDevice_X_GL_GetAttribute(_Self(VideoDevice), MIL_GLattr attrib, int* value)
{
    return -1;
}

int QVFbVideoDevice_X_GL_MakeCurrent(_Self(VideoDevice))
{
    return -1;
}

void QVFbVideoDevice_X_GL_SwapBuffers(_Self(VideoDevice))
{
}

VIRTUAL_METHOD_REGBEGIN(QVFbVideoDevice, VideoDevice)

    DESTRUCTOR_REGISTER(QVFbVideoDevice)
    METHOD_REGISTER(QVFbVideoDevice, videoInit)
    METHOD_REGISTER(QVFbVideoDevice, listModes)
    METHOD_REGISTER(QVFbVideoDevice, setVideoMode)
    METHOD_REGISTER(QVFbVideoDevice, toggleFullScreen)
    METHOD_REGISTER(QVFbVideoDevice, updateMouse)
    METHOD_REGISTER(QVFbVideoDevice, createYUVOverlay)
    METHOD_REGISTER(QVFbVideoDevice, setColors)
    METHOD_REGISTER(QVFbVideoDevice, updateRects)
    METHOD_REGISTER(QVFbVideoDevice, videoQuit)
    METHOD_REGISTER(QVFbVideoDevice, allocHWSurface)
    METHOD_REGISTER(QVFbVideoDevice, checkHWBlit)
    METHOD_REGISTER(QVFbVideoDevice, fillHWRect)
    METHOD_REGISTER(QVFbVideoDevice, setHWColorKey)
    METHOD_REGISTER(QVFbVideoDevice, setHWAlpha)
    METHOD_REGISTER(QVFbVideoDevice, lockHWSurface)
    METHOD_REGISTER(QVFbVideoDevice, unlockHWSurface)
    METHOD_REGISTER(QVFbVideoDevice, flipHWSurface)
    METHOD_REGISTER(QVFbVideoDevice, freeHWSurface)
    METHOD_REGISTER(QVFbVideoDevice, setGamma)
    METHOD_REGISTER(QVFbVideoDevice, getGamma)
    METHOD_REGISTER(QVFbVideoDevice, setGammaRamp)
    METHOD_REGISTER(QVFbVideoDevice, getGammaRamp)
    METHOD_REGISTER(QVFbVideoDevice, GL_LoadLibrary)
    METHOD_REGISTER(QVFbVideoDevice, GL_GetProcAddress)
    METHOD_REGISTER(QVFbVideoDevice, GL_GetAttribute)
    METHOD_REGISTER(QVFbVideoDevice, GL_MakeCurrent)
    METHOD_REGISTER(QVFbVideoDevice, GL_SwapBuffers)

VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(QVFbVideoDevice)
    CONSTRUCTOR_REGISTER(QVFbVideoDevice)
METHOD_REGEND
