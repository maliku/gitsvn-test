/*!============================================================================
 * @file MIL_string.h 
 * @Synopsis MString. 
 * @author DongKai
 * @version 1.0
 *  Organization: http://www.ds0101.net
 */

#ifndef  MIL_STRING_INC
#define  MIL_STRING_INC

#include "MIL_object.h"

typedef enum {
    CASE_INSENSITIVE = 0,
    CASE_SENSITIVE = 1
} MIL_CaseSensitivity;

BEGIN_CLASS_INHERIT(MString, MObject)
    BEGIN_METHOD_EXPAND_DECLARE(MString)
#define MIL_MString_METHOD_TABLE \
        MIL_MObject_METHOD_TABLE\
        MString* (*appendCStr)(_Self(MString), const char* str);\
        MString* (*appendChar)(_Self(MString), const char c);\
        MString* (*appendMStr)(_Self(MString), const MString* str);\
        MString* (*expandCStr)(_Self(MString), const char* a);\
        MString* (*expandMStr)(_Self(MString), const MString* a);\
        char (*at)(_Self(MString), int pos);\
        int  (*capacity)(_CSelf(MString));\
        void (*chop)(_Self(MString), int n);\
        void (*clear)(_Self(MString));\
        MIL_Bool (*compare)(_CSelf(MString), const _Rhs(MString), MIL_CaseSensitivity cs);\
        const char* (*cstr)(_CSelf(MString));\
        MIL_Bool (*containsCStr)(_CSelf(MString), const char* str, MIL_CaseSensitivity cs);\
        MIL_Bool (*containsMStr)(_CSelf(MString), const MString* str, MIL_CaseSensitivity cs);\
        int  (*countChar)(_CSelf(MString), char c);\
        int  (*count)(_CSelf(MString));\
        MIL_Bool (*endsWithCStr)(_CSelf(MString), const char* str);\
        MIL_Bool (*endsWithMStr)(_CSelf(MString), const MString* str);\
        MIL_Bool (*endsWithChar)(_CSelf(MString), char c);\
        MString* (*fill)(_Self(MString), char c, int size);\
        int  (*indexOfCStr)(_CSelf(MString), const char* str, int from, MIL_CaseSensitivity cs);\
        int  (*indexOfMStr)(_CSelf(MString), const MString* str, int from, MIL_CaseSensitivity cs);\
        int  (*indexOfChar)(_CSelf(MString), char c, int from, MIL_CaseSensitivity cs);\
        MString*  (*insertCStr)(_Self(MString), int pos, const char* str);\
        MString*  (*insertMStr)(_Self(MString), int pos, const MString* str);\
        MString*  (*insertChar)(_Self(MString), int pos, const char c);\
        MIL_Bool (*isEmpty)(_CSelf(MString));\
        MString*  (*left)(_CSelf(MString), int n);\
        MString*  (*length)(_CSelf(MString), int n);\
        const Uint16* (*utf16)(_CSelf(MString));\
        MString* (*vsprintf)(const char* cformat, va_list ap);

        METHOD_TABLE(MString)
    END_METHOD_EXPAND_DECLARE

    BEGIN_PRIVATE(MString)
    char* string;
    END_PRIVATE
END_CLASS_INHERIT

#endif   /* ----- #ifndef MIL_STRING_INC  ----- */

