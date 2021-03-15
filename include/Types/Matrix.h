#ifndef MATRIX_H_720USNE8
#define MATRIX_H_720USNE8

#include "Object.h"
#include "TypeClass.h"

ClassHeader(Matrix);
ObjectHeader(MatrixException);

struct Matrix
{
	const struct Object _;
	const void *type;
	unsigned int rows;
	unsigned int columns;
	void ***mass;
};


#endif /* end of include guard: MATRIX_H_720USNE8 */
