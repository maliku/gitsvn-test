#include "coo.h"

CommonVtable g_NonBaseVtable;

void Delete(void *ptr)
{
	if (NULL != ptr)
	{
		assert (NULL != ((CommonVtable*)*(CommonVtable**)ptr));
		((CommonVtable*)*(CommonVtable**)ptr)->destructor(ptr);
		((CommonVtable*)*(CommonVtable**)ptr)->Predestructor(ptr);
		free(ptr);
	}
}