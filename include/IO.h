#ifndef PRINT_H_JMI0NFTM
#define PRINT_H_JMI0NFTM

#include "Object.h"

InterfaceHeader(IOInterface);
ObjectHeader(IOException);

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

int oprint(const void *self);
int oprintln(const void *self);

int _ovfprintf(FILE *stream, const char *fmt, 
		char *file, int line, const char *func, 
		va_list *ap);

int _ofprintf(FILE *stream, const char *fmt
		, char *file, int line, const char *func, 
		...);

int _oprintf(const char *fmt,
		char *file, int line, const char *func, 
		...);

int _ovsnprintf(const char *buffer, size_t maxn, const char *fmt, 
		char *file, int line, const char *func, 
		va_list *ap);

int _osnprintf(const char *buffer, size_t maxn, const char *fmt, 
		char *file, int line, const char *func, 
		...);

int ofwrite(const void *self, FILE *stream);
int owrite(const void *self);

int _ovfscanf(FILE *stream, const char *fmt, 
		char *file, int line, const char *func, 
		va_list *ap);

int _ofscanf(FILE *stream, const char *fmt, 
		char *file, int line, const char *func, 
		...);

int _oscanf(const char *fmt, 
		char *file, int line, const char *func, 
		...);

int _ovsscanf(const char *buffer, const char *fmt, 
		char *file, int line, const char *func, 
		va_list *ap);

int _osscanf(const char *buffer, const char *fmt, 
		char *file, int line, const char *func, 
		...);

int ofread(void *self, FILE *stream);
int oread(void *self);

#define ovfprintf(stream, fmt, ap) _ovfprintf(stream, fmt, __FILE__, __LINE__, "ovfprintf" ap)
#define ofprintf(stream, fmt, ...) _oprintf(stream, fmt, __FILE__, __LINE__, "ofprintf", ## __VA_ARGS__)
#define oprintf(fmt, ...) _oprintf(fmt, __FILE__, __LINE__, "oprintf", ## __VA_ARGS__)

#define ovsnprintf(buffer, maxn, fmt, ap) _osnprintf(buffer, maxn, fmt, __FILE__, __LINE__, "ovsnprintf", ap)
#define osnprintf(buffer, maxn, fmt, ...) _osnprintf(buffer, maxn, fmt, __FILE__, __LINE__, "ovsnprintf", ## __VA_ARGS__)

#define ovfscanf(stream, fmt, ap) _ovfscanf(stream, fmt, __FILE__, __LINE__, "ovfscanf", ap)
#define ofscanf(stream, fmt, ...) _ofscanf(fmt, __FILE__, __LINE__, "ofscanf", ## __VA_ARGS__)
#define oscanf(fmt, ...) _oscanf(fmt, __FILE__, __LINE__, "oscanf", ## __VA_ARGS__)

#define ovsscanf(buffer, fmt, ap) _ovsscanf(buffer, fmt, __FILE__, __LINE__, "ovsscanf", ap)
#define osscanf(buffer, fmt, ...) _osscanf(buffer, fmt, __FILE__, __LINE__, "osscanf", ## __VA_ARGS__)

#endif /* end of include guard: PRINT_H_JMI0NFTM */
