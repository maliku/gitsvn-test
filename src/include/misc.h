/*!============================================================================
 * @file misc.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2010-02-22 17:25:25 
 *  Organization: http://www.ds0101.net
 */

#ifndef  MISC_INC
#define  MISC_INC

#define _friend(type, pobj) ((struct _##type##_private*)(&((type*)pobj)->__))

#endif   /* ----- #ifndef MISC_INC  ----- */

