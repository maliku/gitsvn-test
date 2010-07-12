/*! ============================================================================
 * @file screen.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "screen.h"
#include "pixel_format.h" /* For New(PixelFormat) */
#include "misc.h"

__INLINE__ MIL_Bool is_rect_empty(_RECT* prc)
{
    if( prc->left == prc->right ) return MIL_TRUE;
    if( prc->top == prc->bottom ) return MIL_TRUE;
    return MIL_FALSE;
}

__INLINE__ void normalize_rect(_RECT* pRect)
{
    int iTemp;

    if(pRect->left > pRect->right)
    {
         iTemp = pRect->left;
         pRect->left = pRect->right;
         pRect->right = iTemp;
    }

    if(pRect->top > pRect->bottom)
    {
         iTemp = pRect->top;
         pRect->top = pRect->bottom;
         pRect->bottom = iTemp;
    }
}

__INLINE__ void get_bound_rect(_RECT* pdrc, const _RECT* psrc1, const _RECT* psrc2)
{
    _RECT src1, src2;
    memcpy(&src1, psrc1, sizeof(_RECT));
    memcpy(&src2, psrc2, sizeof(_RECT));

    normalize_rect(&src1);
    normalize_rect(&src2);

    pdrc->left = (src1.left < src2.left) ? src1.left : src2.left;
    pdrc->top  = (src1.top < src2.top) ? src1.top : src2.top;
    pdrc->right = (src1.right > src2.right) ? src1.right : src2.right;
    pdrc->bottom = (src1.bottom > src2.bottom) 
                   ? src1.bottom : src2.bottom;
}


#define MIL_MEMFILL_TEMPLATE(T) \
__INLINE__ void mil_memfill_##T(T *dest, T value, int count)\
{\
    int n = (count + 7) / 8;\
    switch (count & 0x07)\
    {\
    case 0: do { *dest++ = value;\
    case 7:      *dest++ = value;\
    case 6:      *dest++ = value;\
    case 5:      *dest++ = value;\
    case 4:      *dest++ = value;\
    case 3:      *dest++ = value;\
    case 2:      *dest++ = value;\
    case 1:      *dest++ = value;\
    } while (--n > 0);\
    }\
}
MIL_MEMFILL_TEMPLATE(char)
MIL_MEMFILL_TEMPLATE(short)
MIL_MEMFILL_TEMPLATE(int)

#define MIL_RECTFILL_TEMPLATE(T) \
__INLINE__ void mil_rectfill_##T(T *dest, T value,\
                        int x, int y, int width, int height, int stride)\
{\
    char *d = (char*)(dest + x) + y * stride;\
    printf("color=%d.\n", value);\
    if ((Uint32)stride == (width * sizeof(T))) {\
        mil_memfill_##T((T*)(d), value, width * height);\
    } else {\
        int j;\
        for (j = 0; j < height; ++j) {\
            dest = (T*)(d);\
            mil_memfill_##T(dest, value, width);\
            d += stride;\
        }\
    }\
}

MIL_RECTFILL_TEMPLATE(char)
MIL_RECTFILL_TEMPLATE(short)
MIL_RECTFILL_TEMPLATE(int)

#define SOLID_FILL_TEMPLATE(type) \
static void solidFill_##type(MScreen* screen, const MIL_Color* color, \
                               const MIL_Rect* r)\
{\
    type* dest = (type*)(_c(screen)->baseAddr(screen));\
    const MIL_PixelFormat* fmt = (MIL_PixelFormat*)MIL_Ref(_c(screen)->pixelFormat(screen));\
    const type c = (type)_c(fmt)->mapColor(fmt, color);\
    const int stride = _c(screen)->linestep(screen);\
    printf("color=%d.\n", c);\
    MIL_UnRef(fmt);\
    mil_rectfill_##type(dest, c, r->x, r->y, r->w, r->h, stride);\
    /*const QVector<QRect> rects = region->rects(); \
    for (int i = 0; i < rects.size(); ++i) {\
        const QRect r = rects.at(i); \
        rectfill(dest, c, r.x(), r.y(), r.width(), r.height(), stride);\
    }*/\
}

SOLID_FILL_TEMPLATE(char)
SOLID_FILL_TEMPLATE(short)
SOLID_FILL_TEMPLATE(int)

__INLINE__ int mGray(int r, int g, int b)// convert R,G,B to gray 0..255
{ return (r*11+g*16+b*5)/32; }

static __INLINE__ void mil_rectfill_gray4(Uint8 *dest, Uint8 value,
                                     int x, int y, int width, int height,
                                     int stride)
{
    const int pixelsPerByte = 2;
    const int doAlign = x & 1;
    const int doTail = (width - doAlign) & 1;
    const int width8 = (width - doAlign) / pixelsPerByte;
    int j;
    dest += y * stride + x / pixelsPerByte;

    for (j = 0; j < height; ++j) {
        if (doAlign)
            *dest = (*dest & 0xf0) | (value & 0x0f);
        if (width8)
            mil_memfill_char(dest + doAlign, value, width8);
        if (doTail) {
            Uint8 *d = dest + doAlign + width8;
            *d = (*d & 0x0f) | (value & 0xf0);
        }
        dest += stride;
    }
}

static void solidFill_gray4(MScreen *screen, const MIL_Color* color,
                            const MIL_Rect* r)
{
    Uint8 *dest = (Uint8*)(_c(screen)->baseAddr(screen));
    const Uint8 c = mGray(color->r, color->g, color->b) >> 4;
    const Uint8 c8 = (c << 4) | c;

    const int stride = _c(screen)->linestep(screen);
        mil_rectfill_gray4(dest, c8, r->x, r->y, r->w, r->h, stride);
//    const QVector<QRect> rects = region.rects();
//
//    for (int i = 0; i < rects.size(); ++i) {
//        const QRect r = rects.at(i);
//        qt_rectfill_gray4(dest, c8, r.x(), r.y(), r.width(), r.height(),
//                          stride);
//    }
}

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

static __INLINE__ void mil_rectfill_mono(Uint8 *dest, Uint8 value,
                                    int x, int y, int width, int height,
                                    int stride)
{
    const int pixelsPerByte = 8;
    const int alignWidth = MIN(width, (8 - (x & 7)) & 7);
    const int doAlign = (alignWidth > 0 ? 1 : 0);
    const int alignStart = pixelsPerByte - 1 - (x & 7);
    const int alignStop = alignStart - (alignWidth - 1);
    const Uint8 alignMask = ((1 << alignWidth) - 1) << alignStop;
    const int tailWidth = (width - alignWidth) & 7;
    const int doTail = (tailWidth > 0 ? 1 : 0);
    const Uint8 tailMask = (1 << (pixelsPerByte - tailWidth)) - 1;
    const int width8 = (width - alignWidth) / pixelsPerByte;
    int j;

    dest += y * stride + x / pixelsPerByte;
    stride -= (doAlign + width8);

    for (j = 0; j < height; ++j) {
        if (doAlign) {
            *dest = (*dest & ~alignMask) | (value & alignMask);
            ++dest;
        }
        if (width8) {
            mil_memfill_char(dest, value, width8);
            dest += width8;
        }
        if (doTail)
            *dest = (*dest & tailMask) | (value & ~tailMask);
        dest += stride;
    }
}

static void solidFill_mono(MScreen *screen, const MIL_Color* color,
                           const MIL_Rect* r)
{
    Uint8 *dest = (Uint8*)(_c(screen)->baseAddr(screen));
    const Uint8 c8 = (mGray(color->r, color->g, color->b) >> 7) * 0xff;

    const int stride = _c(screen)->linestep(screen);
        mil_rectfill_mono(dest, c8, r->x, r->y, r->w, r->h, stride);
//    const QVector<QRect> rects = region.rects();
//
//    for (int i = 0; i < rects.size(); ++i) {
//        const QRect r = rects.at(i);
//        qt_rectfill_mono(dest, c8, r.x(), r.y(), r.width(), r.height(),
//                         stride);
//    }
}
void solidFill_setup(MScreen* screen, const MIL_Color* color,
                        const MIL_Rect* region)
{
    printf("depth = %d.\n", _c(screen)->depth(screen));
    switch (_c(screen)->depth(screen)) {
        case 32:
                _friend(MScreen, screen)->d_ptr->solidFill = solidFill_int;
            break;
#if 0
        case 24:
                screen->d_ptr->solidFill = solidFill_int;
            break;
        case 18:
            screen->d_ptr->solidFill = solidFill_template<quint18>;
            break;
#endif
        case 16:
                _friend(MScreen, screen)->d_ptr->solidFill = solidFill_short;
            break;
        case 15:
                _friend(MScreen, screen)->d_ptr->solidFill = solidFill_short;
            break;
        case 12:
            _friend(MScreen, screen)->d_ptr->solidFill = solidFill_short;
            break;
        case 8:
            _friend(MScreen, screen)->d_ptr->solidFill = solidFill_char;
            break;
        case 4:
            _friend(MScreen, screen)->d_ptr->solidFill = solidFill_gray4;
            break;
        case 1:
            _friend(MScreen, screen)->d_ptr->solidFill = solidFill_mono;
            break;
        default:
            MIL_SetError("solidFill_setup(): Screen depth %d not supported!",
                    _c(screen)->depth(screen));
            _friend(MScreen, screen)->d_ptr->solidFill = NULL;
            break;
    }
    _friend(MScreen, screen)->d_ptr->solidFill(screen, color, region);
}

void blit_setup(MScreen *screen, const MImage* image,
                   const MIL_Point* topLeft, const MIL_Rect* region)
{
#if 0
    switch (screen->depth()) {
    case 32:
        if (screen->pixelType() == QScreen::NormalPixel)
            screen->d_ptr->blit = blit_32;
        else
            screen->d_ptr->blit = blit_template<qabgr8888, quint32>;
        break;
    case 24:
        if (screen->pixelType() == QScreen::NormalPixel)
            screen->d_ptr->blit = blit_qrgb888;
        else
            screen->d_ptr->blit = blit_24;
        break;
    case 18:
        screen->d_ptr->blit = blit_18;
        break;
    case 16:
#if MIL_BYTEORDER == MIL_BIG_ENDIAN
        if (screen->d_ptr->fb_is_littleEndian)
            screen->d_ptr->blit = blit_16_bigToLittleEndian;
        else
#endif
        if (screen->pixelType() == QScreen::NormalPixel)
            screen->d_ptr->blit = blit_16;
        else
            screen->d_ptr->blit = blit_template<qbgr565, quint16>;
        break;
    case 15:
#if MIL_BYTEORDER == MIL_BIG_ENDIAN
        if (screen->d_ptr->fb_is_littleEndian)
            screen->d_ptr->blit = blit_15_bigToLittleEndian;
        else
#endif // Q_BIG_ENDIAN
        if (screen->pixelType() == QScreen::NormalPixel)
            screen->d_ptr->blit = blit_15;
        else
            screen->d_ptr->blit = blit_template<qbgr555, qrgb555>;
        break;
    case 12:
        screen->d_ptr->blit = blit_12;
        break;
    case 8:
        screen->d_ptr->blit = blit_8;
        break;
    case 4:
        screen->d_ptr->blit = blit_4;
        break;
    case 1:
        screen->d_ptr->blit = blit_1;
        break;
    default:
        qFatal("blit_setup(): Screen depth %d not supported!",
               screen->depth());
        screen->d_ptr->blit = 0;
        break;
    }
#endif
    _friend(MScreen, screen)->d_ptr->blit(screen, image, topLeft, region);
}

CONSTRUCTOR(MScreenPrivate)
{
#ifdef MIL_CLIENTBLIT
    _m(supportsBlitInClients) = MIL_FALSE;
#endif
    _m(classId) = 0;
    _m(q_ptr) = NULL;
    _m(solidFill) = solidFill_setup;
    _m(blit) = blit_setup;
#if MIL_BYTEORDER == MIL_BIG_ENDIAN
    _m(fb_is_littleEndian) = MIL_FALSE;
#endif
//    pixmapFactory = 0;
//    graphicsSystem = &defaultGraphicsSystem;
}

DESTRUCTOR(MScreenPrivate)
{
}

BEGIN_METHOD_MAP(MScreenPrivate, NonBase)
    CONSTRUCTOR_MAP(MScreenPrivate)
    DESTRUCTOR_MAP(MScreenPrivate)
END_METHOD_MAP

CONSTRUCTOR(MScreen)
{
    _private(MScreen)->d_ptr = (MScreenPrivate*)New(MScreenPrivate);
    _private(MScreen)->d_ptr->q_ptr = self;
    _private(MScreen)->depth = 1;
    _private(MScreen)->data = NULL;
    _private(MScreen)->dh = 0;
    _private(MScreen)->dw = 0;
    _private(MScreen)->grayscale = MIL_FALSE;
    _private(MScreen)->h = 0;
    _private(MScreen)->pitch = 0;
    _private(MScreen)->mapsize = 0;
    _private(MScreen)->physHeight = 0;
    _private(MScreen)->physWidth = 0;
    _private(MScreen)->screencols = 0;
    _private(MScreen)->size = 0;
    _private(MScreen)->w = 0;
    memset(_private(MScreen)->screenclut, 0, 256);
    _private(MScreen)->format = (MIL_PixelFormat*)New(PixelFormat);
}

DESTRUCTOR(MScreen)
{
    MIL_UnRef(_private(MScreen)->format);
    Delete(_private(MScreen)->d_ptr);
}

int  METHOD_NAMED(MScreen, colorIndex)(_Self(MScreen), Uint32 r, Uint32 g, Uint32 b)
{
    return -1;
}

Uint8* METHOD_NAMED(MScreen, baseAddr)(_CSelf(MScreen))
{
    return _private(MScreen)->data;
}

void METHOD_NAMED(MScreen, blit)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Rect* rc)
{
}

void METHOD_NAMED(MScreen, blitRg)
    (_Self(MScreen), const MImage* img, const MIL_Point* pt, const MIL_Region* region)
{

}

MIL_Screen_Clsid METHOD_NAMED(MScreen, classID)(_CSelf(MScreen))
{
    return MIL_UNKNOW_SCREEN;
}

MIL_Color* METHOD_NAMED(MScreen, clut)(_Self(MScreen))
{
	return NULL;
}

int  METHOD_NAMED(MScreen, colorCount)(_CSelf(MScreen))
{
    return (1 << 
        _c(_private(MScreen)->format)->getBytesPerPixel(_private(MScreen)->format));
}

MIL_Bool METHOD_NAMED(MScreen, connect)(_CSelf(MScreen), const char* display)
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, depth)(_CSelf(MScreen))
{
    return 
        _c(_private(MScreen)->format)->getBitsPerPixel(_private(MScreen)->format);
}

int  METHOD_NAMED(MScreen, deviceHeight)(_CSelf(MScreen))
{
    return _private(MScreen)->dh;
}

int  METHOD_NAMED(MScreen, deviceWidth)(_CSelf(MScreen))
{
    return _private(MScreen)->dw;
}

void METHOD_NAMED(MScreen, disconnect)(_Self(MScreen))
{
}

int  METHOD_NAMED(MScreen, height)(_Self(MScreen))
{
    return _private(MScreen)->h;
}

MIL_Bool METHOD_NAMED(MScreen, initDevice)(_Self(MScreen))
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, linestep)(_CSelf(MScreen))
{
    return _private(MScreen)->pitch;
}

MIL_Size  METHOD_NAMED(MScreen, mapFromDevice)(_CSelf(MScreen), const MIL_Size* size)
{
    return *size;
}

MIL_Size  METHOD_NAMED(MScreen, mapToDevice)(_CSelf(MScreen), const MIL_Size* size)
{
    return *size;
}

MIL_Status  METHOD_NAMED(MScreen, offest)(_CSelf(MScreen), MIL_Point* pt)
{
    return MIL_NOT_IMPLEMENTED;
}

MIL_Bool METHOD_NAMED(MScreen, onCard)(_Self(MScreen), const char* ptr)
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, physicalHeight)(_CSelf(MScreen))
{
    return _private(MScreen)->physHeight;
}

int  METHOD_NAMED(MScreen, physicalWidth)(_CSelf(MScreen))
{
    return _private(MScreen)->physWidth;
}

const MIL_PixelFormat*  METHOD_NAMED(MScreen, pixelFormat)(_CSelf(MScreen))
{
    return _private(MScreen)->format;
}

int  METHOD_NAMED(MScreen, pixmapDepth)(_CSelf(MScreen))
{
    return _c(_private(MScreen)->format)->getBitsPerPixel(_private(MScreen)->format);
}

int  METHOD_NAMED(MScreen, pixmapLinestepAlignment)(_CSelf(MScreen))
{
    return 64;
}

int  METHOD_NAMED(MScreen, pixmapOffsetAlignment)(_CSelf(MScreen))
{
    return 64;
}

void METHOD_NAMED(MScreen, restore)(_Self(MScreen))
{

}

void METHOD_NAMED(MScreen, save)(_Self(MScreen))
{

}

int  METHOD_NAMED(MScreen, screenSize)(_CSelf(MScreen))
{
    return _private(MScreen)->mapsize;
}

void METHOD_NAMED(MScreen, setDirty)(_Self(MScreen), int numrects, const MIL_Rect* rects)
{
}

void METHOD_NAMED(MScreen, setMode)(_Self(MScreen), int width, int height, int depth)
{
}

void METHOD_NAMED(MScreen, setPixelFormat)(_Self(MScreen), MIL_PixelFormat* format)
{
    if (NULL != format) {
        MIL_Ref(format);
        MIL_UnRef(_private(MScreen)->format);
        _private(MScreen)->format = format;
    }
}

void METHOD_NAMED(MScreen, shutdownDevice)(_Self(MScreen))
{
    /* TODO: Hide mouse cursor. */
}

void METHOD_NAMED(MScreen, solidFill)(_Self(MScreen), const MIL_Color* c, const MIL_Rect* rc)
{
//    QWSDisplay::grab();
    assert(_private(MScreen)->d_ptr);
    _private(MScreen)->d_ptr->solidFill(self, c, rc);
//                     region.translated(-offset()) & QRect(0, 0, dw, dh));
//    QWSDisplay::ungrab();
}

int  METHOD_NAMED(MScreen, subScreenIndexAt)(_CSelf(MScreen), const MIL_Point* pt)
{
    return -1;
}

MIL_Bool METHOD_NAMED(MScreen, supportsDepth)(_Self(MScreen), int depth)
{
    return MIL_FALSE;
}

int  METHOD_NAMED(MScreen, totalSize)(_Self(MScreen))
{
    return _private(MScreen)->mapsize;
}

int  METHOD_NAMED(MScreen, width)(_Self(MScreen))
{
    return _private(MScreen)->w;
}

BEGIN_METHOD_MAP(MScreen, NonBase)

    CONSTRUCTOR_MAP(MScreen)
    DESTRUCTOR_MAP(MScreen)
    METHOD_MAP(MScreen, colorIndex)
    METHOD_MAP(MScreen, baseAddr)
    METHOD_MAP(MScreen, blit)
    METHOD_MAP(MScreen, blitRg)
    METHOD_MAP(MScreen, classID)
    METHOD_MAP(MScreen, clut)
    METHOD_MAP(MScreen, colorCount)
    METHOD_MAP(MScreen, connect)
    METHOD_MAP(MScreen, depth)
    METHOD_MAP(MScreen, deviceHeight)
    METHOD_MAP(MScreen, deviceWidth)
    METHOD_MAP(MScreen, disconnect)
    METHOD_MAP(MScreen, height)
    METHOD_MAP(MScreen, initDevice)
    METHOD_MAP(MScreen, linestep)
    METHOD_MAP(MScreen, mapFromDevice)
    METHOD_MAP(MScreen, mapToDevice)
    METHOD_MAP(MScreen, offest)
    METHOD_MAP(MScreen, onCard)
    METHOD_MAP(MScreen, physicalHeight)
    METHOD_MAP(MScreen, physicalWidth)
    METHOD_MAP(MScreen, pixelFormat)
    METHOD_MAP(MScreen, pixmapDepth)
    METHOD_MAP(MScreen, pixmapLinestepAlignment)
    METHOD_MAP(MScreen, pixmapOffsetAlignment)
    METHOD_MAP(MScreen, restore)
    METHOD_MAP(MScreen, save)
    METHOD_MAP(MScreen, screenSize)
    METHOD_MAP(MScreen, setDirty)
    METHOD_MAP(MScreen, setMode)
    METHOD_MAP(MScreen, setPixelFormat)
    METHOD_MAP(MScreen, shutdownDevice)
    METHOD_MAP(MScreen, solidFill)
    METHOD_MAP(MScreen, subScreenIndexAt)
    METHOD_MAP(MScreen, supportsDepth)
    METHOD_MAP(MScreen, totalSize)
    METHOD_MAP(MScreen, width)

END_METHOD_MAP

#include "qvfb/screen_qvfb.h"
#include "wvfb/screen_wvfb.h"
#define MAX_SCREENS 16
MScreen* g_screen_table[MAX_SCREENS];
int CreateScreen(const char* device_name, int w, int h, int bpp)
{
    g_screen_table[0] = (MScreen*)New(ScreenQVFB);
    if (NULL != g_screen_table[0]) {
        if (_c(g_screen_table[0])->initDevice(g_screen_table[0])) {
            _c(g_screen_table[0])->setMode(g_screen_table[0], w, h, bpp);
        }
        else {
            Delete(g_screen_table[0]);
            g_screen_table[0] = NULL;
        }
        return 0;
    }
    return -1;
}

/* TODO: Complete it. */
MScreen* MIL_TakeScreen(int index)
{
    if (NULL == g_screen_table[index]) {
        CreateScreen("qvfb", 640, 480, 32);
    }
    return g_screen_table[index];
}

