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
    INIT_LIST_HEAD((struct list_head *)(&_m(slots)));
    INIT_LIST_HEAD((struct list_head *)(&_m(group)));
    INIT_LIST_HEAD((struct list_head *)(&_m(group).sub_slots));
#ifdef MIL_USE_SLOT_TYPE
    ((SlotNode*)&_m(group)).type = SLOT_GROUP;
#endif
    _tm(Signal, group).id = ~((unsigned)-1);
    _m(num_slots) = 0;
    _m(mutex) = MIL_CreateMutex();

    return self;
}

DESTRUCTOR(Signal)
{
    _c((Signal*)self)->disconnect(self);
    _c((Signal*)self)->disconnectAllGroup(self);
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
            _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
            list_add((struct list_head *)slotnew, (struct list_head *)(&_tm(Signal, slots))); 
            _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
            return 0;
        }
    }
    return -1;
}

int Signal_X_connectGroup(_SELF, Sint32 group, void* slot)
{
    struct list_head* i = NULL;
    MIL_Bool is_break = MIL_FALSE;

    if (NULL != slot) {
        _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, &_tm(Signal, group).slot_head.list) {
            if (group == ((SlotsGroup*)i)->id) {
                SlotNode* slot_new = (SlotNode*)MIL_malloc(sizeof(*slot_new));
                if (NULL != slot_new) {
                    INIT_LIST_HEAD((struct list_head*)slot_new);
                    slot_new->slot = slot;
#ifdef MIL_USE_SLOT_TYPE
                    slot_new->type = SLOT_FUNCTION;
#endif
                    list_add((struct list_head*)slot_new, &((SlotsGroup*)i)->sub_slots);
                    _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
                    return 0;
                }
                else {
                    _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
                    return -1;
                }
            }
            else if (group < ((SlotsGroup*)i)->id) {
                is_break = MIL_TRUE;
                break;
            }
        }
        _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));

        SlotsGroup* group_new = (SlotsGroup*)MIL_malloc(sizeof(*group_new));
        if (NULL != group_new) {
            INIT_LIST_HEAD((struct list_head*)&group_new->slot_head.list);
            INIT_LIST_HEAD((struct list_head*)&group_new->sub_slots);
            group_new->slot_head.slot = slot;
#ifdef MIL_USE_SLOT_TYPE
            group_new->slot_head.type = SLOT_GROUP;
#endif
            group_new->id = group;
            _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
            if (is_break) {
                list_add_tail((struct list_head*)group_new, i);
            }
            else {
                list_add_tail((struct list_head*)group_new, 
                        &_tm(Signal, group).slot_head.list);
            }
            _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
            return 0;
        }
    }
    return -1;
}

void Signal_X_disconnect(_SELF)
{
    struct list_head* k = NULL;
    struct list_head* n = NULL;
    _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
    list_for_each_safe(k, n, (struct list_head*)(&_tm(Signal, slots))) {
        list_del(k);
        MIL_free(k);
    }
    _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
}

void Signal_X_disconnectGroup(_SELF, Sint32 group)
{
#ifdef MIL_USE_SLOT_TYPE
    if (SLOT_GROUP == _tm(Signal, group)->type)
#endif
    {
        struct list_head* head = (struct list_head*)(&_tm(Signal, group));
        struct list_head* i = NULL;
        struct list_head* k = NULL;
        struct list_head* n = NULL;
        _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, head) {
            if (group == ((SlotsGroup*)i)->id) {
                list_for_each_safe(k, n, (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                    list_del(k);
                }
                list_del(i);
                break;
            }
        }
        _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }
}

void Signal_X_disconnectAllGroup(_SELF)
{
#ifdef MIL_USE_SLOT_TYPE
    if (SLOT_GROUP == _tm(Signal, group)->type)
#endif
    {
        struct list_head* i = NULL;
        struct list_head* k = NULL;
        struct list_head* n = NULL;
        struct list_head* m = NULL;
        _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each_safe(i, m, (struct list_head*)(&_tm(Signal, group))) {
            list_for_each_safe(k, n, 
                    (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                list_del(k);
                MIL_free(k);
            }
            list_del(i);
            MIL_free(i);
        }
        _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }
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

MIL_Bool Signal_X_empty(_SELF)
{
    int ret = 0;
    _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
    ret = list_empty_careful((struct list_head*)(&_tm(Signal, slots)));
    _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
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
        _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, (struct list_head*)(head)) {
            list_for_each(k, (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                node_handle((SlotNode*)k);
            }
            node_handle((SlotNode*)i);
        }
        _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }
    else {
        assert(0);
    }
    return NULL;
}

BEGIN_METHOD_MAP(Signal, NonBase)
    CONSTRUCTOR_MAP(Signal)
    DESTRUCTOR_MAP(Signal)
    METHOD_MAP(Signal, connect)
    METHOD_MAP(Signal, connectGroup)
    METHOD_MAP(Signal, disconnect)
    METHOD_MAP(Signal, disconnectGroup)
    METHOD_MAP(Signal, disconnectAllGroup)
    METHOD_MAP(Signal, emit)
    METHOD_MAP(Signal, num_slots)
    METHOD_MAP(Signal, empty)
END_METHOD_MAP


void* SignalSimple_X_travel(_SELF, SlotNode* head, SimpleSlotHandle node_handle, void* arg)
{
    if (NULL != head && NULL != node_handle
#ifdef MIL_USE_SLOT_TYPE
            && SLOT_GROUP == head->type
#endif
        ) {
        struct list_head* i = NULL;
        struct list_head* k = NULL;
        _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
        list_for_each(i, (struct list_head*)(head)) {
            list_for_each(k, (struct list_head*)&((SlotsGroup*)i)->sub_slots) {
                node_handle((SlotNode*)k, arg);
            }
            node_handle((SlotNode*)i, arg);
        }
        _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
    }
    else {
        assert(0);
    }
    return NULL;
}

void* METHOD_NAMED(SignalSimple,emit)(_SELF, ...)
{
    va_list var_args;
    struct list_head* i = NULL;
    void* arg = NULL;
    va_start(var_args, self);
    arg = va_arg(var_args, void*);
    va_end(var_args);

    SignalSimple_X_travel(self, &_tm(Signal, group).slot_head, CallSlot, arg);
    /* The default slots list has lowest priority. */
    _c(_tm(Signal, mutex))->lock(_tm(Signal, mutex));
    list_for_each(i, &_tm(Signal, slots).list) {
        ((void (*)(void*))(((SlotNode*)i)->slot))(arg);
    }
    _c(_tm(Signal, mutex))->unlock(_tm(Signal, mutex));
}

void* CallSlot(SlotNode* node, void* arg)
{
    return ((void* (*)(void*))(node->slot))(arg);
}

CONSTRUCTOR(SignalSimple)
{
    static MIL_Bool is_virtual_method_checked = MIL_FALSE;
 
    if (!is_virtual_method_checked) {
        VirtualMethodVerify(self, connect);
        VirtualMethodVerify(self, connectGroup);
        VirtualMethodVerify(self, disconnect);
        VirtualMethodVerify(self, disconnectGroup);
        VirtualMethodVerify(self, disconnectAllGroup);
        VirtualMethodVerify(self, emit);
        VirtualMethodVerify(self, num_slots);
        VirtualMethodVerify(self, empty);
        is_virtual_method_checked = MIL_TRUE;
    }
    return self;
}

DESTRUCTOR(SignalSimple)
{

}

BEGIN_METHOD_MAP(SignalSimple, Signal)
    CONSTRUCTOR_MAP(SignalSimple)
    DESTRUCTOR_MAP(SignalSimple)
    METHOD_PLACEHOLDER(connect)
    METHOD_PLACEHOLDER(connectGroup)
    METHOD_PLACEHOLDER(disconnect)
    METHOD_PLACEHOLDER(disconnectGroup)
    METHOD_PLACEHOLDER(disconnectAllGroup)
    METHOD_MAP(SignalSimple, emit)
    METHOD_PLACEHOLDER(num_slots)
    METHOD_PLACEHOLDER(empty)
END_METHOD_MAP


    //METHOD_MAP(SignalSimple, travel)


