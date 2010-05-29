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
    BEGIN_METHOD_DECLARE(MyBase)
        void (*virtual_method)(_Self(MyBase));
    END_METHOD_DECLARE

    int m_count;
};

BEGIN_CLASS_INHERIT(MySub, MyBase)
    NO_METHOD_EXPAND(MySub)
END_CLASS_INHERIT

