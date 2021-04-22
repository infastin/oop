#ifndef TYPECLASS_H_PSBYWVTZ
#define TYPECLASS_H_PSBYWVTZ

#include <stdarg.h>
#include <stddef.h>

#include "Object.h"
#include "IO.h"
#include "ScalarOperatorsInterface.h"
#include "OperatorsInterface.h"
#include "SortInterface.h"

ClassHeader(TypeClass);
ObjectHeader(TypeException);

typedef void *(*inverse_add_f)(void *self);
typedef void *(*inverse_multi_f)(void *self);
typedef void *(*rnd_f)(void *self, va_list *ap);

struct TypeClass
{
	const struct Class _;
	
	method inverse_add;
	method inverse_multi;
	method rnd;

	const struct IOInterface io;
	const struct ScalarOperatorsInterface sc;
	const struct OperatorsInterface oper;
	const struct SortInterface sort;
};


void* sum(const void *self, const void *b);
void* subtract(const void *self, const void *b);
void* product(const void *self, const void *b);
void* divide(const void *self, const void *b);
void* inverse_add(void *self);
void* inverse_multi(void *self);
void* rnd(const void *class, void *self, ...);
void* vrnd(const void *class, void *self, va_list *ap);

#endif /* end of include guard: TYPE_H_PSBYWVTZ */
