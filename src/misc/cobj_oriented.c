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

vtable_t g_NOSUPER_vtable;

INLINE void DELETE(void *ptr)
{
    ((void (*)(void*, int))((*(vtable_t **)ptr)->__entry[3].__address))(ptr, 1);
}

void * look_for_virtual_function(void *obj, const char * func_name)
{
    if (NULL != obj && NULL != func_name)
    {
        vtable_t * vptr = (*(vtable_t **)obj);
        assert(NULL != vptr);
        while (NULL != vptr->__entry) {
            int i = 0;
            while (vptr->__entry[i].__address && vptr->__entry[i].__name) {
                printf("virtual func name: %s\n", vptr->__entry[i].__name);
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

void * find_virtual_function_in_current_class(virtual_function_t* entry, const char * func_name)
{
    int i = 0;
    if (NULL != entry && NULL != func_name)
    {
        while (entry[i].__address && entry[i].__name)
        {
            printf("virtual func name: %s\n", entry[i].__name);
            if (0 == strcmp(func_name, entry[i].__name)) {
                return entry[i].__address;
            }
            ++i;
        }
    }
    return NULL;
}
