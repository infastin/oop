#ifndef IOINTERFACE_H_XVLPIEOX
#define IOINTERFACE_H_XVLPIEOX

#include <stdio.h>

#include "Interface.h"

InterfaceHeader(IOInterface);

typedef int   (*sfscan_f)(void *self, FILE *stream, int bin, const char *buffer, int *numb, 
		int asterisk, int width);

typedef int   (*sfprint_f)(const void *self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision);

struct IOInterface
{
	sfscan_f sfscan;
	sfprint_f sfprint;
};

/**
 * @brief Helper for sfprint for basic types (e.g. Int, Float)
 *
 * @param flag 		Format flag. -1 for not given
 * @param width 	Format minimal field width. -1 for not given
 * @param precision Format precision. -1 for not given
 * @param spec 		Format specifier
 *
 * @return Format string.
 */
char* __getFmtPrint(int flag, int width, int precision, char *spec);

char* __getFmtScan(int asterisk, int width, char *spec);

int sfscan(void *self, FILE *stream, int bin, const char *buffer, int *numb, 
		int asterisk, int width);

/**
 * @brief Prints value of object into the stream or the buffer
 *
 * @param self 		Object
 * @param stream 	If not NULL, will ignore the buffer and work like fprintf
 * @param bin 		If 1, will print in binary mode into the stream
 * @param buffer 	If not NULL, will work like snprintf
 * @param maxn      Maximum number of bytes to be used in the buffer
 * @param flag 		Format flag. -1 for not given
 * @param width 	Format minimal field width. -1 for not given
 * @param precision Format precision. -1 for not given
 *
 * @return If binary mode enabled, will return size of object value
 * @return If the buffer is NULL, will return size of value in C string representation
 * @return Otherwise return value of fprintf or snprintf
 */
int sfprint(const void *self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision);

#endif /* end of include guard: IOINTERFACE_H_XVLPIEOX */
