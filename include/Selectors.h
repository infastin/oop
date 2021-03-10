#ifndef NEW_H_0FEJN6R1
#define NEW_H_0FEJN6R1

#include <stddef.h>
#include <stdarg.h>

#include "Object.h"

typedef void* var;

void* new(const void *object, ...);
void  delete(void *self);

void* ctor(void *self, va_list *props);
void* dtor(void *self);

const void* super(const void *self);
void* super_ctor(const void *class, void *self, va_list *props);
void* super_dtor(const void *class, void *self);

#endif /* end of include guard: NEW_H_0FEJN6R1 */
