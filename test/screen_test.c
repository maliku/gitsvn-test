/*! ============================================================================
 * @file screen.c 
 * @Synopsis screen test. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */
#ifdef WIN32
#include <MIL_paint.h>
#else
#include <milui/MIL_paint.h>
#endif


int main()
{
    MScreen* scr = MIL_TakeScreen(0);
    if (NULL != scr) {
        MIL_Color c = {255, 255, 0};
        MIL_Rect rc = {321, 240, 320, 240};
        _c(scr)->solidFill(scr, &c, &rc);
    }
}
