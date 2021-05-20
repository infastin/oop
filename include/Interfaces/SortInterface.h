#ifndef SORTINTERFACE_H_YOFVKSIZ
#define SORTINTERFACE_H_YOFVKSIZ

#include <stdarg.h>

#include "Macro.h"
#include "Object.h"

InterfaceHeader(SortInterface);
ObjectHeader(SortException);

struct SortInterface
{
	method cmp;
	method swap;
};

int cmp(const void *_self, const void *b, ...);
int vcmp(const void *_self, const void *b, va_list *ap);
void swap(void *_self, void *b);

#endif /* end of include guard: SORTINTERFACE_H_YOFVKSIZ */
