#include "coo.h"

DECLSPEC CommonVtable g_NonBaseVtable;

__INLINE__ void Delete(void *ptr)
{
	if (NULL != ptr) {
		assert ((*(CommonVtable**)ptr)->Predestructor);
        (*(CommonVtable**)ptr)->Predestructor(ptr);
        MIL_free(ptr);
    }
}

__INLINE__ void* SafeCast(void* vtable, void* ptr)
{
    if (NULL != ptr) {
        RTTI* tmp = *(RTTI**)ptr;
        while (tmp) {
            if (vtable == tmp) {
                return ptr;
            }
            /*tmp = *(RTTI**)tmp;*/ /* As same as */ tmp = tmp->__base;
        }
    }
    return NULL;
}

