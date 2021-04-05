#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "TypeClass.h"
#include "Selectors.h"
#include "Exception.h"

/*
 * Methods
 */

static void* TypeClass_ctor(void *_self, va_list *ap)
{
	struct TypeClass *self = super_ctor(TypeClass(), _self, ap);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	self->cmp = NULL;
	self->swap = NULL;
	self->sum = NULL;
	self->subtract = NULL;
	self->product = NULL;
	self->divide = NULL;
	self->scadd = NULL;
	self->scsub = NULL;
	self->scmulti = NULL;
	self->scdivide = NULL;
	self->inverse_add = NULL;
	self->inverse_multi = NULL;
	self->rnd = NULL;

	struct IOInterface *self_io = icast(IOInterface(), _self);

	self_io->sfprint = NULL;
	self_io->sfscan = NULL;

	while ((selector = va_arg(ap_copy, voidf)))
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) cmp)
			self->cmp = (cmp_f) method;
		else if (selector == (voidf) swap)
			self->swap = (swap_f) method;
		else if (selector == (voidf) sum)
			self->sum = (sum_f) method;
		else if (selector == (voidf) subtract)
			self->subtract = (subtract_f) method;
		else if (selector == (voidf) product)
			self->product = (product_f) method;
		else if (selector == (voidf) divide)
			self->divide = (divide_f) method;
		else if (selector == (voidf) scadd)
			self->scadd = (scadd_f) method;
		else if (selector == (voidf) scsub)
			self->scsub = (scsub_f) method;
		else if (selector == (voidf) scmulti)
			self->scmulti = (scmulti_f) method;
		else if (selector == (voidf) scdivide)
			self->scdivide = (scdivide_f) method;
		else if (selector == (voidf) inverse_add)
			self->inverse_add = (inverse_add_f) method;
		else if (selector == (voidf) inverse_multi)
			self->inverse_multi = (inverse_multi_f) method;
		else if (selector == (voidf) rnd)
			self->rnd = (rnd_f) method;
		else if (selector == (voidf) sfprint)
			self_io->sfprint = (sfprint_f) method;
		else if (selector == (voidf) sfscan)
			self_io->sfscan = (sfscan_f) method;
	}

	va_end(ap_copy);
	return self;
}

/*
 * New selectors for Types
 */

// Compare two variables of the same type
int cmp(const void *_self, const void *b)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->cmp == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'cmp' method!",
				class->name);

	return tclass->cmp(_self, b);
}

// Sum two variables of the same type, 
// return new variable with the same type and the result of sum as value.
void* sum(const void *_self, const void *b)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->sum == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'sum' method!",
				class->name);

	return tclass->sum(_self, b);
}

// Subtract one variable from other of the same type, 
// return new variable with the same type and the result of subtraction as value.
void* subtract(const void *_self, const void *b)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->subtract == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'subtract' method!",
				class->name);

	return tclass->subtract(_self, b);
}

// Multiply two variables of the same type, 
// return new variable with the same type and the result of multiplication as value.
void* product(const void *_self, const void *b)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->product == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'product' method!",
				class->name);

	return tclass->product(_self, b);
}

// Divide one variable by other of the same type, 
// return new variable with the same type and the result of division as value.
void* divide(const void *_self, const void *b)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->divide == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'divide' method!",
				class->name);

	return tclass->divide(_self, b);
}

// Add scalar to the variable of some type
void scadd(void *_self, ...)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scadd == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scadd' method!",
				class->name);
	va_list ap;
	va_start(ap, _self);
	tclass->scadd(_self, &ap);
	va_end(ap);
}

void vscadd(void *_self, va_list *ap)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scadd == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scadd' method!",
				class->name);

	tclass->scadd(_self, ap);
}

// Subtract scalar from the variable of some type
void scsub(void *_self, ...)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scsub == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scsub' method!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	tclass->scsub(_self, &ap);
	va_end(ap);
}

void vscsub(void *_self, va_list *ap)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scsub == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scsub' method!",
				class->name);

	tclass->scsub(_self, ap);
}

// Multiply on scalar variable of some type
void scmulti(void *_self, ...)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scmulti == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scmulti' method!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	tclass->scmulti(_self, &ap);
	va_end(ap);
}

void vscmulti(void *_self, va_list *ap)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scmulti == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scmulti' method!",
				class->name);

	tclass->scmulti(_self, ap);
}

// Divide by scalar variable of some type
void scdivide(void *_self, ...)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scdivide == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scdivide' method!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	tclass->scdivide(_self, &ap);
	va_end(ap);
}

void vscdivide(void *_self, va_list *ap)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->scdivide == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'scdivide' method!",
				class->name);

	tclass->scdivide(_self, ap);
}

// Return variable, inversed by addition, of some type  (a copy)
void* inverse_add(void *_self)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->inverse_add == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'inverse_add' method!",
				class->name);
	
	return tclass->inverse_add(_self);
}

// Return variable, inversed by multiplication, of some type (a copy)
void* inverse_multi(void *_self)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->inverse_multi == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'inverse_multi' method!",
				class->name);

	return tclass->inverse_multi(_self);
}

// Swap values of two variables of some type
void swap(void *_self, void *b)
{
	const struct TypeClass *tclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (tclass->swap == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'swap' method!",
				class->name);

	tclass->swap(_self, b);
}

// Create new element width random value (if _self == NULL)
// Or set value of existing element to random
void* rnd(const void *_class, void *_self, ...)
{
	const struct TypeClass *tclass = cast(TypeClass(), _class);
	const struct Class *class = _class;

	if (tclass->rnd == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'rnd' method!",
				class->name);

	va_list ap;

	va_start(ap, _self);
	void *result = tclass->rnd(_self, &ap);
	va_end(ap);

	return result;
}

void* vrnd(const void *_class, void *_self, va_list *ap)
{
	const struct TypeClass *tclass = cast(TypeClass(), _class);
	const struct Class *class = _class;

	if (tclass->rnd == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'rnd' method!",
				class->name);

	return tclass->rnd(_self, ap);
}

/*
 * Initialization
 */

ClassImpl(TypeClass)
{
	if (!_TypeClass) 
	{
		void *tclass = new(Class(), "TypeClass", 
				Class(), sizeof(struct TypeClass),
				ctor, TypeClass_ctor,
				0);

		tclass = implement(tclass, 1,
				IOInterface(), offsetof(struct TypeClass, io));

		_TypeClass = tclass;
	}

	return _TypeClass;
}

/*
 * Exception Initialization
 */

ObjectImpl(TypeException)
{
	if (_TypeException)
	{
		_TypeException = new(ExceptionObject(), "TypeException");
	}

	return _TypeException;
}
