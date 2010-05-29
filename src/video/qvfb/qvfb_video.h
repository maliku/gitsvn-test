/*!============================================================================
 * @file qvfb_video.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-04-20 23:12:54 
 *  Organization: http://www.ds0101.net
 */

#ifndef  QVFb_VIDEO_INC
#define  QVFb_VIDEO_INC

#include "video_device.h"

#define QT_VFB_MOUSE_PIPE       "/tmp/.qtvfb_mouse-%d"

typedef struct
{
    int left, top, right, bottom;
} RECT;

typedef struct 
{
    int width;
    int height;
    int depth;
    int linestep;
    int dataoffset;
    RECT update;
    Uint8 dirty;
    int  numcols;
    unsigned int clut[256];
}QVFbHeader;

typedef struct 
{
    unsigned int unicode;
    unsigned int modifiers;
    MIL_Bool press;
    MIL_Bool repeat;
}QVFbKeyData;

typedef struct {
    unsigned char* shmrgn;
    QVFbHeader* hdr;
} QVFbHardwareDependent;

BEGIN_CLASS_INHERIT(QVFbVideoDevice, VideoDevice)
    NO_METHOD_EXPAND(QVFbVideoDevice)
    
    /* Private display data */
    QVFbHardwareDependent* hw_data;

END_CLASS_INHERIT

#endif   /* ----- #ifndef QVFb_VIDEO_INC  ----- */
