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

/*
 * Create and delete selectors
 */

void* _new(const void *_class, char *file, int line, const char *func, ...)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "new", class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(1, class->size);

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

void* _vnew(const void *_class, char *file, int line, const char* func, va_list *ap)
{
	const struct Class *class = cast(Class(), _class);
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "vnew", class->name);
		exit(EXIT_FAILURE);
	}	

	object = (struct Object*)calloc(1, class->size);

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

void _delete(void *_self, char *file, int line, const char *func)
{
	if (_self) {
		const struct Class *class = classOf(_self);

		const char *name = class->name;
		size_t size = class->size;
		sellog("Trying to delete object of class '%s' with the size of '%lu' bytes.", 
				file, line, func, "delete", name, size);

		free(dtor(_self));

		sellog("Deleted object of class '%s' with the size of '%lu' bytes.", 
				file, line, func, "delete", name, size);
	}
}


void* _copy(const void *_self, char *file, int line, const char *func)
{
	const struct Object *self = cast(Object(), _self);
	const struct Class *class = self->class;
	struct Object *object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "copy", class->name);
		exit(EXIT_FAILURE);
	}

	object = (struct Object*)calloc(1, class->size);

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

int sfprint(const void *_self, FILE *stream, int bin, char *buffer, size_t maxn, 
		int flag, int width, int precision)
{
	const struct Class *class = classOf(_self);

	if (class->sfprint == NULL)
	{
		fprintf(stderr, "sfprint: Error: Class '%s' doesn't have 'sfprint' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	return class->sfprint(_self, stream, bin, buffer, maxn, flag, width, precision);
}

char* __getFmt(int flag, int width, int precision, char *spec)
{
	int fmt_size = 1;

	// Getting format size
	for (char *sp = spec; *sp != 0; fmt_size++, sp++);

	if (flag != -1)
		fmt_size++;

	if (width != -1)
		fmt_size += snprintf(NULL, 0, "%d", width);

	if (precision != -1)
		fmt_size += snprintf(NULL, 0, "%d", precision) + 1;

	// Getting format
	char *fmt = (char*)calloc(sizeof(char), fmt_size + 1);

	if (fmt)
	{
		*fmt = '%';

		char *p = fmt + 1;
		int psize = fmt_size - 1;

		if (flag != -1)
		{
			*p++ = flag;
			psize--;
		}

		if (width != -1)
		{
			int widthN = snprintf(p, psize + 1, "%d", width);
			p += widthN;
			psize -= widthN;
		}

		if (precision != -1)
		{
			int precisionN = snprintf(p, psize + 1, ".%d", precision);
			p += precisionN;
			psize -= precisionN;
		}

		for (char *sp = spec; *sp != 0 && psize > 0; sp++)
		{
			*p++ = *sp;
			psize--;
		}

		*p = 0;
	}

	return fmt;
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
