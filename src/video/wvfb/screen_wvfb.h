/*!============================================================================
 * @file screen_wvfb.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  SCREEN_WVFB_INC
#define  SCREEN_WVFB_INC

#include "screen.h"

struct WVFbHeader
{
    int width;
    int height;
    int depth;
    int linestep;
    int dataoffset;
    RECT  update;
    Uint8 dirty;
    int  numcols;
    unsigned int clut[256];
};

struct WVFbKeyData
{
    unsigned int unicode;
    unsigned int modifiers;
    MIL_Bool press;
    MIL_Bool repeat;
};

STRUCT 
{
    /* Private display data */
    unsigned char* shmrgn;
    struct WVFbHeader* hdr;
} ScreenWVFbPrivate;

BEGIN_CLASS_INHERIT(ScreenWVFB, MScreen)
    NO_METHOD_EXPAND(ScreenWVFB)

    ScreenWVFbPrivate* hw_data;
END_CLASS_INHERIT

#endif   /* ----- #ifndef SCREEN_WVFB_INC  ----- */

