#ifndef NEW_H_0FEJN6R1
#define NEW_H_0FEJN6R1

#include <stddef.h>
#include <stdarg.h>

#include "Object.h"

typedef void* var;

void* new(const void *class, ...);
void* vnew(const void *class, va_list *ap);
void  delete(void *self);
void* copy(const void *self);

void* ctor(void *self, va_list *ap);
void* dtor(void *self);
void* cpy(const void *self, void *object);

const void* super(const void *self);
void* super_ctor(const void *class, void *self, va_list *ap);
void* super_dtor(const void *class, void *self);
void* super_cpy(const void *class, const void *self, void *object);

void  set(void *self, ...);
void  get(const void *self, ...);

void  vset(void *self, va_list *ap);
void  vget(const void *self, va_list *ap);

#endif /* end of include guard: NEW_H_0FEJN6R1 */
