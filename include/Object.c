#include <assert.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "Object.h"
#include "Selectors.h"

/*
 * Object
 */

static void* Object_ctor(void *_self, va_list *props)
{
	return _self;
}

static void* Object_dtor(void *_self)
{
	return _self;
}

void Object_method(void *_self, voidf replace, voidf method)
{
	return;
}

const void* classOf(const void *_self)
{
	const struct Object *self = _self;

	return self->class;
}

size_t sizeOf(const void *_self)
{
	const struct Class *class = classOf(_self);

	return class->size;
}

/*
 * Class
 */

static void* Class_ctor(void *_self, va_list *props)
{
	struct Class *self = _self;
	const size_t offset = offsetof(struct Class, ctor);

	self->name = va_arg(*props, char*);
	self->super = va_arg(*props, struct Class*);
	self->size = va_arg(*props, size_t);

	assert(self->super);

	memcpy((char*) self + offset, 
			(char*) self->super + offset, 
			sizeOf(self->super) - offset);

	voidf selector;

	while ((selector = va_arg(*props, voidf)))
	{	
		voidf method = va_arg(*props, voidf);

		if (selector == (voidf) ctor)
			self->ctor = (ctor_f) method;
		else if (selector == (voidf) dtor)
			self->dtor = (dtor_f) method;
	}

	return self;
}

static void* Class_dtor(void *_self)
{
	struct Class *self = _self;
	fprintf(stderr, "%s: cannot destroy class\n", self->name);
	return 0;
}


void Class_method(void *_self, voidf _replace, voidf _method)
{
	struct Class *self = _self;

	if (_replace == (voidf) ctor)
		self->ctor = (ctor_f) _method;
	if (_replace == (voidf) dtor)
		self->dtor = (dtor_f) _method;
	if (_replace == (voidf) method)
		self->method = (method_f) _method;
}

/*
 *	Initialization
 */

static const struct Class object[] = {
	{ 
		{ object + 1 },
		"Object", object, sizeof(struct Object),
		Object_ctor, Object_dtor, Object_method
	},
	{ 
		{ object + 1 },
		"Class", object, sizeof(struct Class),
		Class_ctor, Class_dtor, Class_method
	}
};

const void *Object = object;
const void *Class = object + 1;
