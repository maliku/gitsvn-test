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
} RTTI;

STRUCT __CommonVtable {
        RTTI __rtti;
        void* (*OrderConstruct)(void*);
        void  (*Predestructor)(void*);
        void  (*Destructor)(void*);
} CommonVtable;
extern CommonVtable g_NonBaseVtable;

/* Macro for class declare. */
#define	CLASS(type)	\
    struct __##type##Vtable;\
typedef struct __##type##Vtable type##Vtable;\
extern struct __##type##Vtable g_##type##Vtable;\
extern struct __##type##Mtable g_##type##Mtable;\
struct __##type;\
typedef struct __##type type;\
extern void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);\
struct __##type

/* Macro for inherit class decalre. */
#if 0
#define	CLASS_INHERIT_BEGIN(type, basetype) \
typedef basetype##Vtable type##Vtable;\
extern type##Vtable g_##type##Vtable;\
extern struct __##type##Mtable g_##type##Mtable;\
struct _##type;\
typedef struct _##type type;\
extern void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);\
struct _##type {\
    union { \
	    basetype __class; \
        type##Vtable *__vptr; \
    }__super;
#endif

#define NO_VIRTUAL_METHOD_EXPAND(type) \
typedef __VtableTypeOf##type##Base type##Vtable;\
extern type##Vtable g_##type##Vtable;\
struct _##type {\
    union { \
	    __BaseOf##type __class; \
        type##Vtable *__vptr; \
    }__super;

#define VIRTUAL_METHOD_EXPAND_DECLARE_BEGIN(type) \
struct __##type##Vtable; \
typedef struct __##type##Vtable type##Vtable;\
extern type##Vtable g_##type##Vtable;\
struct _##type {\
    union { \
	    __BaseOf##type __class; \
    struct __##type##Vtable {\
        RTTI __rtti;\
        void* (*OrderConstruct)(void*);\
        void (*Predestructor)(void*);\
        void (*Destructor)(void*);

/* Macro for virtual member function declare end. */
#define	VIRTUAL_METHOD_EXPAND_DECLARE_END }*__vptr; } __super;

#define	CLASS_INHERIT_BEGIN(type, basetype) \
extern struct __##type##Mtable g_##type##Mtable;\
struct _##type;\
typedef struct _##type type;\
typedef basetype __BaseOf##type; \
typedef basetype##Vtable __VtableTypeOf##type##Base;\
extern void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);\

#define	CLASS_INHERIT_END };

#define	_Self(type)	type* self		/* 'this' pointer for COO framework */
#define	_CSelf(type)	const type* const self		/* 'this' pointer for COO framework */
#define	_Rhs(type)	type* rhs		/* 'rhs' pointer for COO framework */
#define	_SELF   _Self(void)		/* 'this' pointer for COO framework */
#define	_CSELF  _CSelf(void)		/* 'this' pointer for COO framework */
#define	_RHS	_Rhs(void)		/* 'rhs' pointer for COO framework */

/* Macro for call member function of object. */
#define	_MC(pobj)	((pobj)->__mptr)
/* Macro for call virtual member function of object. */
#define	_VC(pobj)	(((pobj)->__super).__vptr)

#if defined(__GNUC__) && __GNUC__ >= 4
#define _vf(p, func) ({ \
        RTTI* tmp = *(RTTI**)p; \
        if (NULL == _VC(p)->func) { \
            while (NULL != tmp) { \
                if (NULL != (_VC(p)->func = _VC(_MC(p)->OT(tmp))->func)) break; \
                tmp = *(RTTI**)tmp; \
            } \
        } \
        _VC(p)->func; })
#else
#define _vf6(p, func) ( _VC(p)->func ? _VC(p)->func : NULL/* Too more depth of inheritance hierarchy! */ )
#define _vf5(p, func) ( _VC(p)->func ? _VC(p)->func : (_VC(p)->func = _vf6(_MC(p)->OT((*(RTTI**)(p))), func)) )
#define _vf4(p, func) ( _VC(p)->func ? _VC(p)->func : (_VC(p)->func = _vf5(_MC(p)->OT((*(RTTI**)(p))), func)) )
#define _vf3(p, func) ( _VC(p)->func ? _VC(p)->func : (_VC(p)->func = _vf4(_MC(p)->OT((*(RTTI**)(p))), func)) )
#define _vf2(p, func) ( _VC(p)->func ? _VC(p)->func : (_VC(p)->func = _vf3(_MC(p)->OT((*(RTTI**)(p))), func)) )
#define _vf1(p, func) ( _VC(p)->func ? _VC(p)->func : (_VC(p)->func = _vf2(_MC(p)->OT((*(RTTI**)(p))), func)) )
#define _vf(p, func)  ( _VC(p)->func ? _VC(p)->func : (_VC(p)->func = _vf1(_MC(p)->OT((*(RTTI**)(p))), func)) )
#endif

/* The macro can verify validity of virtual method pointer with a object;
 * If the virtual method pointer is null, it's will be assigned to the same name method of super class. */
#define VirtualMethodVerify(p, func) assert(p && _vf(p, func))

#define	_m(member)	(self->member)
#define	_tm(type, member)	(((type*)self)->member)

#define _mc(pobj, method, ...) (_MC(pobj)->method ? _MC(pobj)->method((pobj), ##__VA_ARGS__) : assert(0))
#define _mc0(pobj, method) (_MC(pobj)->method ? _MC(pobj)->method((pobj)) : assert(0))
#define _mc1(pobj, method, arg1) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1) : assert(0))
#define _mc2(pobj, method, arg1, arg2) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2) : assert(0))
#define _mc3(pobj, method, arg1, arg2, arg3) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3) : assert(0))
#define _mc4(pobj, method, arg1, arg2, arg3, arg4) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3, arg4) : assert(0))
#define _mc5(pobj, method, arg1, arg2, arg3, arg4, arg5) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3, arg4, arg5) : assert(0))
#define _mc6(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3, arg4, arg5, arg6) : assert(0))
#define _mc7(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3, arg4, arg5, arg6, arg7) : assert(0))
#define _mc8(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) : assert(0))
#define _mc9(pobj, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) (_MC(pobj)->method ? _MC(pobj)->method((pobj), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) : assert(0))

#define _vc(pobj, method, ...) _vf(pobj, method)((pobj), ##__VA_ARGS__)
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

#define PRIVATE_BEGIN(type) \
    char __ [sizeof(struct _##type##_private {

#define PRIVATE_END })];

#define _ptm(type, name) (*(struct _##type##_private*)(&((type*)self)->__)).name
#define _private(type) ((struct _##type##_private*)(&((type*)self)->__))
#define _public(type)  ((type*)self)

/* Macro for virtual member function declare begin. */
#define VIRTUAL_METHOD_DECLARE_BEGIN(type) \
    union { \
        void * __class; \
    struct __##type##Vtable {\
        RTTI __rtti;\
        void* (*OrderConstruct)(void*);\
        void (*Predestructor)(void*);\
        void (*Destructor)(void*);

/* Macro for virtual member function declare end. */
#define	VIRTUAL_METHOD_DECLARE_END }*__vptr; } __super;

/* Macro for member function declare begin. */
#define METHOD_DECLARE_BEGIN(type) \
    struct __##type##Mtable {\
        type* (*OT)(_SELF); \
        type* (*type)(type*); /* The constructor can't declare as virtual function because it's need type info. */

/* Macro for member function declare end. */
#define	METHOD_DECLARE_END }*__mptr;

#define VIRTUAL_METHOD_DECLARE_PLACEHOLDER(type) \
VIRTUAL_METHOD_DECLARE_BEGIN(type) \
VIRTUAL_METHOD_DECLARE_END 

#define METHOD_DECLARE_PLACEHOLDER(type) \
METHOD_DECLARE_BEGIN(type) \
METHOD_DECLARE_END 

#define VIRTUAL_METHOD_MAP_BEGIN(type, basetype) \
PRECONSTRUCTORS(type)\
PREDESTRUCTORS(type)\
type##Vtable g_##type##Vtable = {\
    {(RTTI*)(&g_##basetype##Vtable), #type},\
    type##OrderConstruct,\
    type##Predestructor,

#if defined(__GNUC__) && __GNUC__ >= 4
#define	METHOD_MAP(type, func) .func = type##_X_##func,
#else
#define	METHOD_MAP(type, func) type##_X_##func,
#endif

#define VIRTUAL_METHOD_MAP_END };

#define VIRTUAL_METHOD_MAP_PLACEHOLDER(type, basetype) \
VIRTUAL_METHOD_MAP_BEGIN(type, basetype) \
VIRTUAL_METHOD_MAP_END 

#define MAKE_PURE_VIRTUAL_CLASS(type) VIRTUAL_METHOD_MAP_PLACEHOLDER(type, NonBase)

#define METHOD_MAP_BEGIN(type) \
__INLINE__ type* type##_X_OT(_SELF) { return (type*)self; } \
struct __##type##Mtable g_##type##Mtable = { \
    type##_X_OT,

#define METHOD_MAP_END };

#define METHOD_MAP_PLACEHOLDER(type) \
METHOD_MAP_BEGIN(type) \
METHOD_MAP_END 

#define	METHOD_NAMED(type, func) type##_X_##func

#define PRECONSTRUCTORS(type) \
    void * type##OrderConstruct(_SELF) { \
        type * addr = (type*)self;\
        RTTI * prev = &g_##type##Vtable.__rtti; \
        assert(NULL != prev);\
        if (((CommonVtable*)*(CommonVtable**)prev)->OrderConstruct)\
        ((CommonVtable*)*(CommonVtable**)prev)->OrderConstruct(self);\
        addr->__mptr = &g_##type##Mtable;\
        return (NULL != g_##type##Mtable.type) ? \
        g_##type##Mtable.type(self) : self; \
    } \
__INLINE__ void * type##Preconstructor(_SELF) { \
    *(type##Vtable**)self = &g_##type##Vtable;\
    return type##OrderConstruct(self);\
}\
__INLINE__ type * type##ArrayConstructor(_Self(type), int num) { \
    type * head = self; \
    int  i = num; \
    while (i--) { \
        type##Preconstructor(head++); \
    } \
    return self; \
}

#define PREDESTRUCTORS(type) \
	__INLINE__ void type##Predestructor(_SELF) { \
	RTTI * prev = &g_##type##Vtable.__rtti; \
	assert(NULL != prev);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Destructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Destructor(self);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Predestructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Predestructor(self);\
} \
	__INLINE__ void type##ArrayDestructor(_Self(type), int num) { \
	type * head = self; \
	int  i = num; \
	while (i--) { \
        type##Predestructor(head++); \
    } \
	MIL_free(self); \
}

#define CONSTRUCTOR(type) type* type##Constructor(_Self(type))
#define COPY_CONSTRUCTOR(type) type* type##Duplicator(_Self(type), _Rhs(type))

#define DESTRUCTOR(type) void type##Destructor(_SELF)

#define CONSTRUCTOR_MAP(type) type##Constructor,
#define DESTRUCTOR_MAP(type) type##Destructor,

#define METHOD_PLACEHOLDER(method) NULL,
#define NON_CONSTRUCTOR METHOD_PLACEHOLDER(Constructor)
#define NON_DESTRUCTOR METHOD_PLACEHOLDER(Destructor)
#define PLACEHOLDER(anything) 0

/* Placement new operator. */
#define NewAt(type, p) type##Preconstructor((type *)p)
/* Normal new operator. */
#define New(type) type##Preconstructor((type *)MIL_malloc(sizeof(type)))
/* Copy constructor. */
#define DupNew(type, rhs) type##Duplicator(New(type), (rhs))
/* Constructor a array of type. */
#define News(type, num) type##ArrayConstructor((type *)MIL_malloc(sizeof(type)), (num))
void OrderDestruct(void*);
/* Normal Delete operator. */
void Delete(void*);
/* Delete a array of object. */
void Deletes(void*, size_t);
void* SafeCast(void* vtable, void* ptr);
/* Delete a object without free memory. */
#define DeleteAt(p) OrderDestruct(p)

#define StaticCast(type, ptr) (type*)(ptr)
#define ReinterpretCast(type, ptr)
#define DynamicCast(type, ptr) StaticCast(type, SafeCast(&g_##type##Vtable, ptr))
#define IsTypeOf(type, ptr) (NULL != DynamicCast(type, ptr))
#define GetTypeName(ptr) \
    ((NULL != ptr && NULL != (*(RTTI**)ptr) && NULL != (*(RTTI**)ptr)->__name) ? (*(RTTI**)ptr)->__name : "")

#define MIL_Error(err)
#define MIL_SetError(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
#define MIL_GetError()  ""
#define MIL_ClearError() 
#define MIL_OutOfMemory() 

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef COO_INC ----- */
