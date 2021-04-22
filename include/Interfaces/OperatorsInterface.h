#ifndef OPERATORSINTERFACE_H_KIQ3RKDS
#define OPERATORSINTERFACE_H_KIQ3RKDS

#include "Macro.h"
#include "Object.h"

InterfaceHeader(OperatorsInterface);
ObjectHeader(OperatorsException);

struct OperatorsInterface
{
	method sum;
	method subtract;
	method product;
	method divide;
	method modulo;

	method onecompl;
	method lshift;
	method rshift;
};

void* sum(const void *self, const void *b);
void* subtract(const void *self, const void *b);
void* product(const void *self, const void *b);
void* divide(const void *self, const void *b);
void* modulo(const void *self, const void *b);

void onecompl(void *_self);
void lshift(void *_self, size_t shift);
void rshift(void *_self, size_t shift);

#endif /* end of include guard: OPERATORSINTERFACE_H_KIQ3RKDS */
