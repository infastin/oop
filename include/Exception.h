#ifndef EXCEPTION_H_6E2ZCQYD
#define EXCEPTION_H_6E2ZCQYD

#include <stddef.h>
#include <setjmp.h>

#include "Object.h"
#include "Selectors.h"
#include "ExceptionObject.h"

/*
 * Exception
 */

#define EXCEPTION_MAX_DEPTH 128
#define EXCEPTION_MAX_TRACE 25

ClassHeader(Exception);
ObjectHeader(GlobalException);
ObjectHeader(SomeError);

typedef void (*throw_f)(const void *self, char *fmt, ...);

struct Exception
{
	const struct Object _;
	const struct ExceptionObject *obj;
	size_t   depth;
	int 	 active;
	jmp_buf *buffers[EXCEPTION_MAX_DEPTH];
};

void exception_try(void *self, jmp_buf *env);
void exception_try_end(void *self);
void exception_try_fail(void *self);
void exception_throw(void *self, const void *obj, char *fmt, ...);
const void *exception_catch(void *self, ...);

/*
 * Try-catch macros
 */

#define try { jmp_buf __env; exception_try(GlobalException(), &__env); if (!setjmp(__env))

#define catch(e, ...) else { exception_try_fail(GlobalException()); } exception_try_end(GlobalException()); } \
	for (const void* e = exception_catch(GlobalException(), __VA_ARGS__); e != NULL; e = NULL)

#define throw(object, fmt, ...) exception_throw(GlobalException(), object, fmt, ## __VA_ARGS__)

#endif /* end of include guard: EXCEPTION_H_6E2ZCQYD */
