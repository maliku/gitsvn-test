/*!============================================================================
 * @file signals.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月28日 23时10分14秒 
 *  Organization: http://www.ds0101.net
 */

#ifndef  SIGNALS_INC
#define  SIGNALS_INC

#include "coo.h"

CLASS(Signal)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(Signal)    
        int (*connect)(_SELF, void* slot);
        int (*disconnect)(_SELF, void* slot);
        int (*emit)(_SELF);
        Uint32 (*num_slots)(_SELF);
        MIL_bool (*empty)(_SELF);
    VIRTUAL_METHOD_DECLARE_END
    METHOD_DECLARE_PLACEHOLDER(Signal)

    Uint32 num_slots;
};

CLASS_INHERIT_BEGIN(SignalSimple, Signal)
    void (*slot)(void);
CLASS_INHERIT_END

#endif   /* ----- #ifndef SIGNALS_INC  ----- */

