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

struct TypeClass
{
	const struct Class _;
	
	method inverse_add;
	method inverse_multi;
	method rnd;
	method cmp_to_zero;
	method absolute;
	method set_to_zero;
	method set_to_one;

	const struct IOInterface io;
	const struct ScalarOperatorsInterface sc;
	const struct OperatorsInterface oper;
	const struct SortInterface sort;
};


void* inverse_add(const void *self);
void* inverse_multi(const void *self);
void* rnd(const void *class, void *self, ...);
void* vrnd(const void *class, void *self, va_list *ap);
void* absolute(const void *self);

int   cmp_to_zero(const void *self);
void  set_to_zero(void *self);
void  set_to_one(void *self);

#endif /* end of include guard: TYPE_H_PSBYWVTZ */
