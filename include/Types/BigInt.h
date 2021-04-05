#ifndef BIGINT_H_2ITI5UOH
#define BIGINT_H_2ITI5UOH

#include "Bitset.h"

ClassHeader(BigInt);
ObjectHeader(BigIntException);

struct BigInt
{
	struct Bitset _;
	unsigned int sign;
};

#endif /* end of include guard: BIGINT_H_2ITI5UOH */
