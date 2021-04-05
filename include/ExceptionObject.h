#ifndef EXCEPTIONOBJECT_H_OQI38Z0M
#define EXCEPTIONOBJECT_H_OQI38Z0M

/**
 * @file ExceptionObject.h
 * @brief ExceptionObject and it's selectors
 * @author infastin
 * @version 1.0
 * @date 2021-03-26
 */


#include "Object.h"

ClassHeader(ExceptionObject);

struct ExceptionObject
{
	const struct Object _;
	const char *name;
	char *msg;
	char *file;
	int   line;
	const char *func;
};


/**
 * @brief Get message of exception
 *
 * @param self ExceptionObject
 *
 * @return C string with message
 */
char* exception_message(const void *self);


/**
 * @brief Get name of exception
 *
 * @param self ExceptionObject
 *
 * @return C string with name
 */
const char* exception_name(const void *self);


/**
 * @brief Get full path to file, where exception where thrown
 *
 * @param self ExceptionObject
 *
 * @return C string with full path to file
 */
char* exception_file(const void *self);


/**
 * @brief Get function name, where exception where thrown
 *
 * @param self ExceptionObject
 *
 * @return C string with full path to file
 */
const char* exception_func(const void *self);


/**
 * @brief Get line, where exception where thrown
 *
 * @param self ExceptionObject
 *
 * @return Line number
 */
int   exception_line(const void *self);

#endif /* end of include guard: EXCEPTIONOBJECT_H_OQI38Z0M */
