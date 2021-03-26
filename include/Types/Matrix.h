#ifndef MATRIX_H_720USNE8
#define MATRIX_H_720USNE8

/**
 * @file Matrix.h
 * @brief MatrixClass, it's selectors and Matrix
 * @author infastin
 * @version 1.0
 * @date 2021-03-23
 */

#include <stdarg.h>

#include "Selectors.h"
#include "TypeClass.h"

ClassHeader(Matrix);
ClassHeader(MatrixClass);
ObjectHeader(MatrixException);

typedef void *(*minorOf_f)(const void *self, unsigned int row, unsigned int column);
typedef void *(*matrix_size_f)(const void *self, unsigned int *row, unsigned int *column);
typedef void  (*determinant_f)(const void *self, void **retval);

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

/**
 * @brief Get matrix without row and column (not determinant)
 *
 * @param[in] self   Matrix
 * @param[in] row    Which row to remove
 * @param[in] column Which column to remove
 *
 * @throw MatrixException If object doesn't have corresponding method
 *
 * @return Matrix without row and column
 */
void* minorOf(const void *self, unsigned int row, unsigned int column);

/**
 * @brief Get matrix size
 *
 * @param[in]  self    Matrix
 * @param[out] rows    Where will be number of rows
 * @param[out] columns Where will be number of columns
 *
 * @throw MatrixException If object doesn't have corresponding method
 */
void matrix_size(const void *self, unsigned int *rows, unsigned int *columns);

/**
 * @brief Calculate matrix determinant
 *
 * @param[in]  self   Matrix
 * @param[out] retval Where will be determinant
 *
 * @throw MatrixException If object doesn't have corresponding method
 */
void determinant(const void *self, var *retval);

#endif /* end of include guard: MATRIX_H_720USNE8 */
