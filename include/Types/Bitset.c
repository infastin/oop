#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Bitset.h"
#include "Exception.h"
#include "Selectors.h"

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

	self->bnumb = (numb + CHAR_BIT - 1) / CHAR_BIT;
	self->bytes = (char*)calloc(sizeof(char), self->bnumb);

	if (self->bytes == NULL)
	{
		delete(self);
		throw(BitsetException(), "Fatal Error: Bitset allocation error!");
	}

	return self;
}

static void* Bitset_cpy(const void *_self, void *_object)
{
	const struct Bitset *self = _self;
	struct Bitset *object = super_cpy(Bitset(), _self, _object);

	object->bnumb = self->bnumb;
	object->bytes = (char*)calloc(sizeof(char), object->bnumb);

	if (object->bytes == NULL)
	{
		delete(object);
		throw(BitsetException(), "Fatal Error: Bitset allocation error!");
	}

	for (unsigned int i = 0; i < object->bnumb; i++)
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

	if (bit >= self->bnumb * CHAR_BIT)
		throw(BitsetException(), "Error: Bit at (%u) is out of bounds! (Max: %u)",
				bit, self->bnumb * CHAR_BIT - 1);

	if (value != 1 && value != 0)
		throw(BitsetException(), "Error: Bit can't be equal '%u'! Only 0 and 1 allowed!",
				value);

	unsigned int bytenumb = bit / CHAR_BIT;
	unsigned int bitnumb = bit % CHAR_BIT;

	if (value == 0)
		self->bytes[bytenumb] &= ~(1 << bitnumb);
	else
		self->bytes[bytenumb] |= 1 << bitnumb;
}

static void Bitset_get(void *_self, va_list *ap)
{
	struct Bitset *self = cast(Bitset(), _self);

	unsigned int bit = va_arg(*ap, unsigned int);
	unsigned int *val = va_arg(*ap, unsigned int*);

	if (bit >= self->bnumb * CHAR_BIT)
		throw(BitsetException(), "Error: No such bit with given index (%u) in given bitset! (Max: %u)",
				bit, self->bnumb * CHAR_BIT - 1);

	unsigned int bytenumb = bit / CHAR_BIT;
	unsigned int bitnumb = bit % CHAR_BIT;

	*val = (self->bytes[bytenumb] & (1 << bitnumb)) ? 1 : 0;
}

static int Bitset_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct Bitset *self = cast(Bitset(), _self);

	int size = self->bnumb * CHAR_BIT + self->bnumb - 1;

	if (stream != NULL)
	{
		if (bin)
		{
			size = 0;

			for (unsigned int i = self->bnumb - 1; i >= 0; --i)
			{
				int sz = fwrite(&self->bytes[i], 1, 1, stream);

				if (sz != 1)
					throw(BitsetException(), "Error: some error occured during printing!");

				size += sz;
			}
		}
		else
		{
			for (unsigned int i = self->bnumb - 1; i >= 0; --i)
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

		for (unsigned int i = self->bnumb - 1; i >= 0; --i)
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

/*
 * Selectors
 */

// ---

void bitset_grow(void *_self, unsigned int numb)
{
	struct Bitset *self = cast(Bitset(), _self);

	if (numb <= self->bnumb * CHAR_BIT)
		return;

	int min_numb = (numb + CHAR_BIT - 1) / CHAR_BIT;;

	self->bnumb = min_numb;
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
				cpy, Bitset_cpy,
				set, Bitset_set,
				get, Bitset_get,
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
