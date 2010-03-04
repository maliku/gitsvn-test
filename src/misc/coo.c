#include "coo.h"

CommonVtable g_NonBaseVtable;

void Delete(void *ptr)
{
	if (NULL != ptr)
	{
		assert (NULL != ((CommonVtable*)*(CommonVtable**)ptr));
        if (NULL != ((CommonVtable*)*(CommonVtable**)ptr)->Destructor)
            ((CommonVtable*)*(CommonVtable**)ptr)->Destructor(ptr);
		assert (((CommonVtable*)*(CommonVtable**)ptr)->Predestructor);
        ((CommonVtable*)*(CommonVtable**)ptr)->Predestructor(ptr);
		free(ptr);
	}
}

