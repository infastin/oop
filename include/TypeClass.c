#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "Object.h"
#include "TypeClass.h"
#include "Selectors.h"


/*
 * Methods
 */

static void* TypeClass_ctor(void *_self, va_list *props)
{
	struct TypeClass *self = super_ctor(TypeClass, _self, props);

	voidf selector;
	va_list ap;
	va_copy(ap, *props);

	while ((selector = va_arg(ap, voidf)))
	{
		voidf method = va_arg(ap, voidf);

		if (selector == (voidf) cmp)
			self->cmp = (cmp_f) method;
		else if (selector == (voidf) swap)
			self->swap = (swap_f) method;
		else if (selector == (voidf) set)
			self->set = (set_f) method;
		else if (selector == (voidf) get_vptr)
			self->get_vptr = (get_vptr_f) method;
	}

	return self;
}

/*
 * New selectors for Types
 */

int cmp(const void *_self, const void *b)
{
	const struct TypeClass *class = classOf(_self);

	assert(class->cmp);
	return class->cmp(_self, b);
}

int super_cmp(const void *_class, const void *_self, const void *b)
{
	const struct TypeClass *superclass = super(_class);

	assert(superclass->cmp);
	return superclass->cmp(_self, b);
}

void swap(void *_self, void *b)
{
	const struct TypeClass *class = classOf(_self);

	assert(class->swap);
	class->swap(_self, b);
}

void super_swap(const void *_class, void *_self, void *b)
{
	const struct TypeClass *superclass = super(_class);

	assert(superclass->swap);
	superclass->swap(_self, b);
}

void set(void *_self, ...)
{
	const struct TypeClass *class = classOf(_self);

	assert(class->set);

	va_list props;

	va_start(props, _self);
	class->set(_self, &props);
	va_end(props);
}

// Get type's value pointer
void* get_vptr(void *_self)
{
	const struct TypeClass *class = classOf(_self);
	
	assert(class->get_vptr);
	return class->get_vptr(_self);
}

/*
 * Initialization
 */

const void *TypeClass;

void initTypeClass(void)
{
	if (!TypeClass)
	{
		TypeClass = new(Class, "TypeClass",
				Class, sizeof(struct TypeClass),
				ctor, TypeClass_ctor,
				0);
	}
}
