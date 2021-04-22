#include <stdio.h>
#include <stddef.h>

#include "ExceptionObject.h"
#include "Exception.h"
#include "Interface.h"
#include "Macro.h"
#include "Object.h"
#include "SortInterface.h"
#include "Selectors.h"

int cmp(const void *_self, const void *b)
{
	const struct Class *class = classOf(_self);
	const struct SortInterface *sort = icast(SortInterface, _self);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				class->name);

	typedef int  (*cmp_f)(const void *self, const void *b);


	return ((cmp_f) sort->cmp.method)(_self, b);
}

void swap(void *_self, void *b)
{
	const struct Class *class = classOf(_self);
	const struct SortInterface *sort = icast(SortInterface, _self);

	if (sort->swap.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'swap' method of 'SortInterface'!",
				class->name);

	typedef void (*swap_f)(void *self, void *b);

	((swap_f) sort->swap.method)(_self, b);
}

ObjectImpl(SortException)
{
	if (!_SortException)
	{
		_SortException = new(ExceptionObject(), "SortException");
	}

	return _SortException;
}

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
