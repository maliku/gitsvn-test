/*! ============================================================================
 * @file object.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-02-22
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
    _ptm(Object, p_m1) = 110;
    _public(Object)->m0 = 119;
}

void test(_Self(Object), int i)
{
    printf("Object::test input %d.\n", i);
    printf("The private member Object->p_m1 = %d.\n", _private(Object)->p_m1);
    printf("The public member Object->m0 = %d.\n", _public(Object)->m0);
}

BEGIN_METHOD_MAP(Object, MObject)
    CONSTRUCTOR_MAP(Object)
    DESTRUCTOR_MAP(Object)
END_METHOD_MAP

int main()
{
    Object* obj = New(Object);
//    _MC(obj)->test(obj, 666);
    _c(obj)->release(obj);
    return 0;
}

   
