/*! ============================================================================
 * @file rwops_test.c 
 * @Synopsis RWops test. 
 * @author DongKai
 * @version 1.0
 * @date 2010-03-08
 *  Organization: http://www.ds0101.net
 */
#include <string.h>
#ifdef WIN32
#include <MIL_rwops.h>
#else
#include <milui/MIL_rwops.h>
#endif


int main()
{
    char content[] = "Hello World!";
    char buffer[32] = {'\0'};
    MIL_RWops * ops = MIL_RWFromFile("./rwops.tmp", "w+");
    if (NULL != ops) {
        _c(ops)->write(ops, content, strlen(content), 1);
        _c(ops)->seek(ops, 0, SEEK_SET);
        _c(ops)->read(ops, buffer, sizeof(buffer) - 1, 1);
        printf("read from file:%s.\n", buffer);
        _c(ops)->close(ops);
    }
    else {
        puts("File open failed.");
    }
    ops = MIL_RWFromMem(buffer, sizeof(buffer));
    if (NULL != ops) {
        _c(ops)->write(ops, "olleH", strlen("olleH"), 1);
        _c(ops)->seek(ops, 0, SEEK_SET);
        _c(ops)->read(ops, buffer, sizeof(buffer) - 1, 1);
        printf("read from mem:%s.\n", buffer);
        _c(ops)->close(ops);
    }

    return 0;
}
