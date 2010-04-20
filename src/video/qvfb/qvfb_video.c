/*! ============================================================================
 * @file qvfb_video.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年04月20日
 *  Organization: http://www.ds0101.net
 */

#include "qvfb_video.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

VideoDevice* CreateQVFbVideoDevice(void)
{
    return (VideoDevice*)New(QVFbVideoDevice);
}

CONSTRUCTOR(QVFbVideoDevice)
{
    printf("QVFbVideoDevice %p constructed...\n", self);
    return self;
}

DESTRUCTOR(QVFbVideoDevice)
{
    printf("QVFbVideoDevice %p destructed...\n", self);
}

int METHOD_NAMED(QVFbVideoDevice, videoInit)(_Self(VideoDevice), MIL_PixelFormat *vformat)
{
    char file [50];
    int display = 0;
    key_t key;
    int shmid = 0;
    struct QVFbHardwareDependent* data = ((QVFbVideoDevice*)self)->hw_data;

    if (GetMgEtcIntValue ("qvfb", "display", &display) < 0)
        display = 0;

    sprintf (file, QT_VFB_MOUSE_PIPE, display);
    key = ftok (file, 'b');

    shmid = shmget (key, 0, 0);
    if (shmid != -1)
        data->shmrgn = (unsigned char *)shmat (shmid, 0, 0);

    if ((int)data->shmrgn == -1 || data->shmrgn == NULL) {
        GAL_SetError ("NEWGAL>QVFb: Unable to attach to virtual FrameBuffer server.\n");
        return -1;
    }

     data->hdr = (struct QVFbHeader *) data->shmrgn;

    vformat->BitsPerPixel = data->hdr->depth;
    switch (vformat->BitsPerPixel) {
#ifdef _MIL_SHADOW
        case 1:
            break;
        case 4:
            break;
#endif
        case 8:
            vformat->BytesPerPixel = 1;
            data->hdr->numcols = 256;
            break;
        case 12:
            vformat->BitsPerPixel = 16;
            vformat->BytesPerPixel = 2;
            vformat->Rmask = 0x00000F00;
            vformat->Gmask = 0x000000F0;
            vformat->Bmask = 0x0000000F;
            break;
        case 16:
            vformat->BytesPerPixel = 2;
            vformat->Rmask = 0x0000F800;
            vformat->Gmask = 0x000007E0;
            vformat->Bmask = 0x0000001F;
            break;
        case 32:
            vformat->BytesPerPixel = 4;
            vformat->Amask = 0xFF000000;
            vformat->Rmask = 0x00FF0000;
            vformat->Gmask = 0x0000FF00;
            vformat->Bmask = 0x000000FF;
            break;
        default:
            GAL_SetError ("NEWGAL>QVFb: Not supported depth: %d, "
                "please try to use Shadow NEWGAL engine with targetname qvfb.\n", vformat->BitsPerPixel);
            return -1;
    }

    return 0;
}

VIRTUAL_METHOD_REGBEGIN(QVFbVideoDevice, VideoDevice)
    DESTRUCTOR_REGISTER(QVFbVideoDevice)
    &g_VideoDeviceVtable.videoInit,
VIRTUAL_METHOD_REGEND

METHOD_REGBEGIN(QVFbVideoDevice)
    CONSTRUCTOR_REGISTER(QVFbVideoDevice)
METHOD_REGEND
