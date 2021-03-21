#include <stdarg.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>

#include "Object.h"
#include "Selectors.h"

/*
 * Logging
 */

static FILE *sel_log = NULL;

void sellog(char *fmt, char *file, int line, const char* func, ...)
{
	if (sel_log == NULL)
		sel_log = fopen(__LOGS__"/sel_log.txt", "a+");

	if (sel_log != NULL)
	{
		size_t dt_size = snprintf(NULL, 0, "%s %s: %s:%d: %s: %s", __DATE__, __TIME__, file, line, func, fmt);
		char *dt = (char*)calloc(sizeof(char), dt_size + 1);
		snprintf(dt, dt_size + 1, "%s %s: %s:%d: %s: %s", __DATE__, __TIME__, file, line, func, fmt);

		va_list ap;
		va_start(ap, func);

		vfprintf(sel_log, dt, ap);
		fputc('\n', sel_log);

		va_end(ap);
		free(dt);
	}
}

void selerror(char *fmt, char *file, int line, const char *func, ...)
{
	if (sel_log == NULL)
		sel_log = fopen(__LOGS__"/sel_log.txt", "a+");

	if (sel_log != NULL)
	{
		size_t dt_size = snprintf(NULL, 0, "%s %s: %s:%d: %s: %s", __DATE__, __TIME__, file, line, func, fmt);
		char *dt = (char*)calloc(sizeof(char), dt_size + 1);
		snprintf(dt, dt_size + 1, "%s %s: %s:%d: %s: %s", __DATE__, __TIME__, file, line, func, fmt);

		va_list ap;
		va_start(ap, func);

		vfprintf(sel_log, dt, ap);
		fputc('\n', sel_log);
		
		fprintf(stderr, dt, ap);
		fputc('\n', stderr);

		va_end(ap);
		free(dt);
	}
}

/*
 * Create and delete selectors
 */

void* _new(const void *_class, char *file, int line, ...)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, "new", class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(1, class->size);

	if (object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, "new", class->name);
		exit(EXIT_FAILURE);
	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	va_list ap;

	va_start(ap, line);
	object = ctor(object, &ap);
	va_end(ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, "new", class->name, class->size);

	return object;
}

void* _vnew(const void *_class, char *file, int line, va_list *ap)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, "vnew", class->name);
		exit(EXIT_FAILURE);
	}	

	object = (struct Object*)calloc(1, class->size);

	if (object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, "vnew", class->name);
		exit(EXIT_FAILURE);

	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	object = ctor(object, ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, "vnew", class->name, class->size);	

	return object;
}

void _delete(void *_self, char *file, int line)
{
	if (_self) {
		const struct Class *class = classOf(_self);

		const char *name = class->name;
		size_t size = class->size;
		sellog("Trying to delete object of class '%s' with the size of '%lu' bytes.", 
				file, line, "delete", name, size);

		free(dtor(_self));

		sellog("Deleted object of class '%s' with the size of '%lu' bytes.", 
				file, line, "delete", name, size);
	}
}


void* _copy(const void *_self, char *file, int line)
{
	const struct Object *self = cast(Object(), _self);
	const struct Class *class = self->class;
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, __FUNCTION__, class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(1, class->size);

	if (object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, "copy", class->name);
		exit(EXIT_FAILURE);
	}
	
	object->magic = MAGIC_NUM;
	object->class = class;
	unsigned int ref_count = atomic_load((atomic_uint *) &self->ref_count);
	atomic_init((atomic_uint*) &object->ref_count, ref_count);

	sellog("Trying to copy object of class '%s' with the size '%lu' bytes.", 
			file, line, "copy", class->name, class->size);
	
	void *res = cpy(_self, object);
	
	sellog("Copied object of class '%s' with the size '%lu' bytes.", 
			file, line, "copy", class->name, class->size);

	return res;
}

/*
 * Base selectors
 */

void* dtor(void *_self)
{
	const struct Class *class = classOf(_self);

	if (class->dtor == NULL)
	{
		fprintf(stderr, "dtor: Error: Class '%s' doesn't have 'dtor' method!\n",
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
		fprintf(stderr, "ctor: Error: Class '%s' doesn't have 'ctor' method!\n",
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
		fprintf(stderr, "set: Error: Class '%s' doesn't have 'set' method!\n",
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
		fprintf(stderr, "vset: Error: Class '%s' doesn't have 'set' method!\n",
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
		fprintf(stderr, "get: Error: Class '%s' doesn't have 'get' method!\n",
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
		fprintf(stderr, "vget: Error: Class '%s' doesn't have 'get' method!\n",
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
		fprintf(stderr, "cpy: Error: Class '%s' doesn't have 'cpy' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->cpy(_self, object);
}

// Get string representation of variable of some type
char* stringer(const void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->stringer == NULL) 
	{
		fprintf(stderr, "stringer: Error: Class '%s' doesn't have 'stringer' method!\n",
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
		fprintf(stderr, "vstringer: Error: Class '%s' doesn't have 'stringer' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->stringer(_self, ap);
}

// Read one variable of some type from string
// or from stream, if str == NULL (stream should be given after str)
int reader(const void *_self, const char *str, ...)
{
	const struct Class *class = classOf(_self);

	if (class->reader == NULL) 
	{
		fprintf(stderr, "reader: Error: Class '%s' doesn't have 'reader' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	va_list ap;

	va_start(ap, str);
	int result = class->reader(str, &ap);
	va_end(ap);

	return result;
}

int vreader(const void *_self, const char *str, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->reader == NULL) 
	{
		fprintf(stderr, "vreader: Error: Class '%s' doesn't have 'reader' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->reader(str, ap);
}

/*
 * Super's base selectors
 */

const void* super(const void *_self)
{
	const struct Class *self = cast(Class(), _self);

	if (self->super == NULL) 
	{
		fprintf(stderr, "super: Error: Class '%s' doesn't have superclass!\n",
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
		fprintf(stderr, "super_ctor: Error: Superclass '%s' doesn't have 'ctor' method!\n",
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
		fprintf(stderr, "super_dtor: Error: Superclass '%s' doesn't have 'dtor' method!\n",
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
		fprintf(stderr, "super_cpy: Error: Superclass '%s' doesn't have 'cpy' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}		

	return superclass->cpy(_self, object);
}

