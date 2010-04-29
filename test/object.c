/*! ============================================================================
 * @file object.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年02月22日
 *  Organization: http://www.ds0101.net
 */

#include "object.h"

/* ====================== for class Object ======================*/

DESTRUCTOR(Object)
{
    printf("Object %p destructed...\n", self);
}

CONSTRUCTOR(Object)
{
    printf("Object %p constructed...\n", self);
    return self;
}

void test(_Self(Object), int i)
{
    printf("Object::test input %d.\n", i);
}

VIRTUAL_METHOD_REGBEGIN(Object, CooBase)
    DESTRUCTOR_REGISTER(Object)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(Object)
    CONSTRUCTOR_REGISTER(Object)
    test
    METHOD_REGEND

int main()
{
    Object* obj = New(Object);
    _MC(obj)->test(obj, 666);
    Delete(obj);
    return 0;
}

   
