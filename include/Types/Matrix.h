#ifndef MATRIX_H_720USNE8
#define MATRIX_H_720USNE8

/**
 * @file Matrix.h
 * @brief Matrix Class
 * @author infastin
 * @version 1.0
 * @date 2021-03-23
 */

#include <stdarg.h>

#include "TypeClass.h"

ClassHeader(Matrix);
ClassHeader(MatrixClass);
ObjectHeader(MatrixException);

typedef void *(*minorOf_f)(const void *self, va_list *ap);
typedef void *(*matrix_size_f)(const void *self, va_list *ap);
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
 * @brief Get minor of matrix
 *
 * @param[in]  self    Matrix
 * @param[out] va_args Which row and column to remove
 *
 * @throw MatrixException If object doesn't have corresponding method
 *
 * @return Minor of Matrix
 */
void* minorOf(const void *self, ...);

/**
 * @brief Get matrix size
 *
 * @param[in]  self    Matrix
 * @param[out] va_args Pointers to the coords
 *
 * @throw MatrixException If object doesn't have corresponding method
 */
void matrix_size(const void *self, ...);

/**
 * @brief Calculate square matrix determinant
 *
 * @param[in]  self   Matrix
 * @param[out] retval Pointer to variable
 *
 * @throw MatrixException If object doesn't have corresponding method
 */
void determinant(const void *self, void **retval);

/**
 * @brief Same as minorOf
 */
void* vminorOf(const void *_self, va_list *ap);

/**
 * @brief Same as matrix_size
 */
void vmatrix_size(const void *_self, va_list *ap);

#endif /* end of include guard: MATRIX_H_720USNE8 */
