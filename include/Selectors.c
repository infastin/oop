#include <stdarg.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Object.h"
#include "Selectors.h"

/*
 * Logging
 */

// ---
#if defined (LOGGING)
static FILE *sel_log = NULL;

void sellog(char *fmt, char *file, int line, const char *func, const char *func_name, ...)
{
	if (sel_log == NULL)
		sel_log = fopen(__LOGS__"/sel_log.txt", "a+");

	if (sel_log != NULL)
	{
		size_t dt_size = snprintf(NULL, 0, "%s %s: %s:%d: %s: %s: %s", 
				__DATE__, __TIME__, file, line, func, func_name, fmt);
		
		char *dt = (char*)calloc(sizeof(char), dt_size + 1);
		snprintf(dt, dt_size + 1, "%s %s: %s:%d: %s: %s: %s", 
				__DATE__, __TIME__, file, line, func, func_name, fmt);

		va_list ap;
		va_start(ap, func_name);

		vfprintf(sel_log, dt, ap);
		fputc('\n', sel_log);

		va_end(ap);
		free(dt);
	}
}

void selerror(char *fmt, char *file, int line, const char *func, const char *func_name, ...)
{
	if (sel_log == NULL)
		sel_log = fopen(__LOGS__"/sel_log.txt", "a+");

	if (sel_log != NULL)
	{
		size_t dt_size = snprintf(NULL, 0, "%s %s: %s:%d: %s: %s: %s", 
				__DATE__, __TIME__, file, line, func, func_name, fmt);
		
		char *dt = (char*)calloc(sizeof(char), dt_size + 1);
		snprintf(dt, dt_size + 1, "%s %s: %s:%d: %s: %s: %s", 
				__DATE__, __TIME__, file, line, func, func_name, fmt);

		va_list ap;
		va_start(ap, func_name);

		vfprintf(sel_log, dt, ap);
		fputc('\n', sel_log);
		
		fprintf(stderr, dt, ap);
		fputc('\n', stderr);

		va_end(ap);
		free(dt);
	}
}
#else
void sellog(char *fmt, char *file, int line, const char *func, const char *func_name, ...) {};

void selerror(char *fmt, char *file, int line, const char *func, const char *func_name, ...)
{
	size_t dt_size = snprintf(NULL, 0, "%s %s: %s:%d: %s: %s: %s", 
			__DATE__, __TIME__, file, line, func, func_name, fmt);

	char *dt = (char*)calloc(sizeof(char), dt_size + 1);
	snprintf(dt, dt_size + 1, "%s %s: %s:%d: %s: %s: %s", 
			__DATE__, __TIME__, file, line, func, func_name, fmt);

	va_list ap;
	va_start(ap, func_name);

	fprintf(stderr, dt, ap);
	fputc('\n', stderr);

	va_end(ap);
	free(dt);
}
#endif

/*
// Create, copy and delete selectors
 */

// ---

void* _new(const void *_class, 
		char *classname, char *file, int line, const char *func, 
		...)
{
	const struct Class *class = _cast(Class(), _class, "Class()", classname, file, line, func);
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "new", class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(class->size, 1);

	if (object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, func, "new", class->name);
		exit(EXIT_FAILURE);
	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	va_list ap;

	va_start(ap, func);
	object = ctor(object, &ap);
	va_end(ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, func, "new", class->name, class->size);

	return object;
}

void* _vnew(const void *_class, 
		char *classname, char *file, int line, const char* func, 
		va_list *ap)
{
	const struct Class *class = _cast(Class(), _class, "Class()", classname, file, line, func);
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "vnew", class->name);
		exit(EXIT_FAILURE);
	}	

	object = (struct Object*)calloc(class->size, 1);

	if (object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, func, "vnew", class->name);
		exit(EXIT_FAILURE);

	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	object = ctor(object, ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, func, "vnew", class->name, class->size);	

	return object;
}

void _delete(void *_self, 
		char *selfname, char *file, int line, const char *func)
{
	if (_self) {
		const struct Class *class = _classOf(_self, selfname, file, line, func);

		const char *name = class->name;
		size_t size = class->size;
		sellog("Trying to delete object of class '%s' with the size of '%lu' bytes.", 
				file, line, func, "delete", name, size);

		free(dtor(_self));

		sellog("Deleted object of class '%s' with the size of '%lu' bytes.", 
				file, line, func, "delete", name, size);
	}
}


void* _copy(const void *_self, 
		char *selfname, char *file, int line, const char *func)
{
	const struct Object *self = _cast(Object(), _self, "Object()", selfname, file, line, func);
	const struct Class *class = self->class;
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "copy", class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(class->size, 1);

	if (object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, func, "copy", class->name);
		exit(EXIT_FAILURE);
	}
	
	object->magic = MAGIC_NUM;
	object->class = class;
	unsigned int ref_count = atomic_load((atomic_uint *) &self->ref_count);
	atomic_init((atomic_uint*) &object->ref_count, ref_count);

	sellog("Trying to copy object of class '%s' with the size '%lu' bytes.", 
			file, line, func, "copy", class->name, class->size);
	
	void *res = cpy(_self, object);
	
	sellog("Copied object of class '%s' with the size '%lu' bytes.", 
			file, line, func, "copy", class->name, class->size);

	return res;
}

void* _implement(void *_self, unsigned int impl_number,
		char *selfname, char *file, int line, const char *func,
		...)
{
	struct Class *self = _cast(Class(), _self, "Class()", selfname, file, line, func);

	if (impl_number != 0)
	{
		self->impl_number += impl_number;	
		self->implements = (struct InterfaceElem*)realloc(self->implements, sizeof(struct InterfaceElem) * impl_number);

		if (self->implements == NULL)
		{
			selerror("Fatal Error: List of implemented interfaces of class '%s' allocation error!", 
					file, line, func, "implement", self->name);
			exit(EXIT_FAILURE);
		}

		va_list ap;
		va_start(ap, func);

		for (unsigned int i = 0; i < impl_number; ++i)
		{
			const struct Interface *inter = va_arg(ap, const struct Interface*);
			size_t offset = va_arg(ap, size_t);

			self->implements[i].interface = inter;
			self->implements[i].offset = offset;
		}

		va_end(ap);
	}

	return self;
}

/*
 * Create interface
 */
const void *_inew(char *name, unsigned int ext_number, 
		char *file, int line, const char *func, 
		...)
{
	struct Interface *interface = (struct Interface*)calloc(1, sizeof(struct Interface));

	if (interface == NULL)
	{
		selerror("Fatal Error: Interface '%s' allocation error!", 
				file, line, func, "inew", name);
		exit(EXIT_FAILURE);
	}

	interface->name = name;
	interface->magic = MAGIC_INUM;
	interface->ext_number = ext_number;
	interface->extends = NULL;

	if (ext_number != 0)
	{ 	
		interface->extends = (struct InterfaceElem*)calloc(sizeof(struct InterfaceElem), ext_number);

		if (interface->extends == NULL)
		{
			selerror("Fatal Error: List of extended interfaces of interface '%s' allocation error!", 
					file, line, func, "inew", name);
			exit(EXIT_FAILURE);
		}

		va_list ap;
		va_start(ap, func);

		for (unsigned int i = 0; i < ext_number; ++i)
		{
			const struct Interface *inter = va_arg(ap, const struct Interface*);
			size_t offset = va_arg(ap, size_t);

			interface->extends[i].interface = inter;
			interface->extends[i].offset = offset;
		}

		va_end(ap);
	}

	return (const void*) interface;
}

/*
 * Base selectors
 */

// ---

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

/*
 * Super selectors
 */

// ---

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
