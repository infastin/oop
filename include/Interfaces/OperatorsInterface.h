#ifndef OPERATORSINTERFACE_H_KIQ3RKDS
#define OPERATORSINTERFACE_H_KIQ3RKDS

#include "Interface.h"
#include "Object.h"

InterfaceHeader(OperatorsInterface);
ObjectHeader(OperatorsException);

typedef void *(*sum_f)(const void *self, const void *b);
typedef void *(*subtract_f)(const void *self, const void *b);
typedef void *(*product_f)(const void *self, const void *b);
typedef void *(*divide_f)(const void *self, const void *b);

typedef void  (*onecompl_f)(void *self);
typedef void  (*lshift_f)(void *self, size_t shift);
typedef void  (*rshift_f)(void *self, size_t shift);

struct OperatorsInterface
{
	sum_f sum;
	subtract_f subtract;
	product_f product;
	divide_f divide;

	onecompl_f onecompl;
	lshift_f lshift;
	rshift_f rshift;
};

void* sum(const void *self, const void *b);
void* subtract(const void *self, const void *b);
void* product(const void *self, const void *b);
void* divide(const void *self, const void *b);

void onecompl(void *_self);
void lshift(void *_self, size_t shift);
void rshift(void *_self, size_t shift);

#endif /* end of include guard: OPERATORSINTERFACE_H_KIQ3RKDS */
