/* vim: set fdm=marker : */

#include "OperatorsInterface.h"
#include "Interface.h"
#include "Object.h"
#include "Selectors.h"
#include "Exception.h"

/* Selectors {{{ */

interface_sel(void*, sum, Operators, oper, 
{		
	typedef void *(*sum_f)(const void *self, const void *b);
	
	return ((sum_f) oper->sum.method)(_self, b);
}, 
const void *_self, const void *b);

interface_sel(void*, subtract, Operators, oper,
{		
	typedef void *(*subtract_f)(const void *self, const void *b);

	return ((subtract_f) oper->subtract.method)(_self, b);
}, 
const void *_self, const void *b);

interface_sel(void*, product, Operators, oper, 
{		
	typedef void *(*product_f)(const void *self, const void *b);

	return ((product_f) oper->product.method)(_self, b);
}, 
const void *_self, const void *b);

interface_sel(void*, divide, Operators, oper,
{		
	typedef void *(*divide_f)(const void *self, const void *b);

	return ((divide_f) oper->divide.method)(_self, b);
}, 
const void *_self, const void *b);

interface_sel(void*, modulo, Operators, oper,
{		
	typedef void *(*modulo_f)(const void *self, const void *b);

	return ((modulo_f) oper->modulo.method)(_self, b);
}, 
const void *_self, const void *b);

interface_sel(void, onecompl, Operators, oper,
{		
	typedef void  (*onecompl_f)(void *self);

	((onecompl_f) oper->onecompl.method)(_self);
}, 
void *_self);

interface_sel(void, lshift, Operators, oper,
{		
	typedef void  (*lshift_f)(void *self, size_t shift);

	return ((lshift_f) oper->lshift.method)(_self, shift);
}, 
void *_self, size_t shift);

interface_sel(void, rshift, Operators, oper,
{	
	typedef void  (*rshift_f)(void *self, size_t shift);

	return ((rshift_f) oper->rshift.method)(_self, shift);
}, 
void *_self, size_t shift);

/* }}} */

/* Initialization {{{ */

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

/* Exception Initialization */

ObjectImpl(OperatorsException)
{
	if (_OperatorsException)
	{
		_OperatorsException = new(ExceptionObject(), "OperatorsException");
	}

	return _OperatorsException;
}

/* }}} */
