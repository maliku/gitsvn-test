/*! ============================================================================
 * @file performance.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月15日
 *  Organization: http://www.ds0101.net
 */

#include <stdio.h>
#include <time.h>
#include "performance.h"

CONSTRUCTOR(MyBase)
{
    self->m_count = 0;
    return self;
}

void METHOD_NAMED(MyBase, virtual_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

void METHOD_NAMED(MyBase, normal_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

VIRTUAL_METHOD_REGBEGIN(MyBase, NonBase)
    NON_DESTRUCTOR
    METHOD_REGISTER(MyBase, virtual_method)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MyBase)
    CONSTRUCTOR_REGISTER(MyBase)
    METHOD_REGISTER(MyBase, normal_method)
METHOD_REGEND

void METHOD_NAMED(MySub, virtual_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

VIRTUAL_METHOD_REGBEGIN(MySub, MyBase)
    NON_DESTRUCTOR
    METHOD_REGISTER(MySub, virtual_method)
VIRTUAL_METHOD_REGEND

METHOD_REGISTER_PLACEHOLDER(MySub)

#define TEST_COUNT 50000
int main()
{
    time_t start, end;
    double diff = .0;
    int i = 0;
    MyBase* base = (MyBase*)New(MyBase);
    time(&start);
    for (i = 0; i < TEST_COUNT; i++) {
        _VC(base)->virtual_method(base);
    }
    time(&end);
    diff = difftime(end, start);
    printf("It took you %f seconds to invoke virtual method.\n", diff);
    fgetc(stdin);

    time(&start);
    for (i = 0; i < TEST_COUNT; i++) {
        _MC(base)->normal_method(base);
    }
    time(&end);
    diff = difftime(end, start);
    printf("It took you %f seconds to invoke member method.\n", diff);
    fgetc(stdin);

    Delete(base);

    MySub* sub = (MySub*)New(MySub);
    time(&start);
    for (i = 0; i < TEST_COUNT; i++) {
        _VC(sub)->virtual_method((MyBase*)sub);
    }
    time(&end);
    diff = difftime(end, start);
    printf("It took you %f seconds to invoke virtual method.\n", diff);

    Delete(sub);

    return 0;
}
