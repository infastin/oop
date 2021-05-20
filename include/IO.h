#ifndef PRINT_H_JMI0NFTM
#define PRINT_H_JMI0NFTM

#include "Object.h"
#include "IOInterface.h"

int oprint(const void *self);
int oprintln(const void *self);

int _ovfprintf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt, va_list *ap);

int _ofprintf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt, ...);

int _oprintf(char *file, int line, const char *func, 
		const char *fmt, ...);

int _ovsnprintf(char *file, int line, const char *func, 
		const char *buffer, size_t maxn, const char *fmt, va_list *ap);

int _osnprintf(char *file, int line, const char *func, 
		const char *buffer, size_t maxn, const char *fmt, ...);

int ofwrite(const void *self, FILE *stream);
int owrite(const void *self);

int _ovfscanf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt, va_list *ap);

int _ofscanf(char *file, int line, const char *func, 
		FILE *stream, const char *fmt, ...);

int _oscanf(char *file, int line, const char *func, 
		const char *fmt, ...);

int _ovsscanf(char *file, int line, const char *func, 
		const char *buffer, const char *fmt, va_list *ap);

int _osscanf(char *file, int line, const char *func, 
		const char *buffer, const char *fmt, ...);

int ofread(void *self, FILE *stream);
int oread(void *self);

#define ovfprintf(stream, fmt, ap) _ovfprintf(__FILE__, __LINE__, "ovfprintf", stream, fmt, ap)

#define ofprintf(stream, fmt...) _oprintf(__FILE__, __LINE__, "ofprintf", stream, fmt)
#define oprintf(fmt...) _oprintf(__FILE__, __LINE__, "oprintf", fmt)

#define ovsnprintf(buffer, maxn, fmt, ap) _osnprintf(__FILE__, __LINE__, "ovsnprintf", buffer, maxn, fmt, ap)
#define osnprintf(buffer, maxn, fmt...) _osnprintf(__FILE__, __LINE__, "ovsnprintf", buffer, maxn, fmt)

#define ovfscanf(stream, fmt, ap) _ovfscanf(__FILE__, __LINE__, "ovfscanf", stream, fmt, ap)
#define ofscanf(stream, fmt...) _ofscanf(fmt, __FILE__, __LINE__, "ofscanf", stream, fmt)
#define oscanf(fmt...) _oscanf(__FILE__, __LINE__, "oscanf", fmt)

#define ovsscanf(buffer, fmt, ap) _ovsscanf(__FILE__, __LINE__, "ovsscanf", buffer, fmt, ap)
#define osscanf(buffer, fmt...) _osscanf(__FILE__, __LINE__, "osscanf", buffer, fmt)

#endif /* end of include guard: PRINT_H_JMI0NFTM */
