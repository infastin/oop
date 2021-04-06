#ifndef BITSET_H_FESPW72B
#define BITSET_H_FESPW72B

/**
 * @file IntType.h
 * @brief Bitset
 * @author infastin
 * @version 1.0
 * @date 2021-04-04
 */

#include "TypeClass.h"

ClassHeader(Bitset);
ObjectHeader(BitsetException);

struct Bitset
{
	const struct Object _;
	char *bytes;
	size_t capacity;
	size_t length;
};

/**
 * @brief Grow Bitset to numb of bits
 *
 * @param self Bitset
 * @param numb Number of bits to grow
 */
void bitset_grow(void *self, size_t numb);

/**
 * @brief Unsafe version of Bitset getter
 *
 * @param self Bitset
 * @param bit  Which bit to get
 *
 * @return 0/1
 */
#define bitset_get(self, bit) ((self->bytes[(bit) / CHAR_BIT] & (1 << ((bit) % CHAR_BIT))) ? 1 : 0)

/**
 * @brief Unsafe version of Bitset setter
 *
 * @param self Bitset
 * @param bit  Which bit to set
 * @param val  What value to set
 */
#define bitset_set(self, bit, val) 										\
	if ((val) == 0) 													\
		self->bytes[(bit) / CHAR_BIT] &= ~(1 << ((bit) % CHAR_BIT)); 	\
	else 																\
	{ 																	\
		self->bytes[(bit) / CHAR_BIT] |= 1 << ((bit) % CHAR_BIT); 		\
		if ((bit) >= self->length && (val) == 1) 						\
			self->length = (bit) + 1; 									\
	}


#endif /* end of include guard: BITSET_H_FESPW72B */
