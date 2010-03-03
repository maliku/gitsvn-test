#ifndef  _SUBTEST_INC
#define  _SUBTEST_INC

#include <milui/coobase.h>

CLASS_INHERIT_BEGIN(CooSub, CooBase)
        MEMBER_FUNCTION_DECLARE_BEGIN(CooSub)
        MEMBER_FUNCTION_DECLARE_END
CLASS_INHERIT_END

void CooSubDestructor(void * self)
{
    printf("CooSubDestructor:%p\n", self);
}

CooSub * CooSubConstructor(CooSub* self)
{
    printf("CooSubConstructor:%p\n", self);
    return self;
}

MEMBER_FUNCTION_REGBEGIN(CooSub)
    CooSubConstructor
MEMBER_FUNCTION_REGEND

VIRTUAL_FUNCTION_REGBEGIN(CooSub, CooBase)
    CooSubDestructor
    VIRTUAL_FUNCTION_REGEND
/*============================================================================*/
CLASS_INHERIT_BEGIN(CooLevel1, CooSub)
        MEMBER_FUNCTION_DECLARE_BEGIN(CooLevel1)
        MEMBER_FUNCTION_DECLARE_END
CLASS_INHERIT_END

void CooLevel1Destructor(void * self)
{
    printf("CooLevel1Destructor:%p\n", self);
}

CooLevel1 * CooLevel1Constructor(CooLevel1* self)
{
    printf("CooLevel1Constructor:%p\n", self);
    return self;
}

MEMBER_FUNCTION_REGBEGIN(CooLevel1)
    CooLevel1Constructor
MEMBER_FUNCTION_REGEND

VIRTUAL_FUNCTION_REGBEGIN(CooLevel1, CooSub)
    CooLevel1Destructor
    VIRTUAL_FUNCTION_REGEND

#endif   /* ----- #ifndef _SUBTEST_INC  ----- */

