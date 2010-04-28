/*!============================================================================
 * @file signals.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月28日 23时06分53秒 
 *  Organization: http://www.ds0101.net
 */

#include "signals.h"

int Signal_X_connect(_SELF, void* slot)
{
    return -1;
}

int Signal_X_disconnect(_SELF, void* slot)
{
    return -1;
}

int Signal_X_emit(_SELF)
{
    return -1;
}

Uint32 Signal_X_num_slots(_SELF)
{
    return 0;
}

MIL_bool Signal_X_empty(_SELF)
{
    return MIL_FALSE;
}

VIRTUAL_METHOD_REGBEGIN(Signal, NonBase)
    METHOD_REGISTER(Signal, connect)
    METHOD_REGISTER(Signal, disconnect)
    METHOD_REGISTER(Signal, emit)
    METHOD_REGISTER(Signal, num_slots)
    METHOD_REGISTER(Signal, emit)
VIRTUAL_METHOD_REGEND

METHOD_REGISTER_PLACEHOLDER(Signal)

