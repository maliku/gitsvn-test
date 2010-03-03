/*!============================================================================
 * @file object.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 02/22/2010 06:39:43 PM 
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <milui/coobase.h>

/* ====================== for class Object ======================*/
CLASS_INHERIT_BEGIN(Object, CooBase)
    MEMBER_FUNCTION_DECLARE_BEGIN(Object)
    void (*test)(_Self(Object), int);
    MEMBER_FUNCTION_DECLARE_END
CLASS_INHERIT_END

#endif
