#ifndef TYPECLASS_H_PSBYWVTZ
#define TYPECLASS_H_PSBYWVTZ

#include <stdarg.h>
#include <stddef.h>

#include "Object.h"
#include "IO.h"

ClassHeader(TypeClass);
ObjectHeader(TypeException);

typedef int   (*cmp_f)(const void *self, const void *b);
typedef void *(*sum_f)(const void *self, const void *b);
typedef void *(*subtract_f)(const void *self, const void *b);
typedef void *(*product_f)(const void *self, const void *b);
typedef void *(*divide_f)(const void *self, const void *b);
typedef void *(*inverse_add_f)(void *self);
typedef void *(*inverse_multi_f)(void *self);
typedef void  (*scadd_f)(void *self, va_list *ap);
typedef void  (*scsub_f)(void *self, va_list *ap);
typedef void  (*scmulti_f)(void *self, va_list *ap);
typedef void  (*scdivide_f)(void *self, va_list *ap);
typedef void *(*rnd_f)(void *self, va_list *ap);
typedef void  (*swap_f)(void *self, void *b);

struct TypeClass
{
	const struct Class _;
	
	cmp_f cmp;
	swap_f swap;
	sum_f sum;
	subtract_f subtract;
	product_f product;
	divide_f divide;
	scadd_f scadd;
	scsub_f scsub;
	scmulti_f scmulti;
	scdivide_f scdivide;
	inverse_add_f inverse_add;
	inverse_multi_f inverse_multi;
	rnd_f rnd;

	const struct IOInterface io;
};

int   cmp(const void *self, const void *b);

void* sum(const void *self, const void *b);
void* subtract(const void *self, const void *b);
void* product(const void *self, const void *b);
void* divide(const void *self, const void *b);
void* inverse_add(void *self);
void* inverse_multi(void *self);
void* rnd(const void *class, void *self, ...);

void  scadd(void *self, ...);
void  scsub(void *self, ...);
void  scmulti(void *self, ...);
void  scdivide(void *self, ...);

void* vrnd(const void *class, void *self, va_list *ap);

void  vscadd(void *self, va_list *ap);
void  vscsub(void *self, va_list *ap);
void  vscmulti(void *self, va_list *ap);
void  vscdivide(void *self, va_list *ap);

void  swap(void *self, void *b);

#endif /* end of include guard: TYPE_H_PSBYWVTZ */
