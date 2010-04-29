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
    INIT_LIST_HEAD((struct list_head *)(&_m(group).sub_slots));
    INIT_LIST_HEAD((struct list_head *)(&_m(group).slot_head.list));
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
            INIT_LIST_HEAD((struct list_head*)slotnew);
            slotnew->slot = slot;
            slotnew->type = SLOT_FUNCTION;
            list_add((struct list_head *)slotnew, (struct list_head *)(&_tm(Signal, slots))); 
            return 0;
        }
    }
    return -1;
}

int Signal_X_connectToGroup(_SELF, Sint32 group, void* slot)
{
    struct list_head* i = NULL;
    MIL_bool is_break = MIL_FALSE;

    if (NULL != slot) {
        list_for_each(i, &_tm(Signal, group).slot_head.list) {
            if (group == ((SlotsGroup*)i)->id) {
                SlotNode* slot_new = (SlotNode*)MIL_malloc(sizeof(*slot_new));
                if (NULL != slot_new) {
                    INIT_LIST_HEAD((struct list_head*)slot_new);
                    slot_new->slot = slot;
                    slot_new->type = SLOT_FUNCTION;
                    list_add_tail((struct list_head*)slot_new, &((SlotsGroup*)i)->sub_slots);
                    return 0;
                }
                else {
                    return -1;
                }
            }
            else if (group < ((SlotsGroup*)i)->id) {
                is_break = MIL_TRUE;
                break;
            }
        }

        SlotsGroup* group_new = (SlotsGroup*)MIL_malloc(sizeof(*group_new));
        if (NULL != group_new) {
            INIT_LIST_HEAD((struct list_head*)&group_new->slot_head.list);
            INIT_LIST_HEAD((struct list_head*)&group_new->sub_slots);
            group_new->slot_head.slot = slot;
            group_new->slot_head.type = SLOT_GROUP;
            group_new->id = group;
            if (is_break) {
                list_add_tail((struct list_head*)group_new, i);
            }
            else {
                puts("Append the last.");
                list_add_tail((struct list_head*)group_new, 
                        &_tm(Signal, group).slot_head.list);
            }
            return 0;
        }
    }
    return -1;
}

int Signal_X_disconnect(_SELF, Sint32 group)
{
    return -1;
}

void* Signal_X_emit(_SELF, ...)
{
    /* Do nothing. */
    return ((void*)-1);
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


void* SignalSimple_X_emit(_SELF, ...)
{
    va_list arg_ptr;
    struct list_head* i = NULL;
    void* arg = NULL;
    va_start(arg_ptr, self);
    arg = va_arg(arg_ptr, void*);
    va_end(arg_ptr);

    _MC((SignalSimple*)self)->travel(self, &_tm(Signal, group).slot_head, arg);
    /* The default slots list has lowest priority. */
    list_for_each(i, &_tm(Signal, slots).list) {
        ((void (*)(void*))(((SlotNode*)i)->slot))(arg);
    }
}

void* SignalSimple_X_travel(_SELF, SlotNode* head, void* arg)
{
    struct list_head* i = NULL;
    if (NULL != head) {
        list_for_each(i, (struct list_head*)head) {
            SlotNode* node = (SlotNode*)i;
            if (SLOT_GROUP == node->type) {
                printf("find the group %d.\n", ((SlotsGroup*)node)->id);
                ((void (*)(void*))(((SlotNode*)i)->slot))(arg);
                SignalSimple_X_travel(self, 
                        (SlotNode*)(&((SlotsGroup*)node)->sub_slots), arg);
            }
            else {
                ((void (*)(void*))(((SlotNode*)i)->slot))(arg);
            }
        }
    }
    return NULL;
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

DESTRUCTOR(SignalSimple)
{

}

VIRTUAL_METHOD_REGBEGIN(SignalSimple, Signal)
    DESTRUCTOR_REGISTER(SignalSimple)
    METHOD_PLACEHOLDER(connect)
    METHOD_PLACEHOLDER(connectToGroup)
    METHOD_PLACEHOLDER(disconnect)
    METHOD_REGISTER(SignalSimple, emit)
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(SignalSimple)
    CONSTRUCTOR_REGISTER(SignalSimple)
    METHOD_REGISTER(SignalSimple, travel)
METHOD_REGEND

