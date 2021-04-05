#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

#include "IO.h"
#include "Exception.h"
#include "Matrix.h"
#include "IntType.h"
#include "FloatType.h"
#include "Selectors.h"
#include "Utils.h"

/*
 * Selectors
 */

// ---

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
		fprintf(stderr, "sfprint: Error: Class '%s' doesn't implement 'IOInterface' and doesn't have 'sfprint' method!\n",
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
		fprintf(stderr, "sfscan: Error: Class '%s' doesn't implement 'IOInterface' and doesn't have 'sfscan' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return io->sfscan(_self, stream, bin, buffer, numb, asterisk, width);
}

/*
 * Helpers
 */

// ---

char* __varg_string(int n)
{
	char *spelling = int_spelling(n);

	int res_len = snprintf(NULL, 0, "%d%s variable", n, spelling);
	char *result = (char*)calloc(sizeof(char), res_len + 1);
	snprintf(result, res_len + 1, "%d%s variable", n, spelling);

	return result;
}

/*
 * Print to stream
 */

// ---

int _ovfprintf(FILE *stream, const char *fmt, char *file, int line, const char *func, va_list *ap)
{
	const char *p;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	unsigned int va_args_count = 1;

	int flag = -1,
		length = -1,
		width = -1,
		precision = -1;

	int result = 0;

	for (p = fmt; *p; p++)
	{
		if (*p != '%') {
			result++;
			putc(*p, stream);
		}
		else
		{
			p++;

			// Check flags
			switch (*p) {
				case '-':
					flag = '-';
					p++;

					break;
				case '+':
					flag = '+';
					p++;

					break;
				case '#':
					flag = '#';
					p++;

					break;
				case ' ':
					flag = ' ';
					p++;

					break;
				case '0':
					flag = '0';
					p++;

					break;
			}

			// Check width
			if (*p == '*') 
			{
				width = va_arg(ap_copy, int);
				p++;
			}
			else if (isdigit(*p)) 
			{
				width = 0; 

				for (int digit = (*p - 48); isdigit(*p); p++, digit = (*p - 48))
				{
					if (width <= INT_MAX / 10)
					{
						width *= 10;

						if (width <= INT_MAX - digit)
							width += digit;
						else
							exception_throw(GlobalException(), IOException(), 
									file, line, func, 
									"Error: Too big width! (Higher than %d)", INT_MAX);
					}
					else
						exception_throw(GlobalException(), IOException(), 
								file, line, func, 
								"Error: Too big width! (Higher than %d)", INT_MAX);
				}
			}

			// Check precision
			if (*p == '.')
			{
				p++;

				if (*p == '*')
				{
					precision = va_arg(ap_copy, int);
					p++;
				}
				else if (isdigit(*p))
				{
					precision = 0;

					for (int digit = (*p - 48); isdigit(*p); p++, digit = (*p - 48))
					{
						if (precision <= INT_MAX / 10)
						{
							precision *= 10;

							if (precision <= INT_MAX - digit)
								precision += digit;
							else
								exception_throw(GlobalException(), IOException(), 
										file, line, func,
										"Error: Too big precision! (Higher than %d)", INT_MAX);
						}
						else
							exception_throw(GlobalException(), IOException(), 
									file, line, func,
									"Error: Too big precision! (Higher than %d)", INT_MAX);
					}
				}
			}

			// Check length
			switch (*p) {
			case 'h':
				length = 'h';
				p++;

				break;
			case 'l':
				length = 'l';
				p++;

				break;
			case 'L':
				length = 'L';
				p++;

				break;
			}

			// For error printing
			char *varg_str = __varg_string(va_args_count);

			// Printing
			const void *type = NULL;
			char *type_str;

			switch (*p) {
				case 'd':
					type = Int();
					type_str = "Int()";
					break;
				case 'i':
					type = Int();
					type_str = "Int()";
					break;
				case 'f':
					type = Float();
					type_str = "Float()";
					break;
				case 'm':
					type = Matrix();
					type_str = "Matrix()";
					break;
				case 'v':
					type = Object();
					type_str = "Object()";
					break;
			}	

			int len = 0;

			if (type)
			{
				var obj = _cast(type, va_arg(ap_copy, var), type_str, varg_str, file, line, func);
				len = sfprint(obj, stream, 0, NULL, 0, flag, width, precision);

				if (len != -1)
					result += len;
			}
			else
				putc(*p, stream);

			free(varg_str);

			if (len == -1)
				exception_throw(GlobalException(), 
						IOException(), file, line, func, 
						"Error: some error occured during printing!");

			flag = -1;
			width = -1;
			precision = -1;
			length = -1;
		}
	}

	va_end(ap_copy);
	return result;
}

int _ofprintf(FILE *stream, const char *fmt, char *file, int line, const char *func, ...)
{
	va_list ap;
	va_start(ap, func);
	int result = _ovfprintf(stream, fmt, file, line, func, &ap);
	va_end(ap);

	return result;
}

int _oprintf(const char *fmt, char *file, int line, const char *func, ...)
{
	va_list ap;
	va_start(ap, func);
	int result = _ovfprintf(stdout, fmt, file, line, func, &ap);
	va_end(ap);

	return result;
}

int oprint(const void *_self)
{
	return sfprint(_self, stdout, 0, NULL, 0, -1, -1, -1);
}

int oprintln(const void *_self)
{
	int result = sfprint(_self, stdout, 0, NULL, 0, -1, -1, -1);

	if (result != -1)
		fputc('\n', stdout);

	return result;
}

/*
 * Binary print
 */

// ---

int ofwrite(const void *self, FILE *stream)
{
	return sfprint(self, stream, 1, NULL, 0, -1, -1, -1);
}

int owrite(const void *self)
{
	return sfprint(self, stdout, 1, NULL, 0, -1, -1, -1);
}


/*
 * Scan from stream
 */

// ---

int _ovfscanf(FILE* stream, const char *fmt, char *file, int line, const char *func, va_list *ap)
{
	const char *p;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	unsigned int va_args_count = 1;

	int asterisk = -1,
		width = -1,
		length = -1;

	int result = 0;
	int c = ~EOF;

	for(p = fmt; *p && c != EOF; p++)
	{
		while(isspace(*p)) p++;

		if (*p != '%')
		{
			c = getc(stream);

			if (c != *p) break;
		}
		else
		{
			p++;

			// Check asterisk
			if (*p == '*')
			{
				asterisk = '*';
				p++;
			}

			// Check width
			if (isdigit(*p))
			{
				width = 0; 

				for (int digit = (*p - 48); isdigit(*p); p++, digit = (*p - 48))
				{
					if (width <= INT_MAX / 10)
					{
						width *= 10;

						if (width <= INT_MAX - digit)
							width += digit;
						else
							exception_throw(GlobalException(), IOException(), 
									file, line, func, 
									"Error: Too big width! (Higher than %d)", INT_MAX);
					}
					else
						exception_throw(GlobalException(), IOException(), 
								file, line, func, 
								"Error: Too big width! (Higher than %d)", INT_MAX);
				}
			}

			// Check length
			switch (*p) {
				case 'h':
					length = 'h';
					p++;

					break;
				case 'l':
					length = 'l';
					p++;

					break;
				case 'L':
					length = 'L';
					p++;

					break;
			}

			// For error printing
			char *varg_str = __varg_string(va_args_count);

			// Scanning
			const void *type = NULL;
			char *type_str;

			switch (*p) {
			case 'd':
				type = Int();
				type_str = "Int()";
				break;
			case 'i':
				type = Int();
				type_str = "Int()";
				break;
			case 'f':
				type = Float();
				type_str = "Float()";
				break;
			case 'm':
				type = Matrix();
				type_str = "Matrix()";
				break;
			case 'v':
				type = Object();
				type_str = "Object()";
				break;
			}

			int res = 0;

			if (type)
			{
				var obj = _cast(type, va_arg(ap_copy, var), type_str, varg_str, file, line, func);
				res = sfscan(obj, stream, 0, NULL, NULL, asterisk, width);

				if (res != -1 && res != EOF)
					result += res;
			}

			free(varg_str);

			if (res == -1 || res == EOF)
				exception_throw(GlobalException(), IOException(), 
						file, line, func,
						"Error: some error occured during scanning!");

			asterisk = -1;
			width = -1;
		}
	}
		
	va_end(ap_copy);
	return result;
}

int _ofscanf(FILE *stream, const char *fmt, char *file, int line, const char *func, ...)
{
	va_list ap;
	va_start(ap, func);
	int result = _ovfscanf(stream, fmt, file, line, func, &ap);
	va_end(ap);

	return result;
}

int _oscanf(const char *fmt, char *file, int line, const char *func, ...)
{
	va_list ap;
	va_start(ap, func);
	int result = _ovfscanf(stdin, fmt, file, line, func, &ap);
	va_end(ap);

	return result;
}

/*
 * Scan for string
 */

// ---

int _ovsscanf(const char *buffer, const char *fmt, char *file, int line, const char *func, va_list *ap)
{
	const char *p, *s;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	unsigned int va_args_count = 1;

	int asterisk = -1,
		width = -1,
		length = -1;

	int result = 0;

	for (p = fmt, s = buffer; *p && *s; p++)
	{
		while(isspace(*p)) p++;

		if (*p != '%')
		{
			s++;

			if (*s != *p) break;
		}
		else
		{
			p++;

			// Check asterisk
			if (*p == '*')
			{
				asterisk = '*';
				p++;
			}

			// Check width
			if (isdigit(*p))
			{
				width = 0; 

				for (int digit = (*p - 48); isdigit(*p); p++, digit = (*p - 48))
				{
					if (width <= INT_MAX / 10)
					{
						width *= 10;

						if (width <= INT_MAX - digit)
							width += digit;
						else
							exception_throw(GlobalException(), IOException(), 
									file, line, func, 
									"Error: Too big width! (Higher than %d)", INT_MAX);
					}
					else
						exception_throw(GlobalException(), IOException(), 
								file, line, func, 
								"Error: Too big width! (Higher than %d)", INT_MAX);				
				}
			}

			// Check length
			switch (*p) {
				case 'h':
					length = 'h';
					p++;

					break;
				case 'l':
					length = 'l';
					p++;

					break;
				case 'L':
					length = 'L';
					p++;

					break;
			}

			// For error printing
			char *varg_str = __varg_string(va_args_count);
			
			// Scanning
			const void *type = NULL;
			char *type_str;
			
			switch (*p) {
				case 'd':
					type = Int();
					type_str = "Int()";
					break;
				case 'i':
					type = Int();
					type_str = "Int()";
					break;
				case 'f':
					type = Float();
					type_str = "Float()";
					break;
				case 'm':
					type = Matrix();
					type_str = "Matrix()";
					break;
				case 'v':
					type = Object();
					type_str = "Object()";
					break;
			}

			int res = 0;

			if (type)
			{
				var obj = _cast(type, va_arg(ap_copy, var), type_str, varg_str, file, line, func);

				int n;
				res = sfscan(obj, NULL, 0, s, &n, asterisk, width);

				if (res != -1)
				{
					result += res;
					s+= n;
				}
			}

			free(varg_str);

			if (res == -1 || res == EOF)
				exception_throw(GlobalException(), IOException(), 
						file, line, func,
						"Error: some error occured during scanning!");

			asterisk = -1;
			width = -1;
		}
	}

	va_end(ap_copy);
	return result;
}

int _osscanf(const char *buffer, const char *fmt, char *file, int line, const char *func, ...)
{
	va_list ap;
	va_start(ap, func);
	int result = _ovsscanf(buffer, fmt, file, line, func, &ap);
	va_end(ap);

	return result;
}

/*
 * Binary scan
 */

// ---

int ofread(void *self, FILE *stream)
{
	return sfscan(self, stream, 1, NULL, NULL, -1, -1);
}

int oread(void *self)
{
	return sfscan(self, stdin, 1, NULL, NULL, -1, -1);
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

/*
 * Exception Initialization
 */

// ---

ObjectImpl(IOException)
{
	if (!_IOException)
	{
		_IOException = new(ExceptionObject(), "IOException");
	}

	return _IOException;
}
