#ifndef LIST_H_AK4HWINJ
#define LIST_H_AK4HWINJ

#include "CollectionInterface.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"

ClassHeader(ListClass);
ClassHeader(List);
ObjectHeader(ListException);

struct ListClass
{
	const struct Class _;
	
	const struct CollectionInterface ci;
};

struct ListNode
{
	struct ListNode *next;
	struct ListNode *prev;

	void *value;
};

struct List
{
	const struct Object _;
	const struct Class *datatype;

	size_t size;
	struct ListNode *first;
	struct ListNode *last;
};

#endif /* end of include guard: LIST_H_AK4HWINJ */
