/*! ============================================================================
 * @file qvfb_video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-20
 *  Organization: http://www.ds0101.net
 */

#include "MIL_config.h"

#if MIL_VIDEO_DRIVER_QVFB
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "surface.h"
#include "pixel_format.h"
#include "qvfb_video.h"

#define QVFB_VIDEO_DRIVER_NAME "qvfb"

VideoDevice* CreateQVFbVideoDevice(void)
{
    VideoDevice* vd = (VideoDevice*)New(QVFbVideoDevice);
    if (NULL != vd) {
        PixelFormat *vf = (PixelFormat*)New(PixelFormat);
        if (0 != _VC(vd)->videoInit(vd, vf)) {
            Delete(vd);
            Delete(vf);
            return NULL;
        }
        Delete(vf);
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

static int format_calc(PixelFormat* fmt, int bpp)
{
    PixelFormat* vformat = (PixelFormat*)fmt;
    if (NULL == vformat) return -1;
    vformat->BitsPerPixel = bpp;
    switch (vformat->BitsPerPixel) {
#ifdef _MIL_SHADOW
        case 1:
            break;
        case 4:
            break;
#endif
        case 8:
            vformat->BytesPerPixel = 1;
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
//            vformat->Amask = 0xFF000000;
//          The code seems like useless.
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

int QVFbVideoDevice_X_videoInit(_Self(VideoDevice), PixelFormat *vformat)
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

    format_calc(vformat, data->hdr->depth);
    if (8 == _vc0(vformat, getBitsPerPixel)) {
        data->hdr->numcols = 256;
    }
    return 0;
}

MIL_Rect** QVFbVideoDevice_X_listModes(_Self(VideoDevice), 
        PixelFormat *format, Uint32 flags)
{
    return (MIL_Rect **) -1;
}

Surface* QVFbVideoDevice_X_setVideoMode(_Self(VideoDevice), 
        Surface *current, int width, int height, int bpp, Uint32 flags)
{
    /* Set up the mode framebuffer */
    ((Surface*)current)->flags = (MIL_HWSURFACE | MIL_FULLSCREEN);
    ((Surface*)current)->w = ((QVFbVideoDevice*)self)->hw_data->hdr->width;
    ((Surface*)current)->h = ((QVFbVideoDevice*)self)->hw_data->hdr->height;
    ((Surface*)current)->pitch = ((QVFbVideoDevice*)self)->hw_data->hdr->linestep;
    ((Surface*)current)->pixels = ((QVFbVideoDevice*)self)->hw_data->shmrgn + 
        ((QVFbVideoDevice*)self)->hw_data->hdr->dataoffset;
    ((Surface*)current)->clip_rect.x = 0; 
    ((Surface*)current)->clip_rect.y = 0; 
    ((Surface*)current)->clip_rect.w = ((Surface*)current)->w; 
    ((Surface*)current)->clip_rect.h = ((Surface*)current)->h; 
    if ( ! MIL_ReallocFormat(current, bpp, 0, 0, 0, 0) ) {
        return(NULL);
	}
    format_calc(((Surface*)current)->format, bpp);
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
                                 Uint32 format, Surface *display)
{
    return NULL;
}
int QVFbVideoDevice_X_setColors(_Self(VideoDevice), int firstcolor, 
        int ncolors, MIL_Color *colors)
{
    int i, pixel = firstcolor;

    for (i = 0; i < ncolors; i++) {
        ((QVFbVideoDevice*)self)->hw_data->hdr->clut [pixel] 
                = (0xff << 24) | ((colors[i].r & 0xff) << 16) 
                | ((colors[i].g & 0xff) << 8) | (colors[i].b & 0xff);
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

        // TODO:merge here.
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
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(QVFbVideoDevice)
    CONSTRUCTOR_REGISTER(QVFbVideoDevice)
METHOD_REGEND

#endif

