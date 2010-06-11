/*! ============================================================================
 * @file raster_engine.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */
#include "raster_engine.h"
#include "misc.h"

CONSTRUCTOR(MRasterEngine)
{
    return self;
}

DESTRUCTOR(MRasterEngine)
{

}

MIL_Bool METHOD_NAMED(MRasterEngine, begin)(_Self(MPaintEngine), MPaintDevice* pdev)
{
}

void METHOD_NAMED(MRasterEngine, drawEllipse)(_Self(MPaintEngine), const MIL_Rect* rc)
{

}

void METHOD_NAMED(MRasterEngine, drawImage)(_Self(MPaintEngine), const MImage* image, const MIL_Rect* rc/* TODO: conversion flags */)
{

}

void METHOD_NAMED(MRasterEngine, drawLines)(_Self(MPaintEngine), const MLine* lines, int count)
{

}

void METHOD_NAMED(MRasterEngine, drawPath)(_Self(MPaintEngine), const MPainterPath* path)
{

}

void METHOD_NAMED(MRasterEngine, drawPixmap)
    (_Self(MPaintEngine), const MIL_Rect* r, const MPixmap* pm, const MIL_Rect* sr)
{

}

void METHOD_NAMED(MRasterEngine, drawPoints)(_Self(MPaintEngine), const MIL_Point* points, int count)
{

}

void METHOD_NAMED(MRasterEngine, drawPolygon)
    (_Self(MPaintEngine), const MIL_Point* pts, int count, MIL_PolygonMode mode)
{

}

void METHOD_NAMED(MRasterEngine, drawRects)(_Self(MPaintEngine), const MIL_Rect* rc, int count)
{

}

void METHOD_NAMED(MRasterEngine, drawText)(_Self(MPaintEngine), const MIL_Point* pt, const MText* text)
{

}

void METHOD_NAMED(MRasterEngine, drawTiledPixmap)
    (_Self(MPaintEngine), const MIL_Rect* rc, const MPixmap* pixmap, const MIL_Point* pt)
{

}

MIL_Bool METHOD_NAMED(MRasterEngine, end)(_Self(MPaintEngine))
{

}

MIL_Bool METHOD_NAMED(MRasterEngine, hasCapability)(_CSelf(MPaintEngine), int flag)
{

}

MIL_Bool METHOD_NAMED(MRasterEngine, isActive)(_CSelf(MPaintEngine))
{

}

MPainter* METHOD_NAMED(MRasterEngine, painter)(_CSelf(MPaintEngine))
{

}

MIL_PaintEngineType METHOD_NAMED(MRasterEngine, type)(_CSelf(MPaintEngine))
{
    return MIL_PE_RASTER;
}

BEGIN_METHOD_MAP(MRasterEngine, MPaintEngine)
    CONSTRUCTOR_MAP(MRasterEngine)
    DESTRUCTOR_MAP(MRasterEngine)
    METHOD_MAP(MRasterEngine, begin)
    METHOD_MAP(MRasterEngine, drawEllipse)
    METHOD_MAP(MRasterEngine, drawImage)
    METHOD_MAP(MRasterEngine, drawLines)
    METHOD_MAP(MRasterEngine, drawPath)
    METHOD_MAP(MRasterEngine, drawPixmap)
    METHOD_MAP(MRasterEngine, drawPoints)
    METHOD_MAP(MRasterEngine, drawPolygon)
    METHOD_MAP(MRasterEngine, drawRects)
    METHOD_MAP(MRasterEngine, drawText)
    METHOD_MAP(MRasterEngine, drawTiledPixmap)
    METHOD_MAP(MRasterEngine, end)
    METHOD_MAP(MRasterEngine, hasCapability)
    METHOD_MAP(MRasterEngine, isActive)
    METHOD_MAP(MRasterEngine, painter)
    METHOD_MAP(MRasterEngine, type)

END_METHOD_MAP

