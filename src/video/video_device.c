/*! ============================================================================
 * @file video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月16日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "video_device.h"

static VideoDeviceEntry* g_video_device_entries[] = {
#if MIL_VIDEO_DRIVER_DUMMY
	&g_video_dummy,
#endif
#if MIL_VIDEO_DRIVER_QVFB
	&g_video_qvfb,
#endif
    NULL
};
VideoDevice* g_current_video;

VideoDevice* CreateVideoDevice(const char* driver_name)
{
	VideoDevice* video = NULL;
	int i = 0;
	MIL_PixelFormat vformat;

	/* Check to make sure we don't overwrite 'g_current_video' */
	if ( NULL != g_current_video ) {
        _VC(g_current_video)->videoQuit(g_current_video);
	}

	/* Select the proper video driver */
	if ( NULL != driver_name ) {
		for ( i = 0; NULL != g_video_device_entries[i]; ++i ) {
			if ( MIL_strcasecmp(g_video_device_entries[i]->name, driver_name) == 0) {
				if ( NULL != g_video_device_entries[i]->create ) {
					video = g_video_device_entries[i]->create();
					break;
				}
			}
		}
	} else {
		for ( i=0; NULL != g_video_device_entries[i]; ++i ) {
			if ( NULL != g_video_device_entries[i]->create ) {
				video = g_video_device_entries[i]->create();
				if ( NULL != video ) { /* Find the creatable device in default */
					break;
				}
			}
		}
	}
	if ( video == NULL ) {
		MIL_SetError("No available video device");
		return(NULL);
	}
	return (g_current_video = video);
}

VIRTUAL_METHOD_REGISTER_PLACEHOLDER(VideoDevice, NonBase)
METHOD_REGISTER_PLACEHOLDER(VideoDevice)

