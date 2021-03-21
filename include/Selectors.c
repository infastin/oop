#include <stdarg.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>

#include "Object.h"
#include "Selectors.h"

/*
 * Create and delete selectors
 */

void* _new(const void *_class, char *file, int line, ...)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	if (class->size == 0)
	{
		fprintf(stderr, "%s:%d: new error: Object '%s' can't have zero size!\n", 
				file, line, class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(1, class->size);

	if (object == NULL)
	{
		fprintf(stderr, "%s:%d: new fatal error: Object '%s' allocation error!\n",
				file, line, class->name);
		exit(EXIT_FAILURE);
	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	va_list ap;

	va_start(ap, line);
	object = ctor(object, &ap);
	va_end(ap);
	
	return object;
}

void* _vnew(const void *_class, char *file, int line, va_list *ap)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	if (class->size == 0)
	{
		fprintf(stderr, "%s:%d: vnew error: Object '%s' can't have zero size!\n", 
				file, line, class->name);
		exit(EXIT_FAILURE);
	}	

	object = (struct Object*)calloc(1, class->size);

	if (object == NULL)
	{
		fprintf(stderr, "%s:%d: vnew fatal error: Object '%s' allocation error!\n",
				file, line, class->name);
		exit(EXIT_FAILURE);
	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	object = ctor(object, ap);

	return object;
}

void delete(void *_self)
{
	if (_self)
		free(dtor(_self));
}


void* _copy(const void *_self, char *file, int line)
{
	const struct Object *self = cast(Object(), _self);
	const struct Class *class = self->class;
	struct Object *object;

	if (class->size == 0)
	{
		fprintf(stderr, "%s:%d: copy error: Object '%s' can't have zero size!\n", 
				file, line, class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(1, class->size);

	if (object == NULL)
	{
		fprintf(stderr, "%s:%d: copy fatal error: Object '%s' allocation error!\n",
				file, line, class->name);
		exit(EXIT_FAILURE);
	}
	
	object->magic = MAGIC_NUM;
	object->class = class;
	unsigned int ref_count = atomic_load((atomic_uint *) &self->ref_count);
	atomic_init((atomic_uint*) &object->ref_count, ref_count);

	return cpy(_self, object);
}

/*
 * Base selectors
 */

void* dtor(void *_self)
{
	const struct Class *class = classOf(_self);

	if (class->dtor == NULL)
	{
		fprintf(stderr, "Destructor Error: Class '%s' doesn't have 'dtor' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->dtor(_self);
}

void* ctor(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->dtor == NULL)
	{
		fprintf(stderr, "Constructor Error: Class '%s' doesn't have 'ctor' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->ctor(_self, ap);
}

void set(void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->set == NULL) 
	{
		fprintf(stderr, "Set Error: Class '%s' doesn't have 'set' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	va_list ap;

	va_start(ap, _self);
	class->set(_self, &ap);
	va_end(ap);
}

void vset(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->set == NULL) 
	{
		fprintf(stderr, "VSet Error: Class '%s' doesn't have 'set' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	class->set(_self, ap);
}

// Get type's value pointer
void get(const void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->get == NULL) 
	{
		fprintf(stderr, "Get Error: Class '%s' doesn't have 'get' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}
	
	va_list ap;
	
	va_start(ap, _self);
	class->get(_self, &ap);
	va_end(ap);
}

void vget(const void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->get == NULL) 
	{
		fprintf(stderr, "VGet Error: Class '%s' doesn't have 'get' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	class->get(_self, ap);
}

void* cpy(const void *_self, void *object)
{
	const struct Class *class = classOf(_self);

	if (class->cpy == NULL) 
	{
		fprintf(stderr, "Copy Error: Class '%s' doesn't have 'cpy' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->cpy(_self, object);
}

char* stringer(const void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->stringer == NULL) 
	{
		fprintf(stderr, "Stringer Error: Class '%s' doesn't have 'stringer' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	va_list ap;

	va_start(ap, _self);
	char *result = class->stringer(_self, &ap);
	va_end(ap);

	return result;
}

char* vstringer(const void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->stringer == NULL) 
	{
		fprintf(stderr, "Stringer Error: Class '%s' doesn't have 'stringer' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	char *result = class->stringer(_self, ap);

	return result;
}

/*
 * Super's base selectors
 */

const void* super(const void *_self)
{
	const struct Class *self = cast(Class(), _self);

	if (self->super == NULL) 
	{
		fprintf(stderr, "Super Error: Class '%s' doesn't have superclass!\n",
				self->name);
		exit(EXIT_FAILURE);
	}

	return self->super;
}

void* super_ctor(const void *_class, void *_self, va_list *ap)
{
	const struct Class *superclass = super(_class);

	if (superclass->ctor == NULL) 
	{
		fprintf(stderr, "Super Constructor Error: Superclass '%s' doesn't have 'ctor' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}

	return superclass->ctor(_self, ap);
}

void* super_dtor(const void *_class, void *_self)
{
	const struct Class *superclass = super(_class);

	if (superclass->dtor == NULL) 
	{
		fprintf(stderr, "Super Destructor Error: Superclass '%s' doesn't have 'dtor' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}	

	return superclass->dtor(_self);
}


void* super_cpy(const void *_class, const void *_self, void *object)
{
	const struct Class *superclass = super(_class);

	if (superclass->cpy == NULL) 
	{
		fprintf(stderr, "Super Copy Error: Superclass '%s' doesn't have 'cpy' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}		

	return superclass->cpy(_self, object);
}

