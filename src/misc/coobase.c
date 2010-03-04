/*! ============================================================================
 * @file coobase.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/04/2010
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "coobase.h"

CONSTRUCTOR(CooBase)
{
    printf("CooBaseConstructor:%p\n", self);
    return self;
}

DESTRUCTOR(CooBase)
{
    printf("CooBaseDestructor:%p\n", self);
}

VIRTUAL_FUNCTION_REGBEGIN(CooBase, NonBase)
DESTRUCTOR_REGISTER(CooBase)
VIRTUAL_FUNCTION_REGEND

MEMBER_FUNCTION_REGBEGIN(CooBase)
CONSTRUCTOR_REGISTER(CooBase)
MEMBER_FUNCTION_REGEND

