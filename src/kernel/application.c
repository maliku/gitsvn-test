/*! ============================================================================
 * @file application.c 
 * @Synopsis for Application 
 * @author DongKai
 * @version 1.0
 * @date 2010-04-09
 *  Organization: http://www.ds0101.net
 */

#include "surface.h"
#include "video_device.h"
#include "signals.h"
#include "application.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Application)
METHOD_REGISTER_PLACEHOLDER(MIL_Application)

void cbfunc(void* arg)
{
    printf("A.I got the signal = %p\n", arg);
}
void cbfunc2(void* arg)
{
    printf("B.I got the signal = %p\n", arg);
}
void cbfunc3(void* arg)
{
    printf("C.I got the signal = %p\n", arg);
}

void cbfunc4(void* arg)
{
    printf("D.I got the signal = %p\n", arg);
}
void cbfunc5(void* arg)
{
    printf("E.I got the signal = %p\n", arg);
}
CONSTRUCTOR(Application)
{
    Surface* screen = New(Surface);
    VideoDevice* vd = CreateVideoDevice("qvfb");
    SignalSimple* sig = New(SignalSimple);
    _VC(sig)->connect(sig, cbfunc);
    _VC(sig)->connect(sig, cbfunc2);
    _VC(sig)->connect(sig, cbfunc3);
    _VC(sig)->connectGroup(sig, 15, cbfunc3);
    _VC(sig)->connectGroup(sig, 5, cbfunc2);
    _VC(sig)->connectGroup(sig, 20, cbfunc4);
    _VC(sig)->connectGroup(sig, 20, cbfunc);
    _VC(sig)->connectGroup(sig, 20, cbfunc2);
    _VC(sig)->connectGroup(sig, 20, cbfunc3);
    _VC(sig)->connectGroup(sig, 1, cbfunc);
    _VC(sig)->connectGroup(sig, -1, cbfunc5);
    _VC(sig)->connectGroup(sig, -1, cbfunc);
    _VC(sig)->emit(sig, 0xfefe);
    Delete(sig);

    if (NULL != vd) {
        _VC(vd)->setVideoMode(vd, (Surface*)screen, 640, 480, 32, 0);
        if (NULL != screen->pixels) {
            MIL_Rect rc = {0, 0, 640, 480};
            MIL_Rect rcbmp = {0, 0, 300, 300};
            MIL_Rect rcdst = {50, 50, 300, 300};
            MIL_RWops* ops = MIL_RWFromFile("lena32.bmp", "rb");
            Surface* bmp = MIL_LoadBMP_RW(ops, 1);
            int i, j;
            char *pixels = (char*)screen->pixels;
            for (i = 0; i < 480; ++i)
            {
                memset(pixels, i % 255, _vc0(screen, getBytesPerPixel) * 640);
                pixels += _vc0((Surface*)screen, getPitch);
            }
            _VC(vd)->updateRects(vd, 1, &rc);
            getchar();
            if (NULL != bmp) {
                Surface* convert = _vc2(bmp, convert, screen->format, screen->flags);
                for (j = 0; j < 400; ++j) {
                    rcdst.x = j;
                    _vc3(convert, blitSurface, &rcbmp, screen, &rcdst);
                    _VC(vd)->updateRects(vd, 1, &rc);
                }
            }
        }
    }

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

