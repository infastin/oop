#ifndef COLLECTIONINTERFACE_H_5SR2JXWQ
#define COLLECTIONINTERFACE_H_5SR2JXWQ

#include "Macro.h"
#include "Object.h"

InterfaceHeader(CollectionInterface);
ObjectHeader(CollectionException);

enum
{
	ADD_BY_INDEX,
	ADD_BY_VALUE,
};

struct CollectionInterface
{
	method sort;
	method get_size;
	method push_back;
	method push_front;
	method add_after;
	method add_before;
	method count_entries;
	method find_entry;
	method reverse;
};

void get_size(const void *self, ...);
void sort(void *self, ...);
void push_back(void *self, ...);
void push_front(void *self, ...);

void add_after(void *self, int by_type, ...);
void add_before(void *self, int by_type, ...);

void add_after_index(void *self, ...);
void add_after_value(void *self, ...);
void add_before_index(void *self, ...);
void add_before_value(void *self, ...);

size_t count_entries(const void *self, ...);
void*  find_entry(const void *self, ...);
void   reverse(void *self);

#endif /* end of include guard: COLLECTIONINTERFACE_H_5SR2JXWQ */
