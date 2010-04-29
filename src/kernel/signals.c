/*!============================================================================
 * @file signals.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-28 23:06:53 
 *  Organization: http://www.ds0101.net
 */

#include "signals.h"

CONSTRUCTOR(Signal)
{
    puts("Signal construction.");
    INIT_LIST_HEAD((struct list_head *)(&_m(slots)));
    INIT_LIST_HEAD((struct list_head *)(&_m(group)));
    _m(num_slots) = 0;
    _m(mutex) = MIL_CreateMutex();

    return self;
}

DESTRUCTOR(Signal)
{
    MIL_DestroyMutex(_tm(Signal, mutex));
}

int Signal_X_connect(_SELF, void* slot)
{
    if (NULL != slot) {
        SlotNode* slotnew = (SlotNode*)MIL_malloc(sizeof(*slotnew));
        if (NULL != slotnew) {
            slotnew->slot = slot;
            slotnew->type = SLOT_FUNCTION;
            list_add((struct list_head *)slotnew, (struct list_head *)(&_tm(Signal, slots))); 
            return 0;
        }
    }
    return -1;
}

void Signal_X_connectToGroup(_SELF, Sint32 group, void* slot)
{

}

int Signal_X_disconnect(_SELF, Sint32 group)
{
    return -1;
}

int Signal_X_emit(_SELF, ...)
{
    /* Do nothing. */
    return -1;
}

Uint32 Signal_X_num_slots(_SELF)
{
    return 0;
}

MIL_bool Signal_X_empty(_SELF)
{
    return list_empty_careful((struct list_head*)(&_tm(Signal, slots)));
}

VIRTUAL_METHOD_REGBEGIN(Signal, NonBase)
    DESTRUCTOR_REGISTER(Signal)
    METHOD_REGISTER(Signal, connect)
    METHOD_REGISTER(Signal, connectToGroup)
    METHOD_REGISTER(Signal, disconnect)
    METHOD_REGISTER(Signal, emit)
    METHOD_REGISTER(Signal, num_slots)
    METHOD_REGISTER(Signal, empty)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(Signal)
    CONSTRUCTOR_REGISTER(Signal)
METHOD_REGEND


int SignalSimple_X_emit(_SELF, ...)
{
    va_list arg_ptr;
    struct list_head* i = NULL;
    void* arg = NULL;
    va_start(arg_ptr, self);
    arg = va_arg(arg_ptr, void*);
    va_end(arg_ptr);

    list_for_each(i, &_tm(Signal, slots).list) {
        ((void (*)(void*))(((SlotNode*)i)->slot))(arg);
    }
}

CONSTRUCTOR(SignalSimple)
{
    static MIL_bool is_virtual_method_checked = MIL_FALSE;
 
    if (!is_virtual_method_checked) {
        VirtualMethodVerify(self, connect);
        VirtualMethodVerify(self, disconnect);
        VirtualMethodVerify(self, connectToGroup);
        VirtualMethodVerify(self, emit);
        VirtualMethodVerify(self, num_slots);
        VirtualMethodVerify(self, empty);
        is_virtual_method_checked = MIL_TRUE;
    }
    puts("SignalSimple construction.");
    return self;
}

VIRTUAL_METHOD_REGBEGIN(SignalSimple, Signal)
    NON_DESTRUCTOR
    METHOD_PLACEHOLDER(connect)
    METHOD_PLACEHOLDER(connectToGroup)
    METHOD_PLACEHOLDER(disconnect)
    METHOD_REGISTER(SignalSimple, emit)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(SignalSimple)
    CONSTRUCTOR_REGISTER(SignalSimple)
METHOD_REGEND

