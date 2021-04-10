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
void* _new(char *classname, char *file, int line, const char *func, 
		const void *class, ...);


/**
 * @brief Same as _new
 */
void* _vnew(char *classname, char *file, int line, const char *func, 
		const void *class, va_list *ap);


/**
 * @brief delete wraps this function to provide better logging and debuging
 */
void _delete(char *selfname, char *file, int line, const char *func,
		void *self);


/**
 * @brief copy wraps this function to provide better logging and debuging
 */
void* _copy(char *selfname, char *file, int line, const char *func,
		const void *self);

void* _implement(char *selfname, char *file, int line, const char *func,
		void *_self, unsigned int impl_number, ...);

const void *_inew(char *file, int line, const char *func,
		char *name, unsigned int ext_number, ...);

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
#define new(class...) _new(#class, __FILE__, __LINE__, __FUNCTION__, class)

/**
 * @brief Same as new
 */
#define vnew(class, ap) _vnew(#class, __FILE__, __LINE__, __FUNCTION__, class, ap)


/**
 * @brief Create copy of object
 *
 * @param self Object
 *
 * @return Object
 */
#define copy(self) _copy(#self, __FILE__, __LINE__, __FUNCTION__, self)


/**
 * @brief Destruct object
 *
 * @param self Object
 */
#define delete(self) _delete(#self, __FILE__, __LINE__, __FUNCTION__, self)

#define implement(self, impl_number...) _implement(#self, __FILE__, __LINE__, __FUNCTION__, self, impl_number)
#define inew(name, ext_number...) _inew(__FILE__, __LINE__, __FUNCTION__, name, ext_number)

#endif /* end of include guard: NEW_H_0FEJN6R1 */
