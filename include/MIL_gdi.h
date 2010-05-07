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
extern DECLSPEC MIL_bool
MIL_IntersectRect(const MIL_Rect *A, const MIL_Rect *B, MIL_Rect *intersection);

/** 
 * @synopsis Check the rect whether is empty. 
 * 
 * @param prc Rect will checked.
 * 
 * @returns MIL_TRUE if the rectangle is empty, MIL_FALSE otherwise.
 */
extern DECLSPEC MIL_bool MIL_IsRectEmpty (const MIL_Rect* prc);

/** 
 * @synopsis Check two rectangles whether equivalent.
 * 
 * @param prc1 The rectangle which will to join in the calculation.
 * @param prc2 The rectangle which will to join in the calculation.
 * 
 * @returns MIL_TRUE if rectangles are equivalent, MIL_FALSE otherwise.  
 */
extern DECLSPEC MIL_bool MIL_EqualRect (const MIL_Rect* prc1, const MIL_Rect* prc2);
extern DECLSPEC MIL_bool MIL_DoesIntersect (const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC MIL_bool MIL_UnionRect(MIL_Rect* pdrc, const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC void MIL_GetBoundRect (MIL_Rect* pdrc,  const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC void MIL_SetRectByCorner(MIL_Rect* prc, int left, int top, int right, int bottom);
extern DECLSPEC void MIL_SetRectBySize(MIL_Rect* prc, int x, int y, int w, int h);
extern DECLSPEC void MIL_OffsetRect(MIL_Rect* prc, int x, int y);
extern DECLSPEC void MIL_InflateRect(MIL_Rect* prc, int cx, int cy);
extern DECLSPEC void MIL_InflateRectToPt (MIL_Rect* prc, int x, int y);
extern DECLSPEC MIL_bool MIL_PtInRect(const MIL_Rect* prc, int x, int y);
DECLSPEC int MIL_SubtractRect(MIL_Rect* rc, const MIL_Rect* psrc1, const MIL_Rect* psrc2);

#endif   /* ----- #ifndef MIL_GDI_INC  ----- */

