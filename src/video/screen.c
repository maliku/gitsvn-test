/*! ============================================================================
 * @file screen.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "MIL_paint.h"

CONSTRUCTOR(MScreen)
{
    _private(MScreen)->depth = 0;
    _private(MScreen)->data = NULL;
    _private(MScreen)->dh = 0;
    _private(MScreen)->dw = 0;
    _private(MScreen)->grayscale = MIL_FALSE;
    _private(MScreen)->h = 0;
    _private(MScreen)->lstep = 0;
    _private(MScreen)->mapsize = 0;
    _private(MScreen)->physHeight = 0;
    _private(MScreen)->physWidth = 0;
    _private(MScreen)->screenclut = NULL;
    _private(MScreen)->screencols = 0;
    _private(MScreen)->size = 0;
    _private(MScreen)->w = 0;
}

DESTRUCTOR(MScreen)
{

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

void METHOD_NAMED(MScreen, blitRG)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Region* region)
{

}

MIL_Screen_Clsid METHOD_NAMED(MScreen, classID)(_CSelf(MScreen))
{

}

MIL_Color* METHOD_NAMED(MScreen, clut)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, colorCount)(_CSelf(MScreen))
{

}

MIL_Bool METHOD_NAMED(MScreen, connect)(_CSelf(MScreen), const char* display)
{

}

int  METHOD_NAMED(MScreen, depth)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(MScreen, deviceHeight)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(MScreen, deviceWidth)(_CSelf(MScreen))
{

}

void METHOD_NAMED(MScreen, disconnect)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, height)(_Self(MScreen))
{

}

MIL_Bool METHOD_NAMED(MScreen, initDevice)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, linestep)(_CSelf(MScreen))
{

}

MIL_Size  METHOD_NAMED(MScreen, mapFromDevice)(_CSelf(MScreen), const MIL_Size* size)
{

}

MIL_Size  METHOD_NAMED(MScreen, mapToDevice)(_CSelf(MScreen), const MIL_Size* size)
{

}

MIL_Status  METHOD_NAMED(MScreen, offest)(_CSelf(MScreen), MIL_Point* pt)
{

}

MIL_Bool METHOD_NAMED(MScreen, onCard)(_Self(MScreen), const char* ptr)
{

}

int  METHOD_NAMED(MScreen, physicalHeight)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(MScreen, physicalWidth)(_CSelf(MScreen))
{

}

const MIL_PixelFormat*  METHOD_NAMED(MScreen, pixelFormat)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(MScreen, pixmapDepth)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(MScreen, pixmapLinestepAlignment)(_CSelf(MScreen))
{

}

int  METHOD_NAMED(MScreen, pixmapOffsetAlignment)(_CSelf(MScreen))
{

}

void METHOD_NAMED(MScreen, restore)(_Self(MScreen))
{

}

void METHOD_NAMED(MScreen, save)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, screenSize)(_CSelf(MScreen))
{

}

void METHOD_NAMED(MScreen, setDirty)(_Self(MScreen), const MIL_Rect* rect)
{

}

void METHOD_NAMED(MScreen, setMode)(_Self(MScreen), int w, int h, int depth)
{

}

void METHOD_NAMED(MScreen, setPixelFormat)(_Self(MScreen), MIL_PixelFormat* format)
{

}

void METHOD_NAMED(MScreen, shutdownDevice)(_Self(MScreen))
{

}

void METHOD_NAMED(MScreen, solidFill)(_Self(MScreen), const MIL_Color* c, const MIL_Rect* rc)
{

}

int  METHOD_NAMED(MScreen, subScreenIndexAt)(_CSelf(MScreen), const MIL_Point* pt)
{

}

MIL_Bool METHOD_NAMED(MScreen, supportsDepth)(_Self(MScreen), int depth)
{

}

int  METHOD_NAMED(MScreen, totalSize)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, width)(_Self(MScreen))
{

}

BEGIN_METHOD_MAP(MScreen, NonBase)
    CONSTRUCTOR_MAP(MScreen)
    DESTRUCTOR_MAP(MScreen)
END_METHOD_MAP

