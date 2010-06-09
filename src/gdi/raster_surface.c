/*! ============================================================================
 * @file raster_surface.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "MIL_paint.h"

CONSTRUCTOR(MRasterSurface)
{
    _private(MRasterSurface)->memory = NULL;
    _private(MRasterSurface)->format = NULL;
}

DESTRUCTOR(MRasterSurface)
{
    MIL_free(_private(MRasterSurface)->memory);
    MIL_free(_private(MRasterSurface)->format);
}

int METHOD_NAMED(MRasterSurface, pitch)(_CSelf(MRasterSurface))
{
    if (NULL != _private(MRasterSurface)->format) {
        return _c(_private(MRasterSurface)->format)->getBytesPerPixel(_private(MRasterSurface)->format);
    }
    return 0;
}

const MIL_PixelFormat* METHOD_NAMED(MRasterSurface, format)(_CSelf(MRasterSurface))
{
    return _private(MRasterSurface)->format;
}

void* METHOD_NAMED(MRasterSurface, memory)(_CSelf(MRasterSurface))
{
    return _private(MRasterSurface)->memory;
}

BEGIN_METHOD_MAP(MRasterSurface, MPaintDevice)

    CONSTRUCTOR_MAP(MRasterSurface)
    DESTRUCTOR_MAP(MRasterSurface)
    METHOD_PLACEHOLDER(colorCount)
    METHOD_PLACEHOLDER(depth)
    METHOD_PLACEHOLDER(height)
    METHOD_PLACEHOLDER(logicalDpiX)
    METHOD_PLACEHOLDER(logicalDpiY)
    METHOD_PLACEHOLDER(paintEngine)
    METHOD_PLACEHOLDER(paintingActive)
    METHOD_PLACEHOLDER(physicalDpiX)
    METHOD_PLACEHOLDER(physicalDpiY)
    METHOD_PLACEHOLDER(width)

    METHOD_MAP(MRasterSurface, pitch)
    METHOD_MAP(MRasterSurface, format)
    METHOD_MAP(MRasterSurface, memory)

END_METHOD_MAP

