#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Selectors.h"
#include "IntType.h"
#include "Exception.h"

/*
 * Methods
 */

static void* IntType_ctor(void *_self, va_list *ap)
{
	struct IntType *self = super_ctor(Int(), _self, ap);

	self->value = va_arg(*ap, int);

	return self;
}

static void* IntType_cpy(const void *_self, void *_object)
{
	const struct IntType *self = _self;
	struct IntType *obj = super_cpy(Int(), _self, _object);

	obj->value = self->value;
	return obj;
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

static void IntType_set(void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);
	self->value = va_arg(*ap, int);
}

static void IntType_get(void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);

	int *val = va_arg(*ap, int*);
	*val = self->value;
}

static int IntType_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct IntType *self = cast(Int(), _self);	

	char *fmt = __getFmtPrint(flag, width, precision, "d");

	if (fmt) {
		// Getting result
		int size;
		if (stream != NULL)
		{
			if (bin)
			{
				size = fwrite(&self->value, sizeof(self->value), 1, stream);

				if (size != 1)
					size = -1;
			}
			else
				size = fprintf(stream, fmt, self->value);
		}
		else if (buffer != NULL)
			size = snprintf(buffer, maxn, fmt, self->value);
		else
			size = snprintf(NULL, 0, fmt, self->value);

		free(fmt);
		return size;
	}
	else
	{
		throw(FormatException(), "Error: could not allocate memory for the format string!");
	}

	return -1;  
}

static int IntType_sfscan(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	struct IntType *self = cast(Int(), _self);

	char *fmt = __getFmtScan(asterisk, width, "d");

	if (fmt)
	{
		int size;

		if (stream != NULL)
		{
			if (bin)
			{
				size = fread(&self->value, sizeof(self->value), 1, stream);
				
				if (numb != NULL)
					*numb = size;
			}
			else
			{
				int n;
				size = fscanf(stream, fmt, &self->value, &n);

				if (numb != NULL)
					*numb = n;
			}
		}
		else if (buffer != NULL)
		{
			int n;
			size = sscanf(buffer, fmt, &self->value, &n);

			if (numb != NULL)
				*numb = n;
		}
		else
			size = 0;

		free(fmt);
		return size;
	}
	else
	{
		throw(FormatException(), "Error: could not allocate memory for the format string!");
	}

	return -1;
}

static void* IntType_sum(const void *_self, const void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value + B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void* IntType_subtract(const void *_self, const void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value - B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void* IntType_product(const void *_self, const void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value * B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void* IntType_divide(const void *_self, const void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value / B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void IntType_scadd(void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);
	
	int sc = va_arg(*ap, int);
	self->value += sc;
}

static void IntType_scsub(void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);

	int sc = va_arg(*ap, int);
	self->value -= sc;
}

static void IntType_scmulti(void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);

	int sc = va_arg(*ap, int);
	self->value *= sc;
}

static void IntType_scdivide(void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);

	int sc = va_arg(*ap, int);
	self->value /= sc;
}

static void* IntType_inverse_add(void *_self)
{
	struct IntType *obj = cast(Int(), copy(_self));
	scmulti(obj, (int) - 1);

	return obj;
}

static void* IntType_rnd(void *_self, va_list *ap)
{
	struct IntType *self;

	if (_self)
		self = cast(Int(), _self);
	else
		self = new(Int(), 0);

	unsigned int max = va_arg(*ap, unsigned int);
	int negative = va_arg(*ap, int);
	
	int random = rand();

	if (negative == 1)
		random -= (RAND_MAX / 2);

	if (max)
		random = random % 10;

	self->value = random;
	return self;
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
				cpy, IntType_cpy,
				set, IntType_set,
				get, IntType_get,
				cmp, IntType_cmp,
				swap, IntType_swap,
				sfprint, IntType_sfprint,
				sfscan, IntType_sfscan,
				sum, IntType_sum,
				subtract, IntType_subtract,
				product, IntType_product,
				divide, IntType_divide,
				scadd, IntType_scadd,
				scsub, IntType_scsub,
				scmulti, IntType_scmulti,
				scdivide, IntType_scdivide,
				inverse_add, IntType_inverse_add,
				rnd, IntType_rnd,
				0);
	}

	return _Int;
}
