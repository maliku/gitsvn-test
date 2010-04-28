#include "coo.h"

CommonVtable g_NonBaseVtable;

__inline__ void OrderDestruct(void * ptr)
{
	if (NULL != ptr)
	{
		assert (NULL != (*(CommonVtable**)ptr));
        if (NULL != (*(CommonVtable**)ptr)->Destructor)
            (*(CommonVtable**)ptr)->Destructor(ptr);
		assert ((*(CommonVtable**)ptr)->Predestructor);
        (*(CommonVtable**)ptr)->Predestructor(ptr);
	}
}

__inline__ void Delete(void *ptr)
{
    OrderDestruct(ptr);
    MIL_free(ptr);
}

__inline__ void* SafeCast(void* vtable, void* ptr)
{
/*     printf("vtable=%p, ptr=%p\n", vtable, ptr);
 */
    if (NULL != ptr) {
        RTTI* tmp = *(RTTI**)ptr;
        while (tmp) {
/*             printf("------cur =%p, name = %s.\n", tmp, tmp->__name);
 */
            if (vtable == tmp) {
                return ptr;
            }
            /*tmp = *(RTTI**)tmp;*/ /* As same as */ tmp = tmp->__base;
        }
    }
    return NULL;
}

