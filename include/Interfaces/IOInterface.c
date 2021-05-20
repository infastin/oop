/* vim: set fdm=marker : */

#include <stdlib.h>

#include "Exception.h"
#include "ExceptionObject.h"
#include "IO.h"
#include "IOInterface.h"
#include "Macro.h"
#include "Object.h"
#include "Selectors.h"

/* "Private" functions {{{ */

char* __getFmtPrint(int flag, int width, int precision, char *spec)
{
	int fmt_size = 1;

	// Getting format size
	for (char *sp = spec; *sp != 0; fmt_size++, sp++);

	if (flag != -1)
		fmt_size++;

	if (width != -1)
		fmt_size += snprintf(NULL, 0, "%d", width);

	if (precision != -1)
		fmt_size += snprintf(NULL, 0, "%d", precision) + 1;

	// Getting format
	char *fmt = (char*)calloc(sizeof(char), fmt_size + 1);

	if (fmt)
	{
		*fmt = '%';

		char *p = fmt + 1;
		int psize = fmt_size - 1;

		if (flag != -1)
		{
			*p++ = flag;
			psize--;
		}

		if (width != -1)
		{
			int widthN = snprintf(p, psize + 1, "%d", width);
			p += widthN;
			psize -= widthN;
		}

		if (precision != -1)
		{
			int precisionN = snprintf(p, psize + 1, ".%d", precision);
			p += precisionN;
			psize -= precisionN;
		}

		for (char *sp = spec; *sp != 0 && psize > 0; sp++)
		{
			*p++ = *sp;
			psize--;
		}

		*p = 0;
	}

	return fmt;
}

char* __getFmtScan(int asterisk, int width, char *spec)
{
	int fmt_size = 3;

	// Getting format size
	for (char *sp = spec; *sp != 0; fmt_size++, sp++);

	if (asterisk != -1)
		fmt_size++;

	if (width != -1)
		fmt_size += snprintf(NULL, 0, "%d", width);

	// Getting format
	char *fmt = (char*)calloc(sizeof(char), fmt_size + 1);

	if (fmt)
	{
		*fmt = '%';

		char *p = fmt + 1;
		int psize = fmt_size - 1;

		if (asterisk != -1)
		{
			*p++ = asterisk;
			psize--;
		}

		if (width != -1)
		{
			int widthN = snprintf(p, psize + 1, "%d", width);
			p += widthN;
			psize -= widthN;
		}

		for (char *sp = spec; *sp != 0 && psize > 0; sp++)
		{
			*p++ = *sp;
			psize--;
		}

		*p++ = '%';
		*p++ = 'n';

		*p = 0;
	}

	return fmt;
}

/* }}} */

/* Selectors {{{ */

int sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision)
{
	const struct Class *class = classOf(_self);
	const struct IOInterface *io = icast(IOInterface, class);

	if (io->sfprint.method == NULL)
		throw(IOException(), "Error: Class '%s' doesn't implement 'sfprint' method of 'IOInterface'!",
				class->name);

	typedef int   (*sfprint_f)(const void *self, FILE *stream, int bin, char *buffer, size_t maxn, 
			int flag, int width, int precision);

	return ((sfprint_f) io->sfprint.method)(_self, stream, bin, buffer, maxn, flag, width, precision);
}

int sfscan(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	const struct Class *class = classOf(_self);
	const struct IOInterface *io = icast(IOInterface, class);

	if (io->sfscan.method == NULL)
		throw(IOException(), "Error: Class '%s' doesn't implement 'sfscan' method of 'IOInterface'!",
				class->name);


	typedef int   (*sfscan_f)(void *self, FILE *stream, int bin, const char *buffer, int *numb, 
			int asterisk, int width);

	return ((sfscan_f) io->sfscan.method)(_self, stream, bin, buffer, numb, asterisk, width);
}

/* }}} */

/* Initialization {{{ */

InterfaceImpl(IOInterface)
{
	if (!_IOInterface)
	{
		_IOInterface = inew("IOInterface", 0, 2, 
				(voidf) sfscan, 
				(voidf) sfprint);
	}

	return _IOInterface;
}

/* Exception init */

ObjectImpl(IOException)
{
	if (!_IOException)
	{
		_IOException = new(ExceptionObject(), "IOException");
	}

	return _IOException;
}

/* }}} */
