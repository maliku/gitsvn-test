#ifndef  _SUBTEST_INC
#define  _SUBTEST_INC

#include <milui/coobase.h>

CLASS_INHERIT_BEGIN(CooSub, CooBase)
    NO_VIRTUAL_METHOD_EXPAND(CooSub)

        METHOD_DECLARE_BEGIN(CooSub)
        METHOD_DECLARE_END
CLASS_INHERIT_END

DESTRUCTOR(CooSub)
{
    printf("CooSubDestructor:%p\n", self);
}

CONSTRUCTOR(CooSub)
{
    printf("CooSubConstructor:%p\n", self);
    return self;
}
    VIRTUAL_METHOD_MAP_BEGIN(CooSub, CooBase)
DESTRUCTOR_MAP(CooSub)
    VIRTUAL_METHOD_MAP_END

    METHOD_MAP_BEGIN(CooSub)
CONSTRUCTOR_MAP(CooSub)
    METHOD_MAP_END

/*============================================================================*/
CLASS_INHERIT_BEGIN(CooLevel1, CooSub)
    NO_VIRTUAL_METHOD_EXPAND(CooLevel1)
        METHOD_DECLARE_BEGIN(CooLevel1)
        METHOD_DECLARE_END
CLASS_INHERIT_END

DESTRUCTOR(CooLevel1)
{
    printf("CooLevel1Destructor:%p\n", self);
}

CONSTRUCTOR(CooLevel1)
{
    printf("CooLevel1Constructor:%p\n", self);
    return self;
}

METHOD_MAP_BEGIN(CooLevel1)
    CONSTRUCTOR_MAP(CooLevel1)
METHOD_MAP_END

VIRTUAL_METHOD_MAP_BEGIN(CooLevel1, CooSub)
    DESTRUCTOR_MAP(CooLevel1)
    VIRTUAL_METHOD_MAP_END

#endif   /* ----- #ifndef _SUBTEST_INC  ----- */

