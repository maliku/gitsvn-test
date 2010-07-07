/*! ============================================================================
 * @file coobase.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/04/2010
 *  Organization: http://www.ds0101.net
 */

#include "MIL_object.h"

CONSTRUCTOR(MObject)
{
    _private(MObject)->ref = 1;
    printf("[MILUI]MObject Constructor:%p\n", self);
}

DESTRUCTOR(MObject)
{
    if (_private(MObject)->ref > 0) {
        fprintf(stderr, "[MILUI]MObject %s is deleted force!\n", GetTypeName(self));
    }
    printf("[MILUI]MObject Destructor:%p\n", self);
}

MObject* METHOD_NAMED(MObject, retain)(_SELF)
{
    ++_private(MObject)->ref;
    return (MObject*)self;
}

void METHOD_NAMED(MObject, release)(_SELF)
{
    if (--_private(MObject)->ref <= 0) {
        Delete(self);
    }
}

int METHOD_NAMED(MObject, releaseCount)(_CSELF)
{
    return _private(MObject)->ref;
}

BEGIN_METHOD_MAP(MObject, NonBase)
    CONSTRUCTOR_MAP(MObject)
    DESTRUCTOR_MAP(MObject)
    METHOD_MAP(MObject, retain)
    METHOD_MAP(MObject, release)
    METHOD_MAP(MObject, releaseCount)
END_METHOD_MAP

