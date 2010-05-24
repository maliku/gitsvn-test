/*!============================================================================
 * @file virtual.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-03-03 22:59:01 
 *  Organization: http://www.ds0101.net
 */

#include <milui/coo.h>

CLASS(MyBase)
{
    METHOD_DECLARE_BEGIN(MyBase)
        void (*vtest)(void*, int);
    METHOD_DECLARE_END
    
};

CLASS_INHERIT_BEGIN(MySub, MyBase)
    METHOD_EXPAND_DECLARE_BEGIN(MySub)
        void (*vtest)(void*, int);
        void (*ptest)(void*, int);
    METHOD_EXPAND_DECLARE_END
    
CLASS_INHERIT_END

CLASS_INHERIT_BEGIN(MySub2, MySub)
    NO_METHOD_EXPAND(MySub2)
    
CLASS_INHERIT_END

