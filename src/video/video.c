/*! ============================================================================
 * @file video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月16日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "video_device.h"

VideoDevice* CreateVideoDevice(const char* name)
{
    puts("CreateVideoDevice");
    return CreateDummyVideoDevice();
}

int METHOD_NAMED(VideoDevice, videoInit)(_Self(VideoDevice), MIL_PixelFormat *vformat)
{
    printf("vformat=%p\n", vformat);
}

VIRTUAL_METHOD_REGBEGIN(VideoDevice, NonBase)
    METHOD_REGISTER(VideoDevice, videoInit)
VIRTUAL_METHOD_REGEND

METHOD_REGISTER_PLACEHOLDER(VideoDevice)
