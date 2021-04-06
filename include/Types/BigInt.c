#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "BigInt.h"
#include "Selectors.h"
#include "Object.h"

static void* BigInt_ctor(void *_self, va_list *ap)
{
	struct BigInt *self = super_ctor(BigInt(), _self, ap);

	self->sign = 0;

	return self;
}

static void* BigInt_sum(const void *_self, const void *b)
{
	const struct BigInt *self = cast(BigInt(), _self);
	const struct BigInt *B = cast(BigInt(), b);

	const struct Bitset *ss = _self;
	const struct Bitset *bs = b;

	void *result;

	if (self->sign == B->sign)
	{
		const struct Bitset *higher;
		const struct Bitset *lower;

		if (ss->capacity > bs->capacity)
		{
			higher = ss;
			lower = bs;
		}
		else
		{
			higher = bs;
			lower = ss;
		}


	}
	else if (self->sign == 0 && B->sign == 1)
	{

	}
	else if (self->sign == 1 && B->sign == 0)
	{

	}
}
