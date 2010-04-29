/*! ============================================================================
 * @file application.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日
 *  Organization: http://www.ds0101.net
 */

#include <milui/MIL_application.h>

int main(char ** args, int argc)
{
    MIL_Application * app = CreateApplication(args, argc);
    Delete(app);
    return 0;
}
