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

