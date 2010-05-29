#ifndef  _SUBTEST_INC
#define  _SUBTEST_INC

#include <milui/coobase.h>

BEGIN_CLASS_INHERIT(CooSub, CooBase)
    NO_METHOD_EXPAND(CooSub)
END_CLASS_INHERIT

DESTRUCTOR(CooSub)
{
    printf("CooSubDestructor:%p\n", self);
}

CONSTRUCTOR(CooSub)
{
    printf("CooSubConstructor:%p\n", self);
    return self;
}

BEGIN_METHOD_MAP(CooSub, CooBase)
    CONSTRUCTOR_MAP(CooSub)
    DESTRUCTOR_MAP(CooSub)
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
    return self;
}

BEGIN_METHOD_MAP(CooLevel1, CooSub)
    CONSTRUCTOR_MAP(CooLevel1)
    DESTRUCTOR_MAP(CooLevel1)
END_METHOD_MAP

#endif   /* ----- #ifndef _SUBTEST_INC  ----- */

