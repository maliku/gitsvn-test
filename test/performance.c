/*! ============================================================================
 * @file performance.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-03-15
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

VIRTUAL_METHOD_MAP_BEGIN(MyBase, NonBase)
    CONSTRUCTOR_MAP(MyBase)
    NON_DESTRUCTOR
    METHOD_MAP(MyBase, virtual_method)
VIRTUAL_METHOD_MAP_END

METHOD_MAP_BEGIN(MyBase)
//    METHOD_MAP(MyBase, normal_method)
METHOD_MAP_END

void METHOD_NAMED(MySub, virtual_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

VIRTUAL_METHOD_MAP_BEGIN(MySub, MyBase)
    NON_CONSTRUCTOR
    NON_DESTRUCTOR
    METHOD_MAP(MySub, virtual_method)
VIRTUAL_METHOD_MAP_END

METHOD_MAP_PLACEHOLDER(MySub)

#define TEST_COUNT 50000
int main()
{
    time_t start, end;
    double diff = .0;
    int i = 0;
    MyBase* base = (MyBase*)New(MyBase);
    time(&start);
    for (i = 0; i < TEST_COUNT; i++) {
        _vc0(base, virtual_method);
    }
    time(&end);
    diff = difftime(end, start);
    printf("It took you %f seconds to invoke virtual method.\n", diff);
    fgetc(stdin);

    time(&start);
    for (i = 0; i < TEST_COUNT; i++) {
        MyBase_X_normal_method(base);
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
