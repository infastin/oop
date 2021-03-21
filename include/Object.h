#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>
#include <stdatomic.h>

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

typedef void 	(*voidf)(void);
typedef void   *(*ctor_f)(void *self, va_list *ap);
typedef void   *(*cpy_f)(const void *self, void *object);
typedef void   *(*dtor_f)(void *self);
typedef void 	(*set_f)(void *self, va_list *ap);
typedef void   *(*get_f)(const void *self, va_list *ap);
typedef char   *(*stringer_f)(const void *self, va_list *ap);

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
	cpy_f cpy;
	
	set_f set;
	get_f get;
	stringer_f stringer;
};

const void* classOf(const void *self);
const void* _isObject(const void *self, char* file, int line);
size_t sizeOf(const void *self);
void*  _cast(const void *class, const void* self, char *file, int line);
int    isA(const void *self, const void *class);
int    isOf(const void *self, const void *class);

#define isObject(self) _isObject(self, __FILE__, __LINE__)
#define cast(class, self) _cast(class, self, __FILE__, __LINE__)

#endif /* end of include guard: OBJECT_H_76AZNMVF */
