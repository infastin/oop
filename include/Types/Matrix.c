#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "Exception.h"
#include "Matrix.h"
#include "Object.h"
#include "ReferenceCounter.h"
#include "Selectors.h"
#include "TypeClass.h"
#include "IntType.h"

/*
 * MatrixClass
 */

static void* MatrixClass_ctor(void *_self, va_list *ap)
{
	struct MatrixClass *self = super_ctor(MatrixClass(), _self, ap);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	self->minorOf = NULL;

	while ((selector = va_arg(ap_copy, voidf)))
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) minorOf)
			self->minorOf = (minorOf_f) method;
		else if (selector == (voidf) matrix_size)
			self->matrix_size = (matrix_size_f) method;
		else if (selector == (voidf) determinant)
			self->determinant = (determinant_f) method;
	}

	va_end(ap_copy);
	return self;
}

/*
 * Selectors
 */

void* minorOf(const void *_self, ...)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->minorOf == NULL)
		throw(MatrixException(), "MinorOf Error: Matrix '%s' doesn't have 'minorOf' method!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	void* result = mclass->minorOf(_self, &ap);
	va_end(ap);

	return result;
}

void* vminorOf(const void *_self, va_list *ap)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->minorOf == NULL)
		throw(MatrixException(), "MinorOf Error: Matrix '%s' doesn't have 'minorOf' method!",
				class->name);

	return mclass->minorOf(_self, ap);
}

void matrix_size(const void *_self, ...)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->matrix_size == NULL)
		throw(MatrixException(), "Matrix Size Error: Matrix '%s' doesn't have 'matrix_size' method!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	mclass->matrix_size(_self, &ap);
	va_end(ap);
}

void vmatrix_size(const void *_self, va_list *ap)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->matrix_size == NULL)
		throw(MatrixException(), "Matrix Size Error: Matrix '%s' doesn't have 'matrix_size' method!",
				class->name);

	mclass->matrix_size(_self, ap);
}

void determinant(const void *_self, ...)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->determinant == NULL)
		throw(MatrixException(), "Determinant Error: Matrix '%s' doesn't have 'determinant' method!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	mclass->determinant(_self, &ap);
	va_end(ap);
}

void vdeterminant(const void *_self, va_list *ap)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->determinant == NULL)
		throw(MatrixException(), "Determinant Error: Matrix '%s' doesn't have 'determinant' method!",
				class->name);

	mclass->determinant(_self, ap);
}

/*
 * Matrix
 */

static void* Matrix_ctor(void *_self, va_list *ap)
{
	smart struct Matrix *self = super_ctor(Matrix(), _self, ap);

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	self->type = cast(TypeClass(), va_arg(ap_copy, void*));
	self->rows = va_arg(ap_copy, unsigned int);
	self->columns = va_arg(ap_copy, unsigned int);
	self->mass = NULL;

	if (self->rows == 0 || self->columns == 0)
		throw(MatrixException(), "Constructor Error: Can't create matrix with zero rows or columns! (%u, %u)",
				self->rows, self->columns);

	void ***mass;

	mass = (void***)calloc(sizeof(void**), self->rows);
	if (mass == NULL)
		throw(MatrixException(), "Fatal Error: Matrix pointers allocation error in constructor!");

	mass[0] = (void**)calloc(sizeof(void*), self->rows * self->columns);
	if (mass[0] == NULL)
		throw(MatrixException(), "Fatal Error: Matrix items allocation error in constructor!");

	for (int i = 0; i < self->rows; i++)
	{
		mass[i] = mass[0] + i * self->columns;

		for (int j = 0; j < self->columns; j++)
		{
			mass[i][j] = NULL;
		}
	}

	va_end(ap_copy);
	self->mass = mass;

	return retain(self);
}

static void* Matrix_cpy(const void *_self, void *_object)
{
	const struct Matrix *self = _self;
	smart struct Matrix *object = super_cpy(Matrix(), _self, _object);

	object->rows = self->rows;
	object->columns = self->columns;
	object->type =self->type;

	void ***mass;

	mass = (void***)calloc(sizeof(void**), object->rows);
	if (mass == NULL)
		throw(MatrixException(), "Fatal Error: Matrix pointers allocation error in copy!");

	mass[0] = (void**)calloc(sizeof(void*), object->rows * object->columns);
	if (mass[0] == NULL)
		throw(MatrixException(), "Fatal Error: Matrix items allocation error in copy!");

	for (int i = 0; i < object->rows; i++)
	{
		mass[i] = mass[0] + i * object->columns;

		for (int j = 0; j < object->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				mass[i][j] = NULL;
			else
				mass[i][j] = copy(self->mass[i][j]);
		}
	}

	object->mass = mass;

	return retain(object);
}

static void Matrix_set(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	unsigned int row = va_arg(*ap, unsigned int);
	unsigned int column = va_arg(*ap, unsigned int);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Set Error: Element at (%u, %u) is out of bounds!", 
				row, column);

	if (self->mass[row][column] == NULL)
		self->mass[row][column] = vnew(self->type, ap);
	else
		vset(self->mass[row][column], ap);
}

static void Matrix_get(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	unsigned int row = va_arg(*ap, unsigned int);
	unsigned int column = va_arg(*ap, unsigned int);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Get Error: Element at (%u, %u) is out of bounds!", 
				row, column);

	if (self->mass[row][column] == NULL)
		throw(MatrixException(), "Get Error: Element at (%u, %u) doesn't exists! (equals NULL)", 
				row, column);

	vget(self->mass[row][column], ap);
}

static void* Matrix_dtor(void *_self)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->mass != NULL)
	{
		for (int i = 0; i < self->rows; i++)
		{
			for (int j = 0; j < self->columns; j++)
			{
				delete(self->mass[i][j]);
			}
		}

		free(self->mass[0]);
		free(self->mass);
	}

	return super_dtor(Matrix(), _self);
}

static void* Matrix_sum(void *_self, void *b)
{
	const struct Matrix *self = cast(Matrix(), _self);
	const struct Matrix *B = cast(Matrix(), b);

	if (self->type != B->type) {
		const struct Class *self_type_class = cast(Class(), self->type);
		const struct Class *B_type_class = cast(Class(), B->type);

		throw(MatrixException(), "Sum Error: Trying to sum matrices with different types! (%s and %s)", 
				self_type_class->name, B_type_class->name);
	}

	if (self->rows != B->rows && self->columns != B->columns)
		throw(MatrixException(), "Sum Error: Trying to sum matrices with different dimensions! (%ux%u and %ux%u)", 
				self->rows, self->columns, B->rows, B->columns);

	struct Matrix *result = new(Matrix(), self->type, self->rows, self->columns);

	for (int i = 0; i < self->rows; i++)
	{
		for (int j = 0; j < self->columns; j++)
		{
			result->mass[i][j] = sum(self->mass[i][j], B->mass[i][j]);
		}
	}

	return result;
}

static void* Matrix_subtract(void *_self, void *b)
{
	const struct Matrix *self = cast(Matrix(), _self);
	const struct Matrix *B = cast(Matrix(), b);

	if (self->type != B->type) {
		const struct Class *self_type_class = cast(Class(), self->type);
		const struct Class *B_type_class = cast(Class(), B->type);

		throw(MatrixException(), "Subtract Error: Trying to subtract matrices with different types! (%s and %s)", 
				self_type_class->name, B_type_class->name);
	}

	if (self->rows != B->rows && self->columns != B->columns)
		throw(MatrixException(), "Subtract Error: Trying to subtract matrices with different dimensions! (%ux%u and %ux%u)", 
				self->rows, self->columns, B->rows, B->columns);

	struct Matrix *result = new(Matrix(), self->type, self->rows, self->columns);

	for (int i = 0; i < self->rows; i++)
	{
		for (int j = 0; j < self->columns; j++)
		{
			result->mass[i][j] = subtract(self->mass[i][j], B->mass[i][j]);
		}
	}

	return result;
}

static void* Matrix_product(void *_self, void *b)
{
	const struct Matrix *self = cast(Matrix(), _self);
	const struct Matrix *B = cast(Matrix(), b);

	if (self->type != B->type) {
		const struct Class *self_type_class = cast(Class(), self->type);
		const struct Class *B_type_class = cast(Class(), B->type);

		throw(MatrixException(), "Product Error: Trying to multiply matrices with different types! (%s and %s)", 
				self_type_class->name, B_type_class->name);
	}

	if (self->columns != B->rows)
		throw(MatrixException(), "Product Error: Trying to multiply matrices with different columns and rows number! (%u and %u)", 
				self->columns, B->rows);

	struct Matrix *result = new(Matrix(), self->type, self->rows, B->columns);

	for (int i = 0; i < result->rows; i++)
	{
		for (int j = 0; j < result->columns; j++)
		{
			for (int k = 0; k < self->columns; k++)
			{
				var prod = product(self->mass[i][k], B->mass[k][j]);
				
				if (result->mass[i][j] == NULL)
					result->mass[i][j] = prod;
				else 
				{
					var total = sum(result->mass[i][j], prod);
					
					delete(result->mass[i][j]);
					result->mass[i][j] = total;
					
					delete(prod);
				}
			}
		}
	}

	return result;
}

static void Matrix_scmulti(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	for (int i = 0; i < self->rows; i++)
	{
		va_list ap_copy;
		va_copy(ap_copy, *ap);

		for (int j = 0; j < self->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				throw(MatrixException(), "Scalar Multiply Error: Element at (%u, %u) doesn't exists! (equals NULL)", 
						i, j);

			va_list ap_copy_copy;
			va_copy(ap_copy_copy, ap_copy);

			vscmulti(self->mass[i][j], &ap_copy_copy);
		}
	}
}

static void Matrix_scdivide(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	for (int i = 0; i < self->rows; i++)
	{
		va_list ap_copy;
		va_copy(ap_copy, *ap);

		for (int j = 0; j < self->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				throw(MatrixException(), "Scalar Divide Error: Element at (%u, %u) doesn't exists! (equals NULL)", 
						i, j);

			va_list ap_copy_copy;
			va_copy(ap_copy_copy, ap_copy);

			vscdivide(self->mass[i][j], &ap_copy_copy);
		}
	}
}

static void* Matrix_minorOf(const void *_self, va_list *ap)
{
	const struct Matrix *self = cast(Matrix(), _self);

	unsigned int row = va_arg(*ap, unsigned int);
	unsigned int column = va_arg(*ap, unsigned int);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "MinorOf Error: No such row or/and column with given indexes (%u, %u) in given matrix! (Max: %u, %u)",
				row, column, self->rows - 1, self->columns - 1);

	struct Matrix *result = new(Matrix(), self->type, self->rows - 1, self->columns - 1);
	void ***p = result->mass;

	for (int i = 0; i < self->rows; i++)
	{
		if (i != row)
		{
			for (int j = 0; j < self->columns; j++)
			{
				if (j != column)
				{
					**p = copy(self->mass[i][j]);
					(*p)++;
				}
			}

			(*p) -= self->columns - 1;
			p++;
		}
	}

	return result;
}

static void Matrix_matrix_size(const void *_self, va_list *ap)
{
	const struct Matrix *self = cast(Matrix(), _self);

	unsigned int *rows = va_arg(*ap, unsigned int*);
	unsigned int *columns = va_arg(*ap, unsigned int*);

	*rows = self->rows;
	*columns = self->columns;
}

static void Matrix_determinant(const void *_self, va_list *ap)
{
	const struct Matrix *self = cast(Matrix(), _self);

	if (self->rows != self->columns)
		throw(MatrixException(), "Determinant Error: Can't get determinant of not square matrix! (%u, %u)",
				self->rows, self->columns);

	int *result = va_arg(*ap, int*);
	*result = 0;

	if (self->rows == 2 && self->columns == 2)
	{
		smart var prod1 = product(self->mass[0][0], self->mass[1][1]);
		smart var prod2 = product(self->mass[0][1], self->mass[1][0]);
		smart var sub = subtract(prod1, prod2);

		get(sub, result);
	}
	else
	{
		for (int j = 0; j < self->columns; j++)
		{
			int res;
			smart var minor = minorOf(_self, 0, j);
			determinant(minor, &res);

			if (j % 2 == 0)
			{
				int val;
				get(self->mass[0][j], &val);

				(*result) += val * res;
			}
			else
			{
				smart var tmp_int = copy(self->mass[0][j]);
				scmulti(tmp_int, -1);

				int val;
				get(tmp_int, &val);

				(*result) += val * res;
			}
		}
	}
}

/*
 * Initialization
 */

ClassImpl(MatrixClass)
{
	if (!_MatrixClass)
	{
		_MatrixClass = new(TypeClass(), "MatrixClass",
				TypeClass(), sizeof(struct MatrixClass),
				ctor, MatrixClass_ctor,
				0);
	}

	return _MatrixClass;
}

ClassImpl(Matrix)
{
	if (!_Matrix)
	{
		_Matrix = new(MatrixClass(), "MatrixClass",
				Object(), sizeof(struct Matrix),
				ctor, Matrix_ctor,
				dtor, Matrix_dtor,
				cpy, Matrix_cpy,
				set, Matrix_set,
				get, Matrix_get,
				sum, Matrix_sum,
				subtract, Matrix_subtract,
				product, Matrix_product,
				scmulti, Matrix_scmulti,
				scdivide, Matrix_scdivide,
				minorOf, Matrix_minorOf,
				matrix_size, Matrix_matrix_size,
				determinant, Matrix_determinant,
				0);
	}

	return _Matrix;
}

/*
 * Exception Initialization
 */

ObjectImpl(MatrixException)
{
	if (!_MatrixException)
	{
		_MatrixException = new(ExceptionObject(), "MatrixException");
	}

	return _MatrixException;
}
