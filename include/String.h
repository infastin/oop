#ifndef STRING_H_MFFCDXNP
#define STRING_H_MFFCDXNP

#include <stdarg.h>

#include "IOInterface.h"
#include "Selectors.h"
#include "SortInterface.h"

ClassHeader(StringClass);
ClassHeader(String);
ObjectHeader(StringException);

enum
{
	STRING_CMP_DEFAULT,
	STRING_CMP_LENGTH
};

struct StringClass
{
	const struct Class _;

	method string_len;

	const struct IOInterface io;
	const struct SortInterface sort;
};

struct String
{
	const struct Object _;

	size_t length;
	char *str;
};

size_t string_len(const void *_self);

#endif /* end of include guard: STRING_H_MFFCDXNP */
