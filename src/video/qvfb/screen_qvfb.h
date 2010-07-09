/*!============================================================================
 * @file screen_qvfb.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  SCREEN_QVFB_INC
#define  SCREEN_QVFB_INC

#include "screen.h"

#define QVFB_MOUSE_PIPE       "/tmp/.qtvfb_mouse-%d"

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

/* TODO: change to a class? */
typedef struct {
    unsigned char* shmrgn;
    QVFbHeader* hdr;
    MIL_Bool success;
    int brightness;
    MIL_Bool blank;
} QVFbHardwareDependent;

BEGIN_CLASS_INHERIT(ScreenQVFB, MScreen)
    NO_METHOD_EXPAND(ScreenQVFB)

    QVFbHardwareDependent* hw_data;
END_CLASS_INHERIT

#endif   /* ----- #ifndef SCREEN_QVFB_INC  ----- */

