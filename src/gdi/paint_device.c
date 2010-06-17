/*! ============================================================================
 * @file paint_device.c 
 * @Synopsis Base PaintDevice Impl. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "MIL_paint.h"

CONSTRUCTOR(MPaintDevice)
{
    _private(MPaintDevice)->w = 0;
    _private(MPaintDevice)->h = 0;
    _private(MPaintDevice)->logic_dpix = 0;
    _private(MPaintDevice)->logic_dpiy = 0;
    _private(MPaintDevice)->phys_dpix = 0;
    _private(MPaintDevice)->phys_dpiy = 0;
    _private(MPaintDevice)->depth = 0;
    _private(MPaintDevice)->paint_engine = NULL;
}

/* DESTRUCTOR(MPaintDevice)
 * {
 * 
 * }
 */

int METHOD_NAMED(MPaintDevice, colorCount)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->depth >= 32 ? INT_MAX : 
        (1 << _private(MPaintDevice)->depth);
}

int METHOD_NAMED(MPaintDevice, depth)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->depth;
}

int METHOD_NAMED(MPaintDevice, height)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->h;
}

int METHOD_NAMED(MPaintDevice, logicalDpiX)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->logic_dpix;
}

int METHOD_NAMED(MPaintDevice, logicalDpiY)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->logic_dpiy;
}

MPaintEngine* METHOD_NAMED(MPaintDevice, paintEngine)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->paint_engine;
}

MIL_Bool METHOD_NAMED(MPaintDevice, paintingActive)(_CSelf(MPaintDevice))
{
    if (NULL != _private(MPaintDevice)->paint_engine) {
        MPainter* painter = _c(_private(MPaintDevice)->paint_engine)->painter(_private(MPaintDevice)->paint_engine);
        if (NULL != painter) {
            return _c(painter)->isActive(painter);
        }
    }
    return MIL_FALSE;
}

int METHOD_NAMED(MPaintDevice, physicalDpiX)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->phys_dpix;
}

int METHOD_NAMED(MPaintDevice, physicalDpiY)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->phys_dpiy;
}

int METHOD_NAMED(MPaintDevice, width)(_CSelf(MPaintDevice))
{
    return _private(MPaintDevice)->w;
}

BEGIN_METHOD_MAP(MPaintDevice, NonBase)

    CONSTRUCTOR_MAP(MPaintDevice)
    METHOD_MAP(MPaintDevice, colorCount)
    METHOD_MAP(MPaintDevice, depth)
    METHOD_MAP(MPaintDevice, height)
    METHOD_MAP(MPaintDevice, logicalDpiX)
    METHOD_MAP(MPaintDevice, logicalDpiY)
    METHOD_MAP(MPaintDevice, paintEngine)
    METHOD_MAP(MPaintDevice, paintingActive)
    METHOD_MAP(MPaintDevice, physicalDpiX)
    METHOD_MAP(MPaintDevice, physicalDpiY)
    METHOD_MAP(MPaintDevice, width)

END_METHOD_MAP

