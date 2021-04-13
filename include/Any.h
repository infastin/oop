#ifndef ANY_H_YOFXU5U1
#define ANY_H_YOFXU5U1

#include <stddef.h>

#include "Object.h"

ClassHeader(Any);
ObjectHeader(AnyException);

struct Any
{
	struct Object _;
	
	size_t size;
	char *data;
};

#endif /* end of include guard: ANY_H_YOFXU5U1 */
