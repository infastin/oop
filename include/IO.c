/* vim: set fdm=marker : */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

#include "GenericType.h"
#include "IO.h"
#include "IntTypes.h"
#include "Exception.h"
#include "List.h"
#include "Matrix.h"
#include "FloatTypes.h"
#include "Selectors.h"
#include "Utils.h"
#include "String.h"

/* Helpers {{{ */

char* __n_string(int n, char *str)
{
	char *spelling = int_spelling(n);

	int res_len = snprintf(NULL, 0, "%d%s %s", n, spelling, str);
	char *result = (char*)calloc(sizeof(char), res_len + 1);
	snprintf(result, res_len + 1, "%d%s %s", n, spelling, str);

	return result;
}

int __strtoint(char *file, int line, const char *func, 
		char *name, const char *p, int *digits)
{
	int result = 0;
	*digits = 0;

	for (int digit = (*p - 48); isdigit(*p); p++, digit = (*p - 48))
	{
		if (result <= INT_MAX / 10)
		{
			result *= 10;

			if (result <= INT_MAX - digit)
			{
				result += digit;
				(*digits)++;
			}
			else
				exception_throw(GlobalException(), IOException(), 
						file, line, func, 
						"Error: Too big %s! (Higher than %d)", name, INT_MAX);
		}
		else
			exception_throw(GlobalException(), IOException(), 
					file, line, func, 
					"Error: Too big %s! (Higher than %d)", name, INT_MAX);
	}

	return result;
}

/* }}} */

/* Print to stream {{{1 */

int _ovfprintf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt, va_list *ap)
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
				int digits;
				width = __strtoint(file, line, func, "width", p, &digits); 
				p += digits;
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
					int digits;
					precision = __strtoint(file, line, func, "width", p, &digits); 
					p += digits;
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
			char *varg_str = __n_string(va_args_count, "variable");

			// Printing
			const void *type = NULL;
			char *type_str;

			switch (*p) {
				case 'i':
				case 'd':
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
				case 's':
					type = String();
					type_str = "String()";
					break;
				case 'v':
					type = Object();
					type_str = "Object()";
					break;
			}

			int len = 0;

			if (type)
			{
				var obj = _cast(type_str, varg_str, file, line, func, type, va_arg(ap_copy, var));
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

int _ofprintf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int result = _ovfprintf(file, line, func, stream, fmt, &ap);
	va_end(ap);

	return result;
}

int _oprintf(char *file, int line, const char *func, 
		const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int result = _ovfprintf(file, line, func, stdout, fmt, &ap);
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

/* }}} */

/* Binary print {{{ */

int ofwrite(const void *self, FILE *stream)
{
	return sfprint(self, stream, 1, NULL, 0, -1, -1, -1);
}

int owrite(const void *self)
{
	return sfprint(self, stdout, 1, NULL, 0, -1, -1, -1);
}

/* }}} */

/* Scan from stream {{{ */

int _ovfscanf(char *file, int line, const char *func, 
		FILE* stream, const char *fmt, va_list *ap)
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

	var args = new(List(), GenericType());

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
				int digits;
				width = __strtoint(file, line, func, "width", p, &digits); 
				p += digits;			
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
			char *varg_str = __n_string(va_args_count, "variable");

			// Scanning
			const void *type = NULL;
			char *type_str;

			switch (*p) {
				case 'i':
				case 'd':
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
				case 's':
					type = String();
					type_str = "String()";
					break;
				case 'v':
					type = Object();
					type_str = "Object()";
					break;
			}

			int res = 0;

			if (type)
			{
				var obj = _cast(type_str, varg_str, file, line, func, type, va_arg(ap_copy, var));
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

int _ofscanf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt,  ...)
{
	va_list ap;
	va_start(ap, fmt);
	int result = _ovfscanf(file, line, func, stream, fmt, &ap);
	va_end(ap);

	return result;
}

int _oscanf(char *file, int line, const char *func, 
		const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int result = _ovfscanf(file, line, func, stdin, fmt, &ap);
	va_end(ap);

	return result;
}

/*
 * Scan from string
 */

// ---

int _ovsscanf(char *file, int line, const char *func, 
		const char *buffer, const char *fmt, va_list *ap)
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
				int digits;
				width = __strtoint(file, line, func, "width", p, &digits); 
				p += digits;				
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
			char *varg_str = __n_string(va_args_count, "variable");
			
			// Scanning
			const void *type = NULL;
			char *type_str;
			
			switch (*p) {
				case 'i':
				case 'd':
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
				case 's':
					type = String();
					type_str = "String()";
					break;
				case 'v':
					type = Object();
					type_str = "Object()";
					break;
			}

			int res = 0;

			if (type)
			{
				var obj = _cast(type_str, varg_str, file, line, func, type, va_arg(ap_copy, var));

				int n;
				res = sfscan(obj, NULL, 0, s, &n, asterisk, width);

				if (res != -1)
				{
					result += res;
					s += n;
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

int _osscanf(char *file, int line, const char *func, const char *buffer, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int result = _ovsscanf(file, line, func, buffer, fmt, &ap);
	va_end(ap);

	return result;
}

/* }}} */

/* Binary scan {{{ */

int ofread(void *self, FILE *stream)
{
	return sfscan(self, stream, 1, NULL, NULL, -1, -1);
}

int oread(void *self)
{
	return sfscan(self, stdin, 1, NULL, NULL, -1, -1);
}

/* }}} */
