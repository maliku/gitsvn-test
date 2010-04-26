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
__inline__ void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);\
struct __##type

/* Macro for inherit class decalre. */
#define	CLASS_INHERIT_BEGIN(type, basetype) \
typedef basetype##Vtable type##Vtable;\
extern type##Vtable g_##type##Vtable;\
extern struct __##type##Mtable g_##type##Mtable;\
struct _##type;\
typedef struct _##type type;\
__inline__ void* type##Preconstructor(_SELF);\
void type##Predestructor(_SELF);\
struct _##type {\
    union { \
	    basetype __class; \
        type##Vtable *__vptr; \
    }__super;

#define	CLASS_INHERIT_END };

#define	_Self(type)	type* self		/* 'this' pointer for COO framework */
#define	_CSelf(type)	const type* self		/* 'this' pointer for COO framework */
#define	_Rhs(type)	type* rhs		/* 'rhs' pointer for COO framework */
#define	_SELF   _Self(void)		/* 'this' pointer for COO framework */
#define	_CSELF   _CSelf(void)		/* 'this' pointer for COO framework */
#define	_RHS	_Rhs(void)		/* 'rhs' pointer for COO framework */

/* Macro for call member function of object. */
#define	_MC(pobj)	((pobj)->__mptr)
/* Macro for call virtual member function of object. */
#define	_VC(pobj)	(((pobj)->__super).__vptr)

#define	_m(member)	(self->member)
#define	_tm(type, member)	(((type*)self)->member)
#define _mc(pobj, method, ...) _MC(pobj)->method(pobj, ##__VA_ARGS__)
#define _vc(pobj, method, ...) _VC(pobj)->method(pobj, ##__VA_ARGS__)
		
#define PRIVATE_BEGIN(type) \
    char __ [sizeof(struct _##type##_private {

#define PRIVATE_END })]

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
        type * (*type)(type*); /* The constructor can't declare as virtual function because it's need type info. */

/* Macro for member function declare end. */
#define	METHOD_DECLARE_END }*__mptr;

#define VIRTUAL_METHOD_DECLARE_PLACEHOLDER(type) \
VIRTUAL_METHOD_DECLARE_BEGIN(type) \
VIRTUAL_METHOD_DECLARE_END 

#define METHOD_DECLARE_PLACEHOLDER(type) \
METHOD_DECLARE_BEGIN(type) \
METHOD_DECLARE_END 

#define VIRTUAL_METHOD_REGBEGIN(type, basetype) \
PRECONSTRUCTORS(type)\
PREDESTRUCTORS(type)\
type##Vtable g_##type##Vtable = {\
    {(RTTI*)(&g_##basetype##Vtable), #type},\
    type##OrderConstruct,\
    type##Predestructor,

#ifdef C99 /* TODO: Replace to real c99 macro. */
#define	METHOD_REGISTER(type, func) .func = type##_X_##func,
#else
#define	METHOD_REGISTER(type, func) type##_X_##func,
#endif
#define VIRTUAL_METHOD_REGEND };

#define VIRTUAL_METHOD_REGISTER_PLACEHOLDER(type, basetype) \
VIRTUAL_METHOD_REGBEGIN(type, basetype) \
VIRTUAL_METHOD_REGEND 

#define MAKE_PURE_VIRTUAL_CLASS(type) VIRTUAL_METHOD_REGISTER_PLACEHOLDER(type, NonBase)

#define METHOD_REGBEGIN(type) \
struct __##type##Mtable g_##type##Mtable = {

#define METHOD_REGEND };

#define METHOD_REGISTER_PLACEHOLDER(type) \
METHOD_REGBEGIN(type) \
METHOD_REGEND 

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
__inline__ void * type##Preconstructor(_SELF) { \
    *(type##Vtable**)self = &g_##type##Vtable;\
    return type##OrderConstruct(self);\
}\
__inline__ type * type##ArrayConstructor(_Self(type), int num) { \
    type * head = self; \
    int  i = num; \
    while (i--) { \
        type##Preconstructor(head++); \
    } \
    return self; \
}

#define PREDESTRUCTORS(type) \
	__inline__ void type##Predestructor(_SELF) { \
	RTTI * prev = &g_##type##Vtable.__rtti; \
	assert(NULL != prev);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Destructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Destructor(self);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Predestructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Predestructor(self);\
} \
	__inline__ void type##ArrayDestructor(_Self(type), int num) { \
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

#define CONSTRUCTOR_REGISTER(type) type##Constructor,
#define DESTRUCTOR_REGISTER(type) type##Destructor,

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
void* SafeCast(const char*, void* ptr);
/* Delete a object without free memory. */
#define DeleteAt(p) OrderDestruct(p)

#define StaticCast(type, ptr) (type*)(ptr)
#define ReinterpretCast(type, ptr)
#define DynamicCast(type, ptr) StaticCast(type, SafeCast(#type, ptr))

#define PrintTest(fmt, ...) printf(fmt,##__VA_ARGS__)
#define MIL_Error(err)
#define MIL_SetError(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif   /* ----- #ifndef COO_INC ----- */
