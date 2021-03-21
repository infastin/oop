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

typedef void  (*voidf)(void);
typedef void *(*ctor_f)(void *self, va_list *ap);
typedef void *(*cpy_f)(const void *self, void *object);
typedef void *(*dtor_f)(void *self);
typedef void  (*set_f)(void *self, va_list *ap);
typedef void *(*get_f)(const void *self, va_list *ap);
typedef char *(*stringer_f)(const void *self, va_list *ap);
typedef int   (*reader_f)(const char *str,  va_list *ap);

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
	reader_f reader;
};

const void* _isObject(const void *self, char* file, int line);
void*  _cast(const void *class, const void* self, char *file, int line);

const void* _classOf(const void *self, char* file, int line);
size_t _sizeOf(const void *self, char* file, int line);
int    _isA(const void *self, const void *class, char* file, int line);
int    _isOf(const void *self, const void *class, char* file, int line);

#define isObject(self) _isObject(self, __FILE__, __LINE__)
#define cast(class, self) _cast(class, self, __FILE__, __LINE__)

#define classOf(self) _classOf(self, __FILE__, __LINE__)
#define sizeOf(self) _sizeOf(self, __FILE__, __LINE__)
#define isA(self, class) _isA(self, class, __FILE__, __LINE__)
#define isOf(self, class) _isOf(self, class, __FILE__, __LINE__)

#endif /* end of include guard: OBJECT_H_76AZNMVF */
