/* vim: set fdm=marker : */

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>

#include "Exception.h"
#include "ExceptionObject.h"
#include "List.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"
#include "CollectionInterface.h"
#include "SortInterface.h"

/* Sorting {{{ */

#define SORT_SIZE_THRESHOLD 16

typedef int (*cmp_f)(const void *self, const void *b, va_list *ap);
typedef void (*swap_f)(void *self, void *b);

/* }}} */

/* ListClass {{{ */

static void* ListClass_ctor(void *_self, va_list *ap)
{
	struct ListClass *self = super_ctor(ListClass(), _self, ap);

	struct CollectionInterface *self_ci = go_to_field(self, struct ListClass, ci);

	init_method(self_ci, sort);
	init_method(self_ci, get_size);
	init_method(self_ci, push_back);
	init_method(self_ci, push_front);
	init_method(self_ci, add_after);
	init_method(self_ci, add_before);
	init_method(self_ci, count_entries);
	init_method(self_ci, find_entry);
	init_method(self_ci, reverse);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	while ((selector = va_arg(ap_copy, voidf))) 
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) sort) 
			update_method(self_ci, sort, method);
		else if (selector == (voidf) get_size)
			update_method(self_ci, get_size, method);
		else if (selector == (voidf) push_back)
			update_method(self_ci, push_back, method);
		else if (selector == (voidf) push_front)
			update_method(self_ci, push_front, method);
		else if (selector == (voidf) add_after)
			update_method(self_ci, add_after, method);
		else if (selector == (voidf) add_before)
			update_method(self_ci, add_before, method);
		else if (selector == (voidf) count_entries)
			update_method(self_ci, count_entries, method);
		else if (selector == (voidf) find_entry)
			update_method(self_ci, find_entry, method);
		else if (selector == (voidf) reverse)
			update_method(self_ci, reverse, method);
	}

	va_end(ap_copy);
	return self;
}

/* }}} */

/* List {{{ */

/* Private methods {{{ */

void List_add_after_node(struct List *self, struct ListNode *node, va_list *ap)
{
	struct ListNode *new_node = (struct ListNode*)calloc(sizeof(struct ListNode), 1);

	if (new_node == NULL)
		throw(ListException(), "Fatal Error: Couldn't allocate memory for new list node!");

	new_node->value = vnew(self->datatype, ap);

	if (self->size == 0 || self->first == NULL || self->last == NULL || node == NULL)
	{
		new_node->next = NULL;
		new_node->prev = NULL;

		self->first = new_node;
		self->last = new_node;
		self->size++;

		return;
	}

	new_node->next = node->next;
	new_node->prev = node;
	
	if (node->next)
		node->next->prev = new_node;
	
	node->next = new_node;

	if (self->last == node)
		self->last = new_node;

	self->size++;
}

void List_add_before_node(struct List *self, struct ListNode *node, va_list *ap)
{
	struct ListNode *new_node = (struct ListNode*)calloc(sizeof(struct ListNode), 1);

	if (new_node == NULL)
		throw(ListException(), "Fatal Error: Couldn't allocate memory for new list node!");

	new_node->value = vnew(self->datatype, ap);

	if (self->size == 0 || self->first == NULL || self->last == NULL || node == NULL)
	{
		new_node->next = NULL;
		new_node->prev = NULL;

		self->first = new_node;
		self->last = new_node;
		self->size++;

		return;
	}

	new_node->next = node;
	new_node->prev = node->prev;

	if (node->prev)
		node->prev->next = new_node;

	node->prev = new_node;

	if (self->first == node)
		self->first = new_node;

	self->size++;
}

/* }}} */

/* Public methods {{{ */

static void* List_ctor(void *_self, va_list *ap)
{
	struct List *self = super_ctor(List(), _self, ap);

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	self->datatype = cast(Class(), va_arg(ap_copy, const void*));
	self->size = 0;
	self->first = NULL;
	self->last = NULL;

	return self;
	
}

static void* List_dtor(void *_self)
{
	return _self;
}

static void* List_cpy(const void *_self, void *_object)
{
	return _object;
}

static void List_push_back(void *_self, va_list *ap)
{
	struct List *self = cast(List(), _self);

	List_add_after_node(self, self->last, ap);
}

static void List_push_front(void *_self, va_list *ap)
{
	struct List *self = cast(List(), _self);

	List_add_before_node(self, self->first, ap);
}

static void List_get_size(const void *_self, va_list *ap)
{
	const struct List *self = cast(List(), _self);
	
	size_t *size = va_arg(*ap, size_t*);
	*size = self->size;
}

#define CF_ENTRIES_THRESHOLD 16

static size_t List_count_entries(const void *_self, va_list *ap)
{
	struct List *self = cast(List(), _self);

	if (self->size == 0)
		return 0;

	const void *entry = va_arg(*ap, const void*);
	const struct Class *entry_class = classOf(entry);

	if (entry_class != self->datatype)
		throw(ListException(), "Error: can't compare '%s' with '%s'",
				entry_class->name, self->datatype->name);

	const struct SortInterface *sort = icast(SortInterface, self->datatype);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				self->datatype->name);

	size_t result = 0;

	if (self->size < CF_ENTRIES_THRESHOLD)
	{
		size_t i;
		struct ListNode *node;

		for (i = 0, node = self->first;
				i < self->size; 
				++i, node = node->next) 
		{
			if (((cmp_f) sort->cmp.method)(entry, node->value, ap) == 0)
				result++;
		}
	}
	else
	{
		size_t i, j;
		struct ListNode *left, *right;

		for (i = 0, j = self->size - 1, left = self->first, right = self->last;
				i <= j; 
				++i, --j, left = left->next, right = right->prev) 
		{
			if (((cmp_f) sort->cmp.method)(entry, left->value, ap) == 0)
				result++;

			if (i == j)
				break;

			if (((cmp_f) sort->cmp.method)(entry, right->value, ap) == 0)
				result++;
		}
	}

	return result;
}

static void* List_find_entry(const void *_self, va_list *ap)
{
	struct List *self = cast(List(), _self);

	if (self->size == 0)
		return NULL;

	const void *entry = va_arg(*ap, const void*);
	const struct Class *entry_class = classOf(entry);

	if (entry_class != self->datatype)
		throw(ListException(), "Error: can't compare '%s' with '%s'",
				entry_class->name, self->datatype->name);

	const struct SortInterface *sort = icast(SortInterface, self->datatype);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				self->datatype->name);

	if (self->size < CF_ENTRIES_THRESHOLD)
	{
		size_t i;
		struct ListNode *node;

		for (i = 0, node = self->first;
				i < self->size; 
				++i, node = node->next) 
		{
			if (((cmp_f) sort->cmp.method)(entry, node->value, ap) == 0)
				return node->value;
		}

	}
	else
	{
		size_t i, j;
		struct ListNode *left, *right;

		for (i = 0, j = self->size - 1, left = self->first, right = self->last;
				i <= j; 
				++i, --j, left = left->next, right = right->prev) 
		{
			if (((cmp_f) sort->cmp.method)(entry, left->value, ap) == 0)
				return left->value;

			if (i == j)
				break;

			if (((cmp_f) sort->cmp.method)(entry, right->value, ap) == 0)
				return right->value;
		}
	}

	return NULL;
}

static void List_reverse(void *_self)
{
	struct List *self = cast(List(), _self);

	if (self->size <= 1)
		return;

	size_t i, j;
	struct ListNode *left, *right;

	for(i = 0, j = self->size - 1, left = self->first, right = self->last; 
			i < j;
			++i, --j, left = left->next, right = right->prev)
	{
		void *tmp = left->value;
		left->value = right->value;
		right->value = tmp;
	}
}

/* }}} */

/* }}} */

/* Initialization {{{ */

ClassImpl(ListClass)
{
	if (!_ListClass)
	{
		_ListClass = new(Class(), "ListClass",
				Class(), sizeof(struct ListClass),
				ctor, ListClass_ctor,
				0);
	}

	return _ListClass;
}

ClassImpl(List)
{
	if (!_List)
	{
		_List = new(ListClass(), "List",
				Object(), sizeof(struct List),
				ctor, List_ctor,
				dtor, List_dtor,
				cpy,  List_cpy,
				get_size, List_get_size,
				push_back, List_push_back,
				push_front, List_push_front,
				count_entries, List_count_entries,
				find_entry, List_find_entry,
				reverse, List_reverse,
				0);
	}

	return _List;
}

ObjectImpl(ListException)
{
	if (!_ListException)
	{
		_ListException = new(ExceptionObject(), "ListException");
	}

	return _ListException;
}

/* }}} */
