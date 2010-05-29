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
        void (*vtest)(void*, int);
    END_METHOD_DECLARE
    
};

BEGIN_CLASS_INHERIT(MySub, MyBase)
    BEGIN_METHOD_EXPAND_DECLARE(MySub)
        void (*vtest)(void*, int);
        void (*ptest)(void*, int);
    END_METHOD_EXPAND_DECLARE
    
END_CLASS_INHERIT

BEGIN_CLASS_INHERIT(MySub2, MySub)
    NO_METHOD_EXPAND(MySub2)
    
END_CLASS_INHERIT

