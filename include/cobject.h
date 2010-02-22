/*! ============================================================================
 * @file oop.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2009年12月01日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef __COBJECT_H__
#define __COBJECT_H__

#include "cobj_oriented.h"

CLASS(Cobject)
{
    vtable_t * vptr;
    MEMBER_FUNC_DECBEGIN(Cobject)
    MEMBER_FUNC_DECEND
} Cobject;

#endif
