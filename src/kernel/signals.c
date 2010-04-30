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
    _m(group) = MIL_malloc(sizeof(*_m(group)));
    if (NULL != _m(group)) {
        INIT_LIST_HEAD((struct list_head *)(_m(group)));
        INIT_LIST_HEAD((struct list_head *)(&_m(group)->sub_slots));
        INIT_LIST_HEAD((struct list_head *)(_m(group)));
    }
#ifdef MIL_USE_SLOT_TYPE
    ((SlotNode*)&_m(group))->type = SLOT_GROUP;
#endif
    _m(num_slots) = 0;
    _m(mutex) = MIL_CreateMutex();

    return self;
}

DESTRUCTOR(Signal)
{
    _MC((Signal*)self)->travel(self, &_tm(Signal, group)->slot_head, DelSlot);
    MIL_DestroyMutex(_tm(Signal, mutex));
}

int Signal_X_connect(_SELF, void* slot)
{
    if (NULL != slot) {
        SlotNode* slotnew = (SlotNode*)MIL_malloc(sizeof(*slotnew));
        if (NULL != slotnew) {
            INIT_LIST_HEAD((struct list_head*)slotnew);
            slotnew->slot = slot;
#ifdef MIL_USE_SLOT_TYPE
            slotnew->type = SLOT_FUNCTION;
#endif
            _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
            list_add((struct list_head *)slotnew, (struct list_head *)(&_tm(Signal, slots))); 
            _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
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
        _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, &_tm(Signal, group)->slot_head.list) {
            if (group == ((SlotsGroup*)i)->id) {
                SlotNode* slot_new = (SlotNode*)MIL_malloc(sizeof(*slot_new));
                if (NULL != slot_new) {
                    INIT_LIST_HEAD((struct list_head*)slot_new);
                    slot_new->slot = slot;
#ifdef MIL_USE_SLOT_TYPE
                    slot_new->type = SLOT_FUNCTION;
#endif
                    list_add((struct list_head*)slot_new, &((SlotsGroup*)i)->sub_slots);
                    _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
                    return 0;
                }
                else {
                    _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
                    return -1;
                }
            }
            else if (group < ((SlotsGroup*)i)->id) {
                is_break = MIL_TRUE;
                break;
            }
        }
        _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));

        SlotsGroup* group_new = (SlotsGroup*)MIL_malloc(sizeof(*group_new));
        if (NULL != group_new) {
            INIT_LIST_HEAD((struct list_head*)&group_new->slot_head.list);
            INIT_LIST_HEAD((struct list_head*)&group_new->sub_slots);
            group_new->slot_head.slot = slot;
#ifdef MIL_USE_SLOT_TYPE
            group_new->slot_head.type = SLOT_GROUP;
#endif
            group_new->id = group;
            _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
            if (is_break) {
                list_add_tail((struct list_head*)group_new, i);
            }
            else {
                list_add_tail((struct list_head*)group_new, 
                        &_tm(Signal, group)->slot_head.list);
            }
            _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
            return 0;
        }
    }
    return -1;
}

int Signal_X_disconnect(_SELF, Sint32 group)
{
#ifdef MIL_USE_SLOT_TYPE
    if (SLOT_GROUP == _tm(Signal, group)->type)
#endif
    {
        struct list_head* head = (struct list_head*)(_tm(Signal, group));
        struct list_head* i = NULL;
        struct list_head* k = NULL;
        _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, head) {
            if (group == ((SlotsGroup*)i)->id) {
                list_for_each(k, (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                    list_del(k);
                }
            }
        }
        _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }

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
    int ret = 0;
    _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
    ret = list_empty_careful((struct list_head*)(&_tm(Signal, slots)));
    _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    return ret;
}

void* Signal_X_travel(_SELF, SlotNode* head, SlotHandle node_handle)
{
    if (NULL != head && NULL != node_handle
#ifdef MIL_USE_SLOT_TYPE
            && SLOT_GROUP == head->type
#endif
        ) {
        struct list_head* i = NULL;
        struct list_head* k = NULL;
        _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, (struct list_head*)(head)) {
            printf("find the group %d.\n", ((SlotsGroup*)i)->id);
            list_for_each(k, (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                node_handle((SlotNode*)k);
            }
            node_handle((SlotNode*)i);
        }
        _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }
    else {
        assert(0);
    }
    return NULL;
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
    METHOD_REGISTER(Signal, travel)
METHOD_REGEND


void* SignalSimple_X_emit(_SELF, ...)
{
    va_list arg_ptr;
    struct list_head* i = NULL;
    void* arg = NULL;
    va_start(arg_ptr, self);
    arg = va_arg(arg_ptr, void*);
    va_end(arg_ptr);

    _MC((SignalSimple*)self)->travel(self, &_tm(Signal, group)->slot_head, CallSlot, arg);
    /* The default slots list has lowest priority. */
    _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
    list_for_each(i, &_tm(Signal, slots).list) {
        ((void (*)(void*))(((SlotNode*)i)->slot))(arg);
    }
    _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
}

void* SignalSimple_X_travel(_SELF, SlotNode* head, SimpleSlotHandle node_handle, void* arg)
{
    if (NULL != head && NULL != node_handle
#ifdef MIL_USE_SLOT_TYPE
            && SLOT_GROUP == head->type
#endif
        ) {
        struct list_head* i = NULL;
        struct list_head* k = NULL;
        _VC(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, (struct list_head*)(head)) {
            printf("find the group %d.\n", ((SlotsGroup*)i)->id);
            list_for_each(k, (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                node_handle((SlotNode*)k, arg);
            }
            node_handle((SlotNode*)i, arg);
        }
        _VC(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }
    else {
        assert(0);
    }
    return NULL;
}

void* CallSlot(SlotNode* node, void* arg)
{
    return ((void* (*)(void*))(node->slot))(arg);
}

void* DelSlot(SlotNode* node)
{
    list_del(&node->list);
    printf("Free node %p\n", node);
//    MIL_free(node);
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

