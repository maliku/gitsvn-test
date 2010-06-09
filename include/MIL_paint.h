/*!============================================================================
 * @file MIL_paint.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  MIL_PAINT_INC
#define  MIL_PAINT_INC

#include "coo.h"
#include "MIL_video.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define METHOD_TABLE(type) MIL_##type##_METHOD_TABLE

typedef enum {
    MIL_ENGINE_RASTER = 0,
    MIL_ENGINE_PICTURE = 1
} MIL_PaintEngineType;

typedef int MPainterPath;
typedef int MPixmap;
typedef int MFont;
typedef int MBrush;
typedef int MPen;
typedef int MRegion;
typedef int MFontInfo;
typedef int MFontMetrics;
typedef int MPicture;
typedef int MImage;
typedef int MIL_PolygonMode;
typedef char* MText;
typedef int MLine;
typedef int MWidget;

CLASS_FORWARD_DECLARE(MPaintDevice);
CLASS_FORWARD_DECLARE(MPainter);

CLASS(MPaintEngine)
{
    BEGIN_METHOD_DECLARE(MPaintEngine)
#define MIL_MPaintEngine_METHOD_TABLE \
        MIL_Bool (*begin)(_Self(MPaintEngine), MPaintDevice* pdev);\
        void (*drawEllipse)(_Self(MPaintEngine), const MIL_Rect*);\
        void (*drawImage)(_Self(MPaintEngine), const MImage*, const MIL_Rect* /* TODO: conversion flags */);\
        void (*drawLines)(_Self(MPaintEngine), const MLine*, int count);\
        void (*drawPath)(_Self(MPaintEngine), const MPainterPath*);\
        void (*drawPixmap)(_Self(MPaintEngine), const MIL_Rect* r, const MPixmap* pm, const MIL_Rect* sr);\
        void (*drawPoints)(_Self(MPaintEngine), const MIL_Point*, int count);\
        void (*drawPolygon)(_Self(MPaintEngine), const MIL_Point*, int count, MIL_PolygonMode);\
        void (*drawRects)(_Self(MPaintEngine), const MIL_Rect*, int count);\
        void (*drawText)(_Self(MPaintEngine), const MIL_Point*, const MText*);\
        void (*drawTiledPixmap)(_Self(MPaintEngine), const MIL_Rect*, const MPixmap*, const MIL_Point*);\
        MIL_Bool (*end)(_Self(MPaintEngine));\
        MIL_Bool (*hasCapability)(_CSelf(MPaintEngine), int);\
        MIL_Bool (*isActive)(_CSelf(MPaintEngine));\
        MPaintDevice* (*paintDevice)(_CSelf(MPaintEngine));\
        MPainter* (*painter)(_CSelf(MPaintEngine));\
        MIL_PaintEngineType (*type)(_CSelf(MPaintEngine));
        /* TODO: updateState */
    METHOD_TABLE(MPaintEngine)
    END_METHOD_DECLARE

    BEGIN_PRIVATE(MPaintEngine)
        MPaintDevice* act_device;
    END_PRIVATE
};

CLASS_NEED_FORWARD_DECLARE(MPaintDevice)
{
    BEGIN_METHOD_DECLARE(MPaintDevice)
#define MIL_MPaintDevice_METHOD_TABLE \
        int (*colorCount)(_CSelf(MPaintDevice));\
        int (*depth)(_CSelf(MPaintDevice));\
        int (*height)(_CSelf(MPaintDevice));\
        int (*logicalDpiX)(_CSelf(MPaintDevice));\
        int (*logicalDpiY)(_CSelf(MPaintDevice));\
        MPaintEngine* (*paintEngine)(_CSelf(MPaintDevice));\
        MIL_Bool (*paintingActive)(_CSelf(MPaintDevice));\
        int (*physicalDpiX)(_CSelf(MPaintDevice));\
        int (*physicalDpiY)(_CSelf(MPaintDevice));\
        int (*width)(_CSelf(MPaintDevice));
        METHOD_TABLE(MPaintDevice)
    END_METHOD_DECLARE

    BEGIN_PRIVATE(MPaintDevice)
        int w;
        int h;
        int logic_dpix;
        int logic_dpiy;
        int phys_dpix;
        int phys_dpiy;
        int depth;
        MPaintEngine* paint_engine;
    END_PRIVATE
};

CLASS_NEED_FORWARD_DECLARE(MPainter)
{
    BEGIN_METHOD_DECLARE(MPainter)
#define MIL_MPainter_METHOD_TABLE \
        const MBrush* (*background)(_CSelf(MPainter));\
        MIL_Bool (*begin)(_CSelf(MPainter), MPaintDevice* pdev);\
        void (*beginNativePainting)(_CSelf(MPainter));\
        MIL_Rect (*boundingRect)(_Self(MPainter), const MIL_Rect*, int flags, const char*);\
        MBrush (*brush)(_CSelf(MPainter));\
        MIL_Point (*brushOrigin)(_CSelf(MPainter));\
        MPainterPath (*clipPath)(_CSelf(MPainter));\
        MRegion (*clipRegion)(_CSelf(MPainter));\
        MPaintDevice* (*device)(_CSelf(MPainter));\
        void (*drawArcRC)(_Self(MPainter), const MIL_Rect*, int, int);\
        void (*drawArc)(_Self(MPainter), int, int, int, int, int, int);\
        void (*drawChordRC)(_Self(MPainter), const MIL_Rect*, int, int);\
        void (*drawChord)(_Self(MPainter), int, int, int, int, int, int);\
        void (*drawConvexPolygon)(_Self(MPainter), const MIL_Point*, int);\
        void (*drawEllipseRC)(_Self(MPainter), const MIL_Rect*);\
        void (*drawEllipse)(_Self(MPainter), int, int, int, int);\
        void (*drawImagePT)(_Self(MPainter), const MImage*, const MIL_Point*);\
        void (*drawImageRC)(_Self(MPainter), const MImage*, const MIL_Rect*);\
        void (*drawImageClipPT)(_Self(MPainter), const MImage*, const MIL_Rect*, const MIL_Point*);\
        void (*drawImageClip)(_Self(MPainter), int, int, const MImage*, int, int, int, int);\
        void (*drawLine)(_Self(MPainter), int, int, int, int);\
        void (*drawLinePT)(_Self(MPainter), const MIL_Point*, const MIL_Point*);\
        void (*drawLines)(_Self(MPainter), const MLine*, int);\
        void (*drawPath)(_Self(MPainter), const MPainterPath*);\
        void (*drawPicture)(_Self(MPainter), const MPicture*, int, int);\
        void (*drawPicturePT)(_Self(MPainter), const MPicture*, const MIL_Point*);\
        void (*drawPie)(_Self(MPainter), int, int, int, int, int, int);\
        void (*drawPieRC)(_Self(MPainter), const MIL_Rect*, int, int);\
        void (*drawPixmapPT)(_Self(MPainter), const MImage*, const MIL_Point*);\
        void (*drawPixmapRC)(_Self(MPainter), const MImage*, const MIL_Rect*);\
        void (*drawPixmapClipPT)(_Self(MPainter), const MImage*, const MIL_Rect*, const MIL_Point*);\
        void (*drawPixmapClip)(_Self(MPainter), int, int, const MImage*, int, int, int, int);\
        void (*drawPointPT)(_Self(MPainter), const MIL_Point*);\
        void (*drawPoint)(_Self(MPainter), int x, int y);\
        void (*drawPoints)(_Self(MPainter), const MIL_Point*, int);\
        void (*drawPolygon)(_Self(MPainter), const MIL_Point*, int);\
        void (*drawPolyline)(_Self(MPainter), const MIL_Point*, int);\
        void (*drawRect)(_Self(MPainter), const MIL_Rect*);\
        void (*drawRects)(_Self(MPainter), const MIL_Rect*, int);\
        void (*drawRoundRect)(_Self(MPainter), const MIL_Rect*, float, float);\
        void (*drawTextRC)(_Self(MPainter), const char* text, const MIL_Rect*);\
        void (*drawTextPT)(_Self(MPainter), const char* text, const MIL_Point*);\
        void (*drawText)(_Self(MPainter), const char* text, int, int);\
        void (*drawTilePixmap)(_Self(MPainter), const MIL_Rect* rect, const MPixmap*, const MIL_Point*);\
        MIL_Bool (*end)(_CSelf(MPainter));\
        void (*endNativePainting)(_CSelf(MPainter));\
        const MFont* (*font)(_CSelf(MPainter));\
        const MFontInfo* (*fontInfo)(_CSelf(MPainter));\
        const MFontMetrics* (*fontMetrics)(_CSelf(MPainter));\
        MIL_Bool (*hasClipping)(_CSelf(MPainter));\
        void (*initFromWidget)(_Self(MPainter), const MWidget*);\
        MIL_Bool (*isActive)(_CSelf(MPainter));\
        int  (*layoutDirection)(_CSelf(MPainter));\
        float  (*opacityF)(_CSelf(MPainter));\
        int  (*opacity)(_CSelf(MPainter));\
        MPaintEngine* (*paintEngine)(_CSelf(MPainter));\
        MPen* (*pen)(_CSelf(MPainter));\
        void (*resetTransform)(_Self(MPainter));\
        void (*restore)(_Self(MPainter));\
        void (*rotateF)(_Self(MPainter), float);\
        void (*save)(_Self(MPainter));\
        void (*scaleF)(_Self(MPainter), float, float);\
        void (*setBackground)(_Self(MPainter), const MBrush*);\
        void (*setBrush)(_Self(MPainter), const MBrush*);\
        void (*setBrushOriginPT)(_Self(MPainter), const MBrush*, const MIL_Point*);\
        void (*setBrushOrigin)(_Self(MPainter), const MBrush*, int, int);\
        void (*setClipPath)(_Self(MPainter), const MPainterPath*);\
        void (*setClipRect)(_Self(MPainter), const MIL_Rect*);\
        void (*setClipRegion)(_Self(MPainter), const MRegion*);\
        void (*setClipping)(_Self(MPainter), MIL_Bool);\
        void (*setCompositionMode)(_Self(MPainter), int);\
        void (*setFont)(_Self(MPainter), const MFont*);\
        void (*setLayoutDirection)(_Self(MPainter), int);\
        void (*setOpacity)(_Self(MPainter), int);\
        void (*setOpacityF)(_Self(MPainter), float);\
        void (*setPen)(_Self(MPainter), const MPen*);\
        void (*setPenC)(_Self(MPainter), const MIL_Color*);\
        void (*setRenderHints)(_Self(MPainter), int, MIL_Bool);\
        void (*setViewport)(_Self(MPainter), int, int, int, int);\
        void (*setViewportRC)(_Self(MPainter), const MIL_Rect*);\
        void (*setWindow)(_Self(MPainter), int, int, int, int);\
        void (*setWindowRC)(_Self(MPainter), const MIL_Rect*);\
        void (*setWorldMatrixEnable)(_Self(MPainter), MIL_Bool);\
        void (*share)(_Self(MPainter), float, float);\
        void (*strokePath)(_Self(MPainter), const MPainterPath*, const MPen*);\
        MIL_Bool (*testRenderHint)(_Self(MPainter), int);\
        void (*translate)(_Self(MPainter), int, int);\
        void (*translatePT)(_Self(MPainter), const MIL_Point*);\
        MIL_Bool (*viewTransformEnabled)(_CSelf(MPainter));\
        MIL_Rect (*viewport)(_CSelf(MPainter));\
        MIL_Rect (*window)(_CSelf(MPainter));\
        MIL_Bool (*worldMatrixEnabled)(_CSelf(MPainter));

        METHOD_TABLE(MPainter)
    END_METHOD_DECLARE

    BEGIN_PRIVATE(MPainter)
        MPaintEngine* paint_engine;
        MBrush* brush;
        MPen* pen;
    END_PRIVATE
};

BEGIN_CLASS_INHERIT(MRasterSurface, MPaintDevice)
    BEGIN_METHOD_EXPAND_DECLARE(MRasterSurface)
#define MIL_MRasterSurface_METHOD_TABLE \
        MIL_MPaintDevice_METHOD_TABLE\
        int (*pitch)(_CSelf(MRasterSurface));\
        const MIL_PixelFormat* (*format)(_CSelf(MRasterSurface));\
        void* (*memory)(_CSelf(MRasterSurface));
        METHOD_TABLE(MRasterSurface)
    END_METHOD_EXPAND_DECLARE

    BEGIN_PRIVATE(MRasterSurface)
    void* memory;
    MIL_PixelFormat* format;
    END_PRIVATE

END_CLASS_INHERIT

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef MIL_PAINT_INC  ----- */

