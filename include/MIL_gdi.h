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
#include "MIL_paint.h"

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

/** 
 * @struct MIL_Bitmap
 * @brief A MIL_Bitmap object stores attributes of a bitmap.
 */
STRUCT {
    MIL_PixelFormat* format;
    Uint32 w;
    Uint32 h;
    Sint32 pitch;
    void*  scan0;
} MIL_BitmapData;

/** 
 * @class MIL_GdiObject
 * @brief MIL_GdiObject serves as a base class for all other GDI classes, 
 * so you never need to create an instance of MIL_GdiObject.
 */
CLASS(MIL_GdiObject)
{
    BEGIN_METHOD_DECLARE(MIL_GdiObject)
#define MIL_GDIOBJECT_METHOD_TABLE \
        MIL_GdiObject* (*ref)(_SELF, int type);\
        void (*unRef)(_SELF, int type);\
        int (*getRef)(_SELF, int type);
        MIL_GDIOBJECT_METHOD_TABLE 
    END_METHOD_DECLARE

    BEGIN_PRIVATE(MIL_GdiObject)
        int counters[3];
    END_PRIVATE
};

/**
 * @class MIL_Image
 * @brief A readonly container of kinds of image format.The Image object must be device-independent.
 */
BEGIN_CLASS_INHERIT(MIL_Image, MIL_GdiObject)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_Image)
#define MIL_IMAGE_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE \
        MIL_Image*  (*clone)(_Self(MIL_Image)); \
        MIL_Status  (*getBounds)(_Self(MIL_Image), MIL_Rect* rc); \
        Uint32      (*getWidth)(_Self(MIL_Image)); \
        Uint32      (*getHeight)(_Self(MIL_Image)); \
        MIL_Status  (*getPalette)(_Self(MIL_Image), MIL_Palette* palette); \
        MIL_Status  (*setPalette)(_Self(MIL_Image), const MIL_Palette* palette); \
        int         (*getPaletteSize)(_Self(MIL_Image)); \
        const MIL_PixelFormat* (*getPixelFormat)(_Self(MIL_Image)); \
        const char* (*getRawFormat)(_Self(MIL_Image)); \
        MIL_Status  (*rotateFlip)(_Self(MIL_Image), MIL_RotateFlipType); \
        MIL_Status  (*save)(_Self(MIL_Status), const char* file); \
        MIL_Status  (*loadFile)(_Self(MIL_Image), const char* file);
        MIL_IMAGE_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE

    BEGIN_PRIVATE(MIL_Image)
    MIL_BitmapData* data;
    const char* raw_format;
    void* cache;
    END_PRIVATE

END_CLASS_INHERIT

typedef enum {
    MIL_PIXEL_COPY = 0,
    MIL_PIXEL_AND = 1,
    MIL_PIXEL_XOR = 3,
    MIL_PIXEL_OR = 2,
    MIL_PIXEL_OPERATION_MAX = 4
} MIL_PixelsOperation ;

typedef void (*MIL_DestroyCtxtCB)(void*);

STRUCT {
    /** the pointer to the destination */
    Uint8* cur_dst;

    /** the current pixel value for setpixel and setpixels operation */
    mt_color color;

    /** the step of current pixel operations. */
    int step; /* May be disuse. */

    /** the pixel value shoulb be skipped */
    mt_color color_key; /* May be disuse. */

    /** The user context passed to SetUserCompositionOps */
    void* user_ctxt;

    /** The function for delete user_ctxt */
    MIL_DestroyCtxtCB ctxt_free;
} MIL_PixelsContext;

/** 
 * @class MIL_Bitmap
 * @brief A modifiable container for MIL_Image, you can change it's pixel data.
 */
BEGIN_CLASS_INHERIT(MIL_Bitmap, MIL_Image)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_Bitmap)
#define MIL_BITMAP_METHOD_TABLE \
        MIL_IMAGE_METHOD_TABLE \
        MIL_Status  (*getPixel)(_Self(MIL_Bitmap), int x, int y, MIL_Color* color); \
        MIL_Status  (*lockBits)(_Self(MIL_Bitmap), const MIL_Rect* rc, MIL_BitmapData* locked_data); \
        MIL_Status  (*setPixel)(_Self(MIL_Bitmap), int x, int y, const MIL_Color* color); \
        MIL_Status  (*unlockBits)(_Self(MIL_Bitmap), MIL_BitmapData* locked_data);
        MIL_BITMAP_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE

    BEGIN_PRIVATE(MIL_Bitmap)
        MIL_BitmapData* data;
    END_PRIVATE
END_CLASS_INHERIT

typedef enum  {
    MIL_DASH_SOLID        = 0,
    MIL_DASH_DASH         = 1,
    MIL_DASH_DOT          = 2,
    MIL_DASH_DASHDOT      = 3,
    MIL_DASH_DASHDOTDOT   = 4,
    MIL_DASH_CUSTOM       = 5 
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

enum {
    MIL_LIFE_REF = 0,
    MIL_HOLD_REF = 1,
    MIL_SAVE_REF = 2,
    MIL_MAX_REF_TYPE = 3
} MIL_RefCounterType;


BEGIN_CLASS_INHERIT(MIL_Brush, MIL_GdiObject)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_Brush)
#define MIL_BRUSH_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_BrushType (*getType)(_Self(MIL_Brush));
        MIL_BRUSH_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT_NEED_FORWARD_DECALRE(MIL_Region, MIL_GdiObject)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_Region)
#define MIL_REGION_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_Status (*excludeRect)(_Self(MIL_Region), MIL_Rect*);\
        MIL_Status (*excludeRegion)(_Self(MIL_Region), MIL_Region*);
        MIL_REGION_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(MIL_SolidBrush, MIL_Brush)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_SolidBrush)
#define MIL_SOLIDBRUSH_METHOD_TABLE \
        MIL_BRUSH_METHOD_TABLE \
        MIL_Status (*getColor)(_Self(MIL_SolidBrush), MIL_Color*);\
        MIL_Status (*setColor)(_Self(MIL_SolidBrush), const MIL_Color*);
        MIL_SOLIDBRUSH_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(MIL_HatchBrush, MIL_Brush)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_HatchBrush)
#define MIL_HATCHBRUSH_METHOD_TABLE \
        MIL_BRUSH_METHOD_TABLE \
        MIL_Status (*getBackgroundColor)(_Self(MIL_HatchBrush), MIL_Color*);\
        MIL_Status (*getForegroundColor)(_Self(MIL_HatchBrush), MIL_Color*);\
        MIL_Status (*setBackgroundColor)(_Self(MIL_HatchBrush), const MIL_Color*);\
        MIL_Status (*setForegroundColor)(_Self(MIL_HatchBrush), const MIL_Color*);\
        MIL_HatchStyle (*getHatchStyle)(_Self(MIL_HatchBrush));\
        MIL_HatchStyle (*setHatchStyle)(_Self(MIL_HatchBrush), MIL_HatchBrush);
        MIL_HATCHBRUSH_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(MIL_TextureBrush, MIL_Brush)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_TextureBrush)
#define MIL_TEXTUREBRUSH_METHOD_TABLE \
        MIL_BRUSH_METHOD_TABLE \
        MIL_Image* (*getImage)(_CSelf(MIL_TextureBrush));\
        MIL_WrapMode (*getWrapMode)(_CSelf(MIL_TextureBrush));\
        MIL_Status (*setWrapMode)(_Self(MIL_TextureBrush), MIL_WrapMode);
        MIL_TEXTUREBRUSH_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(MIL_Pen, MIL_GdiObject)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_Pen)
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
        void (*setWidth)(_Self(MIL_Pen), Uint32);
        MIL_PEN_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(MIL_GraphicsContext, MIL_GdiObject)
    BEGIN_METHOD_EXPAND_DECLARE(MIL_GraphicsContext)
#define MIL_GC_METHOD_TABLE \
        MIL_GDIOBJECT_METHOD_TABLE  \
        MIL_Status (*clear)(_Self(MIL_GraphicsContext), MIL_Color*); \
        MIL_Status (*drawImageByPos)(_Self(MIL_GraphicsContext), MIL_Image*, int, int); \
        MIL_Status (*drawImageByRect)(_Self(MIL_GraphicsContext), MIL_Image*, MIL_Rect*); \
        MIL_Status (*drawString)(_Self(MIL_GraphicsContext), const char*, MIL_Rect*); \
        MIL_Status (*includeClip)(_Self(MIL_GraphicsContext), MIL_Rect*); \
        MIL_Status (*excludeClip)(_Self(MIL_GraphicsContext), MIL_Rect*); \
        MIL_Bool   (*isClipEmpty)(_Self(MIL_GraphicsContext)); \
        void       (*store)(_Self(MIL_GraphicsContext)); \
        void       (*restore)(_Self(MIL_GraphicsContext));\
        MIL_Status (*selectPen)(_Self(MIL_GraphicsContext), MIL_Pen*);\
        MIL_Status (*selectBrush)(_Self(MIL_GraphicsContext), MIL_Brush*);\
        MIL_Status (*selectPixelsOperator)(_Self(MIL_GraphicsContext));\
        MIL_Status (*setPixelOperationType)(_Self(MIL_GraphicsContext), MIL_PixelsOperation);\
        MIL_Status (*setPixel)(_Self(MIL_GraphicsContext), Uint32, Uint32);\
        mt_color   (*getPixel)(_Self(MIL_GraphicsContext), Uint32, Uint32);\
        MIL_Status (*drawLine)(_Self(MIL_GraphicsContext), Uint32, Uint32, Uint32, Uint32);
        MIL_GC_METHOD_TABLE
    END_METHOD_EXPAND_DECLARE
END_CLASS_INHERIT
/* Alias of MIL_GraphicsContext */
typedef MIL_GraphicsContext MIL_Graphics;

extern DECLSPEC MIL_Graphics*
MIL_CreateMemGC(Uint32 width, Uint32 height, Uint32 depth, Uint32 Rmask,
        Uint32 Gmask, Uint32 Bmask, Uint32 Amask, Uint32 flags);

MIL_Graphics*
MIL_CreateMemGCFromImage(MIL_Image* img);
/*! 
 * @synopsis Load image from a file.
 * @param file The path of file.
 * @returns Pointer of image object if success, NULL otherwise. 
 */
extern DECLSPEC MIL_Image* LoadImageFromFile(const char* file);

/*! 
 * @synopsis Load image from a file.
 * @param file The path of file.
 * @returns Pointer of Bitmap object if success, NULL otherwise. 
 */
extern DECLSPEC MIL_Bitmap* 
LoadBitmapFromFile(const char* file);

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

