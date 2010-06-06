/*! ============================================================================
 * @file pixels_ops.c 
 * @Synopsis Pixels operations. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "MIL_endian.h"
#include "misc.h"
#include "pixels_ops.h"

CONSTRUCTOR(MIL_PixelsOps)
{
    _private(MIL_PixelsOps)->step = 0;
    _private(MIL_PixelsOps)->cur_dst = NULL;
    _private(MIL_PixelsOps)->skip_pixel = 0;
    _private(MIL_PixelsOps)->cur_pixel = 0;
    _private(MIL_PixelsOps)->user_ctxt = NULL;
    _private(MIL_PixelsOps)->ctxt_free = NULL;
}

DESTRUCTOR(MIL_PixelsOps)
{
    if (NULL != _private(MIL_PixelsOps)->user_ctxt && NULL != _private(MIL_PixelsOps)->ctxt_free) {
        _private(MIL_PixelsOps)->ctxt_free(_private(MIL_PixelsOps)->user_ctxt);
    }
}

void METHOD_NAMED(MIL_PixelsOps, setPixel)(_Self(MIL_PixelsOps))
{
    return;
}

void METHOD_NAMED(MIL_PixelsOps, setHline)(_Self(MIL_PixelsOps), Uint32 w)
{
    return;
}

void METHOD_NAMED(MIL_PixelsOps, putHline)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    return;
}

void METHOD_NAMED(MIL_PixelsOps, putHlineSkip)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    return;
}

void  METHOD_NAMED(MIL_PixelsOps, setStep)(_Self(MIL_PixelsOps), int step)
{
    _private(MIL_PixelsOps)->step = step;
}

void  METHOD_NAMED(MIL_PixelsOps, setColorKey)(_Self(MIL_PixelsOps), mt_color c)
{
    _private(MIL_PixelsOps)->skip_pixel = c;
}

void  METHOD_NAMED(MIL_PixelsOps, setColor)(_Self(MIL_PixelsOps), mt_color c)
{
    _private(MIL_PixelsOps)->cur_pixel = c;
}

void* METHOD_NAMED(MIL_PixelsOps, setUserCtxt)(_Self(MIL_PixelsOps), void* ctxt)
{
    _private(MIL_PixelsOps)->user_ctxt = ctxt;
}

BEGIN_METHOD_MAP(MIL_PixelsOps, MIL_GdiObject)
    CONSTRUCTOR_MAP(MIL_PixelsOps)
    DESTRUCTOR_MAP(MIL_PixelsOps)
    METHOD_PLACEHOLDER(ref)
    METHOD_PLACEHOLDER(unRef)
    METHOD_PLACEHOLDER(getRef)
    METHOD_MAP(MIL_PixelsOps, setPixel)
    METHOD_MAP(MIL_PixelsOps, setHline)
    METHOD_MAP(MIL_PixelsOps, putHline)
    METHOD_MAP(MIL_PixelsOps, putHlineSkip)
    METHOD_MAP(MIL_PixelsOps, setStep)
    METHOD_MAP(MIL_PixelsOps, setColorKey)
    METHOD_MAP(MIL_PixelsOps, setColor)
    METHOD_MAP(MIL_PixelsOps, setUserCtxt)
END_METHOD_MAP

static __INLINE__ void memset_hline_helper (MIL_PixelsOps* ops, int bytes_per_pixel, int w)
{
    int n = w * bytes_per_pixel;
    if (!_friend(MIL_PixelsOps, ops)->cur_pixel && !(n & 3) 
            && !((long)_friend(MIL_PixelsOps, ops)->cur_dst & 3) 
            && (n > 3)) {
        n = n >> 2;
        MIL_memset4 (_friend(MIL_PixelsOps, ops)->cur_dst, 0, n);
    }
    else {
        memset (_friend(MIL_PixelsOps, ops)->cur_dst, _friend(MIL_PixelsOps, ops)->cur_pixel, n);
    }
}

static __INLINE__ void memcpy_hline_helper 
(MIL_PixelsOps* ops, const Uint8* src, int bytes_per_pixel, int w)
{
    int n = w * bytes_per_pixel;

    if (!((long)_friend(MIL_PixelsOps, ops)->cur_dst&3) && !(n&3) && !((long)src&3) && (n > 3)) {
        MIL_memcpy4(_friend(MIL_PixelsOps, ops)->cur_dst, src, n >> 2);
    }
    else
        memcpy(_friend(MIL_PixelsOps, ops)->cur_dst, src, n);
}

/* =================================== 1 Bytes per pixel ==================== */

void METHOD_NAMED(PixelsSet1, setPixel)(_Self(MIL_PixelsOps))
{
    *_private(MIL_PixelsOps)->cur_dst = (Uint8)_private(MIL_PixelsOps)->cur_pixel;
}

void METHOD_NAMED(PixelsSet1, setHline)(_Self(MIL_PixelsOps), Uint32 w)
{
    if (_private(MIL_PixelsOps)->step == 1) {
        memset_hline_helper(self, 1, w);
    }
    else {
        Uint8* row = _private(MIL_PixelsOps)->cur_dst;
        while (w > 0) {
            *row = (Uint8) _private(MIL_PixelsOps)->cur_pixel;
            row += _private(MIL_PixelsOps)->step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet1, putHline)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    if (_private(MIL_PixelsOps)->step == 1) {
        memcpy_hline_helper (self, src, 1, w);
    }
    else {
        Uint8* row = _private(MIL_PixelsOps)->cur_dst;
        while (w > 0) {
            *row = *src;
            row += _private(MIL_PixelsOps)->step;
            src += _private(MIL_PixelsOps)->step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet1, putHlineSkip)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    Uint8* row = _private(MIL_PixelsOps)->cur_dst;
    while (w > 0) {
        if (*src != _private(MIL_PixelsOps)->skip_pixel)
            *row = *src;
        row += _private(MIL_PixelsOps)->step;
        src += _private(MIL_PixelsOps)->step;
        w -= _private(MIL_PixelsOps)->step;
    }
}

BEGIN_METHOD_MAP(PixelsSet1, MIL_PixelsOps)
    NON_CONSTRUCTOR
    NON_DESTRUCTOR
    METHOD_PLACEHOLDER(ref)
    METHOD_PLACEHOLDER(unRef)
    METHOD_PLACEHOLDER(getRef)
    METHOD_MAP(PixelsSet1, setPixel)
    METHOD_MAP(PixelsSet1, setHline)
    METHOD_MAP(PixelsSet1, putHline)
    METHOD_MAP(PixelsSet1, putHlineSkip)
    METHOD_PLACEHOLDER(setStep)
    METHOD_PLACEHOLDER(setColorKey)
    METHOD_PLACEHOLDER(setColor)
    METHOD_PLACEHOLDER(setUserCtxt)
END_METHOD_MAP

/* =================================== 2 Bytes per pixel ==================== */

void METHOD_NAMED(PixelsSet2, setPixel)(_Self(MIL_PixelsOps))
{
    *(Uint16 *) _private(MIL_PixelsOps)->cur_dst = (Uint16) _private(MIL_PixelsOps)->cur_pixel;
}

void METHOD_NAMED(PixelsSet2, setHline)(_Self(MIL_PixelsOps), Uint32 w)
{
    int step = _private(MIL_PixelsOps)->step;
    if (_private(MIL_PixelsOps)->cur_pixel != 0 || _private(MIL_PixelsOps)->step != 1) {
        Uint16 * dest16 = (Uint16 *)_private(MIL_PixelsOps)->cur_dst;
        if (w < 5 || step != 1)
        {
            do {
                *dest16 = (Uint16)_private(MIL_PixelsOps)->cur_pixel;
                dest16 += step;
                w -= step;
            } while (w > 0);
        }
        else
        {
            Uint32 mixture = (((Uint16)_private(MIL_PixelsOps)->cur_pixel << 16) 
                    | (Uint16)_private(MIL_PixelsOps)->cur_pixel);
            Uint32 * dest32;
            int count;
            if ((Uint32)_private(MIL_PixelsOps)->cur_dst & 3) // Ensure 4-bytes alignment.
            {
                *dest16++ = (Uint16)_private(MIL_PixelsOps)->cur_pixel;
                --w;
            }
            count = (w >> 1);
            dest32 = (Uint32 *)dest16;
            do {
                *dest32++ = mixture;
            } while (--count > 0); // Please make sure 3 pixels to processed at least!
            if (0 != (w & 1)) // Dealing with the extra pixel.
            {
                *(Uint16 *)dest32 = _private(MIL_PixelsOps)->cur_pixel;
            }
        }
    }
#ifdef ASM_memset2
    else if (_private(MIL_PixelsOps)->step == 1 && !((long)_private(MIL_PixelsOps)->cur_dst & 2)) {
        ASM_memset2 (_private(MIL_PixelsOps)->cur_dst, _private(MIL_PixelsOps)->cur_pixel, w);
    }
#endif
    else {
        memset_hline_helper (self, 2, w);
    }
}

void METHOD_NAMED(PixelsSet2, putHline)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    if (_private(MIL_PixelsOps)->step == 1) {
        memcpy_hline_helper (self, src, 2, w);
    }
    else {
        Uint16* dstrow = (Uint16*)_private(MIL_PixelsOps)->cur_dst;
        Uint16* srcrow = (Uint16*)src;
        while (w > 0) {
            *dstrow = *srcrow;
            dstrow += _private(MIL_PixelsOps)->step;
            srcrow += _private(MIL_PixelsOps)->step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet2, putHlineSkip)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    Uint16* dstrow = (Uint16*)_private(MIL_PixelsOps)->cur_dst;
    Uint16* srcrow = (Uint16*)src;

    while (w > 0) {
        if (*srcrow != _private(MIL_PixelsOps)->skip_pixel)
            *dstrow = *srcrow;
        dstrow += _private(MIL_PixelsOps)->step;
        srcrow += _private(MIL_PixelsOps)->step;
        w -= _private(MIL_PixelsOps)->step;
    }
}

BEGIN_METHOD_MAP(PixelsSet2, MIL_PixelsOps)
    NON_CONSTRUCTOR
    NON_DESTRUCTOR
    METHOD_PLACEHOLDER(ref)
    METHOD_PLACEHOLDER(unRef)
    METHOD_PLACEHOLDER(getRef)
    METHOD_MAP(PixelsSet2, setPixel)
    METHOD_MAP(PixelsSet2, setHline)
    METHOD_MAP(PixelsSet2, putHline)
    METHOD_MAP(PixelsSet2, putHlineSkip)
    METHOD_PLACEHOLDER(setStep)
    METHOD_PLACEHOLDER(setColorKey)
    METHOD_PLACEHOLDER(setColor)
    METHOD_PLACEHOLDER(setUserCtxt)
END_METHOD_MAP

/* =================================== 3 Bytes per pixel ==================== */
void METHOD_NAMED(PixelsSet3, setPixel)(_Self(MIL_PixelsOps))
{
    SETVAL_24BIT(_private(MIL_PixelsOps)->cur_dst, _private(MIL_PixelsOps)->cur_pixel);
}

void METHOD_NAMED(PixelsSet3, setHline)(_Self(MIL_PixelsOps), Uint32 w)
{
    if (_private(MIL_PixelsOps)->step == 1 && _private(MIL_PixelsOps)->cur_pixel == 0) {
        memset_hline_helper (self, 3, w);
    }
#ifdef ASM_memset3
    else if (_private(MIL_PixelsOps)->step == 1) {
        ASM_memset3 (_private(MIL_PixelsOps)->cur_dst, _private(MIL_PixelsOps)->cur_pixel, w);
    }
#endif
    else {
        Uint8* row = _private(MIL_PixelsOps)->cur_dst;
        int step = (_private(MIL_PixelsOps)->step << 1) + _private(MIL_PixelsOps)->step;

        while (w > 0) {
            SETVAL_24BIT (row, _private(MIL_PixelsOps)->cur_pixel);
            row += step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet3, putHline)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    if (_private(MIL_PixelsOps)->step == 1) {
        memcpy_hline_helper (self, src, 3, w);
    }
    else {
        Uint8* row = (Uint8*)_private(MIL_PixelsOps)->cur_dst;
        int step = (_private(MIL_PixelsOps)->step << 1) + _private(MIL_PixelsOps)->step;

        while (w > 0) {
            *row = *src;
            *(row + 1) = *(src + 1);
            *(row + 2) = *(src + 2);
            row += step;
            src += step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet3, putHlineSkip)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    Uint8* row = (Uint8*)_private(MIL_PixelsOps)->cur_dst;
    int step = (_private(MIL_PixelsOps)->step << 1) + _private(MIL_PixelsOps)->step;

    while (w > 0) {
        if (!EQUAL_24BIT (row, _private(MIL_PixelsOps)->skip_pixel)) {
            *row = *src;
            *(row + 1) = *(src + 1);
            *(row + 2) = *(src + 2);
        }
        row += step;
        src += step;
        w -= _private(MIL_PixelsOps)->step;
    }
}

BEGIN_METHOD_MAP(PixelsSet3, MIL_PixelsOps)
    NON_CONSTRUCTOR
    NON_DESTRUCTOR
    METHOD_PLACEHOLDER(ref)
    METHOD_PLACEHOLDER(unRef)
    METHOD_PLACEHOLDER(getRef)
    METHOD_MAP(PixelsSet3, setPixel)
    METHOD_MAP(PixelsSet3, setHline)
    METHOD_MAP(PixelsSet3, putHline)
    METHOD_MAP(PixelsSet3, putHlineSkip)
    METHOD_PLACEHOLDER(setStep)
    METHOD_PLACEHOLDER(setColorKey)
    METHOD_PLACEHOLDER(setColor)
    METHOD_PLACEHOLDER(setUserCtxt)
END_METHOD_MAP

/* =================================== 4 Bytes per pixel ==================== */

void METHOD_NAMED(PixelsSet4, setPixel)(_Self(MIL_PixelsOps))
{
    *(Uint32 *) _private(MIL_PixelsOps)->cur_dst = (Uint32) _private(MIL_PixelsOps)->cur_pixel;
}

void METHOD_NAMED(PixelsSet4, setHline)(_Self(MIL_PixelsOps), Uint32 w)
{
    Uint32* row = (Uint32*)_private(MIL_PixelsOps)->cur_dst;
    
    if (_private(MIL_PixelsOps)->step == 1) {
        MIL_memset4 (row, _private(MIL_PixelsOps)->cur_pixel, w);
    }
    else {
        while (w > 0) {
            *row = _private(MIL_PixelsOps)->cur_pixel;
            row += _private(MIL_PixelsOps)->step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet4, putHline)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    Uint32* dstrow = (Uint32*)_private(MIL_PixelsOps)->cur_dst;
    Uint32* srcrow = (Uint32*)src;

    if (_private(MIL_PixelsOps)->step == 1) {
        memcpy_hline_helper (self, src, 4, w);
    }
    else {
        while (w > 0) {
            *dstrow = *srcrow;
            dstrow += _private(MIL_PixelsOps)->step;
            srcrow += _private(MIL_PixelsOps)->step;
            w -= _private(MIL_PixelsOps)->step;
        }
    }
}

void METHOD_NAMED(PixelsSet4, putHlineSkip)(_Self(MIL_PixelsOps), Uint8* src, Uint32 w)
{
    Uint32* dstrow = (Uint32*)_private(MIL_PixelsOps)->cur_dst;
    Uint32* srcrow = (Uint32*)src;

    while (w > 0) {
        if (*srcrow != _private(MIL_PixelsOps)->skip_pixel)
            *dstrow = *srcrow;
        dstrow += _private(MIL_PixelsOps)->step;
        srcrow += _private(MIL_PixelsOps)->step;
        w -= _private(MIL_PixelsOps)->step;
    }
}

BEGIN_METHOD_MAP(PixelsSet4, MIL_PixelsOps)
    NON_CONSTRUCTOR
    NON_DESTRUCTOR
    METHOD_PLACEHOLDER(ref)
    METHOD_PLACEHOLDER(unRef)
    METHOD_PLACEHOLDER(getRef)
    METHOD_MAP(PixelsSet4, setPixel)
    METHOD_MAP(PixelsSet4, setHline)
    METHOD_MAP(PixelsSet4, putHline)
    METHOD_MAP(PixelsSet4, putHlineSkip)
    METHOD_PLACEHOLDER(setStep)
    METHOD_PLACEHOLDER(setColorKey)
    METHOD_PLACEHOLDER(setColor)
    METHOD_PLACEHOLDER(setUserCtxt)
END_METHOD_MAP

MIL_PixelsOps* g_pixels_operator[MIL_PIXEL_OPERATION_MAX][4];

MIL_PixelsOps* GetPixelsOperator(MIL_PixelsOperation opt, int bpp)
{
    if (opt >= 0 && opt < MIL_PIXEL_OPERATION_MAX 
            && bpp > 0 && bpp <= 4) {
        int bpp_index = bpp - 1;
        if (NULL == g_pixels_operator[opt][bpp_index]) {
            switch (opt) {
                case MIL_PIXEL_COPY:
                    switch (bpp_index) {
                        case 0:
                            g_pixels_operator[opt][bpp_index] = 
                                (MIL_PixelsOps*)New(PixelsSet1);
                            break;
                        case 1:
                            g_pixels_operator[opt][bpp_index] = 
                                (MIL_PixelsOps*)New(PixelsSet2);
                            break;
                        case 2:
                            g_pixels_operator[opt][bpp_index] = 
                                (MIL_PixelsOps*)New(PixelsSet3);
                            break;
                        case 3:
                            g_pixels_operator[opt][bpp_index] = 
                                (MIL_PixelsOps*)New(PixelsSet4);
                            break;
                    }
                case MIL_PIXEL_AND:
                    break;
                case MIL_PIXEL_XOR:
                    break;
                case MIL_PIXEL_OR:
                    break;
                default:
                    assert(0);
                    break;
            }
        }
        return g_pixels_operator[opt][bpp_index];
    }
    return NULL;
}

