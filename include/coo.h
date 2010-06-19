/*!============================================================================
 * @file coo.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/02/2010 03:33:57 PM 
 *  Organization: http://www.ds0101.net
 */

#ifndef  COO_INC
#define  COO_INC

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "MIL_types.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define STRUCT typedef struct

typedef struct __RTTI {
    struct __RTTI * __base;
    const char * __name;
    int*  __vm_checked; /* is virtual method verified. */
    void* __reserve; /* reserved for align. */
} RTTI;

STRUCT DECLSPEC __CommonVtable {
        RTTI __rtti;
        void  (*OrderConstruct)(void*);
        void  (*Predestructor)(void*);
        void  (*Constructor)(void*);
        void  (*Destructor)(void*);
} CommonVtable;

extern DECLSPEC CommonVtable g_NonBaseVtable;

#define CLASS_FORWARD_DECLARE(type) \
struct __##type;\
typedef struct __##type type;

/* Macro for class declare. */
#define	CLASS_NEED_FORWARD_DECLARE(type)	\
    struct __##type##Vtable;\
typedef struct __##type##Vtable type##Vtable;\
extern DECLSPEC type##Vtable g_##type##Vtable;\
typedef type __BaseOf##type; \
extern void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);\
struct __##type

/* Macro for class declare. */
#define	CLASS(type)	\
CLASS_FORWARD_DECLARE(type)\
CLASS_NEED_FORWARD_DECLARE(type)

/* Declare a interface, it can't include any data member. */
#define BEGIN_INTERFACE(type) \
CLASS(type) \
{\
    BEGIN_METHOD_DECLARE(type)

#define END_INTERFACE \
    END_METHOD_DECLARE };

#define BEGIN_METHOD_EXPAND_DECLARE(type) \
struct __##type##Vtable; \
typedef struct __##type##Vtable type##Vtable;\
extern DECLSPEC type##Vtable g_##type##Vtable;\
struct __##type {\
    union { \
	    __BaseOf##type __class; \
    struct __##type##Vtable {\
        RTTI __rtti;\
        void  (*OrderConstruct)(void*);\
        void  (*Predestructor)(void*);\
        void  (*Constructor)(void*); \
        void  (*Destructor)(void*);

/* Macro for virtual member function declare end. */
#define	END_METHOD_EXPAND_DECLARE }*__vptr; } __super;

#define NO_METHOD_EXPAND(type) \
typedef __VtableTypeOf##type##Base type##Vtable;\
extern DECLSPEC type##Vtable g_##type##Vtable;\
struct __##type {\
    union { \
	    __BaseOf##type __class; \
        type##Vtable *__vptr; \
    }__super;

#define	BEGIN_CLASS_INHERIT_NEED_FORWARD_DECALRE(type, basetype) \
typedef basetype __BaseOf##type; \
typedef basetype##Vtable __VtableTypeOf##type##Base;\
extern void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);

#define	BEGIN_CLASS_INHERIT(type, basetype) \
CLASS_FORWARD_DECLARE(type)\
BEGIN_CLASS_INHERIT_NEED_FORWARD_DECALRE(type, basetype)

#define	END_CLASS_INHERIT };

#define	_Self(type)	type* self		/* 'this' pointer for COO framework */
#define	_CSelf(type)	const type* const self		/* 'this' pointer for COO framework */
#define	_Rhs(type)	type* rhs		/* 'rhs' pointer for COO framework */
#define	_SELF   _Self(void)		/* 'this' pointer for COO framework */
#define	_CSELF  _CSelf(void)		/* 'this' pointer for COO framework */
#define	_RHS	_Rhs(void)		/* 'rhs' pointer for COO framework */

/* Macro for call virtual member function of object. */
    /* TODO: may be we can change the __vptr to constant. */
#define	_c(pobj)	(((pobj)->__super).__vptr)

#define _vf(p, func)  (_c(p)->func)

/* The macro can verify validity of virtual method pointer with a object;
 * If the virtual method pointer is null, it's will be assigned to the same name method of super class. */
#define VirtualMethodVerify(p, func) /*assert(p && _vf(p, func))*/

#define METHOD_VERIFY_ONCE_BEGIN \
    { \
        static int is_verified = MIL_FALSE; \
        if (!is_verified) {

#define	_m(member)	(self->member)
#define	_tm(type, member)	(((type*)self)->member)
#define	_M(method)	(_c(self)->method)
#define	_TM(type, method)	(_c((type*)self)->method)

#if defined(__GNUC__) && __GNUC__ >= 4
#define _vc(pobj, method, ...) _vf(pobj, method)((pobj), ##__VA_ARGS__)
#endif
#define _vc0(pobj, method) _vf(pobj, method)((pobj))
#define _vc1(pobj, method, arg1) _vf(pobj, method)((pobj), arg1)
#define _vc2(pobj, method, arg1, arg2) _vf(pobj, method)((pobj), arg1, arg2)
#define _vc3(pobj, method, arg1, arg2, arg3) _vf(pobj, method)((pobj), arg1, arg2, arg3)
#define _vc4(pobj, method, arg1, arg2, arg3, arg4) _vf(pobj, method)((pobj), arg1, arg2, arg3, arg4)
#define _vc5(pobj, method, arg1, arg2, arg3, arg4, arg5) _vf(pobj, method)((pobj), arg1, arg2, arg3, arg4, arg5)
#define _vc6(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6) _vf(pobj, method)((pobj), arg1, arg2, arg3, arg4, arg5, arg6)
#define _vc7(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7) _vf(pobj, method)((pobj), arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define _vc8(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) _vf(pobj, method)((pobj), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define _vc9(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) _vf(pobj, method)((pobj), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define BEGIN_PRIVATE(type) \
    char __ [sizeof(struct _##type##_private {

#define END_PRIVATE })];

#define _ptm(type, name) (*(struct _##type##_private*)(&((type*)self)->__)).name
#define _private(type) ((struct _##type##_private*)(&((type*)self)->__))
#define _public(type)  ((type*)self)

/* Macro for virtual member function declare begin. */
#define BEGIN_METHOD_DECLARE(type) \
    union { \
        void * __class; \
    struct __##type##Vtable {\
        RTTI __rtti;\
        void  (*OrderConstruct)(void*);\
        void  (*Predestructor)(void*);\
        void  (*Constructor)(void*); \
        void  (*Destructor)(void*);

/* Macro for virtual member function declare end. */
#define	END_METHOD_DECLARE }*__vptr;\
    } __super;

#define METHOD_DECLARE_PLACEHOLDER(type) \
BEGIN_METHOD_DECLARE(type) \
END_METHOD_DECLARE 

#if defined(__GNUC__) && __GNUC__ >= 4

#define VTABLE_BUILD_STATIC 1
#define BEGIN_METHOD_MAP(type, basetype) \
static __INLINE__ void type##VtableBuilder(type##Vtable* vtbl){}\
PRECONSTRUCTORS(type, basetype)\
PREDESTRUCTORS(type, basetype)\
type##Vtable g_##type##Vtable = {\
    {(RTTI*)(&g_##basetype##Vtable), #type, NULL, NULL},\
    type##OrderConstruct,\
    type##Predestructor,

#define	METHOD_MAP(type, func) .func = type##_X_##func,
#define END_METHOD_MAP };

#define CONSTRUCTOR_MAP(type) .Constructor = (void(*)(void*))type##Constructor,
#define DESTRUCTOR_MAP(type) .Destructor = (void(*)(void*))type##Destructor,

#else

#define VTABLE_BUILD_DYNAMIC 1
#define BEGIN_METHOD_MAP(type, basetype)\
static void type##VtableBuilder(type##Vtable*);\
PRECONSTRUCTORS(type, basetype)\
PREDESTRUCTORS(type, basetype)\
type##Vtable g_##type##Vtable = {\
    {NULL, #type, NULL, NULL},\
    type##OrderConstruct,\
    type##Predestructor};\
static void type##VtableBuilder(type##Vtable* vtbl)\
{\
    static char is_first = 1;\
    if (0 == is_first) return ;\
	vtbl->__rtti.__base = (RTTI*)&g_##basetype##Vtable;\

#define	METHOD_MAP(type, method) vtbl->method = type##_X_##method;

#define END_METHOD_MAP \
    is_first = 0;}

#define CONSTRUCTOR_MAP(type) vtbl->Constructor = (void(*)(void*))type##Constructor;
#define DESTRUCTOR_MAP(type) vtbl->Destructor = (void(*)(void*))type##Destructor;

#endif

#define METHOD_MAP_PLACEHOLDER(type, basetype) \
BEGIN_METHOD_MAP(type, basetype) \
END_METHOD_MAP 

#define MAKE_PURE_VIRTUAL_CLASS(type) METHOD_MAP_PLACEHOLDER(type, NonBase)

#define	METHOD_NAMED(type, func) type##_X_##func

#define PRECONSTRUCTORS(type, base) \
void type##OrderConstruct(_SELF) { \
    type##VtableBuilder(&g_##type##Vtable);\
        if (g_##base##Vtable.OrderConstruct) {\
            g_##base##Vtable.OrderConstruct(self); \
            if (!g_##type##Vtable.__rtti.__vm_checked) { /* move out may be mute once if */\
                int i; \
                /* TODO: int* is not suitable for 16 bits machine. */int* ttype = (int*)((char*)(&g_##type##Vtable) + sizeof(CommonVtable)); \
                int* tbase = (int*)((char*)(&g_##base##Vtable) + sizeof(CommonVtable)); \
                for (i = 0; i < sizeof(g_##base##Vtable)/sizeof(void*) - sizeof(CommonVtable)/sizeof(void*); ++i) { \
                    if (0 == ttype[i] && 0 != tbase[i]) { \
                        ttype[i] = tbase[i]; \
                    } \
                } \
                g_##type##Vtable.__rtti.__vm_checked = (int*)1; \
            } \
        }\
        if (NULL != g_##type##Vtable.Constructor) g_##type##Vtable.Constructor(self); \
    } \
__INLINE__ void* type##Preconstructor(_SELF) { \
    *(type##Vtable**)self = &g_##type##Vtable;\
    type##OrderConstruct(self);\
    return self;\
}\
__INLINE__ type * type##ArrayConstructor(_Self(type), int num) { \
    type* head = self; \
    int  i = num; \
    while (i--) { \
        type##Preconstructor(head++); \
    } \
    return self; \
}

#define PREDESTRUCTORS(type, base) \
	__INLINE__ void type##Predestructor(_SELF) { \
    if (g_##type##Vtable.Destructor) { \
        g_##type##Vtable.Destructor(self); } \
    if (g_##base##Vtable.Predestructor) { \
        g_##base##Vtable.Predestructor(self); }\
} \
	__INLINE__ void type##ArrayDestructor(_Self(type), int num) { \
	type * head = self; \
	int  i = num; \
	while (i--) { \
        type##Predestructor(head++); \
    } \
	MIL_free(self); \
}

#define CONSTRUCTOR(type) static void type##Constructor(_Self(type))
#define COPY_CONSTRUCTOR(type) \
static void type##_X_##Clone(_Self(type), _Rhs(type));\
static type* type##_X_##ClonePre(_Self(type), _Rhs(type))\
{\
    type##_X_##Clone(self, rhs);\
    return self;\
}\
static void type##_X_##Clone(_Self(type), _Rhs(type))

#define DESTRUCTOR(type) void type##Destructor(_Self(type))

#define PLACEHOLDER(anything) 0

/* Placement new operator. */
#define NewAt(type, p) type##Preconstructor((type *)p)
/* Normal new operator. */
#define New(type) type##Preconstructor((type *)MIL_malloc(sizeof(type)))
/* Copy constructor. */
#define DupNew(type, rhs) type##_X_##ClonePre(New(type), (rhs))
/* Constructor a array of type. */
#define News(type, num) type##ArrayConstructor((type *)MIL_malloc(sizeof(type)), (num))
void OrderDestruct(void*);
/* Normal Delete operator. */
extern DECLSPEC void Delete(void*);
/* Delete a array of object. */
void Deletes(void*, size_t);
extern DECLSPEC void* SafeCast(void* vtable, void* ptr);
/* Delete a object without free memory. */
#define DeleteAt(p) OrderDestruct(p)

#define StaticCast(type, ptr) (type*)(ptr)
#define ReinterpretCast(type, ptr)
#define DynamicCast(type, ptr) StaticCast(type, SafeCast(&g_##type##Vtable, ptr))
#define IsTypeOf(type, ptr) (NULL != DynamicCast(type, ptr))
#define GetTypeName(ptr) \
    ((NULL != ptr && NULL != (*(RTTI**)ptr) && NULL != (*(RTTI**)ptr)->__name) ? (*(RTTI**)ptr)->__name : "")
#define Super(base) (NULL != DynamicCast(base, self) ? &g_##base##Vtable : NULL)
//#define Super(base) (NULL != DynamicCast(base, self) ? &g_##base##Vtable : assert(NULL))

#define MIL_Error(err)
#define MIL_SetError(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
#define MIL_GetError()  ""
#define MIL_ClearError() 
#define MIL_OutOfMemory() 

#define METHOD_TABLE(type) MIL_##type##_METHOD_TABLE

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef COO_INC ----- */
