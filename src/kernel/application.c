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
#include "bitmap.h"
#include "image.h"
#include "paint.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Application)

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
    _c(sig)->connect(sig, cbfunc);
    _c(sig)->connect(sig, cbfunc2);
    _c(sig)->connect(sig, cbfunc3);
    _c(sig)->connectGroup(sig, 15, cbfunc3);
    _c(sig)->connectGroup(sig, 5, cbfunc2);
    _c(sig)->connectGroup(sig, 20, cbfunc4);
    _c(sig)->connectGroup(sig, 20, cbfunc);
    _c(sig)->connectGroup(sig, 20, cbfunc2);
    _c(sig)->connectGroup(sig, 20, cbfunc3);
    _c(sig)->connectGroup(sig, 1, cbfunc);
    _c(sig)->connectGroup(sig, -1, cbfunc5);
    _c(sig)->connectGroup(sig, -1, cbfunc);
    _c(sig)->emit(sig, 0xfefe);
    Delete(sig);

    if (NULL != vd) {
        _c(vd)->setVideoMode(vd, (Surface*)screen, 640, 480, 16, 0);
        if (NULL != screen->pixels) {
            MIL_Rect rc = {0, 0, 640, 480};
            MIL_Rect rcbmp = {0, 0, 300, 300};
            MIL_Rect rcdst = {50, 10, 200, 300};
            MIL_Point rcpos = {50, 5};
            MIL_Rect rclip = {200, 100, 400, 379};

            MIL_RWops* ops = MIL_RWFromFile("res/lena16.bmp", "rb");
            Surface* bmp = MIL_LoadBMP_RW(ops, MIL_AUTO_FREE);
            MIL_Bitmap* img = LoadBitmapFromFile("res/mil.bmp");
            MIL_Color color = {255, 0, 0};
            int i, j;
            char *pixels = (char*)screen->pixels;
            _vc1(screen, setClipRect, &rclip);
            Delete(img);
            for (i = 0; i < 480; ++i)
            {
                memset(pixels, 255 - i % 255, _vc0(screen, getBytesPerPixel) * 640);
                pixels += _vc0((Surface*)screen, getPitch);
            }
            _vc2(screen, fillRect, &rc, 0xffffffff);
            _c(vd)->updateRects(vd, 1, &rc);
            getchar();
            if (NULL != bmp) {
                Surface* convert = _vc2(bmp, convert, screen->format, screen->flags);
                MIL_Image* screen_img = CreateImageFromSurface(screen);
                if (NULL != screen_img) {
                    MIL_Graphics* gc = MIL_CreateMemGCFromImage(screen_img);
                    MIL_Pen* pen = (MIL_Pen*)New(SolidPen);
                    _c(pen)->setColor(pen, &color);
                    _c(gc)->selectPen(gc, pen);
//                _vc2(convert, setColorKey, MIL_SRCCOLORKEY, 0);
//                _vc2(convert, setAlpha, MIL_SRCALPHA, 5);
                    for (j = 0; j < 400; ++j) {
                        rcdst.x = j;
                        rcpos.x = j;
//                    _c(convert)->stretchBlit(convert, &rcbmp, screen, &rcdst);
                        _vc3(convert, blit, &rcbmp, screen, &rcpos);
                        _c(gc)->setPixel(gc, 10, 10);
                        _c(gc)->drawLine(gc, 1, 1, 639, 479);
                        _c(vd)->updateRects(vd, 1, &rc);
                    }
                }
                Delete(bmp);
                Delete(convert);
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

BEGIN_METHOD_MAP(Application, NonBase)
    CONSTRUCTOR_MAP(Application)
    DESTRUCTOR_MAP(Application)
END_METHOD_MAP




