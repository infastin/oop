/* vim: set fdm=marker : */

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Array.h"
#include "IO.h"
#include "IOInterface.h"
#include "Interface.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"
#include "ExceptionObject.h"
#include "Exception.h"
#include "CollectionInterface.h"
#include "SortInterface.h"

/* Sorting {{{ */

#define SORT_SIZE_THRESHOLD 16

#define SORT_CMP(a, b, c, p, ap) ((p == ARRAY_SORT_REVERSED) ? (~c(a, b, ap) + 1) : c(a, b, ap))

typedef int (*cmp_f)(const void *self, const void *b, va_list *ap);
typedef void (*swap_f)(void *self, void *b);

/* Insertion sort */
static inline void inssort(void **mass, size_t numb, cmp_f cmp_func, swap_f swap_func, int param, va_list *ap)
{
	for (int i = 1; i < numb; ++i) 
	{
		size_t cur = i;

		for (size_t j = i - 1; j >= 0; --j)
		{
			if (SORT_CMP(mass[j], mass[cur], cmp_func, param, ap) <= 0)
				break;

			swap_func(mass[j], mass[cur]);
			cur = j;

			if (j == 0)
				break;
		}
	}
}

/* Heapsort */
static inline void heap(void **mass, size_t start, size_t end, cmp_f cmp_func, swap_f swap_func, int param, va_list *ap)
{
	size_t root = start;

	while ((root << 1) < end)
	{
		size_t child = (root << 1) + 1;

		if ((child < end) && SORT_CMP(mass[child], mass[child + 1], cmp_func, param, ap) < 0)
			child++;

		if (SORT_CMP(mass[root], mass[child], cmp_func, param, ap) < 0)
		{
			swap_func(mass[root], mass[child]);
			root = child;
		}
		else
			return;
	}
}

static inline void heapify(void **mass, size_t numb, cmp_f cmp_func, swap_f swap_func, int param, va_list *ap)
{
	size_t start = (numb - 1) >> 1;

	while (start >= 0)
	{
		heap(mass, start, numb - 1, cmp_func, swap_func, param, ap);

		if (start == 0)
			break;

		start--;
	}
}

static inline void heapsort(void **mass, size_t numb, cmp_f cmp_func, swap_f swap_func, int param, va_list *ap)
{
	size_t end = numb - 1;

	if (numb <= 1)
		return;

	heapify(mass, numb, cmp_func, swap_func, param, ap);

	while (end > 0)
	{
		swap_func(mass[0], mass[end]);
		end--;
		heap(mass, 0, end, cmp_func, swap_func, param, ap);
	}
}

/* Based on Knuth vold. 3 */
static inline size_t quicksort_partition(void **mass, size_t left, size_t right, size_t pivot, cmp_f cmp_func, swap_f swap_func, int param, va_list *ap)
{
	size_t i = left + 1;
	size_t j = right;

	if (pivot != left)
		swap(mass[left], mass[pivot]);

	while (1) 
	{
		while (SORT_CMP(mass[i], mass[left], cmp_func, param, ap) < 0)
			i++;

		while (SORT_CMP(mass[left], mass[j], cmp_func, param, ap) < 0)
			j--;

		if (j <= i)
		{
			swap_func(mass[j], mass[left]);
			return j;
		}

		swap_func(mass[i], mass[j]);

		i++;
		j--;
	}

	return 0;
}

static inline size_t find_median(void **mass, size_t a, size_t b, size_t c, cmp_f cmp_func, int param, va_list *ap)
{
	if (SORT_CMP(mass[a], mass[b], cmp_func, param, ap) > 0)
	{
		if (SORT_CMP(mass[b], mass[c], cmp_func, param, ap) > 0)
			return b;
		else if (SORT_CMP(mass[a], mass[c], cmp_func, param, ap) > 0)
			return c;
		else
			return a;
	}
	else
	{
		if (SORT_CMP(mass[a], mass[c], cmp_func, param, ap) > 0)
			return a;
		else if (SORT_CMP(mass[b], mass[c], cmp_func, param, ap) > 0)
			return c;
		else
			return b;
	}
}

static void quicksort_recursive(void **mass, size_t orig_left, size_t orig_right, cmp_f cmp_func, swap_f swap_func, int param, va_list *ap)
{
	size_t left = orig_left;
	size_t right = orig_right;
	size_t mid;

	size_t pivot;
	size_t new_pivot;

	int loop_count = 0;
	const int max_loops = 64 - __builtin_clzll(right - left);

	while (1) 
	{
		if (right <= left)
			return;

		if ((right - left + 1) <= SORT_SIZE_THRESHOLD)
		{
			inssort(&mass[left], right - left + 1, cmp_func, swap_func, param, ap);
			return;
		}

		if (++loop_count >= max_loops)
		{
			heapsort(&mass[left], right - left + 1, cmp_func, swap_func, param, ap);
			return;
		}

		mid = left + ((right - left) >> 1);
		pivot = find_median(mass, left, mid, right, cmp_func, param, ap);
		new_pivot = quicksort_partition(mass, left, right, pivot, cmp_func, swap_func, param, ap);

		if (new_pivot == 0)
			return;

		if ((new_pivot - left - 1) > (right - new_pivot - 1))
		{
			quicksort_recursive(mass, new_pivot + 1, right, cmp_func, swap_func, param, ap);
			right = new_pivot - 1;
		}
		else
		{
			quicksort_recursive(mass, left, new_pivot - 1, cmp_func, swap_func, param, ap);
			left = new_pivot + 1;
		}
	}
}

/* }}} */

/* ArrayClass {{{ */

static void* ArrayClass_ctor(void *_self, va_list *ap)
{
	struct ArrayClass *self = super_ctor(ArrayClass(), _self, ap);

	init_method(self, set_arrtype);
	init_method(self, set_arrcap);

	struct CollectionInterface *self_ci = go_to_field(self, struct ArrayClass, ci);
	
	init_method(self_ci, sort);
	init_method(self_ci, get_size);
	init_method(self_ci, push_back);
	init_method(self_ci, push_front);
	init_method(self_ci, add_after);
	init_method(self_ci, add_before);
	init_method(self_ci, count_entries);
	init_method(self_ci, find_entry);
	init_method(self_ci, reverse);

	struct IOInterface *self_io = go_to_field(self, struct ArrayClass, io);

	init_method(self_io, sfprint);
	init_method(self_io, sfscan);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	while ((selector = va_arg(ap_copy, voidf))) {
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) set_arrtype)
			update_method(self, set_arrtype, method);
		else if (selector == (voidf) set_arrcap)
			update_method(self, set_arrcap, method);

		else if (selector == (voidf) sort)
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

		else if (selector == (voidf) sfprint)
			update_method(self_io, sfprint, method);
		else if (selector == (voidf) sfscan)
			update_method(self_io, sfscan, method);

	}

	va_end(ap_copy);
	return self;
}

/* Selectors {{{ */

void set_arrtype(void *_self, int arrtype)
{
	const struct Class *class = classOf(_self);
	const struct ArrayClass *aclass = cast(ArrayClass(), class);

	if (aclass->set_arrtype.method == NULL)
		throw(ArrayException(), "Error: Class '%s' doesn't have method 'set_arrtype'!",
				class->name);

	typedef void (*set_arrtype_f)(void *self, int arrtype);

	((set_arrtype_f) aclass->set_arrtype.method)(_self, arrtype);
}

void set_arrcap(void *_self, size_t capacity)
{
	const struct Class *class = classOf(_self);
	const struct ArrayClass *aclass = cast(ArrayClass(), class);

	if (aclass->set_arrcap.method == NULL)
		throw(ArrayException(), "Error: Class '%s' doesn't have method 'set_arrcap'!",
				class->name);

	typedef void (*set_arrcap_f)(void *self, size_t capacity);

	((set_arrcap_f) aclass->set_arrcap.method)(_self, capacity);
}

/* }}} */

/* }}} */

/* Array {{{ */

/* Private methods {{{ */

static void Array_growsize(struct Array *self, size_t add)
{
	if ((self->size + add) <= self->capacity)
		return;

	size_t mincap = self->size + add;

	if (mincap > 64)
		mincap = (size_t) ((double) self->capacity * 1.25);
	else if (mincap > 8)
		mincap = self->capacity * 4;
	else
		mincap = self->capacity * 8;

	self->mass = (void**)realloc(self->mass, sizeof(void*) * mincap);
	self->capacity = mincap;
}

static void Array_growcap(struct Array *self, size_t add)
{
	if (add == 0)
		return;

	size_t mincap = self->capacity + add;

	if (mincap > 64)
		mincap = (size_t) ((double) self->capacity * 1.25);
	else if (mincap > 8)
		mincap = self->capacity * 4;
	else
		mincap = self->capacity * 8;

	self->mass = (void**)realloc(self->mass, sizeof(void*) * mincap);
	self->capacity = mincap;
}

/* }}} */

/* Public methods {{{ */

static void* Array_ctor(void *_self, va_list *ap)
{
	struct Array *self = super_ctor(Array(), _self, ap);

	size_t capacity;
	va_list ap_copy;
	va_copy(ap_copy, *ap);
	
	self->datatype = cast(Class(), va_arg(ap_copy, const void*));
	capacity = va_arg(ap_copy, size_t);

	self->mass = NULL;

	if (capacity == 0)
	{
		delete(self);
		throw(ArrayException(), "Error: Can't create array with zero capacity!");
	}

	int arrtype = va_arg(ap_copy, int);

	if (arrtype != ARRAY_REGULAR && arrtype != ARRAY_GROWING)
		arrtype = ARRAY_REGULAR;

	self->arrtype = arrtype;

	self->capacity = capacity;
	self->size = 0;

	void **mass;
	mass = (void**)calloc(sizeof(void*), self->capacity);
	if (mass == NULL)
	{
		delete(self);
		throw(ArrayException(), "Fatal Error: Array allocation error!");
	}

	va_end(ap_copy);
	self->mass = mass;

	return self;
}

static void* Array_dtor(void *_self)
{
	struct Array *self = cast(Array(), _self);

	if (self->mass != NULL)
	{
		for (size_t i = 0; i < self->size; ++i) 
		{
			if (self->mass[i] != NULL)
				delete(self->mass[i]);
		}

		free(self->mass);
	}

	return super_dtor(Array(), self);
}

static void* Array_cpy(const void *_self, void *_object)
{
	const struct Array *self = cast(Array(), _self);
	struct Array *object = super_cpy(Array(), _self, _object);

	object->datatype = self->datatype;
	object->capacity = self->capacity;
	object->size = self->size;
	object->arrtype = self->arrtype;

	void **mass;
	mass = (void**)calloc(sizeof(void*), object->capacity);
	if (mass == NULL)
	{
		delete(object);
		throw(ArrayException(), "Fatal Error: Array allocation error!");
	}

	for (size_t i = 0; i < object->size; ++i)
	{
		if (self->mass[i] != NULL)
			mass[i] = copy(self->mass[i]);
	}

	object->mass = mass;

	return object;
}

static void Array_set(void *_self, va_list *ap)
{
	struct Array *self = cast(Array(), _self);
	
	size_t index = va_arg(*ap, size_t);

	if (index >= self->capacity)
	{
		if (self->arrtype == ARRAY_GROWING)
			Array_growsize(self, index - self->capacity + 1);
		else
			throw(ArrayException(), "Error: Element at (%lu) is out of bounds!",
					index);
	}

	if (self->mass[index] == NULL)
	{
		self->mass[index] = vnew(self->datatype, ap);
		
		if (index >= self->size)
			self->size = index + 1;
	}
	else
		vset(self->mass[index], ap);
}

static int Array_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct Array *self = cast(Array(), _self);

	int size = 0;

	for (size_t i = 0; i < self->size; ++i) 
	{
		int chck = sfprint(self->mass[i], NULL, 0, NULL, 0, flag, width, precision);

		if (chck > 0)
			size += chck;
		else
			throw(IOException(), "Error: some error occured during printing!");

		if (i != self->size - 1)
			size++;
	}

	if (stream != NULL)
	{
		if (bin)
		{
			size = 0;
			
			for (size_t i = 0; i < self->size; ++i) 
			{
				int sz = sfprint(self->mass[i], stream, 1, NULL, 0, flag, width, precision);

				if (sz > 0)
					size += sz;
				else
					throw(IOException(), "Error: some error occured during printing!");
			}
		}
		else
		{
			for (size_t i = 0; i < self->size; ++i) 
			{
				int sz = sfprint(self->mass[i], stream, 0, NULL, 0, flag, width, precision);

				if (sz <= 0)
					throw(IOException(), "Error: some error occured during printing!");

				if (i != self->size - 1)
					fputc(' ', stream);
			}
		}
	}
	else if (buffer != NULL)
	{
		char *p = buffer;
		int psize;

		if (maxn < size + 1)
			psize = size + 1;
		else
			psize = maxn;

		for (size_t i = 0; i < self->size; ++i) 
		{
			int sz = sfprint(self->mass[i], NULL, 0, p, psize + 1, flag, width, precision);

			if (sz > 0)
			{
				psize -= sz;
				p += sz;

				if (i != self->size - 1)
				{
					*p++ = ' ';
					psize--;
				}
			}
			else
			{
				throw(IOException(), "Error: some error occured during printing!");
			}

			if (psize == 1)
				break;
		}

		*p = 0;
	}

	return size;
}

static int Array_sfscan(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	struct Array *self = cast(Array(), _self);

	int numb_res = 0;
	int result = 0;

	if (stream != NULL)
	{

	}
	else if (buffer != NULL)
	{

	}

	if (numb != NULL)
		*numb = numb_res;

	return result;
}

static void Array_push_back(void *_self, va_list *ap)
{
	struct Array *self = cast(Array(), _self);
	
	if (self->size >= self->capacity)
	{
		if (self->arrtype == ARRAY_GROWING)
			Array_growsize(self, 1);
		else
			throw(ArrayException(), "Error: Array capacity (%lu) was exceeded!",
					self->capacity);
	}

	size_t index = self->size++;

	self->mass[index] = vnew(self->datatype, ap);
}

static void Array_push_front(void *_self, va_list *ap)
{
	struct Array *self = cast(Array(), _self);

	if (self->size >= self->capacity)
	{
		if (self->arrtype == ARRAY_GROWING)
			Array_growsize(self, 1);
		else
			throw(ArrayException(), "Error: Array capacity (%lu) was exceeded!",
					self->capacity);
	}

	if (self->size != 0)
		memmove(self->mass + 1, self->mass, self->size * sizeof(void*));

	self->mass[0] = vnew(self->datatype, ap);
	self->size++;
}

static void Array_set_arrtype(void *_self, int arrtype)
{
	struct Array *self = cast(Array(), _self);

	if (arrtype != ARRAY_REGULAR && arrtype != ARRAY_GROWING)
		throw(ArrayException(), "Error: No such array type!");

	self->arrtype = arrtype;
}

static void Array_set_arrcap(void *_self, size_t capacity)
{
	struct Array *self = cast(Array(), _self);

	if (capacity <= self->capacity)
		return;

	size_t add = capacity - self->capacity;
	Array_growcap(self, add);
}

static void Array_get(const void *_self, va_list *ap)
{
	const struct Array *self = cast(Array(), _self);

	size_t index = va_arg(*ap, size_t);

	if (index >= self->capacity)
		throw(ArrayException(), "Error: Element at (%lu) is out of bounds!",
				index);

	if (self->mass[index] == NULL)
		throw(ArrayException(), "Error: Element at (%lu) doesn't exist (equals NULL)!",
				index);

	vget(self->mass[index], ap);
}

static void Array_get_size(const void *_self, va_list *ap)
{
	const struct Array *self = cast(Array(), _self);

	size_t *size = va_arg(*ap, size_t*);
	*size = self->size;
}

static void Array_sort(void *_self, va_list *ap)
{
	struct Array *self = cast(Array(), _self);

	if (self->size <= 1)
		return;

	int param = va_arg(*ap, int);

	if (param != ARRAY_SORT_REVERSED && param != ARRAY_SORT_NORMAL)
		param = ARRAY_SORT_NORMAL;

	const struct SortInterface *sort = icast(SortInterface, self->datatype);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				self->datatype->name);

	if (sort->swap.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'swap' method of 'SortInterface'!",
				self->datatype->name);

	quicksort_recursive(self->mass, 0, self->size - 1, (cmp_f) sort->cmp.method, (swap_f) sort->swap.method, param, ap);
}

#define CF_ENTRIES_THRESHOLD 16

static size_t Array_count_entries(const void *_self, va_list *ap)
{
	struct Array *self = cast(Array(), _self);

	if (self->size == 0)
		return 0;

	const void *entry = va_arg(*ap, const void*);
	const struct Class *entry_class = classOf(entry);

	if (entry_class != self->datatype)
		throw(ArrayException(), "Error: can't compare '%s' with '%s'!",
				entry_class->name, self->datatype->name);

	const struct SortInterface *sort = icast(SortInterface, self->datatype);

	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				self->datatype->name);

	size_t result = 0;

	if (self->size < CF_ENTRIES_THRESHOLD)
	{
		for (size_t i = 0; i < self->size; ++i)
		{
			if (((cmp_f) sort->cmp.method)(entry, self->mass[i], ap) == 0)
				result++;
		}
	} 
	else
	{
		for (size_t i = 0, j = self->size - 1; i <= j; ++i, --j)
		{
			if (((cmp_f) sort->cmp.method)(entry, self->mass[i], ap) == 0)
				result++;

			if (j == 0 || j == i)
				break;

			if (((cmp_f) sort->cmp.method)(entry, self->mass[j], ap) == 0)
				result++;
		}
	}

	return result;
}

static void* Array_find_entry(const void *_self, va_list *ap)
{
	struct Array *self = cast(Array(), _self);

	if (self->size == 0)
		return NULL;

	const void *entry = va_arg(*ap, const void*);
	const struct Class *entry_class = classOf(entry);

	if (entry_class != self->datatype)
		throw(ArrayException(), "Error: can't compare %s with %s!",
				entry_class->name, self->datatype->name);

	const struct SortInterface *sort = icast(SortInterface, self->datatype);
	
	if (sort->cmp.method == NULL)
		throw(SortException(), "Error: Class '%s' doesn't implement 'cmp' method of 'SortInterface'!",
				self->datatype->name);

	if (self->size < CF_ENTRIES_THRESHOLD)
	{
		for (size_t i = 0; i < self->size; ++i)
		{
			if (((cmp_f) sort->cmp.method)(entry, self->mass[i], ap) == 0)
				return self->mass[i];
		}
	}
	else
	{
		for (size_t i = 0, j = self->size - 1; i <= j; ++i, --j)
		{
			if (((cmp_f) sort->cmp.method)(entry, self->mass[i], ap) == 0)
				return self->mass[i];

			if (i == j)
				break;

			if (((cmp_f) sort->cmp.method)(entry, self->mass[j], ap) == 0)
				return self->mass[j];
		}
	}

	return NULL;
}

static void Array_reverse(void *_self)
{
	struct Array *self = cast(Array(), _self);

	if (self->size <= 1)
		return;

	size_t i, j;

	for (i = 0, j = self->size - 1; i < j; ++i, --j) 
	{
		void *tmp = self->mass[i];
		self->mass[i] = self->mass[j];
		self->mass[j] = tmp;
	}
}

/* }}} */

/* }}} */

/* Initialization {{{ */

ClassImpl(ArrayClass)
{
	if (!_ArrayClass)
	{
		_ArrayClass = new(Class(), "ArrayClass",
				Class(), sizeof(struct ArrayClass),
				ctor, ArrayClass_ctor,
				0);
	}

	return _ArrayClass;
}

ClassImpl(Array)
{
	if (!_Array)
	{
		_Array = new(ArrayClass(), "Array",
				Object(), sizeof(struct Array),
				ctor, Array_ctor,
				dtor, Array_dtor,
				cpy, Array_cpy,
				set, Array_set,
				get, Array_get,
				set_arrtype, Array_set_arrtype,
				set_arrcap, Array_set_arrcap,
				sort, Array_sort,
				get_size, Array_get_size,
				push_back, Array_push_back,
				push_front, Array_push_front,
				count_entries, Array_count_entries,
				find_entry, Array_find_entry,
				reverse, Array_reverse,
				sfprint, Array_sfprint,
				0);
	}

	return _Array;
}

/* Exception init */

ObjectImpl(ArrayException)
{
	if (!_ArrayException)
	{
		_ArrayException = new(ExceptionObject(), "ArrayException");
	}

	return _ArrayException;
}

/* }}} */
