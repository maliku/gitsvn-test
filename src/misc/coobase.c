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
    printf("[MILUI]CooBaseConstructor:%p\n", self);
}

DESTRUCTOR(CooBase)
{
    printf("[MILUI]CooBaseDestructor:%p\n", self);
}

BEGIN_METHOD_MAP(CooBase, NonBase)
CONSTRUCTOR_MAP(CooBase)
DESTRUCTOR_MAP(CooBase)
END_METHOD_MAP




