/*! ============================================================================
 * @file rwops_test.c 
 * @Synopsis RWops test. 
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
    char buffer[32] = {'\0'};
    MIL_RWops * ops = MIL_RWFromFile("./rwops.tmp", "w+");
    if (NULL != ops) {
        _VC(ops)->write(ops, content, strlen(content), 1);
        _VC(ops)->seek(ops, 0, SEEK_SET);
        _VC(ops)->read(ops, buffer, sizeof(buffer) - 1, 1);
        printf("read from file:%s.\n", buffer);
        _VC(ops)->close(ops);
    }
    else {
        puts("File open failed.");
    }
    ops = MIL_RWFromMem(buffer, sizeof(buffer));
    if (NULL != ops) {
        _VC(ops)->write(ops, "olleH", strlen("olleH"), 1);
        _VC(ops)->seek(ops, 0, SEEK_SET);
        _VC(ops)->read(ops, buffer, sizeof(buffer) - 1, 1);
        printf("read from mem:%s.\n", buffer);
        _VC(ops)->close(ops);
    }

    return 0;
}
