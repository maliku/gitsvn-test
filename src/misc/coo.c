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

void* SafeCast(const char* typeName, void* ptr)
{
    if (NULL != ptr) {
        RTTI* tmp = *(RTTI**)ptr;
        while (tmp && tmp->__base && NULL != tmp->__name) {
            /* TODO: Change to g_xxx_Vtable compare.*/
            if (0 == MIL_strcmp(typeName, tmp->__name)) {
                return ptr;
            }
            tmp = tmp->__base;
        }
    }
    return NULL;
}

/* Can change to macro. */
const char* GetTypeName(void* ptr)
{
    RTTI* tmp = *(RTTI**)ptr;
    if (tmp && tmp->__name) {
        return tmp->__name;
    }
    return "";
}

