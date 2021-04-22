#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>

#include "Macro.h"

/*
 * Object and Class
 */

#define MAGIC_NUM 0xAb0bA

ClassHeader(Object);
ClassHeader(Class);

ObjectHeader(FormatException);

/* Generic function pointer */
typedef void  (*voidf)(void);

struct Object
{
	unsigned int magic;
	const void *class;
	atomic_uint ref_count;
};

typedef struct
{
	const char *tag;
	voidf selector;
	voidf method;
}  method;

struct Class
{
	const struct Object _;
	const char *name;
	const struct Class *super;
	size_t size;

	method ctor;
	method dtor;
	method cpy;
	
	method set;
	method get;
};

const void* _isObject(char *selfname, char *file, int line, const char *func,
		const void *self);

void* _cast(char *classname, char *selfname, char *file, int line, const char *func,
		const void *class, const void *self);

const void* _classOf(char *selfname, char *file, int line, const char *func,
		const void *self);

size_t _sizeOf(char *selfname, char *file, int line, const char *func,
		const void *self);

int _isA(char *selfname, char *file, int line, const char *func,
		const void *self, const void *class);

int _isOf(char *selfname, char *file, int line, const char *func,
		const void *self, const void *class);

#define isObject(self) _isObject(TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		self)

#define cast(class, self) _cast(TOSTR(class), TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		class, self)

#define classOf(self) _classOf(TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		self)

#define sizeOf(self) _sizeOf(TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		self)

#define isA(self, class) _isA(TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		self, class)

#define isOf(self, class) _isOf(TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		self, class)

#endif /* end of include guard: OBJECT_H_76AZNMVF */
