/*! ============================================================================
 * @file screen_wvfb.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "MIL_config.h"

#ifdef MIL_SCREEN_WVFB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pixel_format.h"
#include "screen_wvfb.h"
#include "winfb.h"

/* ----------------------------------------------------------------------------------- */

CONSTRUCTOR(ScreenWVFB)
{
    _m(hw_data) = (ScreenWVFbPrivate*)MIL_malloc(sizeof (*_m(hw_data)));
}

DESTRUCTOR(ScreenWVFB)
{
    MIL_free(_m(hw_data));
}

void METHOD_NAMED(ScreenWVFB, setDirty)(_Self(MScreen), int numrects, 
        const MIL_Rect* rects)
{
    int i;
    _RECT bound;
    ScreenWVFbPrivate* data = _tm(ScreenWVFB, hw_data);

    win_FbLock ();

    bound = data->hdr->update;
    if (bound.right == -1) bound.right = 0;
    if (bound.bottom == -1) bound.bottom = 0;

    for (i = 0; i < numrects; i++) {
        _RECT rc = {rects[i].x, rects[i].y, 
                        rects[i].x + rects[i].w, rects[i].y + rects[i].h};
        if (is_rect_empty(&bound))
            bound = rc;
        else if (!is_rect_empty(&rc))
            get_bound_rect(&bound, &bound, &rc);
    }

    data->hdr->update = bound;
    data->hdr->dirty = MIL_TRUE;

    win_FbUnlock ();
}

#if 0
static GAL_VideoDevice *WVFB_CreateDevice (int devindex)
{
    GAL_VideoDevice *self;

    /* Initialize all variables that we clean on shutdown */
    self = (GAL_VideoDevice *)malloc(sizeof(GAL_VideoDevice));
    if (self) {
        memset (self, 0, (sizeof *self));
        self->hw_data = (struct GAL_PrivateVideoData *) malloc ((sizeof *self->hw_data));
    }

    if ((self == NULL) || (self->hw_data == NULL)) {
        GAL_OutOfMemory ();
        if (self) free (self);
        return (0);
    }
    memset (self->hw_data, 0, (sizeof *self->hw_data));

    /* Set the function pointers */
    self->VideoInit = WVFB_VideoInit;
    self->ListModes = WVFB_ListModes;
    self->SetVideoMode = WVFB_SetVideoMode;
    self->SetColors = WVFB_SetColors;
    self->VideoQuit = WVFB_VideoQuit;

    self->AllocHWSurface = WVFB_AllocHWSurface;
    self->FreeHWSurface = WVFB_FreeHWSurface;

    self->UpdateRects = WVFB_UpdateRects;

    self->CheckHWBlit = NULL;
    self->FillHWRect = NULL;
    self->SetHWColorKey = NULL;
    self->SetHWAlpha = NULL;

    self->free = WVFB_DeleteDevice;

    return self;
}

VideoBootStrap WVFB_bootstrap = {
    "wvfb", "Windows Virtual FrameBuffer",
    WVFB_Available, WVFB_CreateDevice
};
#endif

MIL_Bool METHOD_NAMED(ScreenWVFB, initDevice)(_Self(MScreen))
{
    ScreenWVFbPrivate* data = _tm(ScreenWVFB, hw_data);
    PixelFormat* vformat = DynamicCast(PixelFormat, _private(MScreen)->format);

    if (!win_FbAvailable()) return MIL_FALSE;

    data->shmrgn = win_FbInit (0, 0, 0);
    if ((int)data->shmrgn == -1 || data->shmrgn == NULL) {
        MIL_SetError ("SCREEN>WVFB: Unable to attach to virtual FrameBuffer server.\n");
        return MIL_FALSE;
    }
    data->hdr = (struct WVFbHeader *) data->shmrgn;

    vformat->BitsPerPixel = data->hdr->depth;

    switch (vformat->BitsPerPixel) {
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
            vformat->Rmask = 0x00FF0000;
            vformat->Gmask = 0x0000FF00;
            vformat->Bmask = 0x000000FF;
            break;
        default:
            MIL_SetError ("SCREEN>WVFB: Not supported depth: %d.\n", vformat->BitsPerPixel);
            return MIL_FALSE;
    }

    return MIL_TRUE;
}

void METHOD_NAMED(ScreenWVFB, setMode)(_Self(MScreen), int width, int height, int bpp)
{
    /* Set up the mode framebuffer */
//    current->flags = GAL_HWSURFACE | GAL_FULLSCREEN;
    _private(MScreen)->w = _tm(ScreenWVFB, hw_data)->hdr->width;
    _private(MScreen)->h = _tm(ScreenWVFB, hw_data)->hdr->height;
    _private(MScreen)->pitch = _tm(ScreenWVFB, hw_data)->hdr->linestep;
    _private(MScreen)->data = _tm(ScreenWVFB, hw_data)->shmrgn + _tm(ScreenWVFB, hw_data)->hdr->dataoffset;
}

#if 0
static int WVFB_SetColors(_THIS, int firstcolor, int ncolors, GAL_Color *colors)
{
    int i, pixel = firstcolor;

    for (i = 0; i < ncolors; i++) {
        self->hw_data->hdr->clut [pixel] 
                = (0xff << 24) | ((colors[i].r & 0xff) << 16) | ((colors[i].g & 0xff) << 8) | (colors[i].b & 0xff);
        pixel ++;
    }

    return 0;
}
#endif

void METHOD_NAMED(ScreenWVFB, shutdownDevice)(_Self(MScreen))
{
    win_FbClose ();
}

BEGIN_METHOD_MAP(ScreenWVFB, MScreen)
    CONSTRUCTOR_MAP(ScreenWVFB)
    DESTRUCTOR_MAP(ScreenWVFB)

    METHOD_MAP(ScreenWVFB, initDevice)
    METHOD_MAP(ScreenWVFB, setDirty)
    METHOD_MAP(ScreenWVFB, setMode)
    METHOD_MAP(ScreenWVFB, shutdownDevice)

END_METHOD_MAP
#endif /* MIL_SCREEN_WVFB */
