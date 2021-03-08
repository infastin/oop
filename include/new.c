#include <stdarg.h>
#include <stdlib.h>

#include "new.h"

void* new(const void *_object, ...)
{
	const struct Object *object = _object;
	void *p = calloc(1, object->size);

	*(const struct Object **)p = object;

	if (object->ctor)
	{
		va_list props;

		va_start(props, _object);
		p = object->ctor(p, &props);
		va_end(props);
	}
	
	return p;
}

void delete(void *self)
{
	const struct Object **obj = self;

	if (self && *obj && (*obj)->dtor)
		self = (*obj)->dtor(self);

	free(self);
}
