/*! ============================================================================
 * @file bitmap_sub.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#include <milui/MIL_gdi.h>

BEGIN_CLASS_INHERIT(MyBitmap, MIL_Bitmap)
    NO_METHOD_EXPAND(MyBitmap)
    
    int member;
END_CLASS_INHERIT

MIL_Image*  MyBitmap_X_clone(_Self(MIL_Image))
{
//    printf("internal surface addr: %p.\n", _private(Image)->surface);
    return NULL;
}

BEGIN_METHOD_MAP(MyBitmap, MIL_Bitmap)
    METHOD_MAP(MyBitmap, clone)
END_METHOD_MAP

int main()
{
    MyBitmap* bitmap = (MyBitmap*)LoadBitmapFromFile("res/mil.bmp");
    printf("Size of MyBitmap = %d.\n", sizeof(MyBitmap));
    printf("bmp width = %d, height = %d.\n", _c((MIL_Image*)bitmap)->getWidth((MIL_Image*)bitmap), _c((MIL_Image*)bitmap)->getHeight((MIL_Image*)bitmap));
    return 0;
}
