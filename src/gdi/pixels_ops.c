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

static __INLINE__ void memset_hline_helper (MIL_PixelsContext* ops, int bytes_per_pixel, int w)
{
    int n = w * bytes_per_pixel;
    if (!ops && !(n & 3) 
            && !((long)ops->cur_dst & 3) 
            && (n > 3)) {
        n = n >> 2;
        MIL_memset4 (ops->cur_dst, 0, n);
    }
    else {
        memset (ops->cur_dst, ops->color, n);
    }
}

static __INLINE__ void memcpy_hline_helper 
(MIL_PixelsContext* ops, const Uint8* src, int bytes_per_pixel, int w)
{
    int n = w * bytes_per_pixel;

    if (!((long)ops->cur_dst&3) && !(n&3) && !((long)src&3) && (n > 3)) {
        MIL_memcpy4(ops->cur_dst, src, n >> 2);
    }
    else
        memcpy(ops->cur_dst, src, n);
}

/* =================================== 1 Bytes per pixel ==================== */

void SetPixelSet8(MIL_PixelsContext *ctxt)
{
    *ctxt->cur_dst = (Uint8)ctxt->color;
}

void SetHlineSet8(MIL_PixelsContext *ctxt, Uint32 w)
{
    if (ctxt->step == 1) {
        memset_hline_helper(ctxt, 1, w);
    }
    else {
        Uint8* row = ctxt->cur_dst;
        while (w > 0) {
            *row = (Uint8) ctxt->color;
            row += ctxt->step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSet8(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    if (ctxt->step == 1) {
        memcpy_hline_helper (ctxt, src, 1, w);
    }
    else {
        Uint8* row = ctxt->cur_dst;
        while (w > 0) {
            *row = *src;
            row += ctxt->step;
            src += ctxt->step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSkipSet8(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    Uint8* row = ctxt->cur_dst;
    while (w > 0) {
        if (*src != ctxt->color_key)
            *row = *src;
        row += ctxt->step;
        src += ctxt->step;
        w -= ctxt->step;
    }
}

/* =================================== 2 Bytes per pixel ==================== */

void SetPixelSet16(MIL_PixelsContext *ctxt)
{
    *(Uint16 *) ctxt->cur_dst = (Uint16) ctxt->color;
}

void SetHlineSet16(MIL_PixelsContext *ctxt, Uint32 w)
{
    int step = ctxt->step;
    if (ctxt->color != 0 || ctxt->step != 1) {
        Uint16 * dest16 = (Uint16 *)ctxt->cur_dst;
        if (w < 5 || step != 1)
        {
            do {
                *dest16 = (Uint16)ctxt->color;
                dest16 += step;
                w -= step;
            } while (w > 0);
        }
        else
        {
            Uint32 mixture = (((Uint16)ctxt->color << 16) 
                    | (Uint16)ctxt->color);
            Uint32 * dest32;
            int count;
            if ((Uint32)ctxt->cur_dst & 3) // Ensure 4-bytes alignment.
            {
                *dest16++ = (Uint16)ctxt->color;
                --w;
            }
            count = (w >> 1);
            dest32 = (Uint32 *)dest16;
            do {
                *dest32++ = mixture;
            } while (--count > 0); // Please make sure 3 pixels to processed at least!
            if (0 != (w & 1)) // Dealing with the extra pixel.
            {
                *(Uint16 *)dest32 = ctxt->color;
            }
        }
    }
#ifdef ASM_memset2
    else if (ctxt->step == 1 && !((long)ctxt->cur_dst & 2)) {
        ASM_memset2 (ctxt->cur_dst, ctxt->color, w);
    }
#endif
    else {
        memset_hline_helper (ctxt, 2, w);
    }
}

void PutHlineSet16(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    if (ctxt->step == 1) {
        memcpy_hline_helper (ctxt, src, 2, w);
    }
    else {
        Uint16* dstrow = (Uint16*)ctxt->cur_dst;
        Uint16* srcrow = (Uint16*)src;
        while (w > 0) {
            *dstrow = *srcrow;
            dstrow += ctxt->step;
            srcrow += ctxt->step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSkipSet16(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    Uint16* dstrow = (Uint16*)ctxt->cur_dst;
    Uint16* srcrow = (Uint16*)src;

    while (w > 0) {
        if (*srcrow != ctxt->color_key)
            *dstrow = *srcrow;
        dstrow += ctxt->step;
        srcrow += ctxt->step;
        w -= ctxt->step;
    }
}

/* =================================== 3 Bytes per pixel ==================== */
void SetPixelSet24(MIL_PixelsContext *ctxt)
{
    SETVAL_24BIT(ctxt->cur_dst, ctxt->color);
}

void SetHlineSet24(MIL_PixelsContext *ctxt, Uint32 w)
{
    if (ctxt->step == 1 && ctxt->color == 0) {
        memset_hline_helper (ctxt, 3, w);
    }
#ifdef ASM_memset3
    else if (ctxt->step == 1) {
        ASM_memset3 (ctxt->cur_dst, ctxt->color, w);
    }
#endif
    else {
        Uint8* row = ctxt->cur_dst;
        int step = (ctxt->step << 1) + ctxt->step;

        while (w > 0) {
            SETVAL_24BIT (row, ctxt->color);
            row += step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSet24(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    if (ctxt->step == 1) {
        memcpy_hline_helper (ctxt, src, 3, w);
    }
    else {
        Uint8* row = (Uint8*)ctxt->cur_dst;
        int step = (ctxt->step << 1) + ctxt->step;

        while (w > 0) {
            *row = *src;
            *(row + 1) = *(src + 1);
            *(row + 2) = *(src + 2);
            row += step;
            src += step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSkipSet24(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    Uint8* row = (Uint8*)ctxt->cur_dst;
    int step = (ctxt->step << 1) + ctxt->step;

    while (w > 0) {
        if (!EQUAL_24BIT (row, ctxt->color_key)) {
            *row = *src;
            *(row + 1) = *(src + 1);
            *(row + 2) = *(src + 2);
        }
        row += step;
        src += step;
        w -= ctxt->step;
    }
}

/* =================================== 4 Bytes per pixel ==================== */

void SetPixelSet32(MIL_PixelsContext *ctxt)
{
    *(Uint32 *) ctxt->cur_dst = (Uint32) ctxt->color;
}

void SetHlineSet32(MIL_PixelsContext *ctxt, Uint32 w)
{
    Uint32* row = (Uint32*)ctxt->cur_dst;
    
    if (ctxt->step == 1) {
        MIL_memset4 (row, ctxt->color, w);
    }
    else {
        while (w > 0) {
            *row = ctxt->color;
            row += ctxt->step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSet32(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    Uint32* dstrow = (Uint32*)ctxt->cur_dst;
    Uint32* srcrow = (Uint32*)src;

    if (ctxt->step == 1) {
        memcpy_hline_helper (ctxt, src, 4, w);
    }
    else {
        while (w > 0) {
            *dstrow = *srcrow;
            dstrow += ctxt->step;
            srcrow += ctxt->step;
            w -= ctxt->step;
        }
    }
}

void PutHlineSkipSet32(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w)
{
    Uint32* dstrow = (Uint32*)ctxt->cur_dst;
    Uint32* srcrow = (Uint32*)src;

    while (w > 0) {
        if (*srcrow != ctxt->color_key)
            *dstrow = *srcrow;
        dstrow += ctxt->step;
        srcrow += ctxt->step;
        w -= ctxt->step;
    }
}

