#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "Bitset.h"
#include "Exception.h"
#include "Object.h"
#include "Selectors.h"

#define BYTES(bits) (((bits) + CHAR_BIT - 1) / CHAR_BIT)

/*
 * Private methods and macros
 */

static void Bitset_add(void *_self, size_t bit, int value)
{
	struct Bitset *self = _self;

	if (bit >= self->capacity * CHAR_BIT)
		bitset_grow(_self, bit + 1);

	int val = bitset_get(self, bit);

	if (val + value >= 2)
		Bitset_add(self, bit + 1, 1);

	bitset_set(self, bit, (val + value) % 2);
}

static void Bitset_sub(void *_self, size_t bit, int value)
{
	struct Bitset *self = _self;

	if (bit >= self->length)
		throw(BitsetException(), "Error: No such bit with given index (%u) in given bitset! (Max: %u)",
				bit, self->length - 1);

	int val = bitset_get(self, bit);

	if (value > val)
	{
		Bitset_sub(self, bit + 1, 1);
		bitset_set(self, bit, 1);
	}
	else
		bitset_set(self, bit, val - value);
}

/*
 * Methods
 */

static void* Bitset_ctor(void *_self, va_list *ap)
{
	struct Bitset *self = super_ctor(Bitset(), _self, ap);

	size_t numb = va_arg(*ap, size_t);

	if (numb == 0)
	{
		delete(self);
		throw(BitsetException(), "Error: Can't create bitset with zero bits!");
	}

	self->capacity = BYTES(numb);
	self->length = 0;
	self->bytes = (char*)calloc(sizeof(char), self->capacity);

	if (self->bytes == NULL)
	{
		delete(self);
		throw(BitsetException(), "Fatal Error: Bitset allocation error!");
	}

	return self;
}

static void* Bitset_dtor(void *_self)
{
	struct Bitset *self = cast(Bitset(), _self);

	free(self->bytes);

	return super_dtor(Bitset(), _self);
}

static void* Bitset_cpy(const void *_self, void *_object)
{
	const struct Bitset *self = _self;
	struct Bitset *object = super_cpy(Bitset(), _self, _object);

	object->capacity = self->capacity;
	object->length = self->length;
	object->bytes = (char*)calloc(sizeof(char), object->capacity);

	if (object->bytes == NULL)
	{
		delete(object);
		throw(BitsetException(), "Fatal Error: Bitset allocation error!");
	}

	for (size_t i = 0; i < object->capacity; i++)
	{
		object->bytes[i] = self->bytes[i];
	}

	return object;
}

static void Bitset_set(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	size_t bit = va_arg(*ap, size_t);
	int value = va_arg(*ap, int);

	if (bit >= self->capacity * CHAR_BIT)
		throw(BitsetException(), "Error: Bit at (%u) is out of bounds! (Max: %u)",
				bit, self->capacity * CHAR_BIT - 1);

	if (value != 0 && value != 1)
		throw(BitsetException(), "Error: Bit can't be equal '%u'! Only 0 and 1 allowed!",
				value);

	bitset_set(self, bit, value);
}

static void Bitset_get(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	size_t bit = va_arg(*ap, size_t);
	int *val = va_arg(*ap, int*);

	if (bit >= self->length)
		throw(BitsetException(), "Error: No such bit with given index (%u) in given bitset! (Max: %u)",
				bit, self->length - 1);

	*val = bitset_get(self, bit);
}

static int Bitset_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct Bitset *self = cast(Bitset(), _self);

	int size = self->capacity * CHAR_BIT + self->capacity - 1;

	if (stream != NULL)
	{
		if (bin)
		{
			size = 0;

			for (size_t i = self->capacity - 1; i >= 0; --i)
			{
				int sz = fwrite(&self->bytes[i], 1, 1, stream);

				if (sz != 1)
					throw(BitsetException(), "Error: some error occured during printing!");

				size += sz;

				if (i == 0)
					break;
			}
		}
		else
		{
			for (size_t i = self->capacity - 1; i >= 0; --i)
			{
				for (int j = CHAR_BIT - 1; j >= 0; --j)
				{
					int bit = (self->bytes[i] & (1 << j)) ? 1 : 0;
					
					fprintf(stream, "%d", bit);
				}

				if (i != 0)
					fputc(' ', stream);

				if (i == 0)
					break;
			}
		}
	}
	else if (buffer != NULL)
	{
		char *p = buffer;
		int psize;

		if (maxn < size + 1)
			psize = size + 1;
		else
			psize = maxn;

		for (size_t i = self->capacity - 1; i >= 0; --i)
		{
			for (int j = CHAR_BIT - 1; j >= 0 && psize != 1; --j)
			{
				int bit = (self->bytes[i] & (1 << j)) ? 1 : 0;
				int sz = snprintf(buffer, maxn, "%d", bit);

				if (sz < 0)
					throw(BitsetException(), "Error: some error occured during printing!");

				p += sz;
				psize -= sz;

				if (j == 0)
					break;
			}

			if (psize == 1)
				break;

			if (i != 0)
			{
				*p++ = ' ';
				psize--;
			}

			if (i == 0)
				break;
		}

		*p = 0;
	}

	return size;
}

static int Bitset_cmp(const void *_self, const void *b)
{
	const struct Bitset *self = cast(Bitset(), _self);
	const struct Bitset *B = cast(Bitset(), b);

	if (self->length > B->length)
		return 1;
	
	if (self->length < B->length)
		return -1;

	if (self->length == 0)
		return 0;

	for (size_t i = self->length - 1; i >= 0; --i)
	{
		int val1 = bitset_get(self, i);
		int val2 = bitset_get(B, i);

		if (val1 > val2)
			return 1;
		else if (val1 < val2)
			return -1;

		if (i == 0)
			break;
	}

	return 0;
}

static void Bitset_scadd(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	size_t bit = va_arg(*ap, size_t);
	int value = va_arg(*ap, int);

	if (value != 0 && value != 1)
		throw(BitsetException(), "Error: Bit can't be equal '%u'! Only 0 and 1 allowed!",
				value);

	Bitset_add(self, bit, value);
}

static void Bitset_scsub(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	size_t bit = va_arg(*ap, size_t);
	int value = va_arg(*ap, int);

	if (value != 0 && value != 1)
		throw(BitsetException(), "Error: Bit can't be equal '%u'! Only 0 and 1 allowed!",
				value);

	Bitset_sub(self, bit, value);
}

void Bitset_onecompl(void *_self)
{
	struct Bitset *self = cast(Bitset(), _self);

	size_t bits = self->length % CHAR_BIT;

	if (bits <= 4)
	{
		size_t bytes = BYTES(self->length) - 1;

		for (size_t i = 0; i < bytes; ++i)
		{
			self->bytes[i] = ~self->bytes[i];
		}

		for (size_t i = bytes * CHAR_BIT; i < self->length; ++i)
		{
			int val = bitset_get(self, i);
			bitset_set(self, i, (val == 0) ? 1 : 0);
		}
	}
	else
	{
		for (size_t i = 0; i < BYTES(self->length); ++i)
		{
			self->bytes[i] = ~self->bytes[i];
		}

		for (size_t i = self->length; i < CHAR_BIT; ++i)
		{
			int val = bitset_get(self, i);
			bitset_set(self, i, (val == 0) ? 1 : 0);
		}
	}
}

static void* Bitset_sum(const void *_self, const void *b)
{
	const struct Bitset *self = cast(Bitset(), _self);
	const struct Bitset *B = cast(Bitset(), b);

	const struct Bitset *higher, *lower;

	if (Bitset_cmp(self, b) > 0)
	{
		higher = self;
		lower = B;
	}
	else
	{
		higher = B;
		lower = self;
	}

	struct Bitset *result = new(Bitset(), higher->length);

	memcpy(result->bytes, higher->bytes, BYTES(higher->length));
	result->length = higher->length;

	for (size_t i = 0; i < lower->length; ++i)
	{
		int val = bitset_get(B, i);
		Bitset_add(result, i, val);
	}

	return result;
}

static void* Bitset_subtract(const void *_self, const void *b)
{
	const struct Bitset *self = cast(Bitset(), _self);
	const struct Bitset *B = cast(Bitset(), b);

	const struct Bitset *higher, *lower;

	int first;

	if (Bitset_cmp(self, b) > 0)
	{
		first = 1;
		higher = self;
		lower = B;
	}
	else 
	{
		first = 0;
		higher = B;
		lower = self;
	}

	struct Bitset *result = new(Bitset(), higher->length);

	memcpy(result->bytes, higher->bytes, BYTES(higher->length));
	result->length = higher->length;

	for (size_t i = 0; i < lower->length; ++i)
	{
		int val = bitset_get(lower, i);
		Bitset_sub(result, i, val);
	}

	if (first == 0)
		Bitset_onecompl(result);

	return result;
}

void Bitset_lshift(void *_self, size_t shift)
{
	struct Bitset *self = cast(Bitset(), _self);

	if (shift == 0)
		return;

	if (BYTES(self->length + shift) > self->capacity)
		bitset_grow(self, self->length + shift);

	int moved_to_next_bytes = 0;
	if (BYTES(self->length + shift) > BYTES(self->length))
		moved_to_next_bytes = 1;

	if (shift <= 7)
	{
		char bits[2][shift];

		size_t i = 0;
		for (; i < BYTES(self->length); ++i)
		{
			if (i < BYTES(self->length) - 1 || moved_to_next_bytes)
			{
				for (size_t j = CHAR_BIT - shift; j < CHAR_BIT; ++j) 
				{
					bits[i % 2][j - (CHAR_BIT - shift)] = (self->bytes[i] & (1 << j)) ? 1 : 0;
				}
			}

			self->bytes[i] <<= shift;

			if (i > 0)
			{
				for (size_t j = 0; j < shift; ++j)
				{
					if (bits[(i - 1) % 2][j] == 0)
						self->bytes[i] &= ~(1 << j);
					else
						self->bytes[i] |= 1 << j;
				}
			}
		}

		if (moved_to_next_bytes)
		{
			for (size_t j = 0; j < shift; ++j)
			{
				if (bits[(i - 1) % 2][j] == 0)
					self->bytes[i] &= ~(1 << j);
				else
					self->bytes[i] |= 1 << j;			
			}
		}

		self->length += shift;
	}
	else
	{
		memmove(self->bytes + (shift / CHAR_BIT), self->bytes, BYTES(self->length));
		memset(self->bytes, 0, shift / CHAR_BIT);

		self->length += (shift / CHAR_BIT) * CHAR_BIT;

		if (shift % CHAR_BIT != 0)
			Bitset_lshift(self, shift % CHAR_BIT);
	}
}

void Bitset_rshift(void *_self, size_t shift)
{
	struct Bitset *self = cast(Bitset(), _self);

	if (shift == 0)
		return;

	if (shift <= 7)
	{
		char bits[2][shift];

		size_t i = BYTES(self->length) - 1;
		for (; i >= 0; --i)
		{
			if (i > 0)
			{
				for (size_t j = 0; j < shift; ++j)
				{
					bits[i % 2][j] = (self->bytes[i] & (1 << j)) ? 1 : 0;
				}
			}

			self->bytes[i] >>= shift;

			if (i < BYTES(self->length) - 1)
			{
				for (size_t j = CHAR_BIT - shift; j < CHAR_BIT; ++j)
				{
					if (bits[(i + 1) % 2][j - (CHAR_BIT - shift)] == 0)
						self->bytes[i] &= ~(1 << j);
					else
						self->bytes[i] |= 1 << j;
				}
			}

			if (i == 0)
				break;
		}

		self->length -= shift;
	}
	else
	{
		if (shift < self->length)
		{
			memmove(self->bytes, self->bytes + (shift / CHAR_BIT), BYTES(self->length) - (shift / CHAR_BIT));
			memset(self->bytes + BYTES(self->length) - (shift / CHAR_BIT), 0, shift / CHAR_BIT);

			self->length -= (shift / CHAR_BIT) * CHAR_BIT;

			if (shift % CHAR_BIT != 0)
				Bitset_rshift(self, shift % CHAR_BIT);
		}
		else
		{
			memset(self->bytes, 0, self->length);
			self->length = 0;
		}
	}
}

static void* Bitset_product(const void *_self, const void *b)
{
	const struct Bitset *self = cast(Bitset(), _self);
	const struct Bitset *B = cast(Bitset(), b);

	const struct Bitset *higher, *lower;

	if (self->capacity > B->capacity)
	{
		higher = self;
		lower = B;
	}
	else
	{
		higher = B;
		lower = self;
	}

	void *result = new(Bitset(), higher->capacity * CHAR_BIT);


}

/*
 * Selectors
 */

// ---

void bitset_grow(void *_self, size_t numb)
{
	struct Bitset *self = cast(Bitset(), _self);

	if (numb <= self->capacity * CHAR_BIT)
		return;

	int min_numb = BYTES(numb);

	self->capacity = min_numb;
	self->bytes = (char*)realloc(self->bytes, sizeof(char) * min_numb);

	if (self->bytes == NULL)
		throw(BitsetException(), "Fatal Error: Bitset reallocation error!");
}

/*
 * Initialization
 */

// ---

ClassImpl(Bitset)
{
	if (!_Bitset)
	{
		_Bitset = new(TypeClass(), "Bitset",
				Object(), sizeof(struct Bitset),
				ctor, Bitset_ctor,
				dtor, Bitset_dtor,
				cmp, Bitset_cmp,
				cpy, Bitset_cpy,
				set, Bitset_set,
				get, Bitset_get,
				scadd, Bitset_scadd,
				scsub, Bitset_scsub,
				sum, Bitset_sum,
				subtract, Bitset_subtract,
				onecompl, Bitset_onecompl,
				lshift, Bitset_lshift,
				rshift, Bitset_rshift,
				sfprint, Bitset_sfprint,
				0);
	}

	return _Bitset;
}

/*
 * Exception Initialization
 */

// ---

ObjectImpl(BitsetException)
{
	if (!_BitsetException)
	{
		_BitsetException = new(ExceptionObject(), "BitsetException");
	}

	return _BitsetException;
}
