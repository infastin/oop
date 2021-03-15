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
	struct TypeClass *self = super_ctor(TypeClass(), _self, props);

	voidf selector;
	va_list ap;
	va_copy(ap, *props);

	self->cmp = NULL;
	self->swap = NULL;
	self->sum = NULL;
	self->product = NULL;
	self->scadd = NULL;
	self->scmulti = NULL;

	while ((selector = va_arg(ap, voidf)))
	{
		voidf method = va_arg(ap, voidf);

		if (selector == (voidf) cmp)
			self->cmp = (cmp_f) method;
		else if (selector == (voidf) swap)
			self->swap = (swap_f) method;
		else if (selector == (voidf) sum)
			self->sum = (sum_f) method;
		else if (selector == (voidf) product)
			self->product = (product_f) method;
		else if (selector == (voidf) scadd)
			self->scadd = (scadd_f) method;
		else if (selector == (voidf) scmulti)
			self->scmulti = (scmulti_f) method;
	}

	return self;
}

/*
 * New selectors for Types
 */

// Compare two variables of the same type
int cmp(const void *_self, const void *b)
{
	const struct TypeClass *class = cast(TypeClass(), classOf(_self));

	assert(class->cmp);
	return class->cmp(_self, b);
}

int super_cmp(const void *_class, const void *_self, const void *b)
{
	const struct TypeClass *superclass = cast(TypeClass(), super(_class));

	assert(superclass->cmp);
	return superclass->cmp(_self, b);
}

// Sum two variables of the same type, 
// return new variable with the same type and the result of sum as value.
void* sum(const void *_self, const void *b)
{
	const struct TypeClass *class = cast(TypeClass(), classOf(_self));

	assert(class->sum);
	return class->sum(_self, b);
}

void* super_sum(const void *_class, const void *_self, const void *b)
{
	const struct TypeClass *superclass = cast(TypeClass(), super(_class));

	assert(superclass->sum);
	return superclass->sum(_self, b);
}

// Multiply two variables of the same type, 
// return new variable with the same type and the result of multiplication as value.
void* product(const void *_self, const void *b)
{
	const struct TypeClass *class = cast(TypeClass(), classOf(_self));

	assert(class->product);
	return class->product(_self, b);
}

void* super_product(const void *_class, const void *_self, const void *b)
{
	const struct TypeClass *superclass = cast(TypeClass(), super(_class));

	assert(superclass->product);
	return superclass->product(_self, b);
}

// Add scalar to the variable of some type
void scadd(const void *_self, ...)
{
	const struct TypeClass *class = cast(TypeClass(), classOf(_self));

	assert(class->scadd);

	va_list ap;
	va_start(ap, _self);
	class->scadd(_self, &ap);
	va_end(ap);
}

void super_scadd(const void *_class, const void *_self, ...)
{
	const struct TypeClass *superclass = cast(TypeClass(), super(_class));

	assert(superclass->scadd);

	va_list ap;
	va_start(ap, _self);
	superclass->scadd(_self, &ap);
	va_end(ap);
}

// Multiply on scalar variable of some type
void scmulti(const void *_self, ...)
{
	const struct TypeClass *class = cast(TypeClass(), classOf(_self));

	assert(class->scmulti);

	va_list ap;
	va_start(ap, _self);
	class->scmulti(_self, &ap);
	va_end(ap);
}

void super_scmulti(const void *_class, const void *_self, ...)
{
	const struct TypeClass *superclass = cast(TypeClass(), super(_class));

	assert(superclass->scmulti);

	va_list ap;
	va_start(ap, _self);
	superclass->scmulti(_self, &ap);
	va_end(ap);
}

// Swap values of two variables of some type
void swap(void *_self, void *b)
{
	const struct TypeClass *class = cast(TypeClass(), classOf(_self));

	assert(class->swap);
	class->swap(_self, b);
}

void super_swap(const void *_class, void *_self, void *b)
{
	const struct TypeClass *superclass = cast(TypeClass(), super(_class));

	assert(superclass->swap);
	superclass->swap(_self, b);
}

/*
 * Initialization
 */

ClassImpl(TypeClass)
{
	if (!_TypeClass) {
		_TypeClass = new(Class(), "TypeClass", 
				Class(), sizeof(struct TypeClass),
				ctor, TypeClass_ctor,
				0);
	}

	return _TypeClass;
}
