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
#include <milui/MIL_object.h>

/* ====================== for class Object ======================*/
BEGIN_CLASS_INHERIT(Object, MObject)
    BEGIN_METHOD_EXPAND_DECLARE(Object)
        METHOD_TABLE(MObject)
        void (*test)(_Self(Object), int);
    END_METHOD_EXPAND_DECLARE

    int m0;
    BEGIN_PRIVATE(Object)
    int p_m1;
    char p_m2;
    END_PRIVATE
END_CLASS_INHERIT

#endif
