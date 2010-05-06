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
 * A function to calculate the intersection of two rectangles:
 * return true if the rectangles intersect, false otherwise
 */
extern DECLSPEC MIL_bool
MIL_IntersectRect(const MIL_Rect *A, const MIL_Rect *B, MIL_Rect *);
extern DECLSPEC MIL_bool MIL_IsRectEmpty (const MIL_Rect* prc);
extern DECLSPEC MIL_bool MIL_EqualRect (const MIL_Rect* prc1, const MIL_Rect* prc2);
extern DECLSPEC MIL_bool MIL_DoesIntersect (const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC MIL_bool MIL_UnionRect(MIL_Rect* pdrc, const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC void MIL_GetBoundRect (MIL_Rect* pdrc,  const MIL_Rect* psrc1, const MIL_Rect* psrc2);
extern DECLSPEC void MIL_SetRectCorner(MIL_Rect* prc, int left, int top, int right, int bottom);
extern DECLSPEC void MIL_SetRectSize(MIL_Rect* prc, int x, int y, int w, int h);

#endif   /* ----- #ifndef MIL_GDI_INC  ----- */

