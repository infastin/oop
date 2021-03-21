#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Selectors.h"
#include "IntType.h"

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

static char* IntType_stringer(const void *_self, va_list *ap)
{
	struct IntType *self = cast(Int(), _self);

	// Declarations
	char *result;
	int flag, width, precision;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	flag = va_arg(ap_copy, int);
	width = va_arg(ap_copy, int);
	precision = va_arg(ap_copy, int);

	// Getting format size
	size_t fmt_size = 2;

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

	*p++ = 'd';
	*p = 0;

	// Getting result
	size_t size = snprintf(NULL, 0, fmt, self->value);
	result = (char*)calloc(sizeof(char), size + 1);
	snprintf(result, size + 1, fmt, self->value);

	free(fmt);

	va_end(ap_copy);
	return result;
}

static int IntType_reader(const char *str, va_list *ap)
{
	// Declarations
	var *result;
	int number, count;
	int ignore, width;
	
	FILE *stream = NULL;

	va_list ap_copy;
	va_copy(ap_copy, *ap);
	
	if (str == NULL)
		stream = va_arg(ap_copy, FILE*);

	ignore = va_arg(ap_copy, int);
	width = va_arg(ap_copy, int);
	result = va_arg(ap_copy, var*);

	// Getting format size
	size_t fmt_size = 2;

	if (ignore != -1)
		fmt_size++;

	if (width != -1)
		fmt_size += snprintf(NULL, 0, "%d", width);

	// Getting format
	char *fmt = (char*)calloc(sizeof(char), fmt_size + 1);
	*fmt = '%';
	
	char *p = fmt + 1;
	size_t psize = fmt_size - 1;

	if (ignore != -1)
	{
		*p++ = '*';
		psize--;
	}

	if (width != -1)
	{
		int widthN = snprintf(p, psize + 1, "%d", width);
		p += widthN;
		psize -= widthN;
	}

	*p++ = 'd';
	*p = 0;

	// Getting result
	if (stream != NULL)
		count = fscanf(stream, fmt, &number);
	else
		count = sscanf(str, fmt, &number);

	if (count != 0)
		*result = new(Int(), number);

	free(fmt);
	va_end(ap_copy);

	return count;
}

static void* IntType_sum(void *_self, void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value + B->value;
	void *result = new(Int(), result_value);

	return result;
}

static void* IntType_subtract(void *_self, void *b)
{
	const struct IntType *self = cast(Int(), _self);
	const struct IntType *B = cast(Int(), b);

	int result_value = self->value - B->value;
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

static void* IntType_divide(void *_self, void *b)
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
				stringer, IntType_stringer,
				reader, IntType_reader,
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
