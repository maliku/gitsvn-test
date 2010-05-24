/*!============================================================================
 * @file MIL_gdi.h 
 * @Synopsis Graphics device interface. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  MIL_GDI_INC
#define  MIL_GDI_INC

#include "coo.h"
#include "MIL_video.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum  {
    MIL_IMG_NONE                = 0,
    MIL_IMG_SCALABLE            = 0x0001,
    MIL_IMG_HAS_ALPHA           = 0x0002,
    MIL_IMG_HAS_TRANSLUCENT     = 0x0004,
    MIL_IMG_PARTIALLY_SCALABLE  = 0x0008,
    MIL_IMG_COLOR_SPACE_RGB     = 0x0010,
    MIL_IMG_COLOR_SPACE_CMYK    = 0x0020,
    MIL_IMG_COLOR_SPACE_GRAY    = 0x0040,
    MIL_IMG_COLOR_SPACE_YCBCR   = 0x0080,
    MIL_IMG_COLOR_SPACE_YCCK    = 0x0100,
    MIL_IMG_HAS_REAL_DPI        = 0x1000,
    MIL_IMG_HAS_REAL_PIXEL_SIZE = 0x2000,
    MIL_IMG_READONLY            = 0x00010000,
    MIL_IMG_CACHING             = 0x00020000 
} MIL_ImageFlags;

typedef enum  {
    MIL_ROTATE_NONE_FLIP_NONE   = 0,
    MIL_ROTATE_90_FLIP_NONE     = 1,
    MIL_ROTATE_180_FLIP_NONE    = 2,
    MIL_ROTATE_270_FLIP_NONE    = 3,
    MIL_ROTATE_NONE_FLIP_X      = 4,
    MIL_ROTATE_90_FLIP_X        = 5,
    MIL_ROTATE_180_FLIP_X       = 6,
    MIL_ROTATE_270_FLIP_X       = 7,
    MIL_ROTATE_NONE_FLIP_Y      = MIL_ROTATE_180_FLIP_X,
    MIL_ROTATE_90_FLIP_Y        = MIL_ROTATE_270_FLIP_X,
    MIL_ROTATE_180_FLIP_Y       = MIL_ROTATE_NONE_FLIP_X,
    MIL_ROTATE_270_FLIP_Y       = MIL_ROTATE_90_FLIP_X,
    MIL_ROTATE_NONE_FLIP_XY     = MIL_ROTATE_180_FLIP_NONE,
    MIL_ROTATE_90_FLIP_XY       = MIL_ROTATE_270_FLIP_NONE,
    MIL_ROTATE_180_FLIP_XY      = MIL_ROTATE_NONE_FLIP_NONE,
    MIL_ROTATE_270_FLIP_XY      = MIL_ROTATE_90_FLIP_NONE 
} MIL_RotateFlipType;

STRUCT {
    Uint32 w;
    Uint32 h;
    Sint32  stride;
    void*  scan0;
//    MIL_PixelFormat* format;
}MIL_DIBitmapData;

CLASS(MIL_GdiObject)
{
    METHOD_DECLARE_BEGIN(MIL_GdiObject)
#define MIL_GDIOBJECT_METHOD_TABLE \
        MIL_Status (*getLastStatus)(_Self(MIL_GdiObject));\
        void (*addHoldRef)(_Self(MIL_GdiObject));\
        void (*decHoldRef)(_Self(MIL_GdiObject));
        MIL_GDIOBJECT_METHOD_TABLE 
    METHOD_DECLARE_END

    PRIVATE_BEGIN(MIL_GdiObject)
        int hold_counter;
    PRIVATE_END
};

/** 
 * @name MIL_Image
 * @brief A readonly container of kinds of image format.The Image object must be device-independent.
 */
CLASS_INHERIT_BEGIN(MIL_Image, MIL_GdiObject)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_Image)
#define MIL_IMAGE_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE \
        MIL_Image*  (*clone)(_Self(MIL_Image)); \
        MIL_Status  (*getBounds)(_Self(MIL_Image), MIL_Rect* rc); \
        Uint32      (*getWidth)(_Self(MIL_Image)); \
        Uint32      (*getHeight)(_Self(MIL_Image)); \
        MIL_Status  (*getPalette)(_Self(MIL_Image), MIL_Palette* palette); \
        MIL_Status  (*setPalette)(_Self(MIL_Image), const MIL_Palette* palette); \
        int         (*getPaletteSize)(_Self(MIL_Image)); \
        MIL_Status  (*getPixelFormat)(_Self(MIL_Image), MIL_PixelFormat* fmt); \
        const char* (*getRawFormat)(_Self(MIL_Image)); \
        MIL_Status  (*rotateFlip)(_Self(MIL_Image), MIL_RotateFlipType); \
        MIL_Status  (*save)(_Self(MIL_Status), const char* file); \
        MIL_Status  (*loadFile)(_Self(MIL_Image), const char* file);
        MIL_IMAGE_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END

    PRIVATE_BEGIN(MIL_Image)

    /** The width of the bitmap */
    Uint32 width;
    /** The height of the bitmap */
    Uint32 height;
    /** The pitch of the bitmap */
    Uint32 pitch;
    /** The bits of the bitmap */
    void*  bits;
    /** The private pixel format */
    MIL_PixelFormat* format;
    char* raw_format[16];

    PRIVATE_END
CLASS_INHERIT_END

/** 
 * @name MIL_DIBitmap
 * @brief A modifiable container for MIL_Image, you can change it's pixel data.
 */
CLASS_INHERIT_BEGIN(MIL_DIBitmap, MIL_Image)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_DIBitmap)
#define MIL_BITMAP_METHOD_TABLE \
        MIL_IMAGE_METHOD_TABLE \
        MIL_Status  (*getPixel)(_Self(MIL_DIBitmap), int x, int y, MIL_Color* color); \
        MIL_Status  (*lockBits)(_Self(MIL_DIBitmap), const MIL_Rect* rc, MIL_DIBitmapData* locked_data); \
        MIL_Status  (*setPixel)(_Self(MIL_DIBitmap), int x, int y, const MIL_Color* color); \
        MIL_Status  (*unlockBits)(_Self(MIL_DIBitmap), MIL_DIBitmapData* locked_data);
        MIL_BITMAP_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END

    PRIVATE_BEGIN(MIL_DIBitmap)
        MIL_DIBitmapData* data;
    PRIVATE_END
CLASS_INHERIT_END

typedef enum  {
    MIL_DASHSTYLE_SOLID        = 0,
    MIL_DASHSTYLE_DASH         = 1,
    MIL_DASHSTYLE_DOT          = 2,
    MIL_DASHSTYLE_DASHDOT      = 3,
    MIL_DASHSTYLE_DASHDOTDOT   = 4,
    MIL_DASHSTYLE_CUSTOM       = 5 
} MIL_DashStyle;

typedef enum  {
    MIL_LINEJOIN_MITER          = 0,
    MIL_LINEJOIN_BEVEL          = 1,
    MIL_LINEJOIN_ROUND          = 2,
    MIL_LINEJOIN_MITERCLIPPED   = 3 
} MIL_LineJoin;

typedef enum  {
    MIL_BRUSH_SOLIDCOLOR       = 0,
    MIL_BRUSH_HATCHFILL        = 1,
    MIL_BRUSH_TEXTUREFILL      = 2,
    MIL_BRUSH_PATHGRADIENT     = 3,
    MIL_BRUSH_LINEARGRADIENT   = 4 
} MIL_BrushType;

typedef enum  {
    MIL_PEN_SOLIDCOLOR       = MIL_BRUSH_SOLIDCOLOR,
    MIL_PEN_HATCHFILL        = MIL_BRUSH_HATCHFILL,
    MIL_PEN_TEXTUREFILL      = MIL_BRUSH_TEXTUREFILL,
    MIL_PEN_PATHGRADIENT     = MIL_BRUSH_PATHGRADIENT,
    MIL_PEN_LINEARGRADIENT   = MIL_BRUSH_LINEARGRADIENT,
    MIL_PEN_UNKNOWN          = -1 
} MIL_PenType;

typedef enum  {
    MIL_HATCH_HORIZONTAL               = 0,
    MIL_HATCH_VERTICAL                 = 1,
    MIL_HATCH_FORWARDDIAGONAL          = 2,
    MIL_HATCH_BACKWARDDIAGONAL         = 3,
    MIL_HATCH_CROSS                    = 4,
    MIL_HATCH_DIAGONALCROSS            = 5,
    /* TODO: and so on ...*/
} MIL_HatchStyle;

typedef enum  {
    MIL_WRAP_TILE         = 0,
    MIL_WRAP_TILEFLIPX    = 1,
    MIL_WRAP_TILEFLIPY    = 2,
    MIL_WRAP_TILEFLIPXY   = 3,
    MIL_WRAP_CLAMP        = 4 
} MIL_WrapMode;

typedef enum  {
    MIL_MATRIX_PREPEND   = 0,
    MIL_MATRIX_APPEND    = 1 
} MIL_MatrixOrder;

CLASS_INHERIT_BEGIN(MIL_Brush, MIL_GdiObject)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_Brush)
#define MIL_BRUSH_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_BrushType (*getType)(_Self(MIL_Brush));
        MIL_BRUSH_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MIL_Region, MIL_GdiObject)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_Region)
#define MIL_REGION_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_Status (*excludeRect)(_Self(MIL_Region), MIL_Rect*);\
        MIL_Status (*excludeRegion)(_Self(MIL_Region), MIL_Region*);
        MIL_REGION_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MIL_SolidBrush, MIL_Brush)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_SolidBrush)
#define MIL_SOLIDBRUSH_METHOD_TABLE \
        MIL_BRUSH_METHOD_TABLE \
        MIL_Status (*getColor)(_Self(MIL_SolidBrush), MIL_Color*);\
        MIL_Status (*setColor)(_Self(MIL_SolidBrush), const MIL_Color*);
        MIL_SOLIDBRUSH_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MIL_HatchBrush, MIL_Brush)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_HatchBrush)
#define MIL_HATCHBRUSH_METHOD_TABLE \
        MIL_BRUSH_METHOD_TABLE \
        MIL_Status (*getBackgroundColor)(_Self(MIL_HatchBrush), MIL_Color*);\
        MIL_Status (*getForegroundColor)(_Self(MIL_HatchBrush), MIL_Color*);\
        MIL_Status (*setBackgroundColor)(_Self(MIL_HatchBrush), const MIL_Color*);\
        MIL_Status (*setForegroundColor)(_Self(MIL_HatchBrush), const MIL_Color*);\
        MIL_HatchStyle (*getHatchStyle)(_Self(MIL_HatchBrush));\
        MIL_HatchStyle (*setHatchStyle)(_Self(MIL_HatchBrush), MIL_HatchBrush);
        MIL_HATCHBRUSH_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MIL_TextureBrush, MIL_Brush)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_TextureBrush)
#define MIL_TEXTUREBRUSH_METHOD_TABLE \
        MIL_BRUSH_METHOD_TABLE \
        MIL_Image* (*getImage)(_CSelf(MIL_TextureBrush));\
        MIL_WrapMode (*getWrapMode)(_CSelf(MIL_TextureBrush));\
        MIL_Status (*setWrapMode)(_Self(MIL_TextureBrush), MIL_WrapMode);
        MIL_TEXTUREBRUSH_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MIL_Pen, MIL_GdiObject)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_Pen)
#define MIL_PEN_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_Brush* (*getBrush)(_Self(MIL_Pen));\
        MIL_Status (*getColor)(_Self(MIL_Pen), MIL_Color*);\
        MIL_Status (*getDashPattern)(_Self(MIL_Pen), float* dash_array, Uint32 count);\
        Uint32 (*getDashPatternCount)(_Self(MIL_Pen));\
        MIL_DashStyle (*getDashStyle)(_Self(MIL_Pen));\
        MIL_LineJoin (*getLineJoin)(_Self(MIL_Pen));\
        MIL_PenType (*getPenType)(_Self(MIL_Pen));\
        Uint32 (*getWidth)(_Self(MIL_Pen));\
        MIL_Status (*setBrush)(_Self(MIL_Pen), const MIL_Brush*);\
        MIL_Status (*setColor)(_Self(MIL_Pen), const MIL_Color*);\
        MIL_Status (*setDashPattern)(_Self(MIL_Pen), float* dash_array, Uint32 count);\
        MIL_Status (*setDashStyle)(_Self(MIL_Pen), MIL_DashStyle);\
        MIL_Status (*setLineJoin)(_Self(MIL_Pen), MIL_LineJoin);\
        MIL_Status (*setWidth)(_Self(MIL_Pen), Uint32);
        MIL_PEN_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MIL_GraphicsContext, MIL_GdiObject)
    METHOD_EXPAND_DECLARE_BEGIN(MIL_GraphicsContext)
#define MIL_GC_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_Status (*clear)(_Self(MIL_GraphicsContext), MIL_Color*); \
        MIL_Status (*drawImagePos)(_Self(MIL_GraphicsContext), MIL_Image*, int, int); \
        MIL_Status (*drawImageRect)(_Self(MIL_GraphicsContext), MIL_Image*, MIL_Rect*); \
        MIL_Status (*drawString)(_Self(MIL_GraphicsContext), const char*, MIL_Rect*); \
        MIL_Status (*includeClip)(_Self(MIL_GraphicsContext), MIL_Rect*); \
        MIL_Status (*excludeClip)(_Self(MIL_GraphicsContext), MIL_Rect*); \
        MIL_Bool   (*isClipEmpty)(_Self(MIL_GraphicsContext)); \
        void       (*save)(_Self(MIL_GraphicsContext)); \
        void       (*restore)(_Self(MIL_GraphicsContext));
        MIL_GC_METHOD_TABLE
    METHOD_EXPAND_DECLARE_END
CLASS_INHERIT_END
/* Alias of MIL_GraphicsContext */
typedef MIL_GraphicsContext MIL_Graphics;

/** 
 * @synopsis Load image from a file.
 * 
 * @param file The path of file.
 * 
 * @returns Pointer of image object if success, NULL otherwise. 
 */
MIL_Image* LoadImageFromFile(const char* file);
MIL_DIBitmap* LoadBitmapFromFile(const char* file);

/*
 * A function to calculate the intersection of two rectangles.
 */
/** 
 * @synopsis  A function to calculate the intersection of two rectangles.
 * 
 * @param A The rectangle which will to join in the calculation.
 * @param B The rectangle which will to join in the calculation.
 * @param intersection The intersection rectangle of A and B.
 * @returns MIL_TRUE if the rectangles intersect, MIL_FALSE otherwise.
 */
extern DECLSPEC MIL_Bool
MIL_IntersectRect(const MIL_Rect *A, const MIL_Rect *B, MIL_Rect *intersection);

/** 
 * @synopsis Check the rect whether is empty. 
 * 
 * @param prc Rect will checked.
 * 
 * @returns MIL_TRUE if the rectangle is empty, MIL_FALSE otherwise.
 */
extern DECLSPEC MIL_Bool MIL_IsRectEmpty (const MIL_Rect* prc);

/** 
 * @synopsis Check two rectangles whether equivalent.
 * 
 * @param prc1 The rectangle which will to join in the calculation.
 * @param prc2 The rectangle which will to join in the calculation.
 * 
 * @returns MIL_TRUE if rectangles are equivalent, MIL_FALSE otherwise.  
 */
extern DECLSPEC MIL_Bool MIL_EqualRect (const MIL_Rect* prc1, const MIL_Rect* prc2);
extern DECLSPEC MIL_Bool MIL_DoesIntersect (const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC MIL_Bool MIL_UnionRect(MIL_Rect* pdrc, const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC void MIL_GetBoundRect (MIL_Rect* pdrc,  const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC void MIL_SetRectByCorner(MIL_Rect* prc, int left, int top, int right, int bottom);
extern DECLSPEC void MIL_SetRectBySize(MIL_Rect* prc, int x, int y, int w, int h);
extern DECLSPEC void MIL_OffsetRect(MIL_Rect* prc, int x, int y);
extern DECLSPEC void MIL_InflateRect(MIL_Rect* prc, int cx, int cy);
extern DECLSPEC void MIL_InflateRectToPt (MIL_Rect* prc, int x, int y);
extern DECLSPEC MIL_Bool MIL_PtInRect(const MIL_Rect* prc, int x, int y);
DECLSPEC int MIL_SubtractRect(MIL_Rect* rc, const MIL_Rect* psrc1, const MIL_Rect* psrc2);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef MIL_GDI_INC  ----- */

