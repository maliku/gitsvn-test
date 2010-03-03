/*! ============================================================================
 * @file object.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年02月22日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "object.h"

/* ====================== for class Object ======================*/

void ObjectDestructor(_SELF)
{
    printf("Object %p destructed...\n", self);
}

Object* ObjectConstructor(_Self(Object))
{
    printf("Object %p constructed...\n", self);
    return self;
}

void test(_Self(Object), int i)
{
    printf("Object::test input %d.\n", i);
}

VIRTUAL_FUNCTION_REGBEGIN(Object, CooBase)
    ObjectDestructor
VIRTUAL_FUNCTION_REGEND

MEMBER_FUNCTION_REGBEGIN(Object)
    ObjectConstructor,
    test
    MEMBER_FUNCTION_REGEND

int main()
{
    Object* obj = New(Object);
    obj->__mptr->test(obj, 666);
    Delete(obj);

    return 0;
}
