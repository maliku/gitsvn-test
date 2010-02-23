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
#include <milui/cobject.h>

/* ====================== for class level1 ======================*/
SUB_CLASS_DEFINE(level1, Cobject)
    MEMBER_FUNC_DECBEGIN(level1)
    void MEMBER_FUNC_DECLARE(level1, test1)
    MEMBER_FUNC_DECEND
SUB_CLASS_DEFEND(level1)

/* ====================== for class level2 ======================*/
SUB_CLASS_DEFINE(level2, level1)
    MEMBER_FUNC_DECBEGIN(level2)
    void MEMBER_FUNC_DECLARE(level2, test2)
    MEMBER_FUNC_DECEND
SUB_CLASS_DEFEND(level2)

#endif
