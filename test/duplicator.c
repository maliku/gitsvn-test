/*! ============================================================================
 * @file dup_constructor.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-03-04
 *  Organization: http://www.ds0101.net
 */

#include "inherit.h"

COPY_CONSTRUCTOR(CooLevel1)
{
    puts("Duplicator of CooLevel1.");
}

int main()
{
    CooLevel1 * lv1 = NULL;
    CooLevel1 * sub = New(CooLevel1);
    printf("\n");
    lv1 = DupNew(CooLevel1, sub);
    Delete(lv1);
    printf("\n");
    Delete(sub);

    return 0;
}
