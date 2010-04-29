/*!============================================================================
 * @file virtual.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010年03月03日 22时59分01秒 
 *  Organization: http://www.ds0101.net
 */

#include <milui/coo.h>

CLASS(MyBase)
{
    VIRTUAL_METHOD_DECLARE_BEGIN(MyBase)
        void (*vtest)(void*, int);
    VIRTUAL_METHOD_DECLARE_END
    METHOD_DECLARE_PLACEHOLDER(MyBase);
};

CLASS_INHERIT_BEGIN(MySub, MyBase)
    METHOD_DECLARE_PLACEHOLDER(MySub);
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MySub2, MySub)
    METHOD_DECLARE_PLACEHOLDER(MySub2);
CLASS_INHERIT_END

