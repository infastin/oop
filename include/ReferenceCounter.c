#include <stdatomic.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ReferenceCounter.h"
#include "Object.h"
#include "Selectors.h"

/*
 * Stack of freed objects
 */

#define STACK_SIZE 32

static void* last_freed[STACK_SIZE];
static unsigned int lf_count = 0;
static unsigned int lf_index = 0;

static inline void lf_push(void *ptr)
{
	if (lf_count < STACK_SIZE)
		lf_count++;
	
	if (lf_index == STACK_SIZE)
		lf_index = 0;

	last_freed[lf_index++] = ptr;
}

static inline int lf_exists(void *ptr)
{
	for (int i = 0; i < lf_count; i++)
		if (last_freed[i] == ptr) return 1;

	return 0;
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

	if (*conv.real_ptr && !lf_exists(*conv.real_ptr)) {
		struct Object *self = cast(Object(), *conv.real_ptr);

		assert(self->ref_count);

		if (atomic_fetch_sub((atomic_uint*) &self->ref_count, 1) == 1) 
		{
			lf_push(*conv.real_ptr);
			delete(*conv.real_ptr);
			*conv.real_ptr = NULL;
		}
	}
}

void* retain(void *_self)
{
	struct Object *self = cast(Object(), _self);

	assert(self->ref_count);

	atomic_fetch_add((atomic_uint*) &self->ref_count, 1);

	return _self;
}
