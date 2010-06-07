/*!============================================================================
 * @file shape.h 
 * @Synopsis Shape generator. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  SHAPE_INC
#define  SHAPE_INC
typedef void (*MIL_LineCB)(void*, int, int);

MIL_Status 
LineGenerator(void* user_context, int x1, int y1, int x2, int y2, MIL_LineCB cb);
#endif   /* ----- #ifndef SHAPE_INC  ----- */

