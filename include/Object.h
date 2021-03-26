#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>

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

ObjectHeader(FormatException);

typedef void  (*voidf)(void);
typedef void *(*ctor_f)(void *self, va_list *ap);
typedef void *(*cpy_f)(const void *self, void *object);
typedef void *(*dtor_f)(void *self);
typedef void  (*set_f)(void *self, va_list *ap);
typedef void *(*get_f)(const void *self, va_list *ap);
typedef int   (*sfcsan_f)(const char *str,  va_list *ap);

typedef int   (*sfprint_f)(const void *self, FILE *stream, int bin, char *buffer, size_t maxn, 
				int flag, int width, int precision);

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
	sfcsan_f sfscan;
	sfprint_f sfprint;
};

const void* _isObject(const void *self, char *selfname, char *file, int line, const char *func);
void*  _cast(const void *class, const void *self, char *selfname, char *file, int line, const char *func);

const void* _classOf(const void *self, char *selfname, char *file, int line, const char *func);
size_t _sizeOf(const void *self, char *selfname, char *file, int line, const char *func);
int    _isA(const void *self, const void *class, char *selfname, char *file, int line, const char *func);
int    _isOf(const void *self, const void *class, char *selfname, char *file, int line, const char *func);

#define isObject(self) _isObject(self, #self, __FILE__, __LINE__, __FUNCTION__)
#define cast(class, self) _cast(class, self, #self, __FILE__, __LINE__, __FUNCTION__)

#define classOf(self) _classOf(self, #self, __FILE__, __LINE__, __FUNCTION__)
#define sizeOf(self) _sizeOf(self, #self, __FILE__, __LINE__, __FUNCTION__)
#define isA(self, class) _isA(self, class, #self, __FILE__, __LINE__, __FUNCTION__)
#define isOf(self, class) _isOf(self, class, #self, __FILE__, __LINE__, __FUNCTION__)

#endif /* end of include guard: OBJECT_H_76AZNMVF */
