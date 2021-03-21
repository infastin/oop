#ifndef EXCEPTIONOBJECT_H_OQI38Z0M
#define EXCEPTIONOBJECT_H_OQI38Z0M

#include "Object.h"

ClassHeader(ExceptionObject);

struct ExceptionObject
{
	const struct Object _;
	char *name;
	char *msg;
	char *file;
	int   line;
	const char *func;
};

char* exception_message(const void *self);
char* exception_name(const void *self);

#endif /* end of include guard: EXCEPTIONOBJECT_H_OQI38Z0M */
