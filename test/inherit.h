#ifndef  _SUBTEST_INC
#define  _SUBTEST_INC

#include <milui/coobase.h>

CLASS_INHERIT_BEGIN(CooSub, CooBase)
        MEMBER_FUNCTION_DECLARE_BEGIN(CooSub)
        MEMBER_FUNCTION_DECLARE_END
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
    VIRTUAL_FUNCTION_REGBEGIN(CooSub, CooBase)
DESTRUCTOR_REGISTER(CooSub)
    VIRTUAL_FUNCTION_REGEND

    MEMBER_FUNCTION_REGBEGIN(CooSub)
CONSTRUCTOR_REGISTER(CooSub)
    MEMBER_FUNCTION_REGEND

/*============================================================================*/
CLASS_INHERIT_BEGIN(CooLevel1, CooSub)
        MEMBER_FUNCTION_DECLARE_BEGIN(CooLevel1)
        MEMBER_FUNCTION_DECLARE_END
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

MEMBER_FUNCTION_REGBEGIN(CooLevel1)
    CONSTRUCTOR_REGISTER(CooLevel1)
MEMBER_FUNCTION_REGEND

VIRTUAL_FUNCTION_REGBEGIN(CooLevel1, CooSub)
    DESTRUCTOR_REGISTER(CooLevel1)
    VIRTUAL_FUNCTION_REGEND

#endif   /* ----- #ifndef _SUBTEST_INC  ----- */

