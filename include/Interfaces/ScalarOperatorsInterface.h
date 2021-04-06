#ifndef SCALARINTERFACE_H_FJ78U6RL
#define SCALARINTERFACE_H_FJ78U6RL

#include <stdarg.h>

#include "Interface.h"
#include "Object.h"

InterfaceHeader(ScalarOperatorsInterface);
ObjectHeader(ScalarOperatorsException);

typedef void  (*scadd_f)(void *self, va_list *ap);
typedef void  (*scsub_f)(void *self, va_list *ap);
typedef void  (*scmulti_f)(void *self, va_list *ap);
typedef void  (*scdivide_f)(void *self, va_list *ap);

struct ScalarOperatorsInterface
{
	scadd_f scadd;
	scsub_f scsub;
	scmulti_f scmulti;
	scdivide_f scdivide;
};

void  scadd(void *self, ...);
void  scsub(void *self, ...);
void  scmulti(void *self, ...);
void  scdivide(void *self, ...);

void  vscadd(void *self, va_list *ap);
void  vscsub(void *self, va_list *ap);
void  vscmulti(void *self, va_list *ap);
void  vscdivide(void *self, va_list *ap);

#endif /* end of include guard: SCALARINTERFACE_H_FJ78U6RL */
