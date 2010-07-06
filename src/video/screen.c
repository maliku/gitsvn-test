/*! ============================================================================
 * @file screen.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "MIL_paint.h"
#include "pixel_format.h"


CONSTRUCTOR(MScreen)
{
    _private(MScreen)->depth = 0;
    _private(MScreen)->data = NULL;
    _private(MScreen)->dh = 0;
    _private(MScreen)->dw = 0;
    _private(MScreen)->grayscale = MIL_FALSE;
    _private(MScreen)->h = 0;
    _private(MScreen)->pitch = 0;
    _private(MScreen)->mapsize = 0;
    _private(MScreen)->physHeight = 0;
    _private(MScreen)->physWidth = 0;
    _private(MScreen)->screenclut = NULL;
    _private(MScreen)->screencols = 0;
    _private(MScreen)->size = 0;
    _private(MScreen)->w = 0;
    _private(MScreen)->format = (MIL_PixelFormat*)New(PixelFormat);
}

DESTRUCTOR(MScreen)
{
    MIL_UnRef(_private(MScreen)->format);
}

int  METHOD_NAMED(MScreen, colorIndex)(_Self(MScreen), Uint32 r, Uint32 g, Uint32 b)
{

}

Uint8* METHOD_NAMED(MScreen, baseAddr)(_CSelf(MScreen))
{
    return _private(MScreen)->data;
}

void METHOD_NAMED(MScreen, blit)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Rect* rc)
{
}

void METHOD_NAMED(MScreen, blitRg)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Region* region)
{

}

MIL_Screen_Clsid METHOD_NAMED(MScreen, classID)(_CSelf(MScreen))
{
    return MIL_UNKNOW_SCREEN;
}

MIL_Color* METHOD_NAMED(MScreen, clut)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, colorCount)(_CSelf(MScreen))
{
    return (1 << 
        _c(_private(MScreen)->format)->getBytesPerPixel(_private(MScreen)->format));
}

MIL_Bool METHOD_NAMED(MScreen, connect)(_CSelf(MScreen), const char* display)
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, depth)(_CSelf(MScreen))
{
    return 
        _c(_private(MScreen)->format)->getBitsPerPixel(_private(MScreen)->format);
}

int  METHOD_NAMED(MScreen, deviceHeight)(_CSelf(MScreen))
{
    return _private(MScreen)->dh;
}

int  METHOD_NAMED(MScreen, deviceWidth)(_CSelf(MScreen))
{
    return _private(MScreen)->dw;
}

void METHOD_NAMED(MScreen, disconnect)(_Self(MScreen))
{
}

int  METHOD_NAMED(MScreen, height)(_Self(MScreen))
{
    return _private(MScreen)->h;
}

MIL_Bool METHOD_NAMED(MScreen, initDevice)(_Self(MScreen))
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, linestep)(_CSelf(MScreen))
{
    return _private(MScreen)->pitch;
}

MIL_Size  METHOD_NAMED(MScreen, mapFromDevice)(_CSelf(MScreen), const MIL_Size* size)
{
    return *size;
}

MIL_Size  METHOD_NAMED(MScreen, mapToDevice)(_CSelf(MScreen), const MIL_Size* size)
{
    return *size;
}

MIL_Status  METHOD_NAMED(MScreen, offest)(_CSelf(MScreen), MIL_Point* pt)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Bool METHOD_NAMED(MScreen, onCard)(_Self(MScreen), const char* ptr)
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, physicalHeight)(_CSelf(MScreen))
{
    return _private(MScreen)->physHeight;
}

int  METHOD_NAMED(MScreen, physicalWidth)(_CSelf(MScreen))
{
    return _private(MScreen)->physWidth;
}

const MIL_PixelFormat*  METHOD_NAMED(MScreen, pixelFormat)(_CSelf(MScreen))
{
    return _private(MScreen)->format;
}

int  METHOD_NAMED(MScreen, pixmapDepth)(_CSelf(MScreen))
{
    return _c(_private(MScreen)->format)->getBitsPerPixel(_private(MScreen)->format);
}

int  METHOD_NAMED(MScreen, pixmapLinestepAlignment)(_CSelf(MScreen))
{
    return 64;
}

int  METHOD_NAMED(MScreen, pixmapOffsetAlignment)(_CSelf(MScreen))
{
    return 64;
}

void METHOD_NAMED(MScreen, restore)(_Self(MScreen))
{

}

void METHOD_NAMED(MScreen, save)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, screenSize)(_CSelf(MScreen))
{
    return _private(MScreen)->mapsize;
}

void METHOD_NAMED(MScreen, setDirty)(_Self(MScreen), int numrects, const MIL_Rect* rects)
{
}

void METHOD_NAMED(MScreen, setMode)(_Self(MScreen), int width, int height, int depth)
{
}

void METHOD_NAMED(MScreen, setPixelFormat)(_Self(MScreen), MIL_PixelFormat* format)
{
    if (NULL != format) {
        MIL_Ref(format);
        MIL_UnRef(_private(MScreen)->format);
        _private(MScreen)->format = format;
    }
}

void METHOD_NAMED(MScreen, shutdownDevice)(_Self(MScreen))
{
    /* TODO: Hide mouse cursor. */
}

void METHOD_NAMED(MScreen, solidFill)(_Self(MScreen), const MIL_Color* c, const MIL_Rect* rc)
{

}

int  METHOD_NAMED(MScreen, subScreenIndexAt)(_CSelf(MScreen), const MIL_Point* pt)
{
    return -1;
}

MIL_Bool METHOD_NAMED(MScreen, supportsDepth)(_Self(MScreen), int depth)
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, totalSize)(_Self(MScreen))
{
    return _private(MScreen)->mapsize;
}

int  METHOD_NAMED(MScreen, width)(_Self(MScreen))
{
    return _private(MScreen)->w;
}

BEGIN_METHOD_MAP(MScreen, NonBase)

    CONSTRUCTOR_MAP(MScreen)
    DESTRUCTOR_MAP(MScreen)
    METHOD_MAP(MScreen, colorIndex)
    METHOD_MAP(MScreen, baseAddr)
    METHOD_MAP(MScreen, blit)
    METHOD_MAP(MScreen, blitRg)
    METHOD_MAP(MScreen, classID)
    METHOD_MAP(MScreen, clut)
    METHOD_MAP(MScreen, colorCount)
    METHOD_MAP(MScreen, connect)
    METHOD_MAP(MScreen, depth)
    METHOD_MAP(MScreen, deviceHeight)
    METHOD_MAP(MScreen, deviceWidth)
    METHOD_MAP(MScreen, disconnect)
    METHOD_MAP(MScreen, height)
    METHOD_MAP(MScreen, initDevice)
    METHOD_MAP(MScreen, linestep)
    METHOD_MAP(MScreen, mapFromDevice)
    METHOD_MAP(MScreen, mapToDevice)
    METHOD_MAP(MScreen, offest)
    METHOD_MAP(MScreen, onCard)
    METHOD_MAP(MScreen, physicalHeight)
    METHOD_MAP(MScreen, physicalWidth)
    METHOD_MAP(MScreen, pixelFormat)
    METHOD_MAP(MScreen, pixmapDepth)
    METHOD_MAP(MScreen, pixmapLinestepAlignment)
    METHOD_MAP(MScreen, pixmapOffsetAlignment)
    METHOD_MAP(MScreen, restore)
    METHOD_MAP(MScreen, save)
    METHOD_MAP(MScreen, screenSize)
    METHOD_MAP(MScreen, setDirty)
    METHOD_MAP(MScreen, setMode)
    METHOD_MAP(MScreen, setPixelFormat)
    METHOD_MAP(MScreen, shutdownDevice)
    METHOD_MAP(MScreen, solidFill)
    METHOD_MAP(MScreen, subScreenIndexAt)
    METHOD_MAP(MScreen, supportsDepth)
    METHOD_MAP(MScreen, totalSize)
    METHOD_MAP(MScreen, width)

END_METHOD_MAP

#include "qvfb/screen_qvfb.h"
#define MAX_SCREENS 16
MScreen* g_screen_table[MAX_SCREENS];
int CreateScreen(const char* device_name, int w, int h, int bpp)
{
    g_screen_table[0] = (MScreen*)New(ScreenQVFB);
    if (NULL != g_screen_table[0]) {
        if (_c(g_screen_table[0])->initDevice(g_screen_table[0]))
        _c(g_screen_table[0])->setMode(g_screen_table[0], w, h, bpp);
        return 0;
    }
    return -1;
}

/* TODO: Complete it. */
MScreen* MIL_TakeScreen(int index)
{
    if (NULL == g_screen_table[index]) {
        CreateScreen("qvfb", 640, 480, 32);
    }
    return g_screen_table[index];
}

