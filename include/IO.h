#ifndef PRINT_H_JMI0NFTM
#define PRINT_H_JMI0NFTM

#include "Object.h"
#include "IOInterface.h"

ObjectHeader(IOException);

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
