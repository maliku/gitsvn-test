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

#include "MIL_stdinc.h"

typedef struct __RTTI {
    struct __RTTI * __base;
    const char * __name;
} RTTI;

typedef struct __CommonVtable {
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
#define	_Rhs(type)	type* rhs		/* 'rhs' pointer for COO framework */
#define	_SELF   _Self(void)		/* 'this' pointer for COO framework */
#define	_RHS	_Rhs(void)		/* 'rhs' pointer for COO framework */

/* Macro for call member function of object. */
#define	_MC(pobj)	((pobj)->__mptr)
/* Macro for call virtual member function of object. */
//#define	_VC(pobj)	(((pobj)->__mptr)->__GetVptr(pobj))
#define	_VC(pobj)	(((pobj)->__super).__vptr)

#define _mc(pobj, method, ...) _MC(pobj)->method(pobj, ##__VA_ARGS__)
#define _vc(pobj, method, ...) _VC(pobj)->method(pobj, ##__VA_ARGS__)
		
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
        type * (*type)(type*);\

/* Macro for member function declare end. */
#define	METHOD_DECLARE_END }*__mptr;

#define VIRTUAL_FUNC_DECLARE_PLACEHOLDER(type) \
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
#define	METHOD_REGISTER(type, func) type##func,
#define VIRTUAL_METHOD_REGEND };

#define VIRTUAL_FUNC_REGISTER_PLACEHOLDER(type, basetype) \
VIRTUAL_METHOD_REGBEGIN(type, basetype) \
VIRTUAL_METHOD_REGEND 

#define MAKE_PURE_VIRTUAL_CLASS(type) VIRTUAL_FUNC_REGISTER_PLACEHOLDER(type, NonBase)

#define METHOD_REGBEGIN(type) \
struct __##type##Mtable g_##type##Mtable = {

#define METHOD_REGEND };

#define METHOD_REGISTER_PLACEHOLDER(type) \
METHOD_REGBEGIN(type) \
METHOD_REGEND 

#define	METHOD_NAMED(type, func) type##func

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
	free(self); \
}

#define CONSTRUCTOR(type) type* type##Constructor(_Self(type))
#define COPY_CONSTRUCTOR(type) type* type##Duplicator(_Self(type), _Rhs(type))

#define DESTRUCTOR(type) void type##Destructor(_SELF)

#define CONSTRUCTOR_REGISTER(type) type##Constructor,
#define DESTRUCTOR_REGISTER(type) type##Destructor,

#define METHOD_PLACEHOLDER NULL,
#define NON_CONSTRUCTOR METHOD_PLACEHOLDER
#define NON_DESTRUCTOR METHOD_PLACEHOLDER

/* Placement new operator. */
#define NewAt(type, p) type##Preconstructor((type *)p)
/* Normal new operator. */
#define New(type) type##Preconstructor((type *)malloc(sizeof(type)))
/* Copy constructor. */
#define DupNew(type, rhs) type##Duplicator(New(type), (rhs))
/* Constructor a array of type. */
#define News(type, num) type##ArrayConstructor((type *)malloc(sizeof(type)), (num))
void OrderDestruct(void*);
/* Normal Delete operator. */
void Delete(void*);
/* Delete a array of object. */
void Deletes(void*, size_t);
/* Delete a object without free memory. */
#define DeleteAt(p) OrderDestruct(p)

#define PrintTest(fmt, ...) printf(fmt,##__VA_ARGS__)
#define MIL_Error(err)
#define MIL_SetError(err) puts(err)
#endif   /* ----- #ifndef COO_INC 