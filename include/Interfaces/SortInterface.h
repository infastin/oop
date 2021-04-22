#ifndef SORTINTERFACE_H_YOFVKSIZ
#define SORTINTERFACE_H_YOFVKSIZ

#include "Macro.h"
#include "Object.h"

InterfaceHeader(SortInterface);
ObjectHeader(SortException);

struct SortInterface
{
	method cmp;
	method swap;
};

int cmp(const void *_self, const void *b);
void swap(void *_self, void *b);

#endif /* end of include guard: SORTINTERFACE_H_YOFVKSIZ */
