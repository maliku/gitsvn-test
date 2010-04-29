/*! ============================================================================
 * @file coobase.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/04/2010
 *  Organization: http://www.ds0101.net
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

VIRTUAL_METHOD_REGBEGIN(CooBase, NonBase)
DESTRUCTOR_REGISTER(CooBase)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(CooBase)
CONSTRUCTOR_REGISTER(CooBase)
METHOD_REGEND

