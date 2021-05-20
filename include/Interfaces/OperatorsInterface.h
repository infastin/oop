#ifndef OPERATORSINTERFACE_H_KIQ3RKDS
#define OPERATORSINTERFACE_H_KIQ3RKDS

#include "Interface.h"
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

interface_sel_header(void*, sum, const void *self, const void *b);
interface_sel_header(void*, subtract, const void *self, const void *b);
interface_sel_header(void*, product, const void *self, const void *b);
interface_sel_header(void*, divide, const void *self, const void *b);
interface_sel_header(void*, modulo, const void *self, const void *b);

interface_sel_header(void, onecompl, void *self);
interface_sel_header(void, lshift, void *self, size_t shift);
interface_sel_header(void, rshift, void *self, size_t shift);

#endif /* end of include guard: OPERATORSINTERFACE_H_KIQ3RKDS */
