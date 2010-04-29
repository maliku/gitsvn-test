/*!============================================================================
 * @file signals.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-28 23:10:14 
 *  Organization: http://www.ds0101.net
 */

#ifndef  SIGNALS_INC
#define  SIGNALS_INC

#include "MIL_mutex.h"
#include "linux-list.h"

enum SlotType {
    SLOT_FUNCTION = 0,
    SLOT_OBJECT = 0,
    SLOT_GROUP = 1,
};

STRUCT
{
    struct list_head list;
    void* slot;
    enum  SlotType type;
} SlotNode;

STRUCT
{
    SlotNode slot_head;
    struct  list_head sub_slots;
    Sint32  id;
} SlotsGroup;

CLASS(Signal)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(Signal)    
        int  (*connect)(_SELF, void* slot);
        int  (*connectToGroup)(_SELF, Sint32 group, void* slot);
        int  (*disconnect)(_SELF, Sint32 group);
        void*  (*emit)(_SELF, ...);
        Uint32 (*num_slots)(_SELF);
        MIL_bool (*empty)(_SELF);
    VIRTUAL_METHOD_DECLARE_END
    METHOD_DECLARE_PLACEHOLDER(Signal)

    Uint32 num_slots;
    SlotNode   slots; /* Default slots, lowest priority. */
    SlotsGroup group;
    MIL_mutex* mutex;
};

CLASS_INHERIT_BEGIN(SignalSimple, Signal)
    METHOD_DECLARE_BEGIN(SignalSimple)
    void* (*travel)(_SELF, SlotNode*, void*);
    METHOD_DECLARE_END
CLASS_INHERIT_END

#endif   /* ----- #ifndef SIGNALS_INC  ----- */

