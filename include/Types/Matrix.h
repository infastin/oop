#ifndef MATRIX_H_720USNE8
#define MATRIX_H_720USNE8

#include <stdarg.h>

#include "TypeClass.h"

ClassHeader(Matrix);
ClassHeader(MatrixClass);
ObjectHeader(MatrixException);

typedef void *(*minorOf_f)(const void *self, va_list *ap);
typedef void *(*matrix_size_f)(const void *self, va_list *ap);
typedef void  (*determinant_f)(const void *self, va_list *ap);

struct MatrixClass
{
	const struct TypeClass _;
	minorOf_f minorOf;
	matrix_size_f matrix_size;
	determinant_f determinant;
};

struct Matrix
{
	const struct Object _;
	const struct TypeClass *type;
	unsigned int rows;
	unsigned int columns;
	void ***mass;
};

void* minorOf(const void *self, ...);
void  matrix_size(const void *self, ...);
void  determinant(const void *self, ...);

void* vminorOf(const void *_self, va_list *ap);
void  vmatrix_size(const void *_self, va_list *ap);
void  vdeterminant(const void *_self, va_list *ap);

#endif /* end of include guard: MATRIX_H_720USNE8 */
