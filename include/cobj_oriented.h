/*! ============================================================================
 * @file oop.c 
 * @Synopsis  
 * @author DongKai
 * @version 1.0
 * @date 2009年12月01日
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */
#ifndef __COBJ_ORIENTED_H__
#define __COBJ_ORIENTED_H__

#include "coo_common.h"

typedef struct _virtual_function_t {
    const char* __name;
    void *      __address;
    int         __reserve;
} virtual_function_t;

typedef struct _runtime_type_info_t {
    const char * __type_name;
} runtime_type_info_t;

typedef struct _vtable
{
    struct _vtable*      __base;
    runtime_type_info_t* __typeinfo;
    virtual_function_t*  __entry;
} vtable_t;

void * find_virtual_function_in_current_class(virtual_function_t*, const char*);

#define STRUCT(type) \
extern struct type * type##_##preconstructor(struct type *); \
    extern virtual_function_t g_##type##_virtual_functions[]; \
    extern vtable_t g_##type##_vtable; \
    extern runtime_type_info_t g_##type##_rtti; \
    extern struct _##type##_memfuncs g_##type##_member_functions; \
    struct type

#define CLASS(type) \
extern struct type * type##_##preconstructor(struct type *); \
    extern virtual_function_t g_##type##_virtual_functions[]; \
    extern vtable_t g_##type##_vtable; \
    extern runtime_type_info_t g_##type##_rtti; \
    extern struct _##type##_memfuncs g_##type##_member_functions; \
    typedef struct type

#define SUB_CLASS_DEFINE(type, base) \
    CLASS(type) { \
    base __super;
#define SUB_CLASS_DEFEND(type) }type;

extern vtable_t g_NOSUPER_vtable;


/* #define MEMBER_FUNC_NAMED(type, func, ...) type##_##func(type *self, ##__VA_ARGS__)
 */

#define MEMBER_FUNC_DECBEGIN(type) struct _##type##_memfuncs { \
    struct type* (*type)(struct type*);

#define MEMBER_FUNC_DECLARE(type, func, ...) (*func)(struct type *self, ##__VA_ARGS__);
#define MEMBER_FUNC_DECLARE_CONST(type, func, ...) (*func)(const struct type *self, ##__VA_ARGS__);

#define MEMBER_FUNC_DECEND } * __member_functions;

#define MEMBER_FUNC_REGBEGIN(type) struct _##type##_memfuncs g_##type##_member_functions = {

#define MEMBER_FUNC_REGISTER(type, func) .func = type##_##func,

#define MEMBER_FUNC_REGEND };

#define VIRTUAL_FUNC_REGISTER(type, funcname) \
        {#funcname, (void *)type##_##funcname, 0},

#define VIRTUAL_FUNC_REGBEGIN(type, base) \
    PRECONSTRUCTOR_DEFINE(type) \
PREDESTRUCTOR_DEFINE(type) \
extern void type##_##destructor(type *); \
    runtime_type_info_t g_##type##_rtti = {#type}; \
    vtable_t g_##type##_vtable = {&g_##base##_vtable, &g_##type##_rtti, g_##type##_virtual_functions}; \
    virtual_function_t g_##type##_virtual_functions[] = { \
        {"constructor", type##_##preconstructor, 0}, \
        {"destructor", NULL, 0}, \
        {"array_constructor", type##_##array_constructor, 0}, \
        {"array_destructor", type##_##array_destructor, 0}, 

#define VIRTUAL_FUNC_REGEND {NULL, NULL, 0}};
#define MEMBER_FUNC_DEFINE(type, funcname, ...) type##_##funcname(type* self, ##__VA_ARGS__)
#define MEMBER_FUNC_DEFINE_CONST(type, funcname, ...) type##_##funcname(const type* self, ##__VA_ARGS__)

#define CONSTRUCTOR_DEFINE(type) type* type##_##type(type * self)

#define CONSTRUCTOR_ARGS_DEFINE(type, ...) type* type##_##type##_args(type * self, ##__VA_ARGS__)

#define CONSTRUCTOR_DECLARE(type) struct type* MEMBER_FUNC_DECLARE(type, type)
#define CONSTRUCTOR_ARGS_DECLARE(type, ...) struct type* MEMBER_FUNC_DECLARE(type, type##_args, ##__VA_ARGS__)

#define DESTRUCTOR_REGISTER(type) \
{0, 0, 0}, \
[1] = {.__name = "destructor", .__address = type##_##destructor}}; \
struct {int *unused[3];} Please_Take_Destructor_Register_Of_##type##_At_Last = {

#define DESTRUCTOR_DEFINE(type) void type##_##destructor(type * self)

#define PRECONSTRUCTOR_DEFINE(type) \
    struct type * type##_##preconstructor(type * self) { \
        vtable_t * prev = g_##type##_vtable.__base; \
        if (prev->__entry) { \
            ((type* (*)(type*))(prev->__entry[0].__address))(self); \
        } \
        *(vtable_t **)self = &g_##type##_vtable; \
        self->__member_functions = &g_##type##_member_functions; \
        return (NULL != g_##type##_member_functions.type) ? \
        g_##type##_member_functions.type(self): self; \
    } \
       INLINE type * type##_##array_constructor(type *self, int num) { \
            type * head = self; \
            int  i = num; \
            while (i--) { \
                type##_##preconstructor(head++); \
            } \
            return self; \
        }

#if 0
#define PREDESTRUCTOR_DEFINE(type) \
    void type##_##predestructor(type *self) { \
        vtable_t * prev = g_##type##_vtable.__base; \
        void (*destructor)(void*) = find_virtual_function_in_current_class(g_##type##_vtable.__entry, "destructor"); \
        if (NULL != destructor) (*destructor)(self); \
        while (prev->__entry) { \
            destructor = find_virtual_function_in_current_class(prev->__entry, "destructor"); \
            (*destructor)(self); \
        prev = prev->__base; \
        } \
    }
#endif
#define PREDESTRUCTOR_DEFINE(type) \
    void type##_##predestructor(type *self) { \
        vtable_t * prev = g_##type##_vtable.__base; \
        if (NULL != ((*(vtable_t **)self)->__entry[1].__address)) { \
        ((void (*)(void*))((*(vtable_t **)self)->__entry[1].__address))(self); }\
        while (prev->__entry) { \
            ((void (*)(void*))(prev->__entry[1].__address))(self); \
            prev = prev->__base; \
        } \
    } \
INLINE void type##_##array_destructor(type *self, int num) { \
    type * head = self; \
    int  i = num; \
    while (i--) { \
        type##_##predestructor(head++); \
    } \
    free(self); \
}

#define CALL_CONSTRUCTOR_WITH_ARGS(type, obj, ...) ((g_##type##_member_functions.type##_args) ? (g_##type##_member_functions.type##_args)((type *)obj, ##__VA_ARGS__) : (obj))

#define NEW(type) type##_##preconstructor((type *)malloc(sizeof(type)))
#define NEW_ARGS(type, ...) CALL_CONSTRUCTOR_WITH_ARGS(type, type##_##preconstructor(malloc(sizeof(type))), ##__VA_ARGS__)
#define NEW_ARRAY(type, num) type##_##array_constructor(malloc(sizeof(type) * num), num)
#define NEW_ARRAY_ARGS(type, num, ...) ({ \
        int  i = num; \
        type * self = malloc(sizeof(type) * num); \
        if (NULL != self) { \
        type * head = self; \
            while (i--) { \
                type##_##preconstructor(head); \
                if (g_##type##_member_functions.type##_args) (g_##type##_member_functions.type##_args)(head++, ##__VA_ARGS__); \
            } \
        } \
        self; \
    })

void DELETE(void *ptr);
//#define DELETE(ptr) ((void (*)(void*, int))((*(vtable_t **)ptr)->__entry[3].__address))(ptr, 1)
#define DELETE_ARRAY(ptr, num) ((void (*)(void*, int))((*(vtable_t **)ptr)->__entry[3].__address))(ptr, num)

#define CALL_MEMBER_FUNC(obj, func, ...) (obj)->__member_functions->func(obj, ##__VA_ARGS__)
#define CALL_VIRTUAL_FUNC(retype, obj, func) CALL_VIRTUAL_FUNC0(retype, obj, func)
#define CALL_VIRTUAL_FUNC0(retype, obj, func) ((retype (*)(TYPE_OF(obj)))(look_for_virtual_function(obj, #func)))(obj)
#define CALL_VIRTUAL_FUNC1(retype, obj, func, arg1) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1)))(look_for_virtual_function(obj, #func)))(obj, arg1)
#define CALL_VIRTUAL_FUNC2(retype, obj, func, arg1, arg2) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2)
#define CALL_VIRTUAL_FUNC3(retype, obj, func, arg1, arg2, arg3) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3)
#define CALL_VIRTUAL_FUNC4(retype, obj, func, arg1, arg2, arg3, arg4) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3), TYPE_OF(arg4)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3, arg4)
#define CALL_VIRTUAL_FUNC5(retype, obj, func, arg1, arg2, arg3, arg4, arg5) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3), TYPE_OF(arg4), TYPE_OF(arg5)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3, arg4, arg5)
#define CALL_VIRTUAL_FUNC6(retype, obj, func, arg1, arg2, arg3, arg4, arg5, arg6) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3), TYPE_OF(arg4), TYPE_OF(arg5), TYPE_OF(arg6)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3, arg4, arg5, arg6)
#define CALL_VIRTUAL_FUNC7(retype, obj, func, arg1, arg2, arg3, arg4, arg5, arg6, arg7) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3), TYPE_OF(arg4), TYPE_OF(arg5), TYPE_OF(arg6), TYPE_OF(arg7)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define CALL_VIRTUAL_FUNC8(retype, obj, func, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3), TYPE_OF(arg4), TYPE_OF(arg5), TYPE_OF(arg6), TYPE_OF(arg7), TYPE_OF(arg8)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define CALL_VIRTUAL_FUNC9(retype, obj, func, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) ((retype (*)(TYPE_OF(obj), TYPE_OF(arg1), TYPE_OF(arg2), TYPE_OF(arg3), TYPE_OF(arg4), TYPE_OF(arg5), TYPE_OF(arg6), TYPE_OF(arg7), TYPE_OF(arg8), TYPE_OF(arg9)))(look_for_virtual_function(obj, #func)))(obj, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

void * look_for_virtual_function(void *obj, const char * func_name);

/* For Gcc Only */
#define Min(X,Y) ({\
        typeof (X) x_ = (X);\
        typeof (Y) y_ = (Y);\
        (x_ < y_) ? x_ : y_; })
#endif
