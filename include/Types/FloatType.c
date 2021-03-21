#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static char* FloatType_stringer(const void *_self, va_list *ap)
{
	struct FloatType *self = cast(Float(), _self);

	// Declarations
	char *result;
	int flag, width, precision;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	flag = va_arg(ap_copy, int);
	width = va_arg(ap_copy, int);
	precision = va_arg(ap_copy, int);

	// Getting format size
	size_t fmt_size = 3;

	if (flag != -1)
		fmt_size++;

	if (width != -1)
		fmt_size += snprintf(NULL, 0, "%d", width);

	if (precision != -1)
		fmt_size += snprintf(NULL, 0, "%d", precision);

	// Getting format
	char *fmt = (char*)calloc(sizeof(char), fmt_size + 1);
	*fmt = '%';

	char *p = fmt + 1;
	size_t psize = fmt_size - 1;

	if (flag != -1)
	{
		*p++ = flag;
		psize--;
	}

	if (width != -1)
	{
		int widthN = snprintf(p, psize + 1, "%d", width);
		p += widthN;
		psize -= widthN;
	}

	if (precision != -1)
	{
		int precisionN = snprintf(p, psize + 1, ".%d", precision);
		p += precisionN;
		psize -= precisionN;
	}

	*p++ = 'l';
	*p++ = 'f';
	*p = 0;

	// Getting result
	size_t size = snprintf(NULL, 0, fmt, self->value);
	result = (char*)calloc(sizeof(char), size + 1);
	snprintf(result, size + 1, fmt, self->value);	

	free(fmt);

	va_end(ap_copy);
	return result;
}

static void* FloatType_sum(void *_self, void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value + B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void* FloatType_subtract(void *_self, void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value - B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void* FloatType_product(void *_self, void *b)
{
	const struct FloatType *self = cast(Float(), _self);
	const struct FloatType *B = cast(Float(), b);

	double result_value = self->value * B->value;
	void *result = new(Float(), result_value);

	return result;
}

static void* FloatType_divide(void *_self, void *b)
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
				stringer, FloatType_stringer,
				sum, FloatType_sum,
				subtract, FloatType_subtract,
				product, FloatType_product,
				divide, FloatType_divide,
				scadd, FloatType_scadd,
				scsub, FloatType_scsub,
				scmulti, FloatType_scmulti,
				scdivide, FloatType_scdivide,
				0);
	}

	return _Float;
}
