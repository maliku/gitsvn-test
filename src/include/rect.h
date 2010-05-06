/*!============================================================================
 * @file rect.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  RECT_INC
#define  RECT_INC

#include "coo.h"
#include "MIL_video.h"

/*
 * A function to calculate the intersection of two rectangles:
 * return true if the rectangles intersect, false otherwise
 */
extern DECLSPEC MIL_bool 
MIL_IntersectRect(const MIL_Rect *A, const MIL_Rect *B, MIL_Rect *);
extern DECLSPEC MIL_bool IsRectEmpty (const MIL_Rect* prc);
extern DECLSPEC MIL_bool EqualRect (const MIL_Rect* prc1, const MIL_Rect* prc2);
#endif   /* ----- #ifndef RECT_INC  ----- */

