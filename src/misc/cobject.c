/*! ============================================================================
 * @file cobject.c 
 * @Synopsis Cobject was the root class. 
 * @author DongKai
 * @version 1.0
 * @date 2009年12月01日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include <stdio.h>
#include "cobj_oriented.h"
#include "cobject.h"

VIRTUAL_FUNC_REGBEGIN(Cobject, NOSUPER)
DESTRUCTOR_REGISTER(Cobject)
VIRTUAL_FUNC_REGEND

/* constructor is not necessary but at here for style uniform. */
CONSTRUCTOR_DEFINE(Cobject)
{
    return self; /* Self pointer have to return in constructor. */
}

/* Destructor is necessary because it's virtual. */
DESTRUCTOR_DEFINE(Cobject)
{
}


MEMBER_FUNC_REGBEGIN(Cobject)
MEMBER_FUNC_REGISTER(Cobject, Cobject)
MEMBER_FUNC_REGEND

