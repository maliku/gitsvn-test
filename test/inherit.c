/*! ============================================================================
 * @file test.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/02/2010
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "inherit.h"

int main()
{
    CooLevel1 * sub = New(CooLevel1);
    Delete(sub);
    return 0;
}
