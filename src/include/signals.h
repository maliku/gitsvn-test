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
#include "linux_list.h"

#ifdef MIL_USE_SLOT_TYPE
enum SlotType {
    SLOT_FUNCTION = 0,
    SLOT_OBJECT = 0,
    SLOT_GROUP = 1,
};
#endif

STRUCT
{
    struct list_head list;
    void* slot;
#ifdef MIL_USE_SLOT_TYPE
    enum  SlotType type;
#endif
} SlotNode;

STRUCT
{
    SlotNode slot_head;
    struct  list_head sub_slots;
    Sint32  id;
} SlotsGroup;

typedef void*(*SlotHandle)(SlotNode*);
typedef void*(*SimpleSlotHandle)(SlotNode*, void*);

CLASS(Signal)
{
    BEGIN_METHOD_DECLARE(Signal)    
        int  (*connect)(_SELF, void* slot);
        int  (*connectGroup)(_SELF, Sint32 group, void* slot);
        void (*disconnect)(_SELF);
        void (*disconnectGroup)(_SELF, Sint32 group);
        void (*disconnectAllGroup)(_SELF);
        void*  (*emit)(_SELF, ...);
        Uint32 (*num_slots)(_SELF);
        MIL_Bool (*empty)(_SELF);
    END_METHOD_DECLARE

    Uint32 num_slots;
    SlotNode   slots; /* Default slots, lowest priority. */
    SlotsGroup group;
    MIL_mutex* mutex;
};

void* CallSlot(SlotNode*, void* arg);
void* DelSlot(SlotNode*);

BEGIN_CLASS_INHERIT(SignalSimple, Signal)
    NO_METHOD_EXPAND(SignalSimple)
END_CLASS_INHERIT

#endif   /* ----- #ifndef SIGNALS_INC  ----- */

