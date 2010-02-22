/*! ============================================================================
 * @file cobj_oriented.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年02月02日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include <assert.h>
#include <string.h>
#include "cobj_oriented.h"

vtable_t g_NOSUPER_vtable = {NULL, NULL};

void * look_for_virtual_function(void *obj, const char * func_name)
{
    int i = 0;
    if (NULL != obj && NULL != func_name)
    {
        vtable_t * vptr = (*(vtable_t **)obj);
        while (NULL != vptr) {
            while (vptr->__entry[i].__address && vptr->__entry[i].__name)
            {
                if (0 == strcmp(func_name, vptr->__entry[i].__name)) {
                    return vptr->__entry[i].__address;
                }
                ++i;
            }
            vptr = vptr->__base;
        }
    }
    return NULL;
}
