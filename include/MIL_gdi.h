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
    MIL_PixelFormat* format;
}MIL_BitmapData;

/** 
 * @name MIL_Image
 * @brief A readonly container of kinds of image format.
 */
CLASS(MIL_Image)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(MIL_Image)
        MIL_Image*  (*clone)(_Self(MIL_Image));
        MIL_Status  (*getBounds)(_Self(MIL_Image), MIL_Rect* rc);
        Uint32      (*getWidth)(_Self(MIL_Image));
        Uint32      (*getHeight)(_Self(MIL_Image));
        MIL_Status  (*getPalette)(_Self(MIL_Image), MIL_Palette* palette);
        MIL_Status  (*setPalette)(_Self(MIL_Image), const MIL_Palette* palette);
        int         (*getPaletteSize)(_Self(MIL_Image));
        MIL_Status  (*getPixelFormat)(_Self(MIL_Image), MIL_PixelFormat* fmt);
        const char* (*getRawFormat)(_Self(MIL_Image));
        MIL_Status  (*rotateFlip)(_Self(MIL_Image), MIL_RotateFlipType);
        MIL_Status  (*save)(_Self(MIL_Status), const char* file);
        MIL_Status  (*loadFile)(_Self(MIL_Image), const char* file);
    VIRTUAL_METHOD_DECLARE_END

    METHOD_DECLARE_BEGIN(MIL_Image)
    METHOD_DECLARE_END


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

    PRIVATE_END
};

/** 
 * @name MIL_Bitmap
 * @brief A modifiable container for MIL_Image, you can change it's pixel data.
 */
CLASS_INHERIT_BEGIN(MIL_Bitmap, MIL_Image)
    METHOD_DECLARE_BEGIN(MIL_Bitmap)
        MIL_Status  (*getPixel)(_Self(MIL_Bitmap), int x, int y, MIL_Color* color);
        MIL_Status  (*lockBits)(_Self(MIL_Bitmap), const MIL_Rect* rc, MIL_BitmapData* locked_data);
        MIL_Status  (*setPixel)(_Self(MIL_Bitmap), int x, int y, const MIL_Color* color);
        MIL_Status  (*unlockBits)(_Self(MIL_Bitmap), MIL_BitmapData* locked_data);
    METHOD_DECLARE_END

    PRIVATE_BEGIN(MIL_Bitmap)
        MIL_BitmapData* data;
    PRIVATE_END

CLASS_INHERIT_END

/** 
 * @synopsis Load image from a file.
 * 
 * @param file The path of file.
 * 
 * @returns Pointer of image object if success, NULL otherwise. 
 */
MIL_Image* LoadImageFromFile(const char* file);
MIL_Bitmap* LoadBitmapFromFile(const char* file);

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

