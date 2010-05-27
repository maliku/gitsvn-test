/*! ============================================================================
 * @file gfx_context.c 
 * @Synopsis Implementation for graphics context. 
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include "gc.h"

METHOD_MAP_PLACEHOLDER(MIL_GraphicsContext)

CONSTRUCTOR(MemoryGC)
{
    self->surface = NULL;
    self->pen = NULL;
    self->brush = NULL;
    return self;
}

DESTRUCTOR(MemoryGC)
{

}

MIL_Status clear(_Self(MIL_GraphicsContext), MIL_Color*)
{

}

MIL_Status drawImageByPos(_Self(MIL_GraphicsContext), MIL_Image*, int, int)
{

}

MIL_Status drawImageByRect(_Self(MIL_GraphicsContext), MIL_Image*, MIL_Rect*)
{

}

MIL_Status drawString(_Self(MIL_GraphicsContext), const char*, MIL_Rect*)
{

}

MIL_Status includeClip(_Self(MIL_GraphicsContext), MIL_Rect*)
{
    
}

MIL_Status excludeClip(_Self(MIL_GraphicsContext), MIL_Rect*)
{

}

MIL_Bool   isClipEmpty(_Self(MIL_GraphicsContext))
{

}

void       store(_Self(MIL_GraphicsContext))
{

}

void       restore(_Self(MIL_GraphicsContext));
{

}

