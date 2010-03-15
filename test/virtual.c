/*! ============================================================================
 * @file object.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年02月22日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
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

void MyBasevtest(_SELF, int in)
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

void MySubvtest(_SELF, int in)
{
    printf("Obj from MySub %p get %d.\n", (MySub*)self, in);
}

VIRTUAL_METHOD_REGBEGIN(MySub, MyBase)
    DESTRUCTOR_REGISTER(MySub)
METHOD_REGISTER(MySub, vtest)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MySub)
    CONSTRUCTOR_REGISTER(MySub)
    METHOD_REGEND

int main()
{
    MyBase* base = (MyBase*)New(MyBase);
    _VC(base)->vtest(base, 123);
    Delete(base);
    printf("=========================\n");
    MySub* sub = (MySub*)New(MySub);
    _VC(sub)->vtest(sub, 456);
    Delete(sub);

    return 0;
}
