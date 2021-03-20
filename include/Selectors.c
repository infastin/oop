#include <assert.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>

#include "Exception.h"
#include "Object.h"
#include "Selectors.h"

/*
 * Create and delete selectors
 */

void* new(const void *_class, ...)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	assert(class && class->size);
	object = (struct Object*)calloc(1, class->size);

	assert(object);
	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	va_list ap;

	va_start(ap, _class);
	object = ctor(object, &ap);
	va_end(ap);
	
	return object;
}

void* vnew(const void *_class, va_list *ap)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	assert(class && class->size);
	object = (struct Object*)calloc(1, class->size);

	assert(object);
	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	object = ctor(object, ap);

	return object;
}

void delete(void *_self)
{
	if (_self)
		free(dtor(_self));
}


void* copy(const void *_self)
{
	const struct Object *self = cast(Object(), _self);
	const struct Class *class = self->class;
	struct Object *object;

	assert(class && class->size);
	object = (struct Object*)calloc(1, class->size);

	assert(object);
	object->magic = MAGIC_NUM;
	object->class = class;
	unsigned int ref_count = atomic_load((atomic_uint *) &self->ref_count);
	atomic_init((atomic_uint*) &object->ref_count, ref_count);

	return cpy(_self, object);
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

void* ctor(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	assert(class->ctor);
	return class->ctor(_self, ap);
}

void set(void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->set == NULL)
		throw(ObjectException(), "Set Error: Object '%s' doesn't have 'set' method!",
				class->name);

	va_list ap;

	va_start(ap, _self);
	class->set(_self, &ap);
	va_end(ap);
}

void vset(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->set == NULL)
		throw(ObjectException(), "Set Error: Object '%s' doesn't have 'set' method!",
				class->name);

	class->set(_self, ap);
}

// Get type's value pointer
void get(const void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->get == NULL)
		throw(ObjectException(), "Set Error: Object '%s' doesn't have 'get' method!",
				class->name);
	
	va_list ap;
	
	va_start(ap, _self);
	class->get(_self, &ap);
	va_end(ap);
}

void vget(const void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->get == NULL)
		throw(ObjectException(), "Set Error: Object '%s' doesn't have 'get' method!",
				class->name);

	class->get(_self, ap);
}

void* cpy(const void *_self, void *object)
{
	const struct Class *class = classOf(_self);

	assert(class->cpy);
	return class->cpy(_self, object);
}

/*
 * Super's base selectors
 */

const void* super(const void *_self)
{
	const struct Class *self = cast(Class(), _self);

	assert(self->super);
	return self->super;
}

void* super_ctor(const void *_class, void *_self, va_list *ap)
{
	const struct Class *superclass = super(_class);

	assert(superclass->ctor);
	return superclass->ctor(_self, ap);
}

void* super_dtor(const void *_class, void *_self)
{
	const struct Class *superclass = super(_class);

	assert(superclass->dtor);
	return superclass->dtor(_self);
}


void* super_cpy(const void *_class, const void *_self, void *object)
{
	const struct Class *superclass = super(_class);

	assert(superclass->cpy);
	return superclass->cpy(_self, object);
}

