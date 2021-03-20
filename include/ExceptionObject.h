#ifndef EXCEPTIONOBJECT_H_OQI38Z0M
#define EXCEPTIONOBJECT_H_OQI38Z0M

#include "Object.h"
#include <stddef.h>

ClassHeader(ExceptionObject);

struct ExceptionObject
{
	const struct Object _;
	char *name;
	char *msg;
};

char* exception_message(const void *self);
char* exception_name(const void *self);

#endif /* end of include guard: EXCEPTIONOBJECT_H_OQI38Z0M */
