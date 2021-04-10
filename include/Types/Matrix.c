#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "IO.h"
#include "Exception.h"
#include "Matrix.h"
#include "ReferenceCounter.h"
#include "Selectors.h"
#include "TypeClass.h"
#include "Utils.h"

/*
 * MatrixClass
 */

// ---

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

// ---

void* minorOf(const void *_self, unsigned int row, unsigned int column)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->minorOf == NULL)
		throw(MatrixException(), "Error: Matrix '%s' doesn't have 'minorOf' method!",
				class->name);

	void* result = mclass->minorOf(_self, row, column);

	return result;
}

void matrix_size(const void *_self, unsigned int *row, unsigned int *column)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->matrix_size == NULL)
		throw(MatrixException(), "Error: Matrix '%s' doesn't have 'matrix_size' method!",
				class->name);

	mclass->matrix_size(_self, row, column);
}

void determinant(const void *_self, void **retval)
{
	const struct MatrixClass *mclass = cast(TypeClass(), classOf(_self));
	const struct Class *class = _self;

	if (mclass->determinant == NULL)
		throw(MatrixException(), "Error: Matrix '%s' doesn't have 'determinant' method!",
				class->name);

	mclass->determinant(_self, retval);
}

/*
 * Matrix
 */

// ---

static void* Matrix_ctor(void *_self, va_list *ap)
{
	struct Matrix *self = super_ctor(Matrix(), _self, ap);

	unsigned int columns, rows;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	self->type = cast(TypeClass(), va_arg(ap_copy, void*));
	rows = va_arg(ap_copy, unsigned int);
	columns = va_arg(ap_copy, unsigned int);
	self->mass = NULL;

	self->determinant = NULL;
	self->changed = 0;

	if (rows == 0 || columns == 0) {
		delete(self);
		throw(MatrixException(), "Error: Can't create matrix with zero rows or columns! (%u, %u)",
				rows, columns);
	}

	self->rows = rows;
	self->columns = columns;

	void ***mass;

	mass = (void***)calloc(sizeof(void**), self->rows);
	if (mass == NULL)
		throw(MatrixException(), "Fatal Error: Matrix pointers allocation error!");

	mass[0] = (void**)calloc(sizeof(void*), self->rows * self->columns);
	if (mass[0] == NULL)
		throw(MatrixException(), "Fatal Error: Matrix items allocation error!");

	for (unsigned int i = 0; i < self->rows; i++)
	{
		mass[i] = mass[0] + i * self->columns;

		for (unsigned int j = 0; j < self->columns; j++)
		{
			mass[i][j] = NULL;
		}
	}

	va_end(ap_copy);
	self->mass = mass;

	return self;
}

static void* Matrix_cpy(const void *_self, void *_object)
{
	const struct Matrix *self = _self;
	smart struct Matrix *object = super_cpy(Matrix(), _self, _object);

	object->rows = self->rows;
	object->columns = self->columns;
	object->type =self->type;

	object->determinant = copy(self->determinant);
	object->changed = self->changed;

	void ***mass;

	mass = (void***)calloc(sizeof(void**), object->rows);
	if (mass == NULL)
	{
		delete(object);
		throw(MatrixException(), "Fatal Error: Matrix pointers allocation error!");
	}

	mass[0] = (void**)calloc(sizeof(void*), object->rows * object->columns);
	if (mass[0] == NULL)
	{
		free(mass);
		delete(object);
		throw(MatrixException(), "Fatal Error: Matrix items allocation error!");
	}

	for (unsigned int i = 0; i < object->rows; i++)
	{
		mass[i] = mass[0] + i * object->columns;

		for (unsigned int j = 0; j < object->columns; j++)
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
		throw(MatrixException(), "Error: Element at (%u, %u) is out of bounds!", 
				row, column);

	if (self->mass[row][column] == NULL)
		self->mass[row][column] = vnew(self->type, ap);
	else
		vset(self->mass[row][column], ap);

	self->changed = 1;
}

static void Matrix_get(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	unsigned int row = va_arg(*ap, unsigned int);
	unsigned int column = va_arg(*ap, unsigned int);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Error: Element at (%u, %u) is out of bounds!", 
				row, column);

	if (self->mass[row][column] == NULL)
		throw(MatrixException(), "Error: Element at (%u, %u) doesn't exists! (equals NULL)", 
				row, column);

	vget(self->mass[row][column], ap);
}

static void* Matrix_dtor(void *_self)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->mass != NULL)
	{
		for (unsigned int i = 0; i < self->rows; i++)
		{
			for (unsigned int j = 0; j < self->columns; j++)
			{
				delete(self->mass[i][j]);
			}
		}

		free(self->mass[0]);
		free(self->mass);
	}

	return super_dtor(Matrix(), _self);
}

static int Matrix_sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision)
{
	const struct Matrix *self = cast(Matrix(), _self);

	// Getting size
	int size = 0;

	for (unsigned int i = 0; i < self->rows; ++i) 
	{
		for (unsigned int j = 0; j < self->columns; ++j) 
		{
			int chck = sfprint(self->mass[i][j], NULL, 0, NULL, 0, flag, width, precision);
	
			if (chck > 0)
				size += chck;
			else
				throw(IOException(), "Error: some error occured during printing!");

			if (j != self->columns - 1)
				size++;
		}

		if (i != self->rows - 1)
			size++;
	}

	// Getting result
	if (stream != NULL)
	{
		if (bin)
		{
			size = 0;

			for (unsigned int i = 0; i < self->rows; ++i) 
			{
				for (unsigned int j = 0; j < self->columns; ++j) 
				{
					int sz = sfprint(self->mass[i][j], stream, 1, NULL, 0, flag, width, precision);

					if (sz > 0)
						size += sz;
					else
						throw(IOException(), "Error: some error occured during printing!");
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < self->rows; ++i) 
			{
				for (unsigned int j = 0; j < self->columns; ++j) 
				{
					sfprint(self->mass[i][j], stream, 0, NULL, 0, flag, width, precision);

					if (j != self->columns - 1)
						fputc(' ', stream);
				}

				if (i != self->rows - 1)
					fputc('\n', stream);
			}
		}
	}
	else if (buffer != NULL)
	{
		char *p = buffer;
		int psize;

		if (maxn < size + 1)
			psize = size + 1;
		else
			psize = maxn;

		for (unsigned int i = 0; i < self->rows; ++i)
		{
			for (unsigned int j = 0; j < self->columns && psize != 1; ++j) 
			{
				int sz = sfprint(self->mass[i][j], NULL, 0, p, psize + 1, flag, width, precision);

				if (sz > 0)
				{
					
					psize -= sz;
					p += sz;

					if (j != self->columns - 1)
					{
						*p++ = ' ';
						psize--;
					}
				}
				else
				{
					throw(IOException(), "Error: some error occured during printing!");
				}
			}

			if (psize == 1)
				break;

			if (i != self->rows - 1)
			{
				*p++ = '\n';
				psize--;
			}
		}

		*p = 0;
	}

	return size;
}

static int Matrix_sfscan(void *_self, FILE *stream, int bin, const char *buffer, int *numb,
		int asterisk, int width)
{
	struct Matrix *self = cast(Matrix(), _self);

	int numb_res = 0;
	int result = 0;

	if (stream != NULL)
	{
		for (unsigned int i = 0; i < self->rows; ++i) 
		{
			for (unsigned int j = 0; j < self->columns; ++j) 
			{
				if (!self->mass[i][j])
					self->mass[i][j] = new(self->type);

				if (j != 0 && bin == 0)
				{
					int c;
					while (isspace(c = fgetc(stream)))
					{
						if (c == '\n' || c == EOF)
						{
							char *rspel = int_spelling(i + 1);
							char *clspel = int_spelling(j);

							throw(IOException(), "Error: didn't expect newline character after %u%s element at %u%s row!", 
									j, clspel, i + 1, rspel);
						}
					}
				}

				int n;
				int res = sfscan(self->mass[i][j], stream, bin, NULL, &n, asterisk, width);

				if (res > 0)
				{
					result += res;
					numb_res += n;
				}
				else
					throw(IOException(), "Error: some error occured during scanning!");
			}

			if (bin == 0 && i != self->rows - 1)
			{
				int c;
				while ((c = fgetc(stream)) != '\n')
				{
					if (!isspace(c) || c == EOF)
					{
						char *rspel = int_spelling(i + 1);

						throw(IOException(), "Error: couldn't find newline character after scanning %u%s row!", 
								i + 1, rspel);
					}
				}
			}
		}
	}
	else if (buffer != NULL)
	{
		const char *p = buffer;

		for (unsigned int i = 0; i < self->rows; ++i) 
		{
			for (unsigned int j = 0; j < self->columns; ++j) 
			{
				if (!self->mass[i][j])
					self->mass[i][j] = new(self->type);

				if (j != 0)
				{
					while (isspace(*p))
					{
						if (*p == '\n' || *p == 0)
						{
							char *rspel = int_spelling(i + 1);
							char *clspel = int_spelling(j);

							throw(IOException(), "Error: didn't expect newline character after %u%s element at %u%s row!", 
									j, clspel, i + 1, rspel);
						}

						p++;
					}
				}

				int n;
				int res = sfscan(self->mass[i][j], NULL, 0, p, &n, asterisk, width);

				if (res > 0)
				{
					p += n;

					result += res;
					numb_res += n;
				}
				else
					throw(IOException(), "Error: some error occured during scanning!");
			}

			if (i != self->rows - 1)
			{
				while(*p != '\n')
				{
					if (!isspace(*p) || *p == 0)
					{
						char *rspel = int_spelling(i + 1);

						throw(IOException(), "Error: couldn't find newline character after scanning %u%s row!", 
								i + 1, rspel);
					}

					p++;
				}
			}
		}
	}

	if (numb != NULL)
		*numb = numb_res;

	self->changed = 1;

	return result;
}

static void* Matrix_sum(const void *_self, const void *b)
{
	const struct Matrix *self = cast(Matrix(), _self);
	const struct Matrix *B = cast(Matrix(), b);

	if (self->type != B->type) 
	{
		const struct Class *self_type_class = cast(Class(), self->type);
		const struct Class *B_type_class = cast(Class(), B->type);

		throw(MatrixException(), "Error: Trying to sum matrices with different types! (%s and %s)", 
				self_type_class->name, B_type_class->name);
	}

	if (self->rows != B->rows && self->columns != B->columns)
		throw(MatrixException(), "Error: Trying to sum matrices with different dimensions! (%ux%u and %ux%u)", 
				self->rows, self->columns, B->rows, B->columns);

	struct Matrix *result = new(Matrix(), self->type, self->rows, self->columns);

	for (unsigned int i = 0; i < self->rows; i++)
	{
		for (unsigned int j = 0; j < self->columns; j++)
		{
			result->mass[i][j] = sum(self->mass[i][j], B->mass[i][j]);
		}
	}

	return result;
}

static void* Matrix_subtract(const void *_self, const void *b)
{
	const struct Matrix *self = cast(Matrix(), _self);
	const struct Matrix *B = cast(Matrix(), b);

	if (self->type != B->type) 
	{
		const struct Class *self_type_class = cast(Class(), self->type);
		const struct Class *B_type_class = cast(Class(), B->type);

		throw(MatrixException(), "Error: Trying to subtract matrices with different types! (%s and %s)", 
				self_type_class->name, B_type_class->name);
	}

	if (self->rows != B->rows && self->columns != B->columns)
		throw(MatrixException(), "Error: Trying to subtract matrices with different dimensions! (%ux%u and %ux%u)", 
				self->rows, self->columns, B->rows, B->columns);

	struct Matrix *result = new(Matrix(), self->type, self->rows, self->columns);

	for (unsigned int i = 0; i < self->rows; i++)
	{
		for (unsigned int j = 0; j < self->columns; j++)
		{
			result->mass[i][j] = subtract(self->mass[i][j], B->mass[i][j]);
		}
	}

	return result;
}

static void* Matrix_product(const void *_self, const void *b)
{
	const struct Matrix *self = cast(Matrix(), _self);
	const struct Matrix *B = cast(Matrix(), b);

	if (self->type != B->type) 
	{
		const struct Class *self_type_class = cast(Class(), self->type);
		const struct Class *B_type_class = cast(Class(), B->type);

		throw(MatrixException(), "Error: Trying to multiply matrices with different types! (%s and %s)", 
				self_type_class->name, B_type_class->name);
	}

	if (self->columns != B->rows)
		throw(MatrixException(), "Error: Trying to multiply matrices with different columns and rows number! (%u and %u)", 
				self->columns, B->rows);

	struct Matrix *result = new(Matrix(), self->type, self->rows, B->columns);

	for (unsigned int i = 0; i < result->rows; i++)
	{
		for (unsigned int j = 0; j < result->columns; j++)
		{
			for (unsigned int k = 0; k < self->columns; k++)
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

	for (unsigned int i = 0; i < self->rows; i++)
	{
		va_list ap_copy;
		va_copy(ap_copy, *ap);

		for (unsigned int j = 0; j < self->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				throw(MatrixException(), "Error: Element at (%u, %u) doesn't exists! (equals NULL)", 
						i, j);

			va_list ap_copy_copy;
			va_copy(ap_copy_copy, ap_copy);

			vscmulti(self->mass[i][j], &ap_copy_copy);
		}
	}

	self->changed = 1;
}

static void Matrix_scdivide(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	for (unsigned int i = 0; i < self->rows; i++)
	{
		va_list ap_copy;
		va_copy(ap_copy, *ap);

		for (unsigned int j = 0; j < self->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				throw(MatrixException(), "Error: Element at (%u, %u) doesn't exists! (equals NULL)", 
						i, j);

			va_list ap_copy_copy;
			va_copy(ap_copy_copy, ap_copy);

			vscdivide(self->mass[i][j], &ap_copy_copy);
		}
	}

	self->changed = 1;
}

static void* Matrix_minorOf(const void *_self, unsigned int row, unsigned int column)
{
	const struct Matrix *self = cast(Matrix(), _self);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Error: No such row or/and column with given indexes (%u, %u) in given matrix! (Max: %u, %u)",
				row, column, self->rows - 1, self->columns - 1);

	struct Matrix *result = new(Matrix(), self->type, self->rows - 1, self->columns - 1);
	void ***p = result->mass;

	for (unsigned int i = 0; i < self->rows; i++)
	{
		if (i != row)
		{
			for (unsigned int j = 0; j < self->columns; j++)
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

static void Matrix_matrix_size(const void *_self, unsigned int *rows, unsigned int *columns)
{
	const struct Matrix *self = cast(Matrix(), _self);

	*rows = self->rows;
	*columns = self->columns;
}

static void Matrix_determinant(void *_self, void **retval)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->changed == 1 || self->determinant == NULL)
	{
		if (self->rows != self->columns)
			throw(MatrixException(), "Error: Can't get determinant of not square matrix! (%u, %u)",
					self->rows, self->columns);

		var result = NULL;

		if (self->rows == 1 && self->columns == 1)
		{
			result = copy(self->mass[0][0]);
		}
		else if (self->rows == 2 && self->columns == 2)
		{
			smart var prod1 = product(self->mass[0][0], self->mass[1][1]);
			smart var prod2 = product(self->mass[0][1], self->mass[1][0]);
			smart var sub = subtract(prod1, prod2);

			result = retain(sub);
		}
		else
		{
			for (unsigned int j = 0; j < self->columns; j++)
			{
				var res;
				smart var minor = Matrix_minorOf(_self, 0, j);
				Matrix_determinant(minor, &res);

				if (j % 2 == 0)
				{
					smart var prod = product(self->mass[0][j], res);

					if (result)
					{
						smart var summ = sum(result, prod);
						delete(result);
						result = retain(summ);
					}
					else
					{
						result = retain(prod);
					}
				}
				else
				{
					smart var tmp = inverse_add(self->mass[0][j]);
					smart var prod = product(tmp, res);

					if (result)
					{
						smart var summ = sum(result, prod);
						delete(result);
						result = retain(summ);
					}
					else
					{
						result = retain(prod);
					}			
				}
			}
		}

		self->changed = 0;
		self->determinant = result;

		*retval = result;
	}
	else
		*retval = self->determinant;
}

static void Matrix_fast_determinant(void *_self, void **retval)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->changed == 1 || self->determinant == NULL)
	{
		if (self->rows != self->columns)
			throw(MatrixException(), "Error: Can't get determinant of not square matrix! (%u, %u)",
					self->rows, self->columns);

		var result = NULL;

		if (self->rows == 1 && self->columns == 1)
		{
			result = copy(self->mass[0][0]);
		}
		else if (self->rows == 2 && self->columns == 2)
		{
			smart var prod1 = product(self->mass[0][0], self->mass[1][1]);
			smart var prod2 = product(self->mass[0][1], self->mass[1][0]);
			smart var sub = subtract(prod1, prod2);

			result = retain(sub);
		}
		else
		{
			
		}

		self->changed = 0;
		self->determinant = result;

		*retval = result;
	}
	else
		*retval = self->determinant;
}

static void* Matrix_rnd(void *_self, va_list *ap)
{
	struct Matrix *self;

	if (_self)
		self = cast(Matrix(), _self);
	else
	{
		const void *type = cast(TypeClass(), va_arg(*ap, void*));
		unsigned int rows = va_arg(*ap, unsigned int);
		unsigned int columns = va_arg(*ap, unsigned int);

		self = new(Matrix(), type, rows, columns);
	}

	for (unsigned int i = 0; i < self->rows; ++i) 
	{
		for (unsigned int j = 0; j < self->columns; j++) 
		{
			va_list ap_copy;
			va_copy(ap_copy, *ap);
			self->mass[i][j] = vrnd(self->type, self->mass[i][j], &ap_copy);
			va_end(ap_copy);
		}
	}

	self->changed = 1;

	return self;
}

/*
 * Initialization
 */

// ---

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
		_Matrix = new(MatrixClass(), "Matrix",
				Object(), sizeof(struct Matrix),
				ctor, Matrix_ctor,
				dtor, Matrix_dtor,
				cpy, Matrix_cpy,
				set, Matrix_set,
				get, Matrix_get,
				sfprint, Matrix_sfprint,
				sfscan, Matrix_sfscan,
				sum, Matrix_sum,
				subtract, Matrix_subtract,
				product, Matrix_product,
				scmulti, Matrix_scmulti,
				scdivide, Matrix_scdivide,
				minorOf, Matrix_minorOf,
				matrix_size, Matrix_matrix_size,
				determinant, Matrix_determinant,
				rnd, Matrix_rnd,
				0);
	}

	return _Matrix;
}

/*
 * Exception Initialization
 */

// ---

ObjectImpl(MatrixException)
{
	if (!_MatrixException)
	{
		_MatrixException = new(ExceptionObject(), "MatrixException");
	}

	return _MatrixException;
}
