/*! ============================================================================
 * @file application.c 
 * @Synopsis for MIL_Application 
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "video/video_device.h"
#include "MIL_application.h"

CONSTRUCTOR(MIL_Application)
{
    printf("MIL_Application %p constructed...\n", self);
    self->video_device = CreateVideoDevice("dummy");
    return self;
}

DESTRUCTOR(MIL_Application)
{
    printf("MIL_Application %p destructed...\n", self);
}

VIRTUAL_METHOD_REGBEGIN(MIL_Application, NonBase)
    DESTRUCTOR_REGISTER(MIL_Application)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(MIL_Application)
    CONSTRUCTOR_REGISTER(MIL_Application)
METHOD_REGEND

