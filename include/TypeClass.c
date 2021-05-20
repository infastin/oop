/* vim: set fdm=marker : */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "Macro.h"
#include "TypeClass.h"
#include "Selectors.h"
#include "Exception.h"

/* TypeClass {{{ */

static void* TypeClass_ctor(void *_self, va_list *ap)
{
	struct TypeClass *self = super_ctor(TypeClass(), _self, ap);

	init_method(self, rnd);

/*! TODO: Other operators
*  \todo modulo, and, or, xor, left and right shifts
*/	

	init_method(self, inverse_add);
	init_method(self, inverse_multi);
	init_method(self, absolute);
	init_method(self, cmp_to_zero);
	init_method(self, set_to_zero);
	init_method(self, set_to_one);

	struct IOInterface *self_io = go_to_field(self, struct TypeClass, io);
	init_method(self_io, sfprint);
	init_method(self_io, sfscan);

	struct OperatorsInterface *self_oper = go_to_field(self, struct TypeClass, oper);
	init_method(self_oper, sum);
	init_method(self_oper, subtract);
	init_method(self_oper, product);
	init_method(self_oper, divide);
	init_method(self_oper, modulo);
	init_method(self_oper, onecompl);
	init_method(self_oper, lshift);
	init_method(self_oper, rshift);

	struct ScalarOperatorsInterface *self_sc = go_to_field(self, struct TypeClass, sc);
	init_method(self_sc, scadd);
	init_method(self_sc, scsub);
	init_method(self_sc, scmulti);
	init_method(self_sc, scdivide);

	struct SortInterface *self_sort = go_to_field(self, struct TypeClass, sort);
	init_method(self_sort, cmp);
	init_method(self_sort, swap);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	while ((selector = va_arg(ap_copy, voidf)))
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) inverse_add)
			update_method(self, inverse_add, method);
		else if (selector == (voidf) inverse_multi)
			update_method(self, inverse_multi, method);
		else if (selector == (voidf) rnd)
			update_method(self, rnd, method);
		else if (selector == (voidf) cmp_to_zero)
			update_method(self, cmp_to_zero, method);
		else if (selector == (voidf) set_to_zero)
			update_method(self, set_to_zero, method);
		else if (selector == (voidf) set_to_one)
			update_method(self, set_to_one, method);
		else if (selector == (voidf) absolute)
			update_method(self, absolute, method);

		else if (selector == (voidf) sum)
			update_method(self_oper, sum, method);
		else if (selector == (voidf) subtract)
			update_method(self_oper, subtract, method);
		else if (selector == (voidf) product)
			update_method(self_oper, product, method);
		else if (selector == (voidf) divide)
			update_method(self_oper, divide, method);
		else if (selector == (voidf) modulo)
			update_method(self_oper, modulo, method);
		else if (selector == (voidf) onecompl)
			update_method(self_oper, onecompl, method);
		else if (selector == (voidf) lshift)
			update_method(self_oper, lshift, method);
		else if (selector == (voidf) rshift)
			update_method(self_oper, rshift, method);

		else if (selector == (voidf) sfprint)
			update_method(self_io, sfprint, method);
		else if (selector == (voidf) sfscan)
			update_method(self_io, sfscan, method);
		
		else if (selector == (voidf) scadd)
			update_method(self_sc, scadd, method);
		else if (selector == (voidf) scsub)
			update_method(self_sc, scsub, method);
		else if (selector == (voidf) scmulti)
			update_method(self_sc, scmulti, method);
		else if (selector == (voidf) scdivide)
			update_method(self_sc, scdivide, method);

		else if (selector == (voidf) cmp)
			update_method(self_sort, cmp, method);
		else if (selector == (voidf) swap)
			update_method(self_sort, swap, method);
	}

	va_end(ap_copy);
	return self;
}

/* Selectors */

// Return variable, inversed by addition, of some type  (a copy)
void* inverse_add(const void *_self)
{
	const struct Class *class = classOf(_self);
	const struct TypeClass *tclass = cast(TypeClass(), class);

	if (tclass->inverse_add.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'inverse_add' method!",
				class->name);
	
	typedef void *(*inverse_add_f)(const void *self);

	return ((inverse_add_f) tclass->inverse_add.method)(_self);
}

// Return variable, inversed by multiplication, of some type (a copy)
void* inverse_multi(const void *_self)
{
	const struct Class *class = classOf(_self);
	const struct TypeClass *tclass = cast(TypeClass(), class);

	if (tclass->inverse_multi.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'inverse_multi' method!",
				class->name);

	typedef void *(*inverse_multi_f)(const void *self);

	return ((inverse_multi_f) tclass->inverse_multi.method)(_self);
}

// Create new element width random value (if _self == NULL)
// Or set value of existing element to random
void* rnd(const void *_class, void *_self, ...)
{
	const struct TypeClass *tclass = cast(TypeClass(), _class);
	const struct Class *class = _class;

	if (tclass->rnd.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'rnd' method!",
				class->name);

	typedef void *(*rnd_f)(void *self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	void *result = ((rnd_f) tclass->rnd.method)(_self, &ap);
	va_end(ap);

	return result;
}

void* vrnd(const void *_class, void *_self, va_list *ap)
{
	const struct TypeClass *tclass = cast(TypeClass(), _class);
	const struct Class *class = _class;

	if (tclass->rnd.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'rnd' method!",
				class->name);

	typedef void *(*rnd_f)(void *self, va_list *ap);

	return ((rnd_f) tclass->rnd.method)(_self, ap);
}

int cmp_to_zero(const void *_self)
{
	const struct Class *class = classOf(_self);
	const struct TypeClass *tclass = cast(TypeClass(), class);

	if (tclass->cmp_to_zero.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'cmp_to_zero' method!",
				class->name);

	typedef int (*cmp_to_zero_f)(const void *self);

	return ((cmp_to_zero_f) tclass->cmp_to_zero.method)(_self);
}

void set_to_zero(void *_self)
{
	const struct Class *class = classOf(_self);
	const struct TypeClass *tclass = cast(TypeClass(), class);

	if (tclass->set_to_zero.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'set_to_zero' method!",
				class->name);

	typedef int (*set_to_zero_f)(void *self);

	((set_to_zero_f) tclass->set_to_zero.method)(_self);
}

void set_to_one(void *_self)
{
	const struct Class *class = classOf(_self);
	const struct TypeClass *tclass = cast(TypeClass(), class);

	if (tclass->set_to_one.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'set_to_one' method!",
				class->name);

	typedef int (*set_to_one_f)(void *self);

	((set_to_one_f) tclass->set_to_one.method)(_self);
}

void* absolute(const void *_self)
{
	const struct Class *class = classOf(_self);
	const struct TypeClass *tclass = cast(TypeClass(), class);

	if (tclass->absolute.method == NULL)
		throw(TypeException(), "Error: Type '%s' doesn't have 'absolute' method!",
				class->name);

	typedef void *(*absolute_f)(const void *self);

	return ((absolute_f) tclass->absolute.method)(_self);
}

/* }}} */

/* Initialization {{{ */

ClassImpl(TypeClass)
{
	if (!_TypeClass) 
	{
		void *tclass = new(Class(), "TypeClass", 
				Class(), sizeof(struct TypeClass),
				ctor, TypeClass_ctor,
				0);

		_TypeClass = tclass;
	}

	return _TypeClass;
}

/* Exception init */

ObjectImpl(TypeException)
{
	if (!_TypeException)
	{
		_TypeException = new(ExceptionObject(), "TypeException");
	}

	return _TypeException;
}

/* }}} */
