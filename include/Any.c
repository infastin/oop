#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "Any.h"
#include "ExceptionObject.h"
#include "Selectors.h"
#include "Exception.h"

static void* Any_ctor(void *_self, va_list *ap)
{
	struct Any *self = super_ctor(Any(), _self, ap);

	self->size = va_arg(*ap, size_t);

	if (self->size == 0)
	{
		delete(self);
		throw(AnyException(), "Error: Can't create any object with zero size!");
	}

	char *data = va_arg(*ap, char*);
	self->data = (char*)calloc(sizeof(char), self->size);

	if (self->data == NULL)
	{
		delete(self);
		throw(AnyException(), "Fatal Error: Any object allocation error!");
	}

	if (data)
		memcpy(self->data, data, self->size);

	return self;
}

static void* Any_cpy(const void *_self, void *_object)
{
	const struct Any *self = _self;
	struct Any *object = super_cpy(Any(), _self, _object);

	object->size = self->size;
	object->data = (char*)calloc(sizeof(char), object->size);

	if (object->data == NULL)
	{
		delete(object);
		throw(AnyException(), "Fatal Error: Any object allocation error!");
	}

	if (self->data)
		memcpy(object->data, self->data, object->size);

	return object;
}

static void* Any_dtor(void *_self)
{
	struct Any *self = cast(Any(), _self);
	
	free(self->data);

	return super_dtor(Any(), self);
}

static void Any_set(void *_self, va_list *ap)
{
	struct Any *self = cast(Any(), _self);

	void *data = va_arg(*ap, void*);

	if (data)
		memcpy(self->data, data, self->size);
}

static void Any_get(void *_self, va_list *ap)
{
	struct Any *self = cast(Any(), _self);

	void *retval = va_arg(*ap, void*);
	memcpy(retval, self->data, self->size);
}

/*
 * Initialization
 */

// ---

ClassImpl(Any)
{
	if (!_Any)
	{
		_Any = new(Class(), "Any",
				Object(), sizeof(struct Any),
				ctor, Any_ctor,
				dtor, Any_dtor,
				cpy, Any_cpy,
				set, Any_set,
				get, Any_get,
				0);
	}

	return _Any;
}

/*
 * Exception Initialization
 */

ObjectImpl(AnyException)
{
	if (!_AnyException)
	{
		_AnyException = new(ExceptionObject(), "AnyException");
	}

	return _AnyException;
}
