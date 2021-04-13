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


/**
 * @brief Syntax sugar
 */
typedef void* var;

/**
 * @brief new wraps this function to provide better logging and debuging
 */
void* _new(char *classname, char *file, int line, const char *func, 
		const void *class, ...);


/**
 * @brief new_stack wraps this function to provide better logging and debuging
 */
void* _new_stack(char *classname, char *file, int line, const char *func,
		const void *_class, void *_object, ...);


/**
 * @brief Same as _new_stack
 */
void* _vnew_stack(char *classname, char *file, int line, const char *func,
		const void *_class, void *_object, va_list *ap);


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

/**
 * @brief implement wraps this function to provide better logging and debuging
 */
void* _implement(char *selfname, char *file, int line, const char *func,
		void *_self, unsigned int impl_number, ...);


/**
 * @brief inew wraps this function to provide better logging and debuging
 */
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
 * @param ...  Values to set
 */
void set(void *self, ...);


/**
 * @brief Get object value/parameters
 *
 * @param self Object
 * @param ...  Pointers to the variables, where will be values
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
 * @brief Create new object in stack
 *
 * @param class  Class
 * @param object Allocated memory in stack
 * @param ...    List of arguments
 *
 * @return Object
 */
#define new_stack(class, object...) _new_stack(#class, __FILE__, __LINE__, __FUNCTION__, class, object)


/**
 * @brief Create new object in stack
 * This is the helper function for new_stack
 * So you don't need to allocate memory in stack by hand
 *
 * @param class Class
 * @param ...   List of arguments
 *
 * @return Object
 */
#define $(class, ...) new_stack(class(), (char[sizeof(struct class)]){0}, ## __VA_ARGS__)


/**
 * @brief Same as new_stack
 */
#define vnew_stack(class, object, ap) _vnew_stack(#class, __FILE__, __LINE__, __FUNCTION__, class, object, ap)


/**
 * @brief Same as $(class, ...)
 */
#define $v(class, ap) new_stack(class(), (char[sizeof(struct class)]){0}, ap)


/**
 * @brief Create a simple object from any variable
 *
 * @param var Any variable (can't be rvalue)
 * 
 * @return Object
 */
#define any(var) new(Any(), sizeof(typeof(var)), var)


/**
 * @brief Create a simple object from any variable in stack
 *
 * @param var Any variable (can't be rvalue)
 * 
 * @return Object
 */
#define $A(var) $(Any, sizeof(typeof(var)), var)


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


/**
 * @brief Set class' interfaces
 *
 * @param self 		  Class
 * @param impl_number Number of interfaces
 * @param ... 		  Pairs of interfaces and their offsets in the class structure
 *
 * @return Class
 */
#define implement(self, impl_number...) _implement(#self, __FILE__, __LINE__, __FUNCTION__, self, impl_number)


/**
 * @brief Set class' interfaces
 *
 * @param self 		  Class
 * @param impl_number Number of interfaces
 * @param ... 		  Pairs of interfaces and their sizes
 *
 * @return Class
 */
#define inew(name, ext_number...) _inew(__FILE__, __LINE__, __FUNCTION__, name, ext_number)

#endif /* end of include guard: NEW_H_0FEJN6R1 */
