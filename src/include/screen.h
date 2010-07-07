/*!============================================================================
 * @file screen.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  SCREEN_INC
#define  SCREEN_INC

#include "MIL_paint.h"
typedef void (*SolidFillFunc)(MScreen*, const MIL_Color*, const MRegion*);
typedef void (*BlitFunc)(MScreen*, const MImage*, const MIL_Point*, const MRegion*);

CLASS_DEFINE(MScreenPrivate)
{
    BEGIN_METHOD_DECLARE(MScreenPrivate)
    END_METHOD_DECLARE

    SolidFillFunc solidFill;
    BlitFunc blit;

    MIL_Point offset;
    MScreen** subScreens;
//    QPixmapDataFactory* pixmapFactory;
//    QGraphicsSystem* graphicsSystem;
//    QWSGraphicsSystem defaultGraphicsSystem; //###
    MIL_PixelFormat* pixelFormat;
#if MIL_BYTEORDER == MIL_BIG_ENDIAN
    MIL_Bool fb_is_littleEndian;
#endif
#ifdef MIL_CLIENTBLIT
    MIL_Bool supportsBlitInClients;
#endif
    int classId;
    MScreen *q_ptr;
};

#endif   /* ----- #ifndef SCREEN_INC  ----- */

