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
    _private(MIL_GdiObject)->counters[MIL_LIFE_REF] = 1;
}

DESTRUCTOR(MIL_GdiObject)
{
    if (_private(MIL_GdiObject)->counters[MIL_LIFE_REF] > 0 ||
            _private(MIL_GdiObject)->counters[MIL_HOLD_REF] > 0 ||
            _private(MIL_GdiObject)->counters[MIL_SAVE_REF] > 0) {
        fprintf(stderr, "Ref counter of %s is not change to 0!\n", GetTypeName(self));
    }
}

MIL_GdiObject*
METHOD_NAMED(MIL_GdiObject, ref)(_SELF, int type)
{
    if (type > 0 && type < MIL_MAX_REF_TYPE) {
        ++(_private(MIL_GdiObject)->counters[type]);
        return self;
    }
}

void 
METHOD_NAMED(MIL_GdiObject, unRef)(_SELF, int type)
{
    if (type > 0 && type < MIL_MAX_REF_TYPE) {
        --(_private(MIL_GdiObject)->counters[type]);
        if (_private(MIL_GdiObject)->counters[MIL_LIFE_REF] <= 0 &&
                _private(MIL_GdiObject)->counters[MIL_HOLD_REF] <= 0 &&
                _private(MIL_GdiObject)->counters[MIL_SAVE_REF] <= 0) {
            printf("Ref counter of %s make zero, deleted.\n", GetTypeName(self));
            Delete(self);
        }
    }
}

int METHOD_NAMED(MIL_GdiObject, getRef)(_SELF, int type)
{
    if (type > 0 && type < MIL_MAX_REF_TYPE) {
        return (_private(MIL_GdiObject)->counters[type]);
    }
    return -1;
}

BEGIN_METHOD_MAP(MIL_GdiObject, NonBase)
    CONSTRUCTOR_MAP(MIL_GdiObject)
    NON_DESTRUCTOR
    METHOD_MAP(MIL_GdiObject, ref)
    METHOD_MAP(MIL_GdiObject, unRef)
    METHOD_MAP(MIL_GdiObject, getRef)
END_METHOD_MAP

