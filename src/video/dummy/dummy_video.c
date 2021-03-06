/*! ============================================================================
 * @file DummyVideo.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-03-16
 *  Organization: http://www.ds0101.net
 */

#include "dummy_video.h"

#define DUMMY_VIDEO_DRIVER_NAME "dummy"

VideoDevice* CreateDummyVideoDevice(void)
{
    VideoDevice* video = (VideoDevice*)New(DummyVideoDevice);
    if (NULL != video) {
        _c(video)->videoInit(video, NULL);
    }
    return video;
}

VideoDeviceEntry g_video_dummy = {
    DUMMY_VIDEO_DRIVER_NAME,
    CreateDummyVideoDevice
};

CONSTRUCTOR(DummyVideoDevice)
{
    static MIL_Bool s_virtual_method_check = MIL_FALSE;
    printf("DummyVideoDevice %p constructed...\n", self);
    ((VideoDevice*)self)->name = DUMMY_VIDEO_DRIVER_NAME;
}

DESTRUCTOR(DummyVideoDevice)
{
    printf("DummyVideoDevice %p destructed...\n", self);
}

BEGIN_METHOD_MAP(DummyVideoDevice, VideoDevice)
    CONSTRUCTOR_MAP(DummyVideoDevice)
    DESTRUCTOR_MAP(DummyVideoDevice)
END_METHOD_MAP



