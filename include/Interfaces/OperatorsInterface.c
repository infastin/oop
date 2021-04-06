#include "OperatorsInterface.h"
#include "Object.h"
#include "Selectors.h"
#include "Exception.h"

void* sum(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->sum == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'sum' method of 'OperatorsInterface'!",
				class->name);

	return oper->sum(_self, b);
}

void* subtract(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->subtract == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'subtract' method of 'OperatorsInterface'!",
				class->name);

	return oper->subtract(_self, b);
}

void* product(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->product == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'product' method of 'OperatorsInterface'!",
				class->name);

	return oper->product(_self, b);
}

void* divide(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->divide == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'divide' method of 'OperatorsInterface'!",
				class->name);

	return oper->divide(_self, b);
}

void onecompl(void *_self)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->onecompl == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'onecompl' method of 'OperatorsInterface'!",
				class->name);

	return oper->onecompl(_self);
}

void lshift(void *_self, size_t shift)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->lshift == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'lshift' method of 'OperatorsInterface'!",
				class->name);

	return oper->lshift(_self, shift);
}

void rshift(void *_self, size_t shift)
{
	const struct Class *class = classOf(_self);
	const struct OperatorsInterface *oper = icast(OperatorsInterface(), class);

	if (oper->rshift == NULL)
		throw(OperatorsException(), "Error: Class '%s' doesn't implement 'rshift' method of 'OperatorsInterface'!",
				class->name);

	return oper->rshift(_self, shift);
}

/*
 * Interface Initialization
 */
InterfaceImpl(OperatorsInterface)
{
	if (!_OperatorsInterface)
	{
		_OperatorsInterface = inew("OperatorsInterface", 0);
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
