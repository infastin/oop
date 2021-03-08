#ifndef NEW_H_0FEJN6R1
#define NEW_H_0FEJN6R1

#include <stddef.h>
#include <stdarg.h>

struct Object
{
	size_t size;
	void* (*ctor)(void *self, va_list *props);
	void* (*dtor)(void *self);
};

void* new(const void *object, ...);
void  delete(void *self);

#endif /* end of include guard: NEW_H_0FEJN6R1 */
