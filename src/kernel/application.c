/*! ============================================================================
 * @file application.c 
 * @Synopsis for Application 
 * @author DongKai
 * @version 1.0
 * @date 2010年04月09日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include "surface.h"
#include "video_device.h"
#include "signals.h"
#include "application.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Application)
METHOD_REGISTER_PLACEHOLDER(MIL_Application)

void cbfunc(void* arg)
{
    printf("I got the signal = %p\n", arg);
}

CONSTRUCTOR(Application)
{
    Surface *s = New(Surface);
    VideoDevice* vd = CreateVideoDevice("qvfb");
    SignalSimple* sig = New(SignalSimple);
    _VC(sig)->connect(sig, cbfunc);
    _VC(sig)->emit(sig, 0xfefe);

    if (NULL != vd) {
        _VC(vd)->setVideoMode(vd, (MIL_Surface*)s, 640, 480, 32, 0);
        if (NULL != s->pixels) {
            MIL_Rect rc = {0, 0, 640, 480};
            int i, j;
            char *pixels = (char*)s->pixels;
//            printf("pitch = %d.\n", _vc0((MIL_Surface*)s, getBytesPerPixel));
            for (i = 0; i < 480; ++i)
            {
                memset(pixels, i % 255, 4 * 640);
                pixels += _vc0((MIL_Surface*)s, getPitch);
            }
            _VC(vd)->updateRects(vd, 1, &rc);
            puts("draw");
            getchar();
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

