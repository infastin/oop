#include <stdatomic.h>
#include <assert.h>
#include <stdio.h>

#include "ReferenceCounter.h"
#include "Object.h"
#include "Selectors.h"

__attribute__ ((always_inline))
inline void release(void *_self_ptr) {
	void *_self = *(void**) _self_ptr;
	struct Object *self = _self;

	assert(self->ref_count);

	if (atomic_fetch_sub((atomic_size_t*) &self->ref_count, 1) == 1) {
		/* const struct Class *class = classOf(_self);
		printf("Object %s was freed %s.\n", class->name, __FUNCTION__); */

		delete(_self);
	}
}

void* retain(void *_self)
{
	struct Object *self = _self;

	assert(self->ref_count);

	atomic_fetch_add((atomic_uint*) &self->ref_count, 1);

	/* const struct Class *class = classOf(_self);
	printf("Object %s was retained.\n", class->name); */

	return _self;
}
