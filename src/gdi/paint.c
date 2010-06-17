/*! ============================================================================
 * @file paint.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "paint.h"

METHOD_MAP_PLACEHOLDER(MIL_Pen, MIL_GdiObject)

CONSTRUCTOR(SolidPen)
{
    _tm(SolidPen, color) = (MIL_Color*)MIL_malloc(sizeof(MIL_Color));
    _tm(SolidPen, brush) = NULL;
    _tm(SolidPen, dash) = MIL_DASH_SOLID; 
    _tm(SolidPen, line_join) = MIL_LINEJOIN_MITER; 
    _tm(SolidPen, width) = 1;
}

DESTRUCTOR(SolidPen)
{
    MIL_free(_tm(SolidPen, color));
}

MIL_Brush* METHOD_NAMED(SolidPen, getBrush)(_Self(MIL_Pen))
{
    return _tm(SolidPen, brush);
}

MIL_Status METHOD_NAMED(SolidPen, getColor)(_Self(MIL_Pen), MIL_Color* c)
{
    if (NULL != c) {
        memcpy(c, _tm(SolidPen, color), sizeof( *c ));
        return MIL_OK;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(SolidPen, getDashPattern)(_Self(MIL_Pen), float* dash_array, Uint32 count)
{
    return MIL_NOT_IMPLEMENTED;
}

Uint32 METHOD_NAMED(SolidPen, getDashPatternCount)(_Self(MIL_Pen))
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_DashStyle METHOD_NAMED(SolidPen, getDashStyle)(_Self(MIL_Pen))
{
    return _tm(SolidPen, dash);
}

MIL_LineJoin METHOD_NAMED(SolidPen, getLineJoin)(_Self(MIL_Pen))
{
    return _tm(SolidPen, line_join);
}

MIL_PenType METHOD_NAMED(SolidPen, getPenType)(_Self(MIL_Pen))
{
    return MIL_PEN_SOLIDCOLOR;
}

Uint32 METHOD_NAMED(SolidPen, getWidth)(_Self(MIL_Pen))
{
    return _tm(SolidPen, width);
}

MIL_Status METHOD_NAMED(SolidPen, setBrush)(_Self(MIL_Pen), const MIL_Brush* brush)
{
}

MIL_Status METHOD_NAMED(SolidPen, setColor)(_Self(MIL_Pen), const MIL_Color* c)
{
    if (NULL != c) {
        memcpy(_tm(SolidPen, color), c, sizeof( *c ));
        return MIL_OK;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(SolidPen, setDashPattern)(_Self(MIL_Pen), float* dash_array, Uint32 count)
{
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(SolidPen, setDashStyle)(_Self(MIL_Pen), MIL_DashStyle dash_style)
{
    if (dash_style >= MIL_DASH_SOLID 
            && dash_style <= MIL_DASH_CUSTOM) {
        _tm(SolidPen, dash) = dash_style;
        return MIL_OK;
    }
    return MIL_INVALID_PARAMETER;
}

MIL_Status METHOD_NAMED(SolidPen, setLineJoin)(_Self(MIL_Pen), MIL_LineJoin line_join)
{
    if (line_join >= MIL_LINEJOIN_MITER 
            && line_join <= MIL_LINEJOIN_MITERCLIPPED) {
        _tm(SolidPen, line_join) = line_join;
        return MIL_OK;
    }
    return MIL_INVALID_PARAMETER;
}

void METHOD_NAMED(SolidPen, setWidth)(_Self(MIL_Pen), Uint32 w)
{
    _tm(SolidPen, width) = w;
}

BEGIN_METHOD_MAP(SolidPen, MIL_Pen)
    CONSTRUCTOR_MAP(SolidPen)
    DESTRUCTOR_MAP(SolidPen)
    METHOD_MAP(SolidPen, getBrush)
    METHOD_MAP(SolidPen, getColor)
    METHOD_MAP(SolidPen, getDashPattern)
    METHOD_MAP(SolidPen, getDashPatternCount)
    METHOD_MAP(SolidPen, getDashStyle)
    METHOD_MAP(SolidPen, getLineJoin)
    METHOD_MAP(SolidPen, getPenType)
    METHOD_MAP(SolidPen, getWidth)
    METHOD_MAP(SolidPen, setBrush)
    METHOD_MAP(SolidPen, setColor)
    METHOD_MAP(SolidPen, setDashPattern)
    METHOD_MAP(SolidPen, setDashStyle)
    METHOD_MAP(SolidPen, setLineJoin)
    METHOD_MAP(SolidPen, setWidth)
END_METHOD_MAP

