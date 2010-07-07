/*!============================================================================
 * @file coobase.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/02/2010 03:33:57 PM 
 *  Organization: http://www.ds0101.net
 */

#ifndef  COOBASE_INC
#define  COOBASE_INC

#include "coo.h"

CLASS(MObject)
{
    BEGIN_METHOD_DECLARE(MObject)
#define MIL_MObject_METHOD_TABLE \
        MObject* (*retain)(_SELF);\
        void (*release)(_SELF);\
        int  (*releaseCount)(_CSELF);
        MIL_MObject_METHOD_TABLE
    END_METHOD_DECLARE

    BEGIN_PRIVATE(MObject)
        int ref;
    END_PRIVATE
};

#define MIL_Ref(obj) (_c(DynamicCast(MObject, obj))->retain((void*)(obj)))
#define MIL_UnRef(obj) (_c(DynamicCast(MObject, obj))->release((void*)(obj)))
#endif   /* ----- #ifndef COOBASE_INC  ----- */

