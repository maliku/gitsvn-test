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

VIRTUAL_FUNCTION_REGBEGIN(Object, CooBase)
    DESTRUCTOR_REGISTER(Object)
    VIRTUAL_FUNCTION_REGEND

MEMBER_FUNCTION_REGBEGIN(Object)
    CONSTRUCTOR_REGISTER(Object)
    test
    MEMBER_FUNCTION_REGEND

int main()
{
    Object* obj = New(Object);
    _C(obj)->test(obj, 666);
    obj->MCALL(test)(obj, 777);
   