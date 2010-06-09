/*! ============================================================================
 * @file paint_engine.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "MIL_paint.h"

CONSTRUCTOR(MPaintEngine)
{
    _private(MPaintEngine)->act_device = NULL;
}

DESTRUCTOR(MPaintEngine)
{

}

MIL_Bool METHOD_NAMED(MPaintEngine, begin)(_Self(MPaintEngine), MPaintDevice* pdev)
{
    _private(MPaintDevice)->act_device = pdev;
}

void METHOD_NAMED(MPaintEngine, drawEllipse)(_Self(MPaintEngine), const MIL_Rect* rc)
{

}

void METHOD_NAMED(MPaintEngine, drawImage)(_Self(MPaintEngine), const MImage* image, const MIL_Rect* rc/* TODO: conversion flags */)
{

}

void METHOD_NAMED(MPaintEngine, drawLines)(_Self(MPaintEngine), const MLine* lines, int count)
{

}

void METHOD_NAMED(MPaintEngine, drawPath)(_Self(MPaintEngine), const MPainterPath* path)
{

}

void METHOD_NAMED(MPaintEngine, drawPixmap)
    (_Self(MPaintEngine), const MIL_Rect* r, const MPixmap* pm, const MIL_Rect* sr)
{

}

void METHOD_NAMED(MPaintEngine, drawPoints)(_Self(MPaintEngine), const MIL_Point* points, int count)
{

}

void METHOD_NAMED(MPaintEngine, drawPolygon)
    (_Self(MPaintEngine), const MIL_Point* pts, int count, MIL_PolygonMode mode)
{

}

void METHOD_NAMED(MPaintEngine, drawRects)(_Self(MPaintEngine), const MIL_Rect* rc, int count)
{

}

void METHOD_NAMED(MPaintEngine, drawText)(_Self(MPaintEngine), const MIL_Point* pt, const MText* text)
{

}

void METHOD_NAMED(MPaintEngine, drawTiledPixmap)
    (_Self(MPaintEngine), const MIL_Rect* rc, const MPixmap* pixmap, const MIL_Point* pt)
{

}

MIL_Bool METHOD_NAMED(MPaintEngine, end)(_Self(MPaintEngine))
{

}

MIL_Bool METHOD_NAMED(MPaintEngine, hasCapability)(_CSelf(MPaintEngine), int flag)
{

}

MIL_Bool METHOD_NAMED(MPaintEngine, isActive)(_CSelf(MPaintEngine))
{

}

MPaintDevice* METHOD_NAMED(MPaintEngine, CpaintDevice)(_Self(MPaintEngine))
{

}

MPainter* METHOD_NAMED(MPaintEngine, painter)(_CSelf(MPaintEngine))
{

}

MIL_PaintEngineType METHOD_NAMED(MPaintEngine, type)(_CSelf(MPaintEngine))
{

}

BEGIN_METHOD_MAP(MPaintEngine, NonBase)
    CONSTRUCTOR_MAP(MPaintEngine)
    DESTRUCTOR_MAP(MPaintEngine)
    METHOD_MAP(MPaintEngine, begin)
    METHOD_MAP(MPaintEngine, drawEllipse)
    METHOD_MAP(MPaintEngine, drawImage)
    METHOD_MAP(MPaintEngine, drawLines)
    METHOD_MAP(MPaintEngine, drawPath)
    METHOD_MAP(MPaintEngine, drawPixmap)
    METHOD_MAP(MPaintEngine, drawPoints)
    METHOD_MAP(MPaintEngine, drawPolygon)
    METHOD_MAP(MPaintEngine, drawRects)
    METHOD_MAP(MPaintEngine, drawText)
    METHOD_MAP(MPaintEngine, drawTiledPixmap)
    METHOD_MAP(MPaintEngine, end)
    METHOD_MAP(MPaintEngine, hasCapability)
    METHOD_MAP(MPaintEngine, isActive)
    METHOD_MAP(MPaintEngine, CpaintDevice)
    METHOD_MAP(MPaintEngine, painter)
    METHOD_MAP(MPaintEngine, type)

END_METHOD_MAP

