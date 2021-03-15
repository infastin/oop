#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <assert.h>

/*
 * Class and object macro
 */

#define ClassHeader(T) extern const void* const T (void)
#define ObjectHeader(T) extern void* const T (void)

#define ClassImpl(T)  					\
	static const void *_ ## T; 			\
	const void* const T(void)

#define ObjectImpl(T)  					\
	static void *_ ## T; 				\
	void* const T(void)

/*
 * Object and Class
 */

#define MAGIC_NUM 0xAb0bA

ClassHeader(Object);
ClassHeader(Class);

typedef void  (*voidf)(void);
typedef void *(*ctor_f)(void *self, va_list *props);
typedef void *(*dtor_f)(void *self);
typedef void  (*set_f)(void *self, va_list *props);
typedef void *(*get_vptr_f)(void *self);

struct Object
{
	unsigned int magic;
	const void *class;
	atomic_uint ref_count;
};

struct Class
{
	const struct Object _;
	const char *name;
	const void *super;
	size_t size;
	
	ctor_f ctor;
	dtor_f dtor;
	set_f set;
	get_vptr_f get_vptr;
};

const void* classOf(const void *self);
size_t sizeOf(const void *self);
void* cast(const void *class, const void* self);
int isA(const void *self, const void *class);
int isOf(const void *self, const void *class);

#define isObject(p) (assert(p), assert((((struct Object*) (p))->magic) == (MAGIC_NUM)), (p))

#endif /* end of include guard: OBJECT_H_76AZNMVF */
