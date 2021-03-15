#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "Exception.h"
#include "Matrix.h"
#include "Object.h"
#include "Selectors.h"
#include "TypeClass.h"
#include "ExceptionObject.h"

/*
 * Matrix
 */

static void* Matrix_ctor(void *_self, va_list *props)
{
	struct Matrix *self = super_ctor(Matrix(), _self, props);

	va_list ap;
	va_copy(ap, *props);

	self->type = va_arg(*props, void*);
	self->rows = va_arg(*props, unsigned int);
	self->columns = va_arg(*props, unsigned int);

	void ***mass;

	mass = (void***)calloc(sizeof(void**), self->rows);
	if (mass == NULL)
		throw(MatrixException(), "Fatal Error: Matrix pointers allocation error!");

	mass[0] = (void**)calloc(sizeof(void*), self->rows * self->columns);
	if (mass[0] == NULL)
		throw(MatrixException(), "Fatal Error: Matrix items allocation error!");

	for (int i = 1; i < self->rows; i++)
	{
		mass[i] = mass[0] + i * self->columns;
	}

	self->mass = mass;

	return self;
}

ClassImpl(Matrix)
{
	if (!_Matrix)
	{
		_Matrix = new(TypeClass(), "MatrixClass",
				Object(), sizeof(struct Matrix),
				ctor, Matrix_ctor,
				0);
	}

	return _Matrix;
}

ObjectImpl(MatrixException)
{
	if (!_MatrixException)
	{
		_MatrixException = new(ExceptionObject(), "MatrixException");
	}

	return _MatrixException;
}
