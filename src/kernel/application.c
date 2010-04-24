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
#include "application.h"

MAKE_PURE_VIRTUAL_CLASS(MIL_Application)
METHOD_REGISTER_PLACEHOLDER(MIL_Application)

CONSTRUCTOR(Application)
{
    Surface *s = New(Surface);
    VideoDevice* vd = CreateVideoDevice("qvfb");

    if (NULL != vd) {
        _VC(vd)->setVideoMode(vd, (MIL_Surface*)s, 240, 320, 32, 0);
        if (NULL != s->pixels) {
            MIL_Rect rc = {0, 0, 240, 320};
            int i, j;
            for (i = 0; i < 320; ++i)
                for (j = 0; j < 240; ++j)
                    *(Uint32*)(((char*)s->pixels) + i * _VC(s)->getPitch((MIL_Surface*)s) + j) = i;
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

