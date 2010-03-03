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
        void* (*Preconstructor)(void * self);
        void  (*Predestructor)(void * self);
        void  (*destructor)(void*);
} CommonVtable;
extern CommonVtable g_NonBaseVtable;

#define	INLINE 

#define	CLASS(type)	\
    struct __##type##Vtable;\
typedef struct __##type##Vtable type##Vtable;\
extern struct __##type##Vtable g_##type##Vtable;\
extern struct __##type##Mtable g_##type##Mtable;\
struct __##type;\
typedef struct __##type type;\
struct __##type

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
#define	_SELF	void* self		/* 'this' pointer for COO framework */

#define VIRTUAL_FUNCTION_DECLARE_BEGIN(type) \
    struct __##type##Vtable {\
        RTTI __rtti;\
        void * (*Preconstructor)(void*);\
        void (*Predestructor)(void*);\
        void (*destructor)(void*);

#define	VIRTUAL_FUNCTION_DECLARE_END }*__vptr;

#define MEMBER_FUNCTION_DECLARE_BEGIN(type) \
    struct __##type##Mtable {\
        type * (*type)(type*);

#define	MEMBER_FUNCTION_DECLARE_END }*__mptr;

#define VIRTUAL_FUNCTION_REGBEGIN(type, basetype) \
PRECONSTRUCTOR_DEFINE(type)\
PREDESTRUCTOR_DEFINE(type)\
type##Vtable g_##type##Vtable = {\
    {(RTTI*)(&g_##basetype##Vtable), #type},\
    type##Preconstructor,\
    type##Predestructor,
#define	VIRTUAL_FUNCTION_REGISTER(type, func) type##func,
#define VIRTUAL_FUNCTION_REGEND };

#define PrintTest(fmt, ...) printf(fmt,##__VA_ARGS__)
#define MEMBER_FUNCTION_REGBEGIN(type) \
struct __##type##Mtable g_##type##Mtable = {
#define MEMBER_FUNCTION_REGEND };

#define PRECONSTRUCTOR_DEFINE(type) \
    void * type##Preconstructor(void * self) { \
        type * addr = (type*)self;\
        RTTI * prev = &g_##type##Vtable.__rtti; \
        assert(NULL != prev);\
        if (((CommonVtable*)*(CommonVtable**)prev)->Preconstructor)\
        ((CommonVtable*)*(CommonVtable**)prev)->Preconstructor(self);\
        *(type##Vtable**)self = &g_##type##Vtable;\
        addr->__mptr = &g_##type##Mtable;\
        return (NULL != g_##type##Mtable.type) ? \
        g_##type##Mtable.type(self) : self; \
    } \
        INLINE type * type##ArrayConstructor(type *self, int num) { \
            type * head = self; \
            int  i = num; \
            while (i--) { \
                type##Preconstructor(head++); \
            } \
            return self; \
        }

#define PREDESTRUCTOR_DEFINE(type) \
	void type##Predestructor(void * self) { \
	RTTI * prev = &g_##type##Vtable.__rtti; \
	assert(NULL != prev);\
	if (((CommonVtable*)*(CommonVtable**)prev)->destructor)\
	((CommonVtable*)*(CommonVtable**)prev)->destructor(self);\
	if (((CommonVtable*)*(CommonVtable**)prev)->Predestructor)\
	((CommonVtable*)*(CommonVtable**)prev)->Predestructor(self);\
} \
	INLINE void type##ArrayDestructor(type *self, int num) { \
	type * head = self; \
	int  i = num; \
	while (i--) { \
	type##Predestructor(head++); \
	} \
	free(self); \
}

#define New(type) type##Preconstructor((type *)malloc(sizeof(type)))

void Delete(void *ptr);

#endif   /* ----- #ifndef COO_INC  ----- */
