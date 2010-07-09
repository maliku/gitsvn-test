/*! ============================================================================
 * @file screen_qvfb.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "screen_qvfb.h"
#include "pixel_format.h"

static int format_calc(MIL_PixelFormat* fmt, int bpp)
{
    PixelFormat* vformat = DynamicCast(PixelFormat, fmt);
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
            MIL_SetError("Screen>QVFb: Not supported depth\n");
            return -1;
    }
    return 0;
}


CONSTRUCTOR(ScreenQVFB)
{
    _m(hw_data) = (QVFbHardwareDependent*)MIL_malloc(sizeof (*_m(hw_data)));
}

DESTRUCTOR(ScreenQVFB)
{
    MIL_free(_m(hw_data));
}

int  METHOD_NAMED(ScreenQVFB, colorIndex)(_Self(MScreen), Uint32 r, Uint32 g, Uint32 b)
{

}

void METHOD_NAMED(ScreenQVFB, blit)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Rect* rc)
{
}

void METHOD_NAMED(ScreenQVFB, blitRg)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Region* region)
{
}

MIL_Screen_Clsid METHOD_NAMED(ScreenQVFB, classID)(_CSelf(MScreen))
{
    return MIL_QVFB_SCREEN;
}

int  METHOD_NAMED(ScreenQVFB, colorCount)(_CSelf(MScreen))
{

}

MIL_Bool METHOD_NAMED(ScreenQVFB, connect)(_CSelf(MScreen), const char* display)
{

}

void METHOD_NAMED(ScreenQVFB, disconnect)(_Self(MScreen))
{

}

MIL_Bool METHOD_NAMED(ScreenQVFB, initDevice)(_Self(MScreen))
{
    char file [MIL_MAX_PATH];
    int display = 0;
    key_t key;
    int shmid = 0;
    QVFbHardwareDependent* data = _tm(ScreenQVFB, hw_data);
    MIL_PixelFormat* vformat = _private(MScreen)->format;

//    if (GetMgEtcIntValue ("qvfb", "display", &display) < 0)
//        display = 0;

    sprintf (file, QVFB_MOUSE_PIPE, display);
    key = ftok (file, 'b');

    shmid = shmget (key, 0, 0);
    if (shmid != -1)
        data->shmrgn = (unsigned char *)shmat(shmid, 0, 0);

    if ((int)data->shmrgn == -1 || data->shmrgn == NULL) {
        MIL_SetError ("SCREEN>QVFb: Unable to attach to virtual FrameBuffer server.\n");
        return MIL_FALSE;
    }

    data->hdr = (QVFbHeader*) data->shmrgn;

    format_calc(vformat, data->hdr->depth);
    if (8 == _c(vformat)->getBitsPerPixel(vformat)) {
        data->hdr->numcols = 256;
    }
    return MIL_TRUE;
}

MIL_Size  METHOD_NAMED(ScreenQVFB, mapFromDevice)(_CSelf(MScreen), const MIL_Size* size)
{

}

MIL_Size  METHOD_NAMED(ScreenQVFB, mapToDevice)(_CSelf(MScreen), const MIL_Size* size)
{

}

MIL_Status  METHOD_NAMED(ScreenQVFB, offest)(_CSelf(MScreen), MIL_Point* pt)
{

}

MIL_Bool METHOD_NAMED(ScreenQVFB, onCard)(_Self(MScreen), const char* ptr)
{

}

int  METHOD_NAMED(ScreenQVFB, pixmapLinestepAlignment)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(ScreenQVFB, pixmapOffsetAlignment)(_CSelf(MScreen))
{

}

void METHOD_NAMED(ScreenQVFB, restore)(_Self(MScreen))
{

}

void METHOD_NAMED(ScreenQVFB, save)(_Self(MScreen))
{

}

__INLINE__ MIL_Bool is_rect_empty(RECT* prc)
{
    if( prc->left == prc->right ) return MIL_TRUE;
    if( prc->top == prc->bottom ) return MIL_TRUE;
    return MIL_FALSE;
}

__INLINE__ void normalize_rect(RECT* pRect)
{
    int iTemp;

    if(pRect->left > pRect->right)
    {
         iTemp = pRect->left;
         pRect->left = pRect->right;
         pRect->right = iTemp;
    }

    if(pRect->top > pRect->bottom)
    {
         iTemp = pRect->top;
         pRect->top = pRect->bottom;
         pRect->bottom = iTemp;
    }
}

__INLINE__ void get_bound_rect(RECT* pdrc, const RECT* psrc1, const RECT* psrc2)
{
    RECT src1, src2;
    memcpy(&src1, psrc1, sizeof(RECT));
    memcpy(&src2, psrc2, sizeof(RECT));

    normalize_rect(&src1);
    normalize_rect(&src2);

    pdrc->left = (src1.left < src2.left) ? src1.left : src2.left;
    pdrc->top  = (src1.top < src2.top) ? src1.top : src2.top;
    pdrc->right = (src1.right > src2.right) ? src1.right : src2.right;
    pdrc->bottom = (src1.bottom > src2.bottom) 
                   ? src1.bottom : src2.bottom;
}

void METHOD_NAMED(ScreenQVFB, setDirty)(_Self(MScreen), int numrects, 
        const MIL_Rect* rects)
{
    int i;

    RECT bound = _tm(ScreenQVFB, hw_data)->hdr->update;

    for (i = 0; i < numrects; ++i) {
        RECT rc = {rects[i].x, rects[i].y, 
                        rects[i].x + rects[i].w, rects[i].y + rects[i].h};

        if (is_rect_empty(&bound))
            bound = rc;
        else
            get_bound_rect(&bound, &bound, &rc);
    }

    _tm(ScreenQVFB, hw_data)->hdr->update = bound;
    _tm(ScreenQVFB, hw_data)->hdr->dirty = MIL_TRUE;
}

void METHOD_NAMED(ScreenQVFB, setMode)(_Self(MScreen), int w, int h, int depth)
{
    int real_w = _tm(ScreenQVFB, hw_data)->hdr->width;
    int real_h = _tm(ScreenQVFB, hw_data)->hdr->height;
    /* Set up the mode framebuffer */
    _private(MScreen)->w = _private(MScreen)->dw = 
        w <= real_w ? w : real_w;
    _private(MScreen)->h = _private(MScreen)->dh = 
        h <= real_h ? h : real_h;
    _private(MScreen)->pitch = _tm(ScreenQVFB, hw_data)->hdr->linestep;
    _private(MScreen)->data = _tm(ScreenQVFB, hw_data)->shmrgn + 
        _tm(ScreenQVFB, hw_data)->hdr->dataoffset;
    _private(MScreen)->depth = depth;
    _private(MScreen)->mapsize = _private(MScreen)->pitch * _private(MScreen)->h;
    if (NULL != _private(MScreen)->format) {
        MIL_UnRef(_private(MScreen)->format);
    }
    _private(MScreen)->format = MIL_AllocFormat(depth, 0, 0, 0, 0);
//    _c(current)->setClipRect(current, NULL);
//    if ( ! _c(current)->reallocFormat(current, bpp, 0, 0, 0, 0) ) {
//        return(NULL);
//	}
    format_calc(_private(MScreen)->format, depth);
}

void METHOD_NAMED(ScreenQVFB, shutdownDevice)(_Self(MScreen))
{
    shmdt(_tm(ScreenQVFB, hw_data)->shmrgn);
}

void METHOD_NAMED(ScreenQVFB, solidFill)(_Self(MScreen), const MIL_Color* c, const MIL_Rect* rc)
{
    if (NULL != rc && NULL != c) {
        Super(MScreen)->solidFill(self, c, rc);
        _c(self)->setDirty(self, 1, rc);
    }
}

int  METHOD_NAMED(ScreenQVFB, subScreenIndexAt)(_CSelf(MScreen), const MIL_Point* pt)
{

}

MIL_Bool METHOD_NAMED(ScreenQVFB, supportsDepth)(_Self(MScreen), int depth)
{

}

BEGIN_METHOD_MAP(ScreenQVFB, MScreen)
    CONSTRUCTOR_MAP(ScreenQVFB)
    DESTRUCTOR_MAP(ScreenQVFB)
    METHOD_MAP(ScreenQVFB, colorIndex)
    METHOD_MAP(ScreenQVFB, blit)
    METHOD_MAP(ScreenQVFB, blitRg)
    METHOD_MAP(ScreenQVFB, classID)
    METHOD_MAP(ScreenQVFB, colorCount)
    METHOD_MAP(ScreenQVFB, connect)
    METHOD_MAP(ScreenQVFB, disconnect)
    METHOD_MAP(ScreenQVFB, initDevice)
    METHOD_MAP(ScreenQVFB, mapFromDevice)
    METHOD_MAP(ScreenQVFB, mapToDevice)
    METHOD_MAP(ScreenQVFB, offest)
    METHOD_MAP(ScreenQVFB, onCard)
    METHOD_MAP(ScreenQVFB, pixmapLinestepAlignment)
    METHOD_MAP(ScreenQVFB, pixmapOffsetAlignment)
    METHOD_MAP(ScreenQVFB, restore)
    METHOD_MAP(ScreenQVFB, save)
    METHOD_MAP(ScreenQVFB, setDirty)
    METHOD_MAP(ScreenQVFB, setMode)
    METHOD_MAP(ScreenQVFB, shutdownDevice)
    METHOD_MAP(ScreenQVFB, solidFill)
    METHOD_MAP(ScreenQVFB, subScreenIndexAt)
    METHOD_MAP(ScreenQVFB, supportsDepth)

END_METHOD_MAP


