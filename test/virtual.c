/*! ============================================================================
 * @file object.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-02-22
 *  Organization: http://www.ds0101.net
 */

#include "virtual.h"
/* ====================== for class MyBase ======================*/
DESTRUCTOR(MyBase)
{
    printf("MyBase %p destructed...\n", self);
}

CONSTRUCTOR(MyBase)
{
    printf("MyBase %p constructed...\n", self);
}

void METHOD_NAMED(MyBase, vtest)(_SELF, int in)
{
    printf("MyBase::vtest - Obj from MyBase %p get %d.\n", (MyBase*)self, in);
}

BEGIN_METHOD_MAP(MyBase, NonBase)
    CONSTRUCTOR_MAP(MyBase)
    DESTRUCTOR_MAP(MyBase)
METHOD_MAP(MyBase, vtest)
    END_METHOD_MAP


    

/* ====================== for class MyBase ======================*/
DESTRUCTOR(MySub)
{
    printf("MySub %p destructed...\n", self);
}

CONSTRUCTOR(MySub)
{
    printf("MySub %p constructed...\n", self);
}

void METHOD_NAMED(MySub, vtest)(_SELF, int in)
{
    printf("MySub::vtest - Obj from MySub %p get %d.\n", (MySub*)self, in);
}

void METHOD_NAMED(MySub, ptest)(_SELF, int in)
{
    printf("MySub::ptest - Obj from MySub %p get %d.\n", (MySub*)self, in);
}
    
BEGIN_METHOD_MAP(MySub, MyBase)
    CONSTRUCTOR_MAP(MySub)
    DESTRUCTOR_MAP(MySub)
    METHOD_MAP(MySub, vtest)
    METHOD_MAP(MySub, ptest)
    END_METHOD_MAP


    

DESTRUCTOR(MySub2)
{
    printf("MySub2 %p destructed...\n", self);
}

CONSTRUCTOR(MySub2)
{
    printf("MySub2 %p constructed...\n", self);
}

void METHOD_NAMED(MySub2, vtest)(_SELF, int in)
{
    printf("MySub2::vtest - Obj from MySub2 %p get %d.\n", (MySub*)self, in);
}


BEGIN_METHOD_MAP(MySub2, MySub)
    CONSTRUCTOR_MAP(MySub2)
    DESTRUCTOR_MAP(MySub2)
    METHOD_MAP(MySub2, vtest)
    END_METHOD_MAP




int main()
{
	MySub* sub;
	MySub* sub2;
    MyBase* base = (MyBase*)New(MyBase);
    assert(NULL == DynamicCast(MySub, base));
    _c(base)->vtest(base, 123);
    Delete(base);
    printf("==============================\n\n");
    sub = (MySub*)New(MySub2);
    assert(NULL != DynamicCast(MyBase, sub));
    assert(NULL != DynamicCast(MySub2, sub));
    puts(GetTypeName(sub));
    _c(sub)->vtest(sub, 456);
    _c(sub)->ptest(sub, 123);
    Delete(sub);
    printf("==============================\n\n");
    sub2 = (MySub*)New(MySub);
    assert(NULL != DynamicCast(MyBase, sub2));
    assert(NULL != DynamicCast(MySub, sub2));
    puts(GetTypeName(sub2));
    _c(sub2)->vtest(sub2, 789);
    _c(sub2)->ptest(sub2, 456);
    Delete(sub2);

    return 0;
}
