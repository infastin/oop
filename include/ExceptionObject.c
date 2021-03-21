#include <stdarg.h>
#include <stdlib.h>

#include "ExceptionObject.h"
#include "Selectors.h"

/*
 * ExceptionObject
 */

static void* ExceptionObject_ctor(void *_self, va_list *ap)
{
	struct ExceptionObject *self = super_ctor(ExceptionObject(), _self, ap);

	self->name = va_arg(*ap, char*);
	self->msg = NULL;

	return self;
}

static void* ExceptionObject_dtor(void *_self)
{
	struct ExceptionObject *self = cast(ExceptionObject(), _self);

	if (self->msg)
		free(self->msg);

	return super_dtor(ExceptionObject(), _self);
}

/*
 * Selectors
 */
char* exception_message(const void *_self) 
{
	const struct ExceptionObject *self = cast(ExceptionObject(), _self);

	return self->msg;
}

char* exception_name(const void *_self)
{
	const struct ExceptionObject *self = cast(ExceptionObject(), _self);
	
	return self->name;
}

/*
 *	Initialization
 */

ClassImpl(ExceptionObject)
{
	if (!_ExceptionObject)
	{
		_ExceptionObject = new(Class(), "ExceptionObject",
				Object(), sizeof(struct ExceptionObject),
				ctor, ExceptionObject_ctor,
				dtor, ExceptionObject_dtor,
				0);
	}

	return _ExceptionObject;
}
