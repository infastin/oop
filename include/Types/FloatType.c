#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "Exception.h"
#include "Selectors.h"
#include "FloatType.h"

/*
 * Methods
 */

static void* FloatType_ctor(void *_self, va_list *ap)
{
	struct FloatType *self = super_ctor(Float(), _self, ap);

	self->value = va_arg(*ap, double);

	return self;
}

static void* FloatType_cpy(const void *_self, void *_object)
{
	const struct FloatType *self = _self;
	struct FloatType *obj = super_cpy(Float(), _self, _object);

	obj->value = self->value;
	return obj;
}

static int FloatType_cmp(const void *_self, const void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	if (self->value > B->value)
		return 1;
	else if (self->value < B->value)
		return -1;
	else
		return 0;
}

static void FloatType_swap(void *_self, void *b)
{
	struct FloatType *self = cast(Float(), _self);
	struct FloatType *B = cast(Float(), b);

	double tmp = self->value;
	self->value = B->value;
	B->value = tmp;
}

static void FloatType_set(void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);
	self->value = va_arg(*ap, double);
}

static void FloatType_get(void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);

	double *val = va_arg(*ap, double*);
	*val = self->value;
}

static int FloatType_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct FloatType *self = cast(Float(), _self);	

	char *fmt = __getFmtPrint(flag, width, precision, "lf");

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

static int FloatType_sfscan(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	struct FloatType *self = cast(Float(), _self);

	char *fmt = __getFmtScan(asterisk, width, "lf");

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

static void* FloatType_sum(const void *_self, const void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value + B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void* FloatType_subtract(const void *_self, const void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value - B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void* FloatType_product(const void *_self, const void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value * B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void* FloatType_divide(const void *_self, const void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value / B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void FloatType_scadd(void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);
	
	double sc = va_arg(*ap, double);
	self->value += sc;
}

static void FloatType_scsub(void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);

	double sc = va_arg(*ap, double);
	self->value -= sc;
}

static void FloatType_scmulti(void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);

	double sc = va_arg(*ap, double);
	self->value *= sc;
}

static void FloatType_scdivide(void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);

	double sc = va_arg(*ap, double);
	self->value /= sc;
}

static void* FloatType_inverse_add(void *_self)
{
	struct FloatType *obj = cast(Float(), copy(_self));
	scmulti(obj, (double) -1.00);

	return obj;
}

static void* FloatType_inverse_multi(void *_self)
{
	struct FloatType *obj = cast(Float(), copy(_self));
	
	double val;
	get(obj, &val);
	val = 1 / val;
	set(obj, val);

	return obj;
}

static void* FloatType_rnd(void *_self, va_list *ap)
{
	struct FloatType *self;

	if (_self)
		self = cast(Float(), _self);
	else
		self = new(Float(), (double) 0.00);

	unsigned int max = va_arg(*ap, unsigned int);
	int negative = va_arg(*ap, int);

	double random = ((double) rand() / (double) RAND_MAX);

	if (negative == 1)
		random -= ((double) rand() / (double) RAND_MAX);

	if (max == 0)
		random *= rand();
	else
		random *= max;

	if (random == (double) max)
		random *= ((double) rand() / (double) RAND_MAX);

	self->value = random;

	return self;
}


/*
 * Initialization
 */

ClassImpl(Float)
{
	if (!_Float)
	{
		_Float = new(TypeClass(), "Float",
				Object(), sizeof(struct FloatType),
				ctor, FloatType_ctor,
				cpy, FloatType_cpy,
				set, FloatType_set,
				get, FloatType_get,
				cmp, FloatType_cmp,
				swap, FloatType_swap,
				sfprint, FloatType_sfprint,
				sfscan, FloatType_sfscan,
				sum, FloatType_sum,
				subtract, FloatType_subtract,
				product, FloatType_product,
				divide, FloatType_divide,
				scadd, FloatType_scadd,
				scsub, FloatType_scsub,
				scmulti, FloatType_scmulti,
				scdivide, FloatType_scdivide,
				inverse_add, FloatType_inverse_add,
				inverse_multi, FloatType_inverse_multi,
				rnd, FloatType_rnd,
				0);
	}

	return _Float;
}
