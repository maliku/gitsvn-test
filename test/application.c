/*! ============================================================================
 * @file application.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include <milui/MIL_application.h>

int main(char ** args, int argnum)
{
    MIL_Application * app = CreateApplication(args, argnum);
    Delete(app);
    return 0;
}
