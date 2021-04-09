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
	self->rnd = NULL;

/*! TODO: Other operators
*  \todo modulo, and, or, xor, left and right shifts
*/	

	self->inverse_add = NULL;
	self->inverse_multi = NULL;

	struct IOInterface *self_io = icast(IOInterface(), _self);

	self_io->sfprint = NULL;
	self_io->sfscan = NULL;

	struct OperatorsInterface *self_oper = icast(OperatorsInterface(), _self);

	self_oper->sum = NULL;
	self_oper->subtract = NULL;
	self_oper->product = NULL;
	self_oper->divide = NULL;
	self_oper->modulo = NULL;
	self_oper->onecompl = NULL;
	self_oper->rshift = NULL;
	self_oper->lshift = NULL;

	struct ScalarOperatorsInterface *self_sc = icast(ScalarOperatorsInterface(), _self);

	self_sc->scadd = NULL;
	self_sc->scsub = NULL;
	self_sc->scmulti = NULL;
	self_sc->scdivide = NULL;

	while ((selector = va_arg(ap_copy, voidf)))
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) cmp)
			self->cmp = (cmp_f) method;
		else if (selector == (voidf) swap)
			self->swap = (swap_f) method;
		else if (selector == (voidf) inverse_add)
			self->inverse_add = (inverse_add_f) method;
		else if (selector == (voidf) inverse_multi)
			self->inverse_multi = (inverse_multi_f) method;
		else if (selector == (voidf) rnd)
			self->rnd = (rnd_f) method;
	
		else if (selector == (voidf) sum)
			self_oper->sum = (sum_f) method;
		else if (selector == (voidf) subtract)
			self_oper->subtract = (subtract_f) method;
		else if (selector == (voidf) product)
			self_oper->product = (product_f) method;
		else if (selector == (voidf) divide)
			self_oper->divide = (divide_f) method;
		else if (selector == (voidf) modulo)
			self_oper->modulo = (modulo_f) method;
		else if (selector == (voidf) onecompl)
			self_oper->onecompl = (onecompl_f) method;
		else if (selector == (voidf) lshift)
			self_oper->lshift = (lshift_f) method;
		else if (selector == (voidf) rshift)
			self_oper->rshift = (rshift_f) method;

		else if (selector == (voidf) sfprint)
			self_io->sfprint = (sfprint_f) method;
		else if (selector == (voidf) sfscan)
			self_io->sfscan = (sfscan_f) method;
		
		else if (selector == (voidf) scadd)
			self_sc->scadd = (scadd_f) method;
		else if (selector == (voidf) scsub)
			self_sc->scsub = (scsub_f) method;
		else if (selector == (voidf) scmulti)
			self_sc->scmulti = (scmulti_f) method;
		else if (selector == (voidf) scdivide)
			self_sc->scdivide = (scdivide_f) method;
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

		tclass = implement(tclass, 3,
				IOInterface(), offsetof(struct TypeClass, io),
				ScalarOperatorsInterface(), offsetof(struct TypeClass, sc),
				OperatorsInterface(), offsetof(struct TypeClass, oper));

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
