/*! ============================================================================
 * @file gdi_base.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#include "MIL_gdi.h"

enum {
    LIFE_COUNTER = 0,
    HOLD_COUNTER = 1,
    SAVE_COUNTER = 2,
    MAX_COUNTER_TYPE = 3
} RefCounterType;

CONSTRUCTOR(MIL_GdiObject)
{
    memset(_private(MIL_GdiObject)->counters, 0, 
            sizeof(_private(MIL_GdiObject)->counters));
    _private(MIL_GdiObject)->status = MIL_OK;
}

MIL_Status MIL_GdiObject_X_getLastStatus(_Self(MIL_GdiObject))
{
    return _private(MIL_GdiObject)->status;
}

void MIL_GdiObject_X_addHoldRef(_Self(MIL_GdiObject), int type)
{
    if (type > 0 && type < MAX_COUNTER_TYPE) {
        ++(_private(MIL_GdiObject)->counters[type]);
    }
}

void MIL_GdiObject_X_decHoldRef(_Self(MIL_GdiObject), int type)
{
    if (type > 0 && type < MAX_COUNTER_TYPE) {
        --(_private(MIL_GdiObject)->counters[type]);
        if (_private(MIL_GdiObject)->counters[LIFE_COUNTER] <= 0 &&
                _private(MIL_GdiObject)->counters[HOLD_COUNTER] <= 0 &&
                _private(MIL_GdiObject)->counters[SAVE_COUNTER] <= 0) {
            printf("Ref counter of %s make zero, deleted.\n", GetTypeName(self));
            Delete(self);
        }
    }
}

