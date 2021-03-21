#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include "ReferenceCounter.h"
#include "Object.h"
#include "Selectors.h"

/*
 * Logging
 */

static FILE *ref_log = NULL;

void reflog(char *fmt, ...)
{
	if (ref_log == NULL)
		ref_log = fopen(__LOGS__"/ref_log.txt", "a+");

	if (ref_log != NULL)
	{
		size_t dt_size = snprintf(NULL, 0, "%s %s: %s", __DATE__, __TIME__, fmt);
		char *dt = (char*)calloc(sizeof(char), dt_size + 1);
		snprintf(dt, dt_size + 1, "%s %s: %s", __DATE__, __TIME__, fmt);

		va_list ap;
		va_start(ap, fmt);

		vfprintf(ref_log, dt, ap);
		fputc('\n', ref_log);

		va_end(ap);
		free(dt);
	}
}

/*
 * ReferenceCounter
 */

__attribute__ ((always_inline))
inline void release(void *_self_ptr) {
	union {
		void **real_ptr;
		void *ptr;
	} conv;

	conv.ptr = _self_ptr;
	struct Object *self = *conv.real_ptr;

	if (self != NULL && self->magic == MAGIC_NUM) 
	{
		const struct Class *class = self->class;

		if (self->ref_count <= 0)
		{
			fprintf(stderr, "Reference Counter: Error: Can't destroy object of class '%s' with zero reference count!\n", 
					class->name);

			reflog("Error: Can't destroy object of class '%s' with zero reference count!", 
					class->name);
			return;
		}

		if (atomic_fetch_sub((atomic_uint*) &self->ref_count, 1) == 1) 
		{
			reflog("Released object of class '%s' with the size of '%d' bytes.", 
					class->name, class->size);
			delete(*conv.real_ptr);
			*conv.real_ptr = NULL;
		}
	}
}

void* retain(void *_self)
{
	struct Object *self = cast(Object(), _self);

	const struct Class *class = self->class;

	if (self->ref_count <= 0)
	{
		fprintf(stderr, "Reference Counter: Error: Can't retain object of class '%s' with zero reference count!\n", 
				class->name);

		reflog("Error: Can't retain object of class '%s' with zero reference count!\n", 
				class->name);
		return NULL;
	}

	reflog("Retained object of class '%s' with the size of '%d' bytes.", 
			class->name, class->size);

	atomic_fetch_add((atomic_uint*) &self->ref_count, 1);

	return _self;
}
