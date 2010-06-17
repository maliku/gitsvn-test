#ifndef  _SUBTEST_INC
#define  _SUBTEST_INC

#include <milui/MIL_object.h>

BEGIN_CLASS_INHERIT(CooSub, MObject)
    BEGIN_METHOD_EXPAND_DECLARE(CooSub)
#define MIL_CooSub_METHOD_TABLE \
        MIL_MObject_METHOD_TABLE\
        void (*log)(_Self(CooSub));
        METHOD_TABLE(CooSub)
    END_METHOD_DECLARE
END_CLASS_INHERIT

DESTRUCTOR(CooSub)
{
    printf("CooSubDestructor:%p\n", self);
}

CONSTRUCTOR(CooSub)
{
    printf("CooSubConstructor:%p\n", self);
}

void METHOD_NAMED(CooSub, log)(_Self(CooSub))
{
    puts("I'm CooSub::log.");
}

BEGIN_METHOD_MAP(CooSub, MObject)
    CONSTRUCTOR_MAP(CooSub)
    DESTRUCTOR_MAP(CooSub)
    METHOD_MAP(CooSub, log)
END_METHOD_MAP

/*============================================================================*/
BEGIN_CLASS_INHERIT(CooLevel1, CooSub)
    NO_METHOD_EXPAND(CooLevel1)
END_CLASS_INHERIT

DESTRUCTOR(CooLevel1)
{
    printf("CooLevel1Destructor:%p\n", self);
}

CONSTRUCTOR(CooLevel1)
{
    printf("CooLevel1Constructor:%p\n", self);
}

#define SUPER(type) (NULL != DynamicCast(type, self) ? ((type##Vtable*)(self->__super.__vptr->__rtti.__base)) : _c(self))
void METHOD_NAMED(CooLevel1, log)(_Self(CooSub))
{
    puts("I'm CooLevel1::log.");
    SUPER(CooSub)->log((CooSub*)self);
}

BEGIN_METHOD_MAP(CooLevel1, CooSub)
    CONSTRUCTOR_MAP(CooLevel1)
    DESTRUCTOR_MAP(CooLevel1)
    METHOD_MAP(CooLevel1, log)
END_METHOD_MAP

#endif   /* ----- #ifndef _SUBTEST_INC  ----- */

