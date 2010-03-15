/*! ============================================================================
 * @file performance.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月15日
 *  Organization: http://www.ds0101.net
 */

#include <sys/time.h>
#include "performance.h"

CONSTRUCTOR(MyBase)
{
    self->m_count = 0;
    return self;
}

void METHOD_NAMED(MyBase, virtual_func)(_Self(MyBase))
{
    self->m_count++;
}

void METHOD_NAMED(MyBase, normal_func)(_Self(MyBase))
{
    self->m_count++;
}

VIRTUAL_METHOD_REGBEGIN(MyBase, NonBase)
    NON_DESTRUCTOR
    METHOD_REGISTER(MyBase, virtual_func)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MyBase)
    CONSTRUCTOR_REGISTER(MyBase)
    METHOD_REGISTER(MyBase, normal_func)
METHOD_REGEND

void METHOD_NAMED(MySub, virtual_func)(_Self(MyBase))
{
    self->m_count++;
}

VIRTUAL_METHOD_REGBEGIN(MySub, MyBase)
    NON_DESTRUCTOR
    METHOD_REGISTER(MySub, virtual_func)
VIRTUAL_METHOD_REGEND

METHOD_REGISTER_PLACEHOLDER(MySub)

int main()
{
    MyBase* base = (MyBase*)New(MyBase);
    _VC(base)->virtual_method(base);
    _MC(base)->normal_method(base);
    Delete(base);
    printf("=========================\n");
    MySub* sub = (MySub*)New(MySub);
    _VC(sub)->virtual_method((MyBase*)sub);
    Delete(sub);

    return 0;
}
