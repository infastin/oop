#ifndef ARRAY_H_X5J84X7L
#define ARRAY_H_X5J84X7L

#include "Macro.h"
#include "Object.h"
#include "Selectors.h"
#include "CollectionInterface.h"
#include "IOInterface.h"

ClassHeader(ArrayClass);
ClassHeader(Array);
ObjectHeader(ArrayException);

enum
{
	ARRAY_REGULAR,
	ARRAY_GROWING
};

enum
{
	ARRAY_SORT_NORMAL,
	ARRAY_SORT_REVERSED
};

struct ArrayClass
{
	const struct Class _;
	method set_arrtype;
	method set_arrcap;

	const struct CollectionInterface ci;
	const struct IOInterface io;
};

struct Array
{
	const struct Object _;
	const struct Class *datatype;
	
	int    arrtype;
	size_t size;
	size_t capacity;

	void **mass;
};

void set_arrtype(void *_self, int arrtype);
void set_arrcap(void *_self, size_t capacity);


#endif /* end of include guard: ARRAY_H_X5J84X7L */
