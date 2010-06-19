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
}

void METHOD_NAMED(MyBase, virtual_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

void METHOD_NAMED(MyBase, normal_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

BEGIN_METHOD_MAP(MyBase, NonBase)
    CONSTRUCTOR_MAP(MyBase)
    METHOD_MAP(MyBase, virtual_method)
END_METHOD_MAP


//    METHOD_MAP(MyBase, normal_method)


void METHOD_NAMED(MySub, virtual_method)(_Self(MyBase))
{
    printf("Now count = %d.\n", self->m_count++);
}

BEGIN_METHOD_MAP(MySub, MyBase)
    METHOD_MAP(MySub, virtual_method)
END_METHOD_MAP

#define TEST_COUNT 50000
int main()
{
    time_t start, end;
    double diff = .0;
    int i = 0;
	MySub* sub = NULL;
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

    sub = (MySub*)New(MySub);
    time(&start);
    for (i = 0; i < TEST_COUNT; i++) {
        _c(sub)->virtual_method((MyBase*)sub);
    }
    time(&end);
    diff = difftime(end, start);
    printf("It took you %f seconds to invoke virtual method.\n", diff);

    Delete(sub);

    return 0;
}
