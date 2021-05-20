/* vim: set fdm=marker : */

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

#include "ExceptionObject.h"
#include "Exception.h"
#include "Interface.h"
#include "Macro.h"
#include "Object.h"
#include "SortInterface.h"
#include "Selectors.h"

/* Selectors {{{ */

int cmp(const void *_self, const void *b, ...)
{
	const struct Class *class = classOf(_self);
	const struct SortInterface *sort = icast(SortInterface, class);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				class->name);

	typedef int (*cmp_f)(const void *self, const void *b, va_list *ap);

	va_list ap;

	va_start(ap, b);
	int result = ((cmp_f) sort->cmp.method)(_self, b, &ap);
	va_end(ap);

	return result;
}

int vcmp(const void *_self, const void *b, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct SortInterface *sort = icast(SortInterface, class);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				class->name);

	typedef int  (*cmp_f)(const void *self, const void *b, va_list *ap);

	return ((cmp_f) sort->cmp.method)(_self, b, ap);
}

void swap(void *_self, void *b)
{
	const struct Class *class = classOf(_self);
	const struct SortInterface *sort = icast(SortInterface, class);

	if (sort->swap.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'swap' method of 'SortInterface'!",
				class->name);

	typedef void (*swap_f)(void *self, void *b);

	((swap_f) sort->swap.method)(_self, b);
}

/* }}} */

/* Initialization {{{ */

InterfaceImpl(SortInterface)
{
	if (!_SortInterface)
	{
		_SortInterface = inew("SortInterface", 0, 2,
				(voidf) cmp,
				(voidf) swap);
	}

	return _SortInterface;
}

/* Exception init */

ObjectImpl(SortException)
{
	if (!_SortException)
	{
		_SortException = new(ExceptionObject(), "SortException");
	}

	return _SortException;
}

/* }}} */
