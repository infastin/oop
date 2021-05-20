/* vim: set fdm=marker : */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "CollectionInterface.h"
#include "Exception.h"
#include "ExceptionObject.h"
#include "Interface.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"

/* Selectors {{{ */

void sort(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->sort.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'sort' method of 'CollectionInterface'!",
				class->name);

	typedef void (*sort_f)(void *_self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((sort_f) ci->sort.method)(_self, &ap);
	va_end(ap);
}

void get_size(const void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->get_size.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'get_size' method of 'CollectionInterface'!",
				class->name);

	typedef void (*get_size_f)(const void *_self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((get_size_f) ci->get_size.method)(_self, &ap);
	va_end(ap);
}

void push_back(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->push_back.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'push_back' method of 'CollectionInterface'!",
				class->name);


	typedef void (*push_back_f)(void *self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((push_back_f) ci->push_back.method)(_self, &ap);
	va_end(ap);
}

void push_front(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->push_front.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'push_front' method of 'CollectionInterface'!",
				class->name);


	typedef void (*push_front_f)(void *self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((push_front_f) ci->push_front.method)(_self, &ap);
	va_end(ap);
}

void add_after(void *_self, int by_type, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->add_after.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'add_after' method of 'CollectionInterface'!",
				class->name);

	typedef void (*add_after_f)(void *self, int by_type, va_list *ap);

	va_list ap;

	va_start(ap, by_type);
	((add_after_f) ci->add_after.method)(_self, by_type, &ap);
	va_end(ap);
}

void add_before(void *_self, int by_type, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->add_before.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'add_before' method of 'CollectionInterface'!",
				class->name);

	typedef void (*add_before_f)(void *self, int by_type, va_list *ap);

	va_list ap;

	va_start(ap, by_type);
	((add_before_f) ci->add_before.method)(_self, by_type, &ap);
	va_end(ap);
}

void add_after_index(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->add_after.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'add_after' method of 'CollectionInterface'!",
				class->name);

	typedef void (*add_after_f)(void *self, int by_type, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((add_after_f) ci->add_after.method)(_self, ADD_BY_INDEX, &ap);
	va_end(ap);

}

void add_after_value(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->add_after.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'add_after' method of 'CollectionInterface'!",
				class->name);

	typedef void (*add_after_f)(void *self, int by_type, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((add_after_f) ci->add_after.method)(_self, ADD_BY_VALUE, &ap);
	va_end(ap);

}

void add_before_index(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->add_before.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'add_before' method of 'CollectionInterface'!",
				class->name);

	typedef void (*add_before_f)(void *self, int by_type, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((add_before_f) ci->add_before.method)(_self, ADD_BY_INDEX, &ap);
	va_end(ap);
}

void add_before_value(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->add_before.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'add_before' method of 'CollectionInterface'!",
				class->name);

	typedef void (*add_before_f)(void *self, int by_type, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((add_before_f) ci->add_before.method)(_self, ADD_BY_VALUE, &ap);
	va_end(ap);
}

size_t count_entries(const void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->count_entries.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'count_entries' method of 'CollectionInterface'!",
				class->name);

	typedef size_t (*count_entries_f)(const void *self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	size_t result = ((count_entries_f) ci->count_entries.method)(_self, &ap);
	va_end(ap);

	return result;
}

void* find_entry(const void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->find_entry.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'find_entry' method of 'CollectionInterface'!",
				class->name);

	typedef void* (*find_entry_f)(const void *self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	void* result = ((find_entry_f) ci->find_entry.method)(_self, &ap);
	va_end(ap);

	return result;
}

void reverse(void *_self)
{
	const struct Class *class = classOf(_self);
	const struct CollectionInterface *ci = icast(CollectionInterface, class);

	if (ci->reverse.method == NULL)
		throw(CollectionException(), "Error: Class '%s' doesn't implement 'reverse' method of 'CollectionInterface'!",
				class->name);

	typedef void (*reverse_f)(const void *self);
	((reverse_f) ci->reverse.method)(_self);
}

/* }}} */

/* Initialization {{{ */

InterfaceImpl(CollectionInterface)
{
	if (!_CollectionInterface)
	{
		_CollectionInterface = inew("CollectionInterface", 0, 9,
				(voidf) sort,
				(voidf) get_size,
				(voidf) push_back,
				(voidf) push_front,
				(voidf) add_after,
				(voidf) add_before,
				(voidf) count_entries,
				(voidf) find_entry,
				(voidf) reverse);
	}

	return _CollectionInterface;
}

/* Exception init */

ObjectImpl(CollectionException)
{
	if (!_CollectionException)
	{
		_CollectionException = new(ExceptionObject(), "CollectionException");
	}

	return _CollectionException;
}

/* }}} */
