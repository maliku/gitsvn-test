/*! ============================================================================
 * @file shape.c 
 * @Synopsis Shapes generator. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "MIL_gdi.h"
#include "shape.h"

MIL_Status 
LineGenerator(void* user_context, int x1, int y1, int x2, int y2, MIL_LineCB cb)
{
    if (NULL != cb) {
        int  dt_x = x2 - x1, dt_y = y2 - y1, step_x, step_y, change;

        if (dt_x < 0) {
            dt_x = -dt_x; /* Draw a line from right to left .*/
            step_x = -1;
        }
        else {
            step_x = 1;
        }  /* Draw a line from left to right.*/

        if (dt_y < 0) {
            dt_y = -dt_y;    /* Draw a line from bottom to top.*/
            step_y = -1;
        }
        else {
            step_y = 1;
        }
        /* Draw a line fromtop to bottom .*/
        if (dt_x > dt_y) {  /* x-value changed faster than y-value */
            change = dt_x >> 1;
            while (x1 != x2) {
                cb(user_context, x1, y1);
                x1 += step_x;
                change += dt_y;
                if(change > dt_x) {
                    y1 += step_y;
                    change -= dt_x;
                }
            }
        }
        else {                /* y-value changed faster than x-value */
            change = dt_y >> 1;
            while(y1 != y2) {
                cb(user_context, x1, y1);
                y1 += step_y;
                change += dt_x;
                if(change > dt_y) {
                    x1 += step_x;
                    change -= dt_y;
                }
            }
        }
        cb(user_context, x2, y2);
        return MIL_OK;
    }
    return MIL_INVALID_PARAMETER;
}

