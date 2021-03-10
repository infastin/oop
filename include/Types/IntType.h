#ifndef INTTYPE_H_ITLFXA2E
#define INTTYPE_H_ITLFXA2E

#include "TypeClass.h"

extern const void* Int;

struct IntType
{
	const struct Object _;
	int value;
};

void initIntType(void);

#endif /* end of include guard: INTTYPE_H_ITLFXA2E */
