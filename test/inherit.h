#ifndef  _SUBTEST_INC
#define  _SUBTEST_INC

#include <milui/coobase.h>

CLASS_INHERIT_BEGIN(CooSub, CooBase)
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
    VIRTUAL_METHOD_REGBEGIN(CooSub, CooBase)
DESTRUCTOR_REGISTER(CooSub)
    VIRTUAL_METHOD_REGEND

    METHOD_REGBEGIN(CooSub)
CONSTRUCTOR_REGISTER(CooSub)
    METHOD_REGEND

/*============================================================================*/
CLASS_INHERIT_BEGIN(CooLevel1, CooSub)
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

METHOD_REGBEGIN(CooLevel1)
    CONSTRUCTOR_REGISTER(CooLevel1)
METHOD_REGEND

VIRTUAL_METHOD_REGBEGIN(CooLevel1, CooSub)
    DESTRUCTOR_REGISTER(CooLevel1)
    VIRTUAL_METHOD_REGEND

#endif   /* ----- #ifndef _SUBTEST_INC  ----- */

