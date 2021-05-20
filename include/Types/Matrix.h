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

#include "CollectionInterface.h"
#include "Selectors.h"
#include "TypeClass.h"

ClassHeader(MatrixClass);
ClassHeader(Matrix);
ObjectHeader(MatrixException);

struct MatrixClass
{
	const struct TypeClass _;
	method minorOf;
	method slow_determinant;
	method fast_determinant;
	const struct CollectionInterface ci;
};

struct Matrix
{
	const struct Object _;
	const struct TypeClass *type;
	size_t rows;
	size_t columns;
	void ***mass;

	int changed;
	void* determinant;
};

/**
 * @brief Get matrix without row and column (not determinant)
 *
 * @param[in] self   Matrix
 * @param[in] row    Which row to remove
 * @param[in] column Which column to remove
 *
 * @throw MatrixException
 *
 * @return Matrix without row and column
 */
void* minorOf(const void *self, size_t row, size_t column);

/**
 * @brief Slowly calculate matrix determinant
 *
 * @param[in]  self   Matrix
 * @param[out] retval Where will be determinant
 *
 * @throw MatrixException
 */
void slow_determinant(const void *self, var *retval);

/**
 * @brief Fastly calculate matrix determinant
 *
 * @param[in]  self   Matrix
 * @param[out] retval Where will be determinant
 *
 * @throw MatrixException
 */
void fast_determinant(const void *self, var *retval);

#endif /* end of include guard: MATRIX_H_720USNE8 */
