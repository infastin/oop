#include <assert.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>

#include "Object.h"
#include "Selectors.h"

/*
 * Create and delete selectors
 */

void* new(const void *_class, ...)
{
	const struct Class *class = _class;
	struct Object *object;

	assert(class && class->size);
	object = (struct Object*)calloc(1, class->size);

	assert(object);
	object->class = class;
	atomic_init((atomic_size_t*) &object->ref_count, 1);

	va_list props;

	va_start(props, _class);
	object = ctor(object, &props);
	va_end(props);
	
	return object;
}

void delete(void *_self)
{
	if (_self)
	{
		free(dtor(_self));
		_self = NULL;
	}
}

/*
 * Base selectors
 */

void* dtor(void *_self)
{
	const struct Class *class = classOf(_self);

	assert(class->dtor);
	return class->dtor(_self);
}

void* ctor(void *_self, va_list *props)
{
	const struct Class *class = classOf(_self);

	assert(class->ctor);
	return class->ctor(_self, props);
}

/*
 * Super's base selectors
 */

const void* super(const void *_self)
{
	const struct Class *self = _self;

	assert(self->super);
	return self->super;
}

void* super_ctor(const void *_class, void *_self, va_list *props)
{
	const struct Class *superclass = super(_class);

	assert(superclass->ctor);
	return superclass->ctor(_self, props);
}

void* super_dtor(const void *_class, void *_self)
{
	const struct Class *superclass = super(_class);

	assert(superclass->dtor);
	return superclass->dtor(_self);
}
