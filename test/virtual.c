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
    printf("Obj from MyBase %p get %d.\n", (MyBase*)self, in);
}

VIRTUAL_METHOD_REGBEGIN(MyBase, NonBase)
    DESTRUCTOR_REGISTER(MyBase)
METHOD_REGISTER(MyBase, vtest)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MyBase)
    CONSTRUCTOR_REGISTER(MyBase)
    METHOD_REGEND

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
    printf("Obj from MySub %p get %d.\n", (MySub*)self, in);
}

VIRTUAL_METHOD_REGBEGIN(MySub, MyBase)
    DESTRUCTOR_REGISTER(MySub)
//METHOD_REGISTER(MySub, vtest)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MySub)
    CONSTRUCTOR_REGISTER(MySub)
    METHOD_REGEND

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
    printf("Obj from MySub %p get %d.\n", (MySub*)self, in);
}

VIRTUAL_METHOD_REGBEGIN(MySub2, MySub)
    DESTRUCTOR_REGISTER(MySub2)
//METHOD_REGISTER(MySub, vtest)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MySub2)
    CONSTRUCTOR_REGISTER(MySub2)
    METHOD_REGEND

int main()
{
    MyBase* base = (MyBase*)New(MyBase);
    assert(NULL == DynamicCast(MySub, base));
    _VC(base)->vtest(base, 123);
    Delete(base);
    printf("==============================\n");
    MyBase* sub = (MyBase*)New(MySub);
    assert(NULL != DynamicCast(MyBase, sub));
    puts(GetTypeName(sub));
    VirtualMethodVerify(sub, vtest);
    _VC(sub)->vtest(sub, 456);
    Delete(sub);
    printf("==============================\n");
    MySub2* sub2 = (MySub2*)New(MySub2);
    assert(NULL != DynamicCast(MyBase, sub2));
    assert(NULL != DynamicCast(MySub2, sub2));
    assert(NULL != DynamicCast(MySub, sub2));
    puts(GetTypeName(sub2));
    VirtualMethodVerify(sub2, vtest);
    _VC(sub2)->vtest(sub2, 789);
    Delete(sub2);

    return 0;
}
