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
void oprintf(const char *fmt, ...)
{
	const char *p;

	va_list ap;
	va_start(ap, fmt);

	int flag = -1,
		length = -1,
		width = -1,
		precision = -1;

	for (p = fmt; *p; p++)
	{
		if (*p != '%')
			putchar(*p);
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
							throw(PrintException(), "Object Printf Error: Too big width! (Higher than %d)",
									INT_MAX);
						}
					}
					else
					{
						throw(PrintException(), "Object Printf Error: Too big width! (Higher than %d)",
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
								throw(PrintException(), "Object Printf Error: Too big precision! (Higher than %d)",
										INT_MAX);
							}
						}
						else
						{
							throw(PrintException(), "Object Printf Error: Too big precision! (Higher than %d)",
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
				var type = cast(Int(), va_arg(ap, var));
				char *str = stringer(type, flag, width, precision);

				printf("%s", str);
				free(str);
			}
			else if (*p == 'm')
			{
				var type = cast(Matrix(), va_arg(ap, var));
				char *str = stringer(type, flag, width, precision);

				printf("%s", str);
				free(str);
			}
			else if (*p == 'f')
			{
				var type = cast(Float(), va_arg(ap, var));
				char *str = stringer(type, flag, width, precision);

				printf("%s", str);
				free(str);
			}

			flag = -1;
			width = -1;
			precision = -1;
			length = -1;
		}
	}

	va_end(ap);
}

void oprint(const void *_self)
{
	char *str = stringer(_self);

	puts(str);
	free(str);
}

void oprintln(const void *_self)
{
	char *str = stringer(_self);

	puts(str);
	putchar('\n');
	free(str);
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
