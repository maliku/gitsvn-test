/*! ============================================================================
 * @file application.c 
 * @Synopsis for Application 
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "video_device.h"
#include "application.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Application)
METHOD_REGISTER_PLACEHOLDER(MIL_Application)

CONSTRUCTOR(Application)
{
    CreateQVFbVideoDevice();
    printf("Application %p constructed...\n", self);
    return self;
}

DESTRUCTOR(Application)
{
    printf("Application %p destructed...\n", self);
}
    
MIL_Application* CreateApplication(char **args, int num)
{
    return New(Application);
}

VIRTUAL_METHOD_REGBEGIN(Application, NonBase)
    DESTRUCTOR_REGISTER(Application)
    VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(Application)
    CONSTRUCTOR_REGISTER(Application)
METHOD_REGEND

