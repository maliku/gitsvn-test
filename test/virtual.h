/*!============================================================================
 * @file virtual.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月03日 22时59分01秒 
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#include <milui/coo.h>

CLASS(MyBase)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(MyBase)
        void (*vtest)(void*, int);
    VIRTUAL_METHOD_DECLARE_END
    FUNC_DECLARE_PLACEHOLDER(MyBase);
};

CLASS_INHERIT_BEGIN(MySub, MyBase)
    FUNC_DECLARE_PLACEHOLDER(MySub);
CLASS_INHERIT_END

