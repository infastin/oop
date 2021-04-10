#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>

#include "Interface.h"
#include "Macro.h"

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
	const struct Class *super;
	size_t size;
	
	struct InterfaceElem *implements;
	unsigned int impl_number;

	ctor_f ctor;
	dtor_f dtor;
	cpy_f cpy;
	
	set_f set;
	get_f get;
};

const void* _isObject(const void *self, 
		char *selfname, char *file, int line, const char *func);

void* _cast(const void *class, const void *self, 
		char *classname, char *selfname, char *file, int line, const char *func);

const void* _classOf(const void *self, 
		char *selfname, char *file, int line, const char *func);

size_t _sizeOf(const void *self, 
		char *selfname, char *file, int line, const char *func);

int _isA(const void *self, const void *class, 
		char *selfname, char *file, int line, const char *func);

int _isOf(const void *self, const void *class, 
		char *selfname, char *file, int line, const char *func);

#define isObject(self) _isObject(self, \
	#self, __FILE__, __LINE__, __FUNCTION__)

#define cast(class, self) _cast(class, self, \
	#class, #self, __FILE__, __LINE__, __FUNCTION__)

#define classOf(self) _classOf(self, \
	#self, __FILE__, __LINE__, __FUNCTION__)

#define sizeOf(self) _sizeOf(self, \
	#self, __FILE__, __LINE__, __FUNCTION__)

#define isA(self, class) _isA(self, class, \
	#self, __FILE__, __LINE__, __FUNCTION__)

#define isOf(self, class) _isOf(self, class, \
	#self, __FILE__, __LINE__, __FUNCTION__)

#endif /* end of include guard: OBJECT_H_76AZNMVF */
