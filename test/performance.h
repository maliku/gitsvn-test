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
    VIRTUAL_METHOD_DECLARE_BEGIN(MyBase)
        void (*virtual_method)(_Self(MyBase));
    VIRTUAL_METHOD_DECLARE_END

    METHOD_DECLARE_BEGIN(MyBase)
        void (*normal_method)(_Self(MyBase));
    METHOD_DECLARE_END
    
    int m_count;
};

CLASS_INHERIT_BEGIN(MySub, MyBase)
    METHOD_DECLARE_PLACEHOLDER(MySub)
CLASS_INHERIT_END

