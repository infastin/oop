#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

#include "Print.h"
#include "Exception.h"
#include "Matrix.h"
#include "IntType.h"
#include "FloatType.h"
#include "Selectors.h"

// Print object
int _oprintf(const char *fmt, char *file, int line, ...)
{
	const char *p;

	va_list ap;
	va_start(ap, line);

	int flag = -1,
		length = -1,
		width = -1,
		precision = -1;

	int result = 0;;
	
	for (p = fmt; *p; p++)
	{
		if (*p != '%') {
			result++;
			putchar(*p);
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
				width = va_arg(ap, int);
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
						{
							width += digit;
						}
						else
						{
							throw(PrintException(), "Error: Too big width! (Higher than %d)",
									INT_MAX);
						}
					}
					else
					{
						throw(PrintException(), "Error: Too big width! (Higher than %d)",
								INT_MAX);
					}
				}
			}

			// Check precision
			if (*p == '.')
			{
				p++;

				if (*p == '*')
				{
					precision = va_arg(ap, int);
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
							{
								precision += digit;
							}
							else
							{
								throw(PrintException(), "Error: Too big precision! (Higher than %d)",
										INT_MAX);
							}
						}
						else
						{
							throw(PrintException(), "Error: Too big precision! (Higher than %d)",
									INT_MAX);
						}
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

			if (*p == 'd' || *p == 'i')
			{
				var type = _cast(Int(), va_arg(ap, var), file, line);
				int len = sfprint(type, stdout, 0, NULL, 0, flag, width, precision);
				if (len != -1)
					result += len;
			}
			else if (*p == 'm')
			{
				var type = _cast(Matrix(), va_arg(ap, var), file, line);
				int len = sfprint(type, stdout, 0, NULL, 0, flag, width, precision);
				if (len != -1)
					result += len;
			}
			else if (*p == 'f')
			{
				var type = _cast(Float(), va_arg(ap, var), file, line);
				int len = sfprint(type, stdout, 0, NULL, 0, flag, width, precision);
				if (len != -1)
					result += len;
			}
			else if (*p == 'v')
			{
				var type = _cast(Object(), va_arg(ap, var), file, line);
				int len = sfprint(type, stdout, 0, NULL, 0, flag, width, precision);
				if (len != -1)
					result += len;

			}
			else
			{
				putchar(*p);
			}

			flag = -1;
			width = -1;
			precision = -1;
			length = -1;
		}
	}

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

int oscanf(const char *fmt, ...)
{
	
}

/*
 * Exception Initialization
 */

ObjectImpl(PrintException)
{
	if (_PrintException)
	{
		_PrintException = new(ExceptionObject(), "PrintException");
	}

	return _PrintException;
}
