/* vim: set fdm=marker : */

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "IO.h"
#include "Exception.h"
#include "Macro.h"
#include "Matrix.h"
#include "ReferenceCounter.h"
#include "Selectors.h"
#include "TypeClass.h"
#include "Utils.h"
#include "Matrix.h"
#include "CollectionInterface.h"

/* MatrixClass {{{ */

static void* MatrixClass_ctor(void *_self, va_list *ap)
{
	struct MatrixClass *self = super_ctor(MatrixClass(), _self, ap);

	init_method(self, minorOf);
	init_method(self, slow_determinant);
	init_method(self, fast_determinant);

	struct CollectionInterface *self_ci = go_to_field(self, struct MatrixClass, ci);
	init_method(self_ci, sort);
	init_method(self_ci, get_size);

	voidf selector;
	va_list ap_copy;
	va_copy(ap_copy, *ap);

	while ((selector = va_arg(ap_copy, voidf)))
	{
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) minorOf)
			update_method(self, minorOf, method);
		else if (selector == (voidf) slow_determinant)
			update_method(self, slow_determinant, method);
		else if (selector == (voidf) fast_determinant)
			update_method(self, fast_determinant, method);

		else if (selector == (voidf) get_size)
			update_method(self_ci, get_size, method);
		else if (selector == (voidf) sort)
			update_method(self_ci, get_size, method);

	}

	va_end(ap_copy);
	return self;
}

/* Selectors */

void* minorOf(const void *_self, size_t row, size_t column)
{
	const struct Class *class = classOf(_self);
	const struct MatrixClass *mclass = cast(TypeClass(), class);

	if (mclass->minorOf.method == NULL)
		throw(MatrixException(), "Error: Class '%s' doesn't have 'minorOf' method!",
				class->name);

	typedef void *(*minorOf_f)(const void *self, size_t row, size_t column);

	void* result = ((minorOf_f) mclass->minorOf.method)(_self, row, column);

	return result;
}

void slow_determinant(const void *_self, void **retval)
{
	const struct Class *class = classOf(_self);
	const struct MatrixClass *mclass = cast(TypeClass(), class);

	if (mclass->slow_determinant.method == NULL)
		throw(MatrixException(), "Error: Class '%s' doesn't have 'slow_determinant' method!",
				class->name);

	typedef void (*slow_determinant_f)(const void *self, void **retval);

	((slow_determinant_f) mclass->slow_determinant.method)(_self, retval);
}

void fast_determinant(const void *_self, void **retval)
{
	const struct Class *class = classOf(_self);
	const struct MatrixClass *mclass = cast(TypeClass(), class);

	if (mclass->fast_determinant.method == NULL)
		throw(MatrixException(), "Error: Class '%s' doesn't have 'fast_determinant' method!",
				class->name);

	typedef void (*fast_determinant_f)(const void *self, void **retval);

	((fast_determinant_f) mclass->fast_determinant.method)(_self, retval);
}

/* }}} */

/* Matrix {{{ */

static void* Matrix_ctor(void *_self, va_list *ap)
{
	struct Matrix *self = super_ctor(Matrix(), _self, ap);

	size_t columns, rows;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	self->type = cast(TypeClass(), va_arg(ap_copy, const void*));
	rows = va_arg(ap_copy, size_t);
	columns = va_arg(ap_copy, size_t);
	self->mass = NULL;

	self->determinant = NULL;
	self->changed = 0;

	if (rows == 0 || columns == 0) {
		delete(self);
		throw(MatrixException(), "Error: Can't create matrix with zero rows or columns! (%lu, %lu)",
				rows, columns);
	}

	self->rows = rows;
	self->columns = columns;

	void ***mass;

	mass = (void***)calloc(sizeof(void**), self->rows);
	if (mass == NULL)
	{
		delete(self);
		throw(MatrixException(), "Fatal Error: Matrix pointers allocation error!");
	}

	mass[0] = (void**)calloc(sizeof(void*), self->rows * self->columns);
	if (mass[0] == NULL)
	{
		delete(self);
		throw(MatrixException(), "Fatal Error: Matrix items allocation error!");
	}

	for (size_t i = 0; i < self->rows; i++)
	{
		mass[i] = mass[0] + i * self->columns;
	}

	va_end(ap_copy);
	self->mass = mass;

	return self;
}

static void* Matrix_cpy(const void *_self, void *_object)
{
	const struct Matrix *self = cast(Matrix(), _self);
	struct Matrix *object = super_cpy(Matrix(), _self, _object);

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

	for (size_t i = 0; i < object->rows; i++)
	{
		mass[i] = mass[0] + i * object->columns;

		for (size_t j = 0; j < object->columns; j++)
		{
			if (self->mass[i][j] != NULL)
				mass[i][j] = copy(self->mass[i][j]);
		}
	}

	object->mass = mass;

	return object;
}

static void Matrix_set(void *_self, va_list *ap)
{
	struct Matrix *self = cast(Matrix(), _self);

	size_t row = va_arg(*ap, size_t);
	size_t column = va_arg(*ap, size_t);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Error: Element at (%lu, %lu) is out of bounds!", 
				row, column);

	if (self->mass[row][column] == NULL)
		self->mass[row][column] = vnew(self->type, ap);
	else
		vset(self->mass[row][column], ap);

	self->changed = 1;
}

static void Matrix_get(const void *_self, va_list *ap)
{
	const struct Matrix *self = cast(Matrix(), _self);

	size_t row = va_arg(*ap, size_t);
	size_t column = va_arg(*ap, size_t);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Error: Element at (%lu, %lu) is out of bounds!", 
				row, column);

	if (self->mass[row][column] == NULL)
		throw(MatrixException(), "Error: Element at (%lu, %lu) doesn't exist! (equals NULL)", 
				row, column);

	vget(self->mass[row][column], ap);
}

static void* Matrix_dtor(void *_self)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->mass != NULL)
	{
		if (self->mass[0] != NULL)
		{
			for (size_t i = 0; i < self->rows; ++i) 
			{
				for (size_t j = 0; j < self->columns; ++j) 
				{
					if (self->mass[i][j] != NULL)
						delete(self->mass[i][j]);
				}
			}

			free(self->mass[0]);
		}
		
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

	for (size_t i = 0; i < self->rows; ++i) 
	{
		for (size_t j = 0; j < self->columns; ++j) 
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

			for (size_t i = 0; i < self->rows; ++i) 
			{
				for (size_t j = 0; j < self->columns; ++j) 
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
			for (size_t i = 0; i < self->rows; ++i) 
			{
				for (size_t j = 0; j < self->columns; ++j) 
				{
					int sz = sfprint(self->mass[i][j], stream, 0, NULL, 0, flag, width, precision);

					if (sz <= 0)
						throw(IOException(), "Error: some error occured during printing!");

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

		for (size_t i = 0; i < self->rows; ++i)
		{
			for (size_t j = 0; j < self->columns && psize != 1; ++j) 
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
		for (size_t i = 0; i < self->rows; ++i) 
		{
			for (size_t j = 0; j < self->columns; ++j) 
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

							throw(IOException(), "Error: didn't expect newline character after %lu%s element at %lu%s row!", 
									j, clspel, i + 1, rspel);
						}
					}
					ungetc(c, stream);
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

						throw(IOException(), "Error: couldn't find newline character after scanning %lu%s row!", 
								i + 1, rspel);
					}
				}
			}
		}
	}
	else if (buffer != NULL)
	{
		const char *p = buffer;

		for (size_t i = 0; i < self->rows; ++i) 
		{
			for (size_t j = 0; j < self->columns; ++j) 
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

							throw(IOException(), "Error: didn't expect newline character after %lu%s element at %lu%s row!", 
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

						throw(IOException(), "Error: couldn't find newline character after scanning %lu%s row!", 
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
		throw(MatrixException(), "Error: Trying to sum matrices with different dimensions! (%lux%lu and %lux%lu)", 
				self->rows, self->columns, B->rows, B->columns);

	struct Matrix *result = new(Matrix(), self->type, self->rows, self->columns);

	for (size_t i = 0; i < self->rows; i++)
	{
		for (size_t j = 0; j < self->columns; j++)
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
		throw(MatrixException(), "Error: Trying to subtract matrices with different dimensions! (%lux%lu and %lux%lu)", 
				self->rows, self->columns, B->rows, B->columns);

	struct Matrix *result = new(Matrix(), self->type, self->rows, self->columns);

	for (size_t i = 0; i < self->rows; i++)
	{
		for (size_t j = 0; j < self->columns; j++)
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
		throw(MatrixException(), "Error: Trying to multiply matrices with different columns and rows number! (%lu and %lu)", 
				self->columns, B->rows);

	struct Matrix *result = new(Matrix(), self->type, self->rows, B->columns);

	for (size_t i = 0; i < result->rows; i++)
	{
		for (size_t j = 0; j < result->columns; j++)
		{
			for (size_t k = 0; k < self->columns; k++)
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

	for (size_t i = 0; i < self->rows; i++)
	{
		va_list ap_copy;
		va_copy(ap_copy, *ap);

		for (size_t j = 0; j < self->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				throw(MatrixException(), "Error: Element at (%lu, %lu) doesn't exists! (equals NULL)", 
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

	for (size_t i = 0; i < self->rows; i++)
	{
		va_list ap_copy;
		va_copy(ap_copy, *ap);

		for (size_t j = 0; j < self->columns; j++)
		{
			if (self->mass[i][j] == NULL)
				throw(MatrixException(), "Error: Element at (%lu, %lu) doesn't exists! (equals NULL)", 
						i, j);

			va_list ap_copy_copy;
			va_copy(ap_copy_copy, ap_copy);

			vscdivide(self->mass[i][j], &ap_copy_copy);
		}
	}

	self->changed = 1;
}

static void* Matrix_minorOf(const void *_self, size_t row, size_t column)
{
	const struct Matrix *self = cast(Matrix(), _self);

	if (row >= self->rows || column >= self->columns)
		throw(MatrixException(), "Error: No such row or/and column with given indexes (%lu, %lu) in given matrix! (Max: %lu, %lu)",
				row, column, self->rows - 1, self->columns - 1);

	struct Matrix *result = new(Matrix(), self->type, self->rows - 1, self->columns - 1);
	void ***p = result->mass;

	for (size_t i = 0; i < self->rows; i++)
	{
		if (i != row)
		{
			for (size_t j = 0; j < self->columns; j++)
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

static void Matrix_get_size(const void *_self, va_list *ap)
{
	const struct Matrix *self = cast(Matrix(), _self);

	size_t *rows = va_arg(*ap, size_t*);
	size_t *columns = va_arg(*ap, size_t*);

	*rows = self->rows;
	*columns = self->columns;
}

static void Matrix_slow_determinant(void *_self, void **retval)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->changed == 1 || self->determinant == NULL)
	{
		if (self->rows != self->columns)
			throw(MatrixException(), "Error: Can't get determinant of not square matrix! (%lu, %lu)",
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
			for (size_t j = 0; j < self->columns; j++)
			{
				var res;
				smart var minor = Matrix_minorOf(_self, 0, j);
				Matrix_slow_determinant(minor, &res);

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
	}
	
	*retval = copy(self->determinant);
}

static void Matrix_fast_determinant(void *_self, void **retval)
{
	struct Matrix *self = cast(Matrix(), _self);

	if (self->changed == 1 || self->determinant == NULL)
	{
		if (self->rows != self->columns)
			throw(MatrixException(), "Error: Can't get determinant of not square matrix! (%lu, %lu)",
					self->rows, self->columns);

		var result = new(self->type);
		set_to_one(result);

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
			for (size_t i = 0; i < self->rows; ++i) 
			{
				int k = i;

				for (size_t j = i + 1; j < self->rows; ++j) 
				{
					smart var abs1 = absolute(self->mass[j][i]);
					smart var abs2 = absolute(self->mass[k][i]);

					if (cmp(abs1, abs2) > 0)
					{
						k = j;
					}
				}

				smart var tmp1 = absolute(self->mass[k][i]);
				if (cmp_to_zero(tmp1) == 0)
				{
					result = new(self->type);
					set_to_zero(result);
					break;
				}

				if (k != i)
				{
					void **tmp2 = self->mass[i];
					self->mass[i] = self->mass[k];
					self->mass[k] = tmp2;

					inverse_add(result);
				}

				smart var prod1 = product(result, self->mass[i][i]);
				delete(result);
				result = retain(prod1);

				for (size_t j = i + 1; j < self->columns; ++j)
				{
					smart var div = divide(self->mass[i][j], self->mass[i][i]);
					delete(self->mass[i][j]);
					self->mass[i][j] = retain(div);
				}

				set_to_one(self->mass[i][i]);

				for (size_t j = i + 1; j < self->rows; ++j) 
				{
					smart var tmp2 = absolute(self->mass[j][i]);
					if (cmp_to_zero(tmp2) > 0)
					{
						smart var tmp3 = copy(self->mass[j][i]);

						for (size_t l = i; l < self->columns; ++l) 
						{
							smart var div = divide(tmp3, self->mass[i][i]);
							smart var prod2 = product(div, self->mass[i][l]);
							smart var sub = subtract(self->mass[j][l], prod2);
							delete(self->mass[j][l]);
							self->mass[j][l] = retain(sub);
						}
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

static void* Matrix_rnd(void *_self, va_list *ap)
{
	struct Matrix *self;

	if (_self)
		self = cast(Matrix(), _self);
	else
	{
		const void *type = cast(TypeClass(), va_arg(*ap, void*));
		size_t rows = va_arg(*ap, size_t);
		size_t columns = va_arg(*ap, size_t);

		self = new(Matrix(), type, rows, columns);
	}

	for (size_t i = 0; i < self->rows; ++i) 
	{
		for (size_t j = 0; j < self->columns; j++) 
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

/* }}} */

/* Initialization {{{ */


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
				get_size, Matrix_get_size,
				slow_determinant, Matrix_slow_determinant,
				fast_determinant, Matrix_fast_determinant,
				rnd, Matrix_rnd,
				0);
	}

	return _Matrix;
}

/* Exception init */

ObjectImpl(MatrixException)
{
	if (!_MatrixException)
	{
		_MatrixException = new(ExceptionObject(), "MatrixException");
	}

	return _MatrixException;
}

/* }}} */
