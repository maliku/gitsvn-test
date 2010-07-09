/*!============================================================================
 * @file winfb.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 *  Company: www.ds0101.net
 */

#ifndef  WINFB_INC
#define  WINFB_INC

int win_FbAvailable (void);
void* win_FbInit (int w, int h, int depth);
void win_FbClose (void);
void win_FbLock (void);
void win_FbUnlock (void);

#endif   /* ----- #ifndef WINFB_INC  ----- */

