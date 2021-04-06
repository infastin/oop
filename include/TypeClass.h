#ifndef TYPECLASS_H_PSBYWVTZ
#define TYPECLASS_H_PSBYWVTZ

#include <stdarg.h>
#include <stddef.h>

#include "Object.h"
#include "IO.h"
#include "ScalarOperatorsInterface.h"
#include "OperatorsInterface.h"

ClassHeader(TypeClass);
ObjectHeader(TypeException);

typedef int   (*cmp_f)(const void *self, const void *b);
typedef void *(*inverse_add_f)(void *self);
typedef void *(*inverse_multi_f)(void *self);
typedef void *(*rnd_f)(void *self, va_list *ap);
typedef void  (*swap_f)(void *self, void *b);

struct TypeClass
{
	const struct Class _;
	
	cmp_f cmp;
	swap_f swap;
	inverse_add_f inverse_add;
	inverse_multi_f inverse_multi;
	rnd_f rnd;

	const struct IOInterface io;
	const struct ScalarOperatorsInterface sc;
	const struct OperatorsInterface oper;
};

int   cmp(const void *self, const void *b);

void* sum(const void *self, const void *b);
void* subtract(const void *self, const void *b);
void* product(const void *self, const void *b);
void* divide(const void *self, const void *b);
void* inverse_add(void *self);
void* inverse_multi(void *self);
void* rnd(const void *class, void *self, ...);

void* vrnd(const void *class, void *self, va_list *ap);

void  swap(void *self, void *b);

#endif /* end of include guard: TYPE_H_PSBYWVTZ */
