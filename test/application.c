/*! ============================================================================
 * @file application.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include <milui/MIL_application.h>

int main()
{
    MIL_Application * app = New(MIL_Application);
    Delete(app);
    return 0;
}
