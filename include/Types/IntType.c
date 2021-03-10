#include <stdarg.h>
#include <stdio.h>

#include "TypeClass.h"
#include "Selectors.h"
#include "IntType.h"

/*
 * Methods
 */

static void* IntType_ctor(void *_self, va_list *props)
{
	struct IntType *self = super_ctor(Int, _self, props);

	self->value = va_arg(*props, int);

	return self;
}

static int IntType_cmp(const void *_self, const void *b)
{
	const struct IntType *self = _self;
	const struct IntType *B = b;

	return self->value - B->value;
}

static void IntType_swap(void *_self, void *b)
{
	struct IntType *self = _self;
	struct IntType *B = b;

	int tmp = self->value;
	self->value = B->value;
	B->value = tmp;
}

static void IntType_set(void *_self, va_list *props)
{
	struct IntType *self = _self;
	self->value = va_arg(*props, int);
}

static void* IntType_get_vptr(void *_self)
{
	struct IntType *self = _self;
	return &self->value;
}

/*
 * Initialization
 */

const void* Int;

void initIntType()
{
	if (!Int)
	{
		initTypeClass();

		Int = new(TypeClass, "Int",
				Object, sizeof(struct IntType),
				ctor, IntType_ctor,
				cmp, IntType_cmp,
				swap, IntType_swap,
				set, IntType_set,
				get_vptr, IntType_get_vptr,
				0);
	}
}
