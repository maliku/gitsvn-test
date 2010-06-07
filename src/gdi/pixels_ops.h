/*!============================================================================
 * @file pixels_ops.h 
 * @Synopsis Pixels operations. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  PIXELS_OPS_INC
#define  PIXELS_OPS_INC

#include "MIL_gdi.h"

void SetPixelSet8(MIL_PixelsContext *ctxt);
void SetHlineSet8(MIL_PixelsContext *ctxt, Uint32 w);
void PutHlineSet8(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void PutHlineSkipSet8(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void SetPixelSet16(MIL_PixelsContext *ctxt);
void SetHlineSet16(MIL_PixelsContext *ctxt, Uint32 w);
void PutHlineSet16(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void PutHlineSkipSet16(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void SetPixelSet24(MIL_PixelsContext *ctxt);
void SetHlineSet24(MIL_PixelsContext *ctxt, Uint32 w);
void PutHlineSet24(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void PutHlineSkipSet24(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void SetPixelSet32(MIL_PixelsContext *ctxt);
void SetHlineSet32(MIL_PixelsContext *ctxt, Uint32 w);
void PutHlineSet32(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
void PutHlineSkipSet32(MIL_PixelsContext *ctxt, Uint8* src, Uint32 w);
#endif   /* ----- #ifndef PIXELS_OPS_INC  ----- */

