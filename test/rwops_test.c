/*! ============================================================================
 * @file rwops_test.c 
 * @Synopsis   
 * @author DongKai
 * @version 1.0
 * @date 2010年03月08日
 *  Organization: http://www.ds0101.net
 */
#include <string.h>
#include <milui/MIL_rwops.h>

int main()
{
    char content[] = "Hello World!";
    MIL_RWops * ops = MIL_RWFromFile("./rwops.tmp", "rw");
    if (NULL != ops) {
        _VC(ops)->write(ops, content, strlen(content), 1);
        _VC(ops)->close(ops);
    }
    else {
        puts("File open failed.");
    }
    return 0;
}
