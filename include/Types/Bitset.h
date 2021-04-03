#ifndef BITSET_H_FESPW72B
#define BITSET_H_FESPW72B

#include "TypeClass.h"

ClassHeader(Bitset);
ObjectHeader(BitsetException);

struct Bitset
{
	const struct Object _;
	char *bytes;
	unsigned int bnumb;
};

void bitset_grow(void *self, unsigned int numb);

#endif /* end of include guard: BITSET_H_FESPW72B */
