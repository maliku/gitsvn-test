/*! ============================================================================
 * @file test.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/02/2010
 *  Organization: http://www.ds0101.net
 */

#include "inherit.h"

int main()
{
    CooLevel1 * sub = New(CooLevel1);
    Delete(sub);
    return 0;
}
