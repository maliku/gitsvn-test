/*! ============================================================================
 * @file test.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/01/1010
 *  Organization: http://www.ds0101.net
 */

#include "inherit.h"

int main()
{
    CooLevel2 * sub = (CooLevel2*)New(CooLevel2);
    _c(sub)->log((CooSub*)sub);
    _c(sub)->release(sub);
    return 0;
}
