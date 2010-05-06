/*! ============================================================================
 * @file rect.c 
 * @Synopsis Rectangle operation. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "rect.h"

__INLINE__ MIL_bool 
MIL_IntersectRect(const MIL_Rect *A, const MIL_Rect *B, MIL_Rect *intersection)
{
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
}

__INLINE__ MIL_bool IsRectEmpty (const MIL_Rect* prc)
{
    if( 0 == prc->w ) return MIL_TRUE;
    if( 0 == prc->h ) return MIL_TRUE;
    return MIL_FALSE;
}

__INLINE__ MIL_bool EqualRect (const MIL_Rect* prc1, const MIL_Rect* prc2)
{
    if(prc1->x != prc2->x) return MIL_FALSE;
    if(prc1->y != prc2->y) return MIL_FALSE;
    if(prc1->w != prc2->w) return MIL_FALSE;
    if(prc1->h != prc2->h) return MIL_FALSE;

    return MIL_TRUE;
}

__INLINE__ MIL_bool IsCovered(const MIL_Rect* prc1, const MIL_Rect* prc2)
{
    if (prc1->x < prc2->x
            || prc1->y < prc2->y
            || prc1->x + prc1->w > prc2->x + prc2->w
            || prc1->y + prc1->h > prc2->y + prc2->h) {
        return MIL_FALSE;
    }

    return MIL_TRUE;
}

#if 0
MIL_bool IntersectRect(MIL_Rect* pdrc, const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    pdrc->left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
    pdrc->top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
    pdrc->right = (psrc1->right < psrc2->right) ? psrc1->right : psrc2->right;
    pdrc->bottom = (psrc1->bottom < psrc2->bottom) 
                   ? psrc1->bottom : psrc2->bottom;

    if(pdrc->left >= pdrc->right || pdrc->top >= pdrc->bottom)
        return MIL_FALSE;

    return MIL_TRUE;
}

MIL_bool DoesIntersect (const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    int left, top, right, bottom;
    
    left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
    top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
    right = (psrc1->right < psrc2->right) ? psrc1->right : psrc2->right;
    bottom = (psrc1->bottom < psrc2->bottom) 
                   ? psrc1->bottom : psrc2->bottom;

    if(left >= right || top >= bottom)
        return MIL_FALSE;

    return MIL_TRUE;
}

MIL_bool UnionRect(MIL_Rect* pdrc, const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    MIL_Rect src1, src2;
    memcpy(&src1, psrc1, sizeof(MIL_Rect));
    memcpy(&src2, psrc2, sizeof(MIL_Rect));

    NormalizeRect(&src1);
    NormalizeRect(&src2);

    if (src1.left == src2.left 
        && src1.right == src2.right) {
        if (src1.top <= src2.top && src2.top <= src1.bottom) {
            pdrc->left = src1.left;
            pdrc->right = src1.right;
            pdrc->top = src1.top;
            pdrc->bottom = MAX(src1.bottom, src2.bottom);

            return MIL_TRUE;
        }
        else if (src1.top >= src2.top && src2.bottom >= src1.top) {
            pdrc->left = src1.left;
            pdrc->right = src1.right;
            pdrc->top = src2.top;
            pdrc->bottom = MAX(src1.bottom, src2.bottom);

            return MIL_TRUE;
       }

       return MIL_FALSE;
    }

    if (src1.top == src2.top 
        && src1.bottom == src2.bottom) {
        if (src1.left <= src2.left && src2.left <= src1.right) {
            pdrc->top = src1.top;
            pdrc->bottom = src1.bottom;
            pdrc->left = src1.left;
            pdrc->right = MAX(src1.right, src2.right);

            return MIL_TRUE;
        }
        else if (src1.left >= src2.left && src2.right >= src1.left) {
            pdrc->top = src1.top;
            pdrc->bottom = src1.bottom;
            pdrc->left = src2.left;
            pdrc->right = MAX(src1.right, src2.right);

            return MIL_TRUE;
       }

       return MIL_FALSE;
    }

    return MIL_FALSE;
}

void GetBoundRect (PMIL_Rect pdrc,  const MIL_Rect* psrc1, const MIL_Rect* psrc2)
{
    MIL_Rect src1, src2;
    memcpy(&src1, psrc1, sizeof(MIL_Rect));
    memcpy(&src2, psrc2, sizeof(MIL_Rect));

    NormalizeRect(&src1);
    NormalizeRect(&src2);

    pdrc->left = (src1.left < src2.left) ? src1.left : src2.left;
    pdrc->top  = (src1.top < src2.top) ? src1.top : src2.top;
    pdrc->right = (src1.right > src2.right) ? src1.right : src2.right;
    pdrc->bottom = (src1.bottom > src2.bottom) 
                   ? src1.bottom : src2.bottom;
}

int SubtractRect(MIL_Rect* rc, const MIL_Rect* psrc1, const MIL_Rect* psrc2)
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
