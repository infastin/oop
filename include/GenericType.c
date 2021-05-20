/* vim: set fdm=marker : */

#include <stdarg.h>
#include <stdlib.h>

#include "GenericType.h"
#include "ExceptionObject.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"
#include "Exception.h"

/* GenericType {{{ */

/* Private methods {{{ */

static void GenericType_set_value(struct GenericType *self, va_list *ap)
{
	switch (self->type) {
		case GT_SHORT:
			self->value.gt_short = (short) va_arg(*ap, int);
			break;
		case GT_USHORT:
			self->value.gt_ushort = (unsigned short) va_arg(*ap, unsigned int);
			break;
		case GT_INT:
			self->value.gt_int = va_arg(*ap, int);
			break;
		case GT_UINT:
			self->value.gt_uint = va_arg(*ap, unsigned int);
			break;
		case GT_LONG:
			self->value.gt_long = va_arg(*ap, long);
			break;
		case GT_ULONG:
			self->value.gt_ulong = va_arg(*ap, unsigned long);
			break;
		case GT_LLONG:
			self->value.gt_llong = va_arg(*ap, long long);
			break;
		case GT_ULLONG:
			self->value.gt_ullong = va_arg(*ap, unsigned long long);
			break;
		case GT_FLOAT:
			self->value.gt_float = (float) va_arg(*ap, double);
			break;
		case GT_DOUBLE:
			self->value.gt_double = va_arg(*ap, double);
			break;
		case GT_LDOUBLE:
			self->value.gt_ldouble = va_arg(*ap, long double);
			break;
		case GT_POINTER:
			self->value.gt_pointer = va_arg(*ap, void*);
			break;
	}
}

/* }}} */

static void* GenericType_ctor(void *_self, va_list *ap)
{
	struct GenericType *self = super_ctor(GenericType(), _self, ap);

	int type = va_arg(*ap, int);

	if (type < GT_SHORT || type > GT_POINTER)
	{
		delete(self);
		throw(GenericTypeException(), "Error: Wrong type!");
	}

	self->type = type;
	
	GenericType_set_value(self, ap);

	return self;
}

static void* GenericType_cpy(const void *_self, void *_object)
{
	const struct GenericType *self = cast(GenericType(), _self);
	struct GenericType *object = super_cpy(GenericType(), _self, _object);

	object->type = self->type;
	object->value = self->value;

	return object;
}

static void GenericType_set(void *_self, va_list *ap)
{
	struct GenericType *self = cast(GenericType(), _self);

	GenericType_set_value(self, ap);
}

static void GenericType_get(const void *_self, va_list *ap)
{
	const struct GenericType *self = cast(GenericType(), _self);

	if (self->type == GT_SHORT)
	{
		short *retval = va_arg(*ap, short*);
		*retval = self->value.gt_short;
	}
	else if (self->type == GT_USHORT)
	{
		unsigned short *retval = va_arg(*ap, unsigned short*);
		*retval = self->value.gt_ushort;
	}
	else if (self->type == GT_INT)
	{
		int *retval = va_arg(*ap, int*);
		*retval = self->value.gt_int;
	}
	else if (self->type == GT_UINT)
	{
		unsigned int *retval = va_arg(*ap, unsigned int*);
		*retval = self->value.gt_uint;
	}
	else if (self->type == GT_LONG)
	{
		long *retval = va_arg(*ap, long*);
		*retval = self->value.gt_long;
	}
	else if (self->type == GT_ULONG)
	{
		unsigned long *retval = va_arg(*ap, unsigned long*);
		*retval = self->value.gt_ulong;
	}
	else if (self->type == GT_LLONG)
	{
		long long *retval = va_arg(*ap, long long*);
		*retval = self->value.gt_llong;
	}
	else if (self->type == GT_ULLONG)
	{
		unsigned long long *retval = va_arg(*ap, unsigned long long*);
		*retval = self->value.gt_ullong;
	}
	else if (self->type == GT_FLOAT)
	{
		float *retval = (float*) va_arg(*ap, float*);
		*retval = self->value.gt_float;
	}
	else if (self->type == GT_DOUBLE)
	{
		double *retval = va_arg(*ap, double*);
		*retval = self->value.gt_double;
	}
	else if (self->type == GT_LDOUBLE)
	{
		long double *retval = va_arg(*ap, long double*);
		*retval = self->value.gt_ldouble;
	}
	else if (self->type == GT_POINTER)
	{
		void *retval = va_arg(*ap, void*);
		retval = self->value.gt_pointer;
	}
}

/* }}} */

/* Initialization {{{ */

ClassImpl(GenericType)
{
	if (!_GenericType)
	{
		_GenericType = new(Class(), "GenericType",
				Object(), sizeof(struct GenericType),
				ctor, GenericType_ctor,
				cpy, GenericType_cpy,
				set, GenericType_set,
				get, GenericType_get,
				0);
	}

	return _GenericType;
}

/* Exception init */

ObjectImpl(GenericTypeException)
{
	if (!_GenericTypeException)
	{
		_GenericTypeException = new(ExceptionObject(), "GenericTypeException");
	}

	return _GenericTypeException;
}

/* }}} */
