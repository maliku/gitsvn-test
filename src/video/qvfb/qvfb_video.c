/*! ============================================================================
 * @file qvfb_video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月20日
 *  Organization: http://www.ds0101.net
 */

#include "surface.h"
#include "qvfb_video.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

VideoDevice* CreateQVFbVideoDevice(void)
{
    MIL_PixelFormat vf;
    Surface *s = New(Surface);
    VideoDevice* vd = (VideoDevice*)New(QVFbVideoDevice);
    _VC(vd)->videoInit(vd, &vf);
    _VC(vd)->setVideoMode(vd, (MIL_Surface*)s, 240, 320, 32, 0);
    if (NULL != s->pixels) {
        MIL_Rect rc = {0, 0, 240, 320};
        int i, j;
        for (i = 0; i < 320; ++i)
            for (j = 0; j < 240; ++j)
                *(Uint32*)(((char*)s->pixels) + i * _VC(s)->getPitch((MIL_Surface*)s) + j) = i;
        _VC(vd)->updateRects(vd, 1, &rc);
        puts("draw");
        getchar();
    }
    return vd;
}

CONSTRUCTOR(QVFbVideoDevice)
{
    printf("QVFbVideoDevice %p constructed...\n", self);
    _m(hw_data) = (QVFbHardwareDependent*)MIL_malloc(sizeof (*_m(hw_data)));
    return self;
}

DESTRUCTOR(QVFbVideoDevice)
{
    printf("QVFbVideoDevice %p destructed...\n", self);
    MIL_free(((QVFbVideoDevice*)self)->hw_data);
}

int METHOD_NAMED(QVFbVideoDevice, videoInit)(_Self(VideoDevice), MIL_PixelFormat *vformat)
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
        data->shmrgn = (unsigned char *)shmat (shmid, 0, 0);

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

MIL_Rect** METHOD_NAMED(QVFbVideoDevice, listModes)(_Self(VideoDevice), 
        MIL_PixelFormat *format, Uint32 flags)
{
    return (MIL_Rect **) NULL;
}

MIL_Surface* METHOD_NAMED(QVFbVideoDevice, setVideoMode)(_Self(VideoDevice), 
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

int METHOD_NAMED(QVFbVideoDevice, setColors)(_Self(VideoDevice), int firstcolor, 
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

void METHOD_NAMED(QVFbVideoDevice, updateRects)(_Self(VideoDevice), int numrects, 
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

void METHOD_NAMED(QVFbVideoDevice, videoQuit)(_Self(VideoDevice))
{
    shmdt(((QVFbVideoDevice*)self)->hw_data->shmrgn);
}

int METHOD_NAMED(QVFbVideoDevice, allocHWSurface)(_Self(VideoDevice), MIL_Surface *surface)
{
    return -1;
}

VIRTUAL_METHOD_REGBEGIN(QVFbVideoDevice, VideoDevice)
    DESTRUCTOR_REGISTER(QVFbVideoDevice)
    METHOD_REGISTER(QVFbVideoDevice, videoInit)
    METHOD_REGISTER(QVFbVideoDevice, listModes)
    METHOD_REGISTER(QVFbVideoDevice, setVideoMode)
    PLACEHOLDER(toggleFullScreen),
    PLACEHOLDER(updateMouse),
    PLACEHOLDER(createYUVOverlay),
    METHOD_REGISTER(QVFbVideoDevice, setColors)
    METHOD_REGISTER(QVFbVideoDevice, updateRects)
    METHOD_REGISTER(QVFbVideoDevice, videoQuit)
    METHOD_REGISTER(QVFbVideoDevice, allocHWSurface)
    PLACEHOLDER(checkHWBlit),
    PLACEHOLDER(fillHWRect),
    PLACEHOLDER(setHWColorKey),
    PLACEHOLDER(setHWAlpha),
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(QVFbVideoDevice)
    CONSTRUCTOR_REGISTER(QVFbVideoDevice)
METHOD_REGEND
