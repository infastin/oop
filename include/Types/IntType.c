#include <stdarg.h>
#include <stdio.h>

#include "Object.h"
#include "TypeClass.h"
#include "Selectors.h"
#include "IntType.h"

/*
 * Methods
 */

static void* IntType_ctor(void *_self, va_list *props)
{
	struct IntType *self = super_ctor(Int(), _self, props);

	self->value = va_arg(*props, int);

	return self;
}

static int IntType_cmp(const void *_self, const void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	return self->value - B->value;
}

static void IntType_swap(void *_self, void *b)
{
	struct IntType *self = cast(Int(), _self);
	struct IntType *B = cast(Int(), b);

	int tmp = self->value;
	self->value = B->value;
	B->value = tmp;
}

static void IntType_set(void *_self, va_list *props)
{
	struct IntType *self = cast(Int(), _self);
	self->value = va_arg(*props, int);
}

static void* IntType_get_vptr(void *_self)
{
	struct IntType *self = cast(Int(), _self);
	return &self->value;
}

static void* IntType_sum(void *_self, void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value + B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void* IntType_product(void *_self, void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value * B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void IntType_scadd(void *_self, va_list *props)
{
	struct IntType *self = cast(Int(), _self);
	
	int sc = va_arg(*props, int);
	self->value += sc;
}

static void IntType_scmulti(void *_self, va_list *props)
{
	struct IntType *self = cast(Int(), _self);

	int sc = va_arg(*props, int);
	self->value *= sc;
}

/*
 * Initialization
 */

ClassImpl(Int)
{
	if (!_Int)
	{
		_Int = new(TypeClass(), "Int",
				Object(), sizeof(struct IntType),
				ctor, IntType_ctor,
				set, IntType_set,
				get_vptr, IntType_get_vptr,
				cmp, IntType_cmp,
				swap, IntType_swap,
				sum, IntType_sum,
				product, IntType_product,
				scadd, IntType_scadd,
				scmulti, IntType_scmulti,
				0);
	}

	return _Int;
}
