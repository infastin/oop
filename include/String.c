/* vim: set fdm=marker : */

#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Exception.h"
#include "ExceptionObject.h"
#include "IOInterface.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"
#include "String.h"
#include "TypeClass.h"

/* StringClass {{{ */

static void* StringClass_ctor(void *_self, va_list *ap)
{
	struct StringClass *self = super_ctor(StringClass(), _self, ap);

	init_method(self, string_len);

	struct IOInterface *self_io = go_to_field(self, struct StringClass, io);
	init_method(self_io, sfprint);
	init_method(self_io, sfscan);

	struct SortInterface *self_sort = go_to_field(self, struct StringClass, sort);
	init_method(self_sort, cmp);
	init_method(self_sort, swap);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	while ((selector = va_arg(ap_copy, voidf))) 
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) string_len)
			update_method(self, string_len, method);

		else if (selector == (voidf) sfprint)
			update_method(self_io, sfprint, method);
		else if (selector == (voidf) sfscan)
			update_method(self_io, sfscan, method);

		else if (selector == (voidf) cmp)
			update_method(self_sort, cmp, method);
		else if (selector == (voidf) swap)
			update_method(self_sort, swap, method);
	}

	va_end(ap_copy);
	return self;
}

/* Selectors {{{ */

size_t string_len(const void *_self)
{
	const struct Class *class = classOf(_self);
	const struct StringClass *sclass = cast(StringClass(), class);

	if (sclass->string_len.method == NULL)
		throw(StringException(), "Error: Class '%s' doesn't have method 'string_len'!",
				class->name);

	typedef size_t (*string_len_f)(const void *self);
	
	return ((string_len_f) sclass->string_len.method)(_self);
}

/* }}} */

/* }}} */

/* String {{{ */

static void* String_ctor(void *_self, va_list *ap)
{
	struct String *self = super_ctor(String(), _self, ap);

	char *str;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	str = va_arg(ap_copy, char*);
	
	if (str == NULL)
	{
		self->str = NULL;
		self->length = 0; 
	}
	else
	{
		self->str = strdup(str);
		self->length = strlen(str);
	}

	va_end(ap_copy);

	return self;
}

static void* String_cpy(const void *_self, void *_object)
{
	const struct String *self = cast(String(), _self);
	struct String *object = super_cpy(String(), _self, _object);

	object->length = self->length;
	
	if (self->str == NULL)
		object->str = NULL;
	else
		object->str = strdup(self->str);

	return object;
}

static void String_set(void *_self, va_list *ap)
{
	struct String *self = cast(String(), _self);

	char *str = va_arg(*ap, char*);

	if (self->str) 
		free(self->str);

	if (str == NULL)
	{
		self->str = NULL;
		self->length = 0; 
	}
	else
	{
		self->str = strdup(str);
		self->length = strlen(str);
	}
}

static int String_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn,
		int flag, int width, int precision)
{
	const struct String *self = cast(String(), _self);

	char *fmt = __getFmtPrint(flag, width, precision, "s");

	if (fmt)
	{
		int size;

		if (stream != NULL)
			size = fprintf(stream, fmt, self->str);
		else if (buffer != NULL)
			size = snprintf(buffer, maxn, fmt, self->str);
		else
			size = snprintf(NULL, 0, fmt, self->str);

		free(fmt);
		return size;
	}
	else
	{
		throw(FormatException(), "Error: could not allocate memory for the format string!");
	}

	return -1;
}

static int String_sfscan(const void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	struct String *self = cast(String(), _self);

	char *fmt;

	if (width != -1 || asterisk != -1)
		fmt = __getFmtScan(asterisk, width, "s");
	else
		fmt = __getFmtScan(asterisk, width, "ms");

	if (fmt)
	{
		if (self->str && width == -1 && asterisk == -1)
		{
			self->length = 0;
			free(self->str);
		}

		int size;

		if (stream != NULL)
		{
			int n;

			if (width == -1 || asterisk == -1)
				size = fscanf(stream, fmt, &self->str, &n);
			else
				size = fscanf(stream, fmt, self->str, &n);

			self->length = n;

			if (numb != NULL)
				*numb = n;
		}
		else if (buffer != NULL)
		{
			int n;

			if (width == -1 || asterisk == -1)
				size = sscanf(buffer, fmt, &self->str, &n);
			else
				size = sscanf(buffer, fmt, self->str, &n);

			self->length = n;

			if (numb != NULL)
				*numb = n;
		}
		else
			size = 0;

		free(fmt);
		return size;
	}
	else
	{
		throw(FormatException(), "Error: could not allocate memory for the format string!");
	}

	return -1;
}

static void String_get(void *_self, va_list *ap)
{
	struct String *self = cast(String(), _self);

	char **retval = va_arg(*ap, char**);

	*retval = self->str;
}

static void* String_dtor(void *_self)
{
	struct String *self = cast(String(), _self);

	if (self->str) free(self->str);

	return super_dtor(String(), _self);
}

static int String_cmp(const void *_self, const void *b, va_list *ap)
{
	const struct String *self = cast(String(), _self);
	const struct String *B = cast(String(), b);

	int param = va_arg(*ap, int);

	if (param < STRING_CMP_DEFAULT || param > STRING_CMP_LENGTH)
		param = STRING_CMP_DEFAULT;

	switch (param) {
		case STRING_CMP_LENGTH:
			return self->length - B->length;
		case STRING_CMP_DEFAULT:
		default:
			return strcmp(self->str, B->str);
	}
}

static void String_swap(void *_self, void *b)
{
	struct String *self = cast(String(), _self);
	struct String *B = cast(String(), b);

	size_t tmp_len = self->length;
	self->length = B->length;
	B->length = tmp_len;

	char *tmp = self->str;
	self->str = B->str;
	B->str = tmp;
}

static size_t String_string_len(const void *_self)
{
	const struct String *self = cast(String(), _self);

	return self->length;
}

/* }}} */

/* Initialization {{{ */

ClassImpl(StringClass)
{
	if (!_StringClass)
	{
		_StringClass = new(Class(), "StringClass",
				Class(), sizeof(struct StringClass),
				ctor, StringClass_ctor,
				0);
	}

	return _StringClass;
}

ClassImpl(String)
{
	if (!_String)
	{
		_String = new(StringClass(), "String",
				Object(), sizeof(struct String),
				ctor, String_ctor,
				dtor, String_dtor,
				cpy, String_cpy,
				set, String_set,
				get, String_get,
				sfprint, String_sfprint,
				sfscan, String_sfscan,
				cmp, String_cmp,
				swap, String_swap,
				string_len, String_string_len,
				0);
	}

	return _String;
}

/* Exception init */

ObjectImpl(StringException)
{
	if (!_StringException)
	{
		_StringException = new(ExceptionObject(), "StringException");
	}

	return _StringException;
}

/* }}} */
