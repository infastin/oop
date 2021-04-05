#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Bitset.h"
#include "Exception.h"
#include "Selectors.h"

/*
 * Private methods and macros
 */

static void Bitset_add(void *_self, unsigned int bit, unsigned int value)
{
	struct Bitset *self = _self;

	if (bit >= self->capacity * CHAR_BIT)
		bitset_grow(_self, bit + 1);

	unsigned int val = bitset_get(self, bit);

	if (val + value >= 2)
		Bitset_add(_self, bit + 1, 1);

	bitset_set(self, bit, (val + value) % 2);
}

/*
 * Methods
 */

static void* Bitset_ctor(void *_self, va_list *ap)
{
	struct Bitset *self = super_ctor(Bitset(), _self, ap);

	unsigned int numb = va_arg(*ap, unsigned int);

	if (numb == 0)
	{
		delete(self);
		throw(BitsetException(), "Error: Can't create bitset with zero bits!");
	}

	self->capacity = (numb + CHAR_BIT - 1) / CHAR_BIT;
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
	object->bytes = (char*)calloc(sizeof(char), object->capacity);

	if (object->bytes == NULL)
	{
		delete(object);
		throw(BitsetException(), "Fatal Error: Bitset allocation error!");
	}

	for (unsigned int i = 0; i < object->capacity; i++)
	{
		object->bytes[i] = self->bytes[i];
	}

	return object;
}

static void Bitset_set(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	unsigned int bit = va_arg(*ap, unsigned int);
	unsigned int value = va_arg(*ap, unsigned int);

	if (bit >= self->capacity * CHAR_BIT)
		throw(BitsetException(), "Error: Bit at (%u) is out of bounds! (Max: %u)",
				bit, self->capacity * CHAR_BIT - 1);

	if (value > 1)
		throw(BitsetException(), "Error: Bit can't be equal '%u'! Only 0 and 1 allowed!",
				value);

	bitset_set(self, bit, value);
}

static void Bitset_get(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	unsigned int bit = va_arg(*ap, unsigned int);
	unsigned int *val = va_arg(*ap, unsigned int*);

	if (bit >= self->capacity * CHAR_BIT)
		throw(BitsetException(), "Error: No such bit with given index (%u) in given bitset! (Max: %u)",
				bit, self->capacity * CHAR_BIT - 1);

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

			for (unsigned int i = self->capacity - 1; i >= 0; --i)
			{
				int sz = fwrite(&self->bytes[i], 1, 1, stream);

				if (sz != 1)
					throw(BitsetException(), "Error: some error occured during printing!");

				size += sz;
			}
		}
		else
		{
			for (unsigned int i = self->capacity - 1; i >= 0; --i)
			{
				for (unsigned int j = CHAR_BIT - 1; j >= 0; --j)
				{
					unsigned int bit = (self->bytes[i] & (1 << j)) ? 1 : 0;
					
					fprintf(stream, "%d", bit);

					if (j == 0)
						break;
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

		for (unsigned int i = self->capacity - 1; i >= 0; --i)
		{
			for (unsigned int j = CHAR_BIT - 1; j >= 0 && psize != 1; --j)
			{
				unsigned int bit = (self->bytes[i] & (1 << j)) ? 1 : 0;
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

	for (unsigned int i = self->length - 1; i >= 0; --i)
	{
		unsigned int val1 = bitset_get(self, i);
		unsigned int val2 = bitset_get(B, i);

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

	unsigned int bit = va_arg(*ap, unsigned int);
	unsigned int value = va_arg(*ap, unsigned int);

	if (value > 1)
		throw(BitsetException(), "Error: Bit can't be equal '%u'! Only 0 and 1 allowed!",
				value);

	Bitset_add(self, bit, value);
}

static void* Bitset_sum(const void *_self, const void *b)
{
	const struct Bitset *self = cast(Bitset(), _self);
	const struct Bitset *B = cast(Bitset(), b);

	const struct Bitset *higher;
	const struct Bitset *lower;

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

	for (unsigned int i = 0; i < lower->capacity * CHAR_BIT; ++i)
	{
		unsigned int val1 = bitset_get(self, i);
		unsigned int val2 = bitset_get(B, i);

		if (val1 + val2 >= 2)
			Bitset_add(result, i + 1, 1);

		Bitset_add(result, i, (val1 + val2) % 2);
	}

	if (higher->capacity != lower->capacity)
	{
		for (unsigned int i = lower->capacity * CHAR_BIT; i < higher->capacity * CHAR_BIT; ++i)
		{
			unsigned int val = bitset_get(self, i);

			Bitset_add(result, i, val);
		}
	}

	return result;
}

/*
 * Selectors
 */

// ---

void bitset_grow(void *_self, unsigned int numb)
{
	struct Bitset *self = cast(Bitset(), _self);

	if (numb <= self->capacity * CHAR_BIT)
		return;

	int min_numb = (numb + CHAR_BIT - 1) / CHAR_BIT;;

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
				sum, Bitset_sum,
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
