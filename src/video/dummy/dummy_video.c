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
        _VC(video)->videoInit(video, NULL);
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
    if (!s_virtual_method_check) {
        VirtualMethodVerify(self, videoInit);
        VirtualMethodVerify(self, listModes);
        VirtualMethodVerify(self, setVideoMode);
        VirtualMethodVerify(self, toggleFullScreen);
        VirtualMethodVerify(self, updateRects);
        VirtualMethodVerify(self, updateMouse);
        VirtualMethodVerify(self, createYUVOverlay);
        VirtualMethodVerify(self, videoQuit);
        s_virtual_method_check = MIL_TRUE;
    }
    printf("DummyVideoDevice %p constructed...\n", self);
    ((VideoDevice*)self)->name = DUMMY_VIDEO_DRIVER_NAME;
    return self;
}

DESTRUCTOR(DummyVideoDevice)
{
    printf("DummyVideoDevice %p destructed...\n", self);
}

VIRTUAL_METHOD_MAP_BEGIN(DummyVideoDevice, VideoDevice)
    CONSTRUCTOR_MAP(DummyVideoDevice)
    DESTRUCTOR_MAP(DummyVideoDevice)
VIRTUAL_METHOD_MAP_END

METHOD_MAP_BEGIN(DummyVideoDevice)
METHOD_MAP_END
