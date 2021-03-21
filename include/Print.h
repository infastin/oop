#ifndef PRINT_H_JMI0NFTM
#define PRINT_H_JMI0NFTM

#include "Object.h"

ObjectHeader(PrintException);

void oprintf(const char *fmt, ...);
void oprint(const void *self);
void oprintln(const void *self);

#endif /* end of include guard: PRINT_H_JMI0NFTM */
