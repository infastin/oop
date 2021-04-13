#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "Exception.h"
#include "Macro.h"
#include "Selectors.h"
#include "FloatTemplate.h"

/*
 * Methods
 */

static void* TEMPLATE(N, ctor)(void *_self, va_list *ap)
{
	struct N *self = super_ctor(N(), _self, ap);

#if defined (FLOAT_FLOAT)
	self->value = va_arg(*ap, double);
#else
	self->value = va_arg(*ap, T);
#endif

	return self;
}

static void* TEMPLATE(N, cpy)(const void *_self, void *_object)
{
	const struct N *self = _self;
	struct N *obj = super_cpy(N(), _self, _object);

	obj->value = self->value;
	return obj;
}

static int TEMPLATE(N, cmp)(const void *_self, const void *b)
{
	const struct N *self = cast(N(), _self);
	const struct N *B = cast(N(), b);

	if (self->value > B->value)
		return 1;
	else if (self->value < B->value)
		return -1;
	else
		return 0;
}

static void TEMPLATE(N, swap)(void *_self, void *b)
{
	struct N *self = cast(N(), _self);
	struct N *B = cast(N(), b);

	T tmp = self->value;
	self->value = B->value;
	B->value = tmp;
}

static void TEMPLATE(N, set)(void *_self, va_list *ap)
{
	struct N *self = cast(N(), _self);

#if defined (FLOAT_FLOAT)
	self->value = va_arg(*ap, double);
#else
	self->value = va_arg(*ap, T);
#endif
}

static void TEMPLATE(N, get)(void *_self, va_list *ap)
{
	struct N *self = cast(N(), _self);

	T *val = va_arg(*ap, T*);
	*val = self->value;
}

static int TEMPLATE(N, sfprint)(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct N *self = cast(N(), _self);	

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

static int TEMPLATE(N, sfscan)(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	struct N *self = cast(N(), _self);

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

static void* TEMPLATE(N, sum)(const void *_self, const void *b)
{
	const struct N *self = cast(N(), _self);
	const struct N *B = cast(N(), b);

	double result_value = self->value + B->value;
	struct N *result = new(N(), result_value);

	return result;
}

static void* TEMPLATE(N, subtract)(const void *_self, const void *b)
{
	const struct N *self = cast(N(), _self);
	const struct N *B = cast(N(), b);

	double result_value = self->value - B->value;
	struct N *result = new(N(), result_value);

	return result;
}

static void* TEMPLATE(N, product)(const void *_self, const void *b)
{
	const struct N *self = cast(N(), _self);
	const struct N *B = cast(N(), b);

	double result_value = self->value * B->value;
	struct N *result = new(N(), result_value);

	return result;
}

static void* TEMPLATE(N, divide)(const void *_self, const void *b)
{
	const struct N *self = cast(N(), _self);
	const struct N *B = cast(N(), b);

	double result_value = self->value / B->value;
	struct N *result = new(N(), result_value);

	return result;
}

static void TEMPLATE(N, scadd)(void *_self, va_list *ap)
{
	struct N *self = cast(N(), _self);

#if defined (FLOAT_FLOAT)
	T sc = va_arg(*ap, double);
#else
	T sc = va_arg(*ap, T);
#endif

	self->value += sc;
}

static void TEMPLATE(N, scsub)(void *_self, va_list *ap)
{
	struct N *self = cast(N(), _self);

#if defined (FLOAT_FLOAT)
	T sc = va_arg(*ap, double);
#else
	T sc = va_arg(*ap, T);
#endif	

	self->value -= sc;
}

static void TEMPLATE(N, scmulti)(void *_self, va_list *ap)
{
	struct N *self = cast(N(), _self);

#if defined (FLOAT_FLOAT)
	T sc = va_arg(*ap, double);
#else
	T sc = va_arg(*ap, T);
#endif	

	self->value *= sc;
}

static void TEMPLATE(N, scdivide)(void *_self, va_list *ap)
{
	struct N *self = cast(N(), _self);

#if defined (FLOAT_FLOAT)
	T sc = va_arg(*ap, double);
#else
	T sc = va_arg(*ap, T);
#endif	

	self->value /= sc;
}

static void* TEMPLATE(N, inverse_add)(void *_self)
{
	struct N *obj = cast(N(), copy(_self));
	scmulti(obj, (T) -1);

	return obj;
}

static void* TEMPLATE(N, inverse_multi)(void *_self)
{
	struct N *obj = cast(N(), copy(_self));

	double val;
	get(obj, &val);
	val = 1 / val;
	set(obj, val);

	return obj;
}

static void* TEMPLATE(N, rnd)(void *_self, va_list *ap)
{
	struct N *self;

	if (_self)
		self = cast(N(), _self);
	else
		self = new(N(), (double) 0.00);

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

ClassImpl(N)
{
	if (!PRIVATE(N))
	{
		PRIVATE(N) = new(TypeClass(), TOSTR(N),
				Object(), sizeof(struct N),
				ctor, TEMPLATE(N, ctor),
				cpy, TEMPLATE(N, cpy),
				set, TEMPLATE(N, set),
				get, TEMPLATE(N, get),
				cmp, TEMPLATE(N, cmp),
				swap, TEMPLATE(N, swap),
				sfprint, TEMPLATE(N, sfprint),
				sfscan, TEMPLATE(N, sfscan),
				sum, TEMPLATE(N, sum),
				subtract, TEMPLATE(N, subtract),
				product, TEMPLATE(N, product),
				divide, TEMPLATE(N, divide),
				scadd, TEMPLATE(N, scadd),
				scsub, TEMPLATE(N, scsub),
				scmulti, TEMPLATE(N, scmulti),
				scdivide, TEMPLATE(N, scdivide),
				inverse_add, TEMPLATE(N, inverse_add),
				inverse_multi, TEMPLATE(N, inverse_multi),
				rnd, TEMPLATE(N, rnd),
				0);
	}

	return PRIVATE(N);
}

