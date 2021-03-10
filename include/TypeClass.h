#ifndef TYPECLASS_H_PSBYWVTZ
#define TYPECLASS_H_PSBYWVTZ

#include "Selectors.h"

#define get(self, type) (*(type*) get_vptr(self))

extern const void *TypeClass;

typedef int   (*cmp_f)(const void *self, const void *b);
typedef void  (*set_f)(void *self, va_list *props);
typedef void *(*get_vptr_f)(void *self);
typedef void  (*swap_f)(void *self, void *b);

struct TypeClass
{
	const struct Class _;
	cmp_f cmp;
	swap_f swap;
	set_f set;
	get_vptr_f get_vptr;
};

int cmp(const void *self, const void *b);
int super_cmp(const void *class, const void *self, const void *b);

void swap(void *self, void *b);
void super_swap(const void *class, void *self, void *b);

void* get_vptr(void *self);
void  set(void *self, ...);

void initTypeClass(void);

#endif /* end of include guard: TYPE_H_PSBYWVTZ */
