#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>

extern const void *Object, *Class;

typedef void  (*voidf)(void);
typedef void *(*ctor_f)(void *self, va_list *props);
typedef void *(*dtor_f)(void *self);
typedef void  (*method_f)(void *self, voidf selection, voidf method);

struct Object
{
	const struct Class *class;	
};

struct Class
{
	const struct Object _;
	const char *name;
	const struct Class *super;
	size_t size;
	
	ctor_f ctor;
	dtor_f dtor;
	method_f method;
};

const void* classOf(const void *self);
size_t sizeOf(const void *self);


#endif /* end of include guard: OBJECT_H_76AZNMVF */
