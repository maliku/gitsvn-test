/*! ============================================================================
 * @file object.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年02月22日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "object.h"

/* ====================== for class level1 ======================*/
DESTRUCTOR_DEFINE(level1)
{
    printf("level1 %p destructed...\n", self);
}

CONSTRUCTOR_DEFINE(level1)
{
    printf("level1 %p constructed...\n", self);
    return self;
}

void MEMBER_FUNC_DEFINE(level1, test)
{
    puts("level1::test\n");
}

void MEMBER_FUNC_DEFINE(level1, virtual_function_test, int a)
{
    printf("level1:virtual_function_test(%d).\n", a);
}

VIRTUAL_FUNC_REGBEGIN(level1, Cobject)
DESTRUCTOR_REGISTER(level1)
VIRTUAL_MEMFUNC_REGISTER(level1, virtual_function_test)
VIRTUAL_FUNC_REGEND

MEMBER_FUNC_REGBEGIN(level1)
MEMBER_FUNC_REGISTER(level1, level1)
MEMBER_FUNC_REGISTER(level1, test)
MEMBER_FUNC_REGEND

/* ====================== for class level2 ======================*/
DESTRUCTOR_DEFINE(level2)
{
    printf("level2 %p destructed...\n", self);
}

CONSTRUCTOR_DEFINE(level2)
{
    printf("level2 %p constructed...\n", self);
    return self;
}

void MEMBER_FUNC_DEFINE(level2, test)
{
    puts("level2::test\n");
}

void MEMBER_FUNC_DEFINE(level2, virtual_function_test, int a)
{
    printf("level2:virtual_function_test(%d).\n", a);
}

VIRTUAL_FUNC_REGBEGIN(level2, level1)
DESTRUCTOR_REGISTER(level2)
VIRTUAL_MEMFUNC_REGISTER(level2, virtual_function_test)
VIRTUAL_FUNC_REGEND

MEMBER_FUNC_REGBEGIN(level2)
MEMBER_FUN