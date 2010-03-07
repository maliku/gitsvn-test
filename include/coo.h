/*!============================================================================
 * @file coo.h 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 03/02/2010 03:33:57 PM 
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef  COO_INC
#define  COO_INC

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

#define	INLINE 

/* Macro for class declare. */
#define	CLASS(type)	\
    struct __##type##Vtable;\
typedef struct __##type##Vtable type##Vtable;\
extern struct __##type##Vtable g_##type##Vtable;\
extern struct __##type##Mtable g_##type##Mtable;\
struct __##type;\
typedef struct __##type type;\
struct __##type

/* Macro for inherit class decalre. */
#define	CLASS_INHERIT_BEGIN(type, basetype) \
typedef basetype##Vtable type##Vtable;\
extern type##Vtable g_##type##Vtable;\
extern struct __##type##Mtable g_##type##Mtable;\
struct _##type;\
typedef struct _##type type;\
struct _##type {\
	basetype __super;

#define	CLASS_INHERIT_END };

#define	_Self(type)	type* self		/* 'this' pointer for COO framework */
#define	_Rhs(type)	type* rhs		/* 'this' pointer for COO framework */
#define	_SELF   _Self(void)		/* 'this' pointer for COO framework */
#define	_RHS	_Rhs(void)		/* 'this' pointer for COO framework */

/* Macro for call member function of object. */
#define	_MC(pobj)	((pobj)->__mptr)
/* Macro for call virtual member function of object. */
#define	_VC(pobj)	(((pobj)->__mptr)->__GetVptr(pobj))
		
/* Macro for virtual member function declare begin. */
#define VIRTUAL_FUNCTION_DECLARE_BEGIN(type) \
    struct __##type##Vtable {\
        RTTI __rtti;\
        void* (*OrderConstruct)(void*);\
        void (*Predestructor)(void*);\
        void (*Destructor)(void*);

/* Macro for virtual member function declare end. */
#define VIRTUAL_FUNCTION_DECLARE_BEGIN(type) \
#define	VIRTUAL_FUNCTION_DECLARE_END }*__vptr;

/* Macro for member function declare begin. */
#define MEMBER_FUNCTION_DECLARE_BEGIN(type) \
    struct __##type##Mtable {\
        type##Vtable* (*__GetVptr)(type*);\
        type * (*type)(type*);\

/* Macro for member function declare end. */
#define	MEMBER_FUNCTION_DECLARE_END }*__mptr;

#define VIRTUAL_FUNC_DECLARE_PLACEHOLDER(type) \
VIRTUAL_FUNCTION_DECLARE_BEGIN(type) \
VIRTUAL_FUNCTION_DECLARE_END 

#define MEMBER_FUNC_DECLARE_PLACEHOLDER(type) \
MEMBER_FUNCTION_DECLARE_BEGIN(type) \
MEMBER_FUNCTION_DECLARE_END 

#define VIRTUAL_FUNCTION_REGBEGIN(type, basetype) \
PRECONSTRUCTORS(type)\
PREDESTRUCTORS(type)\
type##Vtable g_##type##Vtable = {\
    {(RTTI*)(&g_##basetype##Vtable), #type},\
    type##OrderConstruct,\
    type##Predestructor,
#define	FUNCTION_REGISTER(type, func) type##func,
#define VIRTUAL_FUNCTION_REGEND };

#define VIRTUAL_FUNC_REGISTER_PLACEHOLDER(type) \
VIRTUAL_FUNCTION_REGBEGIN(type) \
VIRTUAL_FUNCTION_REGEND 

#define MEMBER_FUNCTION_REGBEGIN(type) \
LOCATE_VTABLE(type);\
struct __##type##Mtable g_##type##Mtable = { \
    type##GetVPTR,
#define MEMBER_FUNCTION_REGEND };

#define MEMBER_FUNC_REGISTER_PLACEHOLDER(type) \
MEMBER_FUNCTION_REGBEGIN(type) \
MEMBER_FUNCTION_REGEND 

#define LOCATE_VTABLE(type) \
    type##Vtable* type##GetVPTR(type* self) { return ((type##Vtable*)*(type##Vtable**)self); }
    
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
INLINE void * type##Preconstructor(_SELF) { \
    *(type##Vtable**)self = &g_##type##Vtable;\
    return type##OrderConstruct(self);\
}\
INLINE type * type##ArrayConstructor(_Self(type), int num) { \
    type * head = self; \
    int  i = num; \
    while (i--) { \
        type##Preconstructor(head++); \
    } \
    return self; \
}

#define PREDESTRUCTORS(type) \
	void type##Predestructor(_SELF) { \
	RTTI * prev = &g_##type##Vtable.__rtti; \
	assert(NULL != prev);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Destructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Destructor(self);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Predestructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Predestructor(self);\
} \
	INLINE void type##ArrayDestructor(_Self(type), int num) { \
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

#define FUNCTION_PLACEHOLDER NULL,
#define NON_CONSTRUCTOR FUNCTION_PLACEHOLDER
#define NON_DESTRUCTOR FUNCTION_PLACEHOLDER

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
#endif   /* ----- #ifndef COO_INC  ----- */
