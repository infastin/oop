#ifndef TYPECLASS_H_PSBYWVTZ
#define TYPECLASS_H_PSBYWVTZ

#include <stddef.h>

#include "Object.h"

ClassHeader(TypeClass);

typedef int   (*cmp_f)(const void *self, const void *b);
typedef void *(*sum_f)(const void *self, const void *b);
typedef void *(*product_f)(const void *self, const void *b);
typedef void  (*scadd_f)(const void *self, va_list *props);
typedef void  (*scmulti_f)(const void *self, va_list *props);
typedef void  (*swap_f)(void *self, void *b);

struct TypeClass
{
	const struct Class _;
	cmp_f cmp;
	swap_f swap;
	sum_f sum;
	product_f product;
	scadd_f scadd;
	scmulti_f scmulti;
};

int   cmp(const void *self, const void *b);
int   super_cmp(const void *class, const void *self, const void *b);

void* sum(const void *self, const void *b);
void* super_sum(const void *class, const void *self, const void *b);

void* product(const void *self, const void *b);
void* super_product(const void *class, const void *self, const void *b);

void  scadd(const void *self, ...);
void  super_scadd(const void *class, const void *self, ...);

void  scmulti(const void *self, ...);
void  super_scmulti(const void *class, const void *self, ...);

void  swap(void *self, void *b);
void  super_swap(const void *class, void *self, void *b);

#endif /* end of include guard: TYPE_H_PSBYWVTZ */
