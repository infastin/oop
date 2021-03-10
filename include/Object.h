#ifndef OBJECT_H_76AZNMVF
#define OBJECT_H_76AZNMVF

#include <stddef.h>
#include <stdarg.h>
#include <stdatomic.h>

extern const void *Object, *Class;

typedef void  (*voidf)(void);
typedef void *(*ctor_f)(void *self, va_list *props);
typedef void *(*dtor_f)(void *self);

struct Object
{
	const struct Class *class;
	atomic_size_t ref_count;
};

struct Class
{
	const struct Object _;
	const char *name;
	const struct Class *super;
	size_t size;
	
	ctor_f ctor;
	dtor_f dtor;
};

const void* classOf(const void *self);
size_t sizeOf(const void *self);


#endif /* end of include guard: OBJECT_H_76AZNMVF */
