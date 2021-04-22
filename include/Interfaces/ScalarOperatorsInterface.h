#ifndef SCALARINTERFACE_H_FJ78U6RL
#define SCALARINTERFACE_H_FJ78U6RL

#include <stdarg.h>

#include "Macro.h"
#include "Object.h"

InterfaceHeader(ScalarOperatorsInterface);
ObjectHeader(ScalarOperatorsException);

struct ScalarOperatorsInterface
{
	method scadd;
	method scsub;
	method scmulti;
	method scdivide;
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
