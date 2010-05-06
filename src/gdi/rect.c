/*! ============================================================================
 * @file rect.c 
 * @Synopsis Rectangle operation. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "MIL_gdi.h"

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

__INLINE__ MIL_bool
MIL_IntersectRect(const MIL_Rect *A, const MIL_Rect *B, MIL_Rect *intersection)
{
#if 1
    intersection->x = (A->x > B->x) ? A->x : B->x;
    intersection->y  = (A->y > B->y) ? A->y : B->y;
    intersection->w = (A->x + A->w < B->x + B->w) ? 
        (A->x + A->w - intersection->x) : (B->x + B->w - intersection->x);
    intersection->h = (A->y + A->h < B->h + B->h) ? 
        (A->y + A->h - intersection->y) : (B->y + B->h - intersection->y);

    return (intersection->w && intersection->h);
#else
    int Amin, Amax, Bmin, Bmax;

    /* Horizontal intersection */
    Amin = A->x;
    Amax = Amin + A->w;
    Bmin = B->x;
    Bmax = Bmin + B->w;
    if(Bmin > Amin)
        Amin = Bmin;
    intersection->x = Amin;
    if(Bmax < Amax)
        Amax = Bmax;
    intersection->w = Amax - Amin > 0 ? Amax - Amin : 0;

    /* Vertical intersection */
    Amin = A->y;
    Amax = Amin + A->h;
    Bmin = B->y;
    Bmax = Bmin + B->h;
    if(Bmin > Amin)
        Amin = Bmin;
    intersection->y = Amin;
    if(Bmax < Amax)
        Amax = Bmax;
    intersection->h = Amax - Amin > 0 ? Amax - Amin : 0;

    return (intersection->w && intersection->h);
#endif
}

__INLINE__ MIL_bool MIL_IsRectEmpty (const MIL_Rect* prc)
{
    if( 0 == prc->w ) return MIL_TRUE;
    if( 0 == prc->h ) return MIL_TRUE;
    return MIL_FALSE;
}

__INLINE__ MIL_bool MIL_EqualRect (const MIL_Rect* prc1, const MIL_Rect* prc2)
{
    if(prc1->x != prc2->x) return MIL_FALSE;
    if(prc1->y != prc2->y) return MIL_FALSE;
    if(prc1->w != prc2->w) return MIL_FALSE;
    if(prc1->h != prc2->h) return MIL_FALSE;

    return MIL_TRUE;
}

__INLINE__ MIL_bool MIL_IsCovered(const MIL_Rect* prc1, const MIL_Rect* prc2)
{
    if (prc1->x < prc2->x
            || prc1->y < prc2->y
            || prc1->x + prc1->w > prc2->x + prc2->w
            || prc1->y + prc1->h > prc2->y + prc2->h) {
        return MIL_FALSE;
    }

    return MIL_TRUE;
}

__INLINE__ MIL_bool MIL_DoesIntersect (const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    int left, top, right, bottom;
    
    left = (psrc1->x > psrc2->x) ? psrc1->x : psrc2->x;
    top  = (psrc1->y > psrc2->y) ? psrc1->y : psrc2->y;
    right = (psrc1->x + psrc1->w < psrc2->x + psrc2->w) ? 
        psrc1->x + psrc1->w : psrc2->x + psrc2->w;
    bottom = (psrc1->y + psrc1->h < psrc2->y + psrc2->h) 
                   ? psrc1->y + psrc1->h : psrc2->y + psrc2->h;

    return (left >= right || top >= bottom) ? MIL_FALSE : MIL_TRUE;
}

__INLINE__ MIL_bool MIL_UnionRect(MIL_Rect* pdrc, const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    MIL_Rect src1, src2;
    memcpy(&src1, psrc1, sizeof(MIL_Rect));
    memcpy(&src2, psrc2, sizeof(MIL_Rect));

    if (src1.x == src2.x 
        && (src1.x+src1.w) == (src2.x+src2.w)) {
        if (src1.y <= src2.y && src2.y <= (src1.y+src1.h)) {
            pdrc->x = src1.x;
            pdrc->w = src1.w;
            pdrc->y = src1.y;
            pdrc->h = MAX(src1.h, src2.h);

            return MIL_TRUE;
        }
        else if (src1.y >= src2.y && (src2.y+src2.h) >= src1.y) {
            pdrc->x = src1.x;
            pdrc->w = src1.w;
            pdrc->y = src2.y;
            pdrc->h = MAX(src1.h, src2.h);

            return MIL_TRUE;
       }

       return MIL_FALSE;
    }

    if (src1.y == src2.y 
        && (src1.y+src1.h) == (src2.y+src2.h)) {
        if (src1.x <= src2.x && src2.x <= (src1.x+src1.w)) {
            pdrc->y = src1.y;
            pdrc->h = src1.h;
            pdrc->x = src1.x;
            pdrc->w = MAX(src1.w, src2.w);

            return MIL_TRUE;
        }
        else if (src1.x >= src2.x && (src2.x+src2.w) >= src1.x) {
            pdrc->y = src1.y;
            pdrc->h = src1.h;
            pdrc->x = src2.x;
            pdrc->w = MAX(src1.w, src2.w);

            return MIL_TRUE;
       }

/*        return MIL_FALSE;
 */
    }

    return MIL_FALSE;
}

__INLINE__ void MIL_GetBoundRect (MIL_Rect* pdrc,  const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    MIL_Rect src1, src2;
    memcpy(&src1, psrc1, sizeof(MIL_Rect));
    memcpy(&src2, psrc2, sizeof(MIL_Rect));

    pdrc->x = (src1.x < src2.x) ? src1.x : src2.x;
    pdrc->y  = (src1.y < src2.y) ? src1.y : src2.y;
    pdrc->w = ((src1.x + src1.w) > (src2.x + src2.w)) 
        ? src1.w : src2.w;
    pdrc->h = ((src1.y + src1.h) > (src2.y + src2.h)) 
                   ? src1.h : src2.h;
}

__INLINE__ void MIL_SetRectCorner(MIL_Rect* prc, int left, int top, int right, int bottom)
{
    prc->x = left;
    prc->y = top;
    prc->w = right - left;
    prc->h = bottom - top;
}

__INLINE__ void MIL_SetRectSize(MIL_Rect* prc, int x, int y, int w, int h)
{
    prc->x = x;
    prc->y = y;
    prc->w = w;
    prc->h = h;
}
#if 0
void OffsetRect(MIL_Rect* prc, int x, int y)
{
    prc->left += x;
    prc->top += y;
    prc->right += x;
    prc->bottom += y;
}

void InflateRect(MIL_Rect* prc, int cx, int cy)
{
    prc->left -= cx;
    prc->top -= cy;
    prc->right += cx;
    prc->bottom += cy;
}

void InflateRectToPt (MIL_Rect* prc, int x, int y)
{
    if (x < prc->left) prc->left = x;
    if (y < prc->top) prc->top = y;
    if (x > prc->right) prc->right = x;
    if (y > prc->bottom) prc->bottom = y;
}

MIL_bool PtInRect(const MIL_Rect* prc, int x, int y)
{
    if (   x >= prc->left 
        && x < prc->right 
        && y >= prc->top 
        && y < prc->bottom) 
        return MIL_TRUE;
    
    return MIL_FALSE;
}

int MIL_SubtractRect(MIL_Rect* rc, const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    MIL_Rect src, rcExpect, *prcExpect;
    int nCount = 0;
    
    src = *psrc1;
    rcExpect = *psrc2;
    prcExpect = &rcExpect;

    if (!DoesIntersect (&src, prcExpect)) {
        nCount = 1;
        rc[0] = src;
    }
    else {
        if(prcExpect->top > src.top)
        {
            rc[nCount].left  = src.left;
            rc[nCount].top   = src.top;
            rc[nCount].right = src.right;
            rc[nCount].bottom = prcExpect->top;
            nCount++;
            src.top = prcExpect->top;
        }
        if(prcExpect->bottom < src.bottom)
        {
            rc[nCount].top  = prcExpect->bottom;
            rc[nCount].left   = src.left;
            rc[nCount].right = src.right;
            rc[nCount].bottom = src.bottom;
            nCount++;
            src.bottom = prcExpect->bottom;
        }
        if(prcExpect->left > src.left)
        {
            rc[nCount].left  = src.left;
            rc[nCount].top   = src.top;
            rc[nCount].right = prcExpect->left;
            rc[nCount].bottom = src.bottom;
            nCount++;
        }
        if(prcExpect->right < src.right)
        {
            rc[nCount].left  = prcExpect->right;
            rc[nCount].top   = src.top;
            rc[nCount].right = src.right;
            rc[nCount].bottom = src.bottom;
            nCount++;
        }
    }

    return nCount;
}
#endif
