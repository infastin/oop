#include <stdlib.h>

#include "IOInterface.h"
#include "Object.h"
#include "Selectors.h"

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

int sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision)
{
	const struct Class *class = classOf(_self);
	const struct IOInterface *io = icast(IOInterface(), class);

	if (io->sfprint == NULL)
	{
		fprintf(stderr, "sfprint: Error: Class '%s' doesn't implement 'sfprint' method of 'IOInterface'!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return io->sfprint(_self, stream, bin, buffer, maxn, flag, width, precision);
}

int sfscan(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	const struct Class *class = classOf(_self);
	const struct IOInterface *io = icast(IOInterface(), class);

	if (io->sfscan == NULL) 
	{
		fprintf(stderr, "sfscan: Error: Class '%s' doesn't implement 'sfscan' method of 'IOInterface'!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return io->sfscan(_self, stream, bin, buffer, numb, asterisk, width);
}

/*
 * Interface Initialization
 */
InterfaceImpl(IOInterface)
{
	if (!_IOInterface)
	{
		_IOInterface = inew("IOInterface", 0);
	}

	return _IOInterface;
}
