#ifndef PRINT_H_JMI0NFTM
#define PRINT_H_JMI0NFTM

#include "Object.h"

ObjectHeader(PrintException);

int _oprintf(const char *fmt, char *file, int line, ...);
int oprint(const void *self);
int oprintln(const void *self);

#define oprintf(fmt, ...) _oprintf(fmt, __FILE__, __LINE__, ## __VA_ARGS__)

#endif /* end of include guard: PRINT_H_JMI0NFTM */
