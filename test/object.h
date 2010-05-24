/*!============================================================================
 * @file object.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 02/22/2010 06:39:43 PM 
 *  Organization: http://www.ds0101.net
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <milui/coobase.h>

/* ====================== for class Object ======================*/
CLASS_INHERIT_BEGIN(Object, CooBase)
    NO_METHOD_EXPAND(Object)

    int m0;
    PRIVATE_BEGIN(Object)
    int p_m1;
    char p_m2;
    PRIVATE_END
CLASS_INHERIT_END

#endif
