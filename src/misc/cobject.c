/*! ============================================================================
 * @file oop.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2009年12月01日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include <stdio.h>
#include "cobj_oriented.h"
#include "cobject.h"

VIRTUAL_FUNC_REGISTER(Cobject, NOSUPER)
DESTRUCTOR_REGISTER(Cobject)
VIRTUAL_FUNC_REGEND

CONSTRUCTOR_DEFINE(Cobject)
{
}

DESTRUCTOR_DEFINE(Cobject)
{
}

MEMBER_FUNC_REGBEGIN(Cobject)
MEMBER_FUNC_REGISTER(Cobject, Cobject)
MEMBER_FUNC_REGEND

