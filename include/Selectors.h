#ifndef NEW_H_0FEJN6R1
#define NEW_H_0FEJN6R1

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "Object.h"

/**
 * @file Selectors.h
 * @brief Basic object selectors
 * @author infastin
 * @version 1.0
 * @date 2021-03-23
 */

typedef void* var;

/**
 * @brief new wraps this function to provide better logging and debuging
 */
void* _new(const void *class, char *file, int line, const char *func, ...);


/**
 * @brief Same as _new
 */
void* _vnew(const void *class, char *file, int line, const char *func, va_list *ap);


/**
 * @brief delete wraps this function to provide better logging and debuging
 */
void _delete(void *self, char *file, int line, const char *func);


/**
 * @brief copy wraps this function to provide better logging and debuging
 */
void* _copy(const void *self, char *file, int line, const char *func);


/**
 * @brief Calls method ctor of object's class
 *
 * @param self Object
 * @param ap   List of arguments
 *
 * @return Object
 */
void* ctor(void *self, va_list *ap);


/**
 * @brief Calls method dtor of object's class
 *
 * @param self Object
 * @param ap   List of arguments
 *
 * @return Object
 */
void* dtor(void *self);


/**
 * @brief Calls method cpy of object's class
 *
 * @param self Object
 * @param ap   List of arguments
 *
 * @return Object
 */
void* cpy(const void *self, void *object);


/**
 * @brief Get superclass of object's class
 *
 * @param self Object
 *
 * @return Class
 */
const void* super(const void *self);


/**
 * @brief Calls method ctor of superclass of object's class
 *
 * @param class Class
 * @param self  Object
 * @param ap    List of arguments
 *
 * @return Object
 */
void* super_ctor(const void *class, void *self, va_list *ap);


/**
 * @brief Calls method dtor of superclass of object's class
 *
 * @param class Class
 * @param self  Object
 * @param ap    List of arguments
 *
 * @return Object
 */
void* super_dtor(const void *class, void *self);


/**
 * @brief Calls method cpy of superclass of object's class
 *
 * @param class Class
 * @param self  Object
 * @param ap    List of arguments
 *
 * @return Object
 */
void* super_cpy(const void *class, const void *self, void *object);

int sfscan(const void *_self, const char *str, ...);

/**
 * @brief Prints value of object into the stream or the buffer
 *
 * @param self 		Object
 * @param stream 	If not NULL, will ignore the buffer and work like fprintf
 * @param bin 		If 1, will print in binary mode into the stream
 * @param buffer 	If not NULL, will work like snprintf
 * @param maxn      Maximum number of bytes to be used in the buffer
 * @param flag 		Format flag. -1 for not given
 * @param width 	Format minimal field width. -1 for not given
 * @param precision Format precision. -1 for not given
 *
 * @return If binary mode enabled, will return size of object value
 * @return If the buffer is NULL, will return size of value in C string representation
 * @return Otherwise return value of fprintf or snprintf
 */
int sfprint(const void *self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision);


/**
 * @brief Helper for sfprint for basic types (e.g. Int, Float)
 *
 * @param flag 		Format flag. -1 for not given
 * @param width 	Format minimal field width. -1 for not given
 * @param precision Format precision. -1 for not given
 * @param spec 		Format specifier
 *
 * @return Format string.
 */
char* __getFmt(int flag, int width, int precision, char *spec);


/**
 * @brief Set object value/parameters
 *
 * @param self Object
 * @param ...  List of arguments
 */
void set(void *self, ...);


/**
 * @brief Get object value/parameters
 *
 * @param self Object
 * @param ...  List of arguments
 */
void get(const void *self, ...);


/**
 * @brief Same as set
 */
void vset(void *self, va_list *ap);


/**
 * @brief Same as get
 */
void vget(const void *self, va_list *ap);


/**
 * @brief Create new object
 *
 * @param class Class
 * @param ...   List of arguments
 *
 * @return Object
 */
#define new(class, ...) _new(class, __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__)


/**
 * @brief Same as new
 */
#define vnew(class, ap) _vnew(class, __FILE__, __LINE__, __FUNCTION__, ap)


/**
 * @brief Create copy of object
 *
 * @param self Object
 *
 * @return Object
 */
#define copy(self) _copy(self, __FILE__, __LINE__, __FUNCTION__)


/**
 * @brief Destruct object
 *
 * @param self Object
 */
#define delete(self) _delete(self, __FILE__, __LINE__, __FUNCTION__)

#endif /* end of include guard: NEW_H_0FEJN6R1 */
