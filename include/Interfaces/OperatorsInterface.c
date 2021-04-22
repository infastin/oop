#include "OperatorsInterface.h"
#include "Object.h"
#include "Selectors.h"
#include "Exception.h"

void* sum(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->sum.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'sum' method of 'OperatorsInterface'!",
				class->name);

	typedef void *(*sum_f)(const void *self, const void *b);

	return ((sum_f) oper->sum.method)(_self, b);
}

void* subtract(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->subtract.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'subtract' method of 'OperatorsInterface'!",
				class->name);

	typedef void *(*subtract_f)(const void *self, const void *b);

	return ((subtract_f) oper->subtract.method)(_self, b);
}

void* product(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->product.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'product' method of 'OperatorsInterface'!",
				class->name);

	typedef void *(*product_f)(const void *self, const void *b);

	return ((product_f) oper->product.method)(_self, b);
}

void* divide(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->divide.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'divide' method of 'OperatorsInterface'!",
				class->name);

	typedef void *(*divide_f)(const void *self, const void *b);

	return ((divide_f) oper->divide.method)(_self, b);
}

void* modulo(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->modulo.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'modulo' method of 'OperatorsInterface'!",
				class->name);

	typedef void *(*modulo_f)(const void *self, const void *b);

	return ((modulo_f) oper->modulo.method)(_self, b);
}

void onecompl(void *_self)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->onecompl.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'onecompl' method of 'OperatorsInterface'!",
				class->name);

	typedef void  (*onecompl_f)(void *self);

	return ((onecompl_f) oper->onecompl.method)(_self);
}

void lshift(void *_self, size_t shift)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->lshift.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'lshift' method of 'OperatorsInterface'!",
				class->name);

	typedef void  (*lshift_f)(void *self, size_t shift);

	return ((lshift_f) oper->lshift.method)(_self, shift);
}

void rshift(void *_self, size_t shift)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface, _self);

	if (oper->rshift.method == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'rshift' method of 'OperatorsInterface'!",
				class->name);

	typedef void  (*rshift_f)(void *self, size_t shift);

	return ((rshift_f) oper->rshift.method)(_self, shift);
}

/*
 * Interface Initialization
 */
InterfaceImpl(OperatorsInterface)
{
	if (!_OperatorsInterface)
	{
		_OperatorsInterface = inew("OperatorsInterface", 0, 8,
				(voidf) sum, 
				(voidf) subtract,
				(voidf) product, 
				(voidf) divide,
				(voidf) modulo,
				(voidf) onecompl,
				(voidf) lshift,
				(voidf) rshift);
	}

	return _OperatorsInterface;
}

/*
 * Exception Initialization
 */

ObjectImpl(OperatorsException)
{
	if (_OperatorsException)
	{
		_OperatorsException = new(ExceptionObject(), "OperatorsException");
	}

	return _OperatorsException;
}
