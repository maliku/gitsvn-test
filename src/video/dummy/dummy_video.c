/*! ============================================================================
 * @file DummyVideo.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月16日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "dummy_video.h"

#define DUMMY_VIDEO_DRIVER_NAME "dummy"

VideoDevice* CreateDummyVideoDevice(void)
{
    return (VideoDevice*)New(DummyVideoDevice);
}

VideoDeviceEntry g_video_dummy = {
    DUMMY_VIDEO_DRIVER_NAME,
    CreateDummyVideoDevice
};

CONSTRUCTOR(DummyVideoDevice)
{
    printf("DummyVideoDevice %p constructed...\n", self);
    ((VideoDevice*)self)->name = DUMMY_VIDEO_DRIVER_NAME;
    return self;
}

DESTRUCTOR(DummyVideoDevice)
{
    printf("DummyVideoDevice %p destructed...\n", self);
}
VIRTUAL_METHOD_REGBEGIN(DummyVideoDevice, VideoDevice)
    DESTRUCTOR_REGISTER(DummyVideoDevice)
    &g_VideoDeviceVtable.videoInit,
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(DummyVideoDevice)
    CONSTRUCTOR_REGISTER(DummyVideoDevice)
METHOD_REGEND
