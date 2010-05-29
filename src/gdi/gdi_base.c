/*! ============================================================================
 * @file gdi_base.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "gdi.h"

CONSTRUCTOR(MIL_GdiObject)
{
    memset(_private(MIL_GdiObject)->counters, 0, 
            sizeof(_private(MIL_GdiObject)->counters));
    _private(MIL_GdiObject)->status = MIL_OK;
}

MIL_Status METHOD_NAMED(MIL_GdiObject, getLastStatus)
    (_Self(MIL_GdiObject))
{
    return _private(MIL_GdiObject)->status;
}

MIL_GdiObject*
METHOD_NAMED(MIL_GdiObject, ref)(_SELF, int type)
{
    if (type > 0 && type < MAX_REF_TYPE) {
        ++(_private(MIL_GdiObject)->counters[type]);
    }
}

void 
METHOD_NAMED(MIL_GdiObject, unRef)(_SELF, int type)
{
    if (type > 0 && type < MAX_REF_TYPE) {
        --(_private(MIL_GdiObject)->counters[type]);
        if (_private(MIL_GdiObject)->counters[LIFE_REF] <= 0 &&
                _private(MIL_GdiObject)->counters[HOLD_REF] <= 0 &&
                _private(MIL_GdiObject)->counters[SAVE_REF] <= 0) {
            printf("Ref counter of %s make zero, deleted.\n", GetTypeName(self));
            Delete(self);
        }
    }
}

int METHOD_NAMED(MIL_GdiObject, getRef)(_SELF, int type)
{
    if (type > 0 && type < MAX_REF_TYPE) {
        return (_private(MIL_GdiObject)->counters[type]);
    }
    return -1;
}

BEGIN_METHOD_MAP(MIL_GdiObject, NonBase)
    CONSTRUCTOR_MAP(MIL_GdiObject)
    NON_DESTRUCTOR
    METHOD_MAP(MIL_GdiObject, getLastStatus)
    METHOD_MAP(MIL_GdiObject, ref)
    METHOD_MAP(MIL_GdiObject, unRef)
    METHOD_MAP(MIL_GdiObject, getRef)
END_METHOD_MAP

