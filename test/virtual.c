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
    return self;
}

void METHOD_NAMED(MyBase, vtest)(_SELF, int in)
{
    printf("MyBase::vtest - Obj from MyBase %p get %d.\n", (MyBase*)self, in);
}

VIRTUAL_METHOD_MAP_BEGIN(MyBase, NonBase)
    DESTRUCTOR_MAP(MyBase)
METHOD_MAP(MyBase, vtest)
    VIRTUAL_METHOD_MAP_END

METHOD_MAP_BEGIN(MyBase)
    CONSTRUCTOR_MAP(MyBase)
    METHOD_MAP_END

/* ====================== for class MyBase ======================*/
DESTRUCTOR(MySub)
{
    printf("MySub %p destructed...\n", self);
}

CONSTRUCTOR(MySub)
{
    printf("MySub %p constructed...\n", self);
    return self;
}

void METHOD_NAMED(MySub, vtest)(_SELF, int in)
{
    printf("MySub::vtest - Obj from MySub %p get %d.\n", (MySub*)self, in);
}

void METHOD_NAMED(MySub, ptest)(_SELF, int in)
{
    printf("MySub::ptest - Obj from MySub %p get %d.\n", (MySub*)self, in);
}
    
VIRTUAL_METHOD_MAP_BEGIN(MySub, MyBase)
    DESTRUCTOR_MAP(MySub)
    NULL,
//    METHOD_MAP(MySub, vtest)
    METHOD_MAP(MySub, ptest)
    VIRTUAL_METHOD_MAP_END

METHOD_MAP_BEGIN(MySub)
    CONSTRUCTOR_MAP(MySub)
    METHOD_MAP_END

DESTRUCTOR(MySub2)
{
    printf("MySub2 %p destructed...\n", self);
}

CONSTRUCTOR(MySub2)
{
    printf("MySub2 %p constructed...\n", self);
    return self;
}

void METHOD_NAMED(MySub2, vtest)(_SELF, int in)
{
    printf("MySub2::vtest - Obj from MySub2 %p get %d.\n", (MySub*)self, in);
}


VIRTUAL_METHOD_MAP_BEGIN(MySub2, MySub)
    DESTRUCTOR_MAP(MySub2)
    METHOD_MAP(MySub2, vtest)
    VIRTUAL_METHOD_MAP_END

METHOD_MAP_BEGIN(MySub2)
    CONSTRUCTOR_MAP(MySub2)
METHOD_MAP_END

int main()
{
    MyBase* base = (MyBase*)New(MyBase);
    assert(NULL == DynamicCast(MySub, base));
    _VC(base)->vtest(base, 123);
    Delete(base);
    printf("==============================\n\n");
    MySub* sub = (MySub*)New(MySub);
    assert(NULL != DynamicCast(MyBase, sub));
    puts(GetTypeName(sub));
    VirtualMethodVerify(sub, vtest);
    _VC(sub)->vtest(sub, 456);
    _VC(sub)->ptest(sub, 123);
    Delete(sub);
    printf("==============================\n\n");
    MySub* sub2 = (MySub*)New(MySub2);
    assert(NULL != DynamicCast(MyBase, sub2));
    assert(NULL != DynamicCast(MySub2, sub2));
    assert(NULL != DynamicCast(MySub, sub2));
    puts(GetTypeName(sub2));
    VirtualMethodVerify(sub2, vtest);
    _VC(sub2)->vtest(sub2, 789);
    _vc1(sub2, ptest, 456);
    Delete(sub2);

    return 0;
}
