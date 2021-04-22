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

void* _new(char *classname, char *file, int line, const char *func, 
		const void *_class, ...)
{
	const struct Class *class = _cast("Class()", classname, file, line, func, 
			Class(), _class);
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

	va_start(ap, _class);
	object = ctor(object, &ap);
	va_end(ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, func, "new", class->name, class->size);

	return object;
}

void* _new_stack(char *classname, char *file, int line, const char *func,
		const void *_class, void *_object, ...)
{
	const struct Class *class = _cast("Class()", classname, file, line, func, 
			Class(), _class);
	struct Object *object;

	if (_object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, func, "stack", class->name);
		exit(EXIT_FAILURE);
	}

	object = _object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "stack", class->name);
		exit(EXIT_FAILURE);
	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	va_list ap;

	va_start(ap, _object);
	object = ctor(object, &ap);
	va_end(ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, func, "stack", class->name, class->size);

	return object;
}

void* _vnew(char *classname, char *file, int line, const char* func, 
		const void *_class, va_list *ap)
{
	const struct Class *class = _cast("Class()", classname, file, line, func, 
			Class(), _class);
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

void* _vnew_stack(char *classname, char *file, int line, const char *func,
		const void *_class, void *_object, va_list *ap)
{
	const struct Class *class = _cast("Class()", classname, file, line, func,
			Class(), _class);
	struct Object *object;

	if (_object == NULL)
	{
		selerror("Fatal Error: Object of class '%s' allocation error!", 
				file, line, func, "stack", class->name);
		exit(EXIT_FAILURE);
	}

	object = _object;

	if (class->size == 0)
	{
		selerror("Error: Object of class '%s' can't have zero size!", 
				file, line, func, "stack", class->name);
		exit(EXIT_FAILURE);
	}

	object->magic = MAGIC_NUM;
	object->class = class;
	atomic_init((atomic_uint*) &object->ref_count, 1);

	object = ctor(object, ap);

	sellog("Created object of class '%s' with the size of '%lu' bytes.", 
			file, line, func, "stack", class->name, class->size);

	return object;
}


void _delete(char *selfname, char *file, int line, const char *func,
		void *_self)
{
	if (_self) {
		const struct Class *class = _classOf(selfname, file, line, func,
				_self);

		const char *name = class->name;
		size_t size = class->size;
		sellog("Trying to delete object of class '%s' with the size of '%lu' bytes.", 
				file, line, func, "delete", name, size);

		free(dtor(_self));

		sellog("Deleted object of class '%s' with the size of '%lu' bytes.", 
				file, line, func, "delete", name, size);
	}
}


void* _copy(char *selfname, char *file, int line, const char *func,
		const void *_self)
{
	const struct Object *self = _cast("Object()", selfname, file, line, func,
			Object(), _self);
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

/*
 * Interfaces
 */

// --

const void* _inew(char *file, int line, const char *func,
		const char *name, unsigned int extended_nb, unsigned int methods_nb, ...)
{
	if (methods_nb == 0 && extended_nb == 0)
	{
		selerror("Fatal Error: Interface '%s' can't have zero methods!", 
				file, line, func, "inew", name);
		exit(EXIT_FAILURE);
	}

	struct Interface *interface = (struct Interface*)calloc(sizeof(struct Interface), 1);

	if (interface == NULL)
	{
		selerror("Fatal Error: Interface '%s' allocation error!", 
				file, line, func, "inew", name);
		exit(EXIT_FAILURE);
	}

	interface->name = name;
	interface->magic = INTERFACE_MAGIC;
	interface->method_sels_nb = methods_nb;

	va_list ap;
	va_start(ap, methods_nb);

	if (extended_nb != 0)
	{
		va_list ap_copy;
		va_copy(ap_copy, ap);

		const void *_inter;

		for (int i = 0; i < extended_nb && (_inter = va_arg(ap_copy, const void*)); ++i) 
		{
			const struct Interface *inter = isInterface(_inter);
			interface->method_sels_nb += inter->method_sels_nb;
		}

		va_end(ap_copy);
	}

	interface->method_sels = (voidf*)calloc(sizeof(voidf), interface->method_sels_nb);

	if (interface->method_sels == NULL)
	{
		selerror("Fatal Error: Interface's methods '%s' allocation error!", 
				file, line, func, "inew", name);
		exit(EXIT_FAILURE);
	}

	unsigned int mnb = 0;
	const void *_inter;

	for (int i = 0; i < extended_nb && (_inter = va_arg(ap, const void*)); ++i) 
	{
		const struct Interface *inter = isInterface(_inter);

		for (int j = 0; j < inter->method_sels_nb; ++j, ++mnb) 
		{
			interface->method_sels[mnb] = inter->method_sels[j];
		}
	}

	if (methods_nb != 0)
	{
		voidf selector;

		for (; mnb < methods_nb && (selector = va_arg(ap, voidf)); ++mnb) 
		{
			interface->method_sels[mnb] = selector;
		}
	}

	va_end(ap);

	return interface;
}

/*
 * Base selectors
 */

// ---

void* dtor(void *_self)
{
	const struct Class *class = classOf(_self);

	if (class->dtor.method == NULL)
	{
		fprintf(stderr, "dtor: Error: Class '%s' doesn't have 'dtor' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	typedef void *(*dtor_f)(void *self);

	return ((dtor_f) class->dtor.method)(_self);
}

void* ctor(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->ctor.method == NULL)
	{
		fprintf(stderr, "ctor: Error: Class '%s' doesn't have 'ctor' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	typedef void *(*ctor_f)(void *self, va_list *ap);

	return ((ctor_f) class->ctor.method)(_self, ap);
}

void set(void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->set.method == NULL) 
	{
		fprintf(stderr, "set: Error: Class '%s' doesn't have 'set' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	typedef void (*set_f)(void *self, va_list *ap);

	va_list ap;

	va_start(ap, _self);
	((set_f) class->set.method)(_self, &ap);
	va_end(ap);
}

void vset(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->set.method == NULL) 
	{
		fprintf(stderr, "vset: Error: Class '%s' doesn't have 'set' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	typedef void (*set_f)(void *self, va_list *ap);

	((set_f) class->set.method)(_self, ap);
}

void get(const void *_self, ...)
{
	const struct Class *class = classOf(_self);

	if (class->get.method == NULL) 
	{
		fprintf(stderr, "get: Error: Class '%s' doesn't have 'get' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}
	
	typedef void *(*get_f)(const void *self, va_list *ap);

	va_list ap;
	
	va_start(ap, _self);
	((get_f) class->get.method)(_self, &ap);
	va_end(ap);
}

void vget(const void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);

	if (class->get.method == NULL) 
	{
		fprintf(stderr, "vget: Error: Class '%s' doesn't have 'get' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	typedef void *(*get_f)(const void *self, va_list *ap);

	((get_f) class->get.method)(_self, ap);
}

void* cpy(const void *_self, void *object)
{
	const struct Class *class = classOf(_self);

	if (class->cpy.method == NULL) 
	{
		fprintf(stderr, "cpy: Error: Class '%s' doesn't have 'cpy' method!\n",
				class->name);
		exit(EXIT_FAILURE);
	}

	typedef void *(*cpy_f)(const void *self, void *object);

	return ((cpy_f) class->cpy.method)(_self, object);
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

	if (superclass->ctor.method == NULL) 
	{
		fprintf(stderr, "super_ctor: Error: Superclass '%s' doesn't have 'ctor' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}

	typedef void *(*ctor_f)(void *self, va_list *ap);

	return ((ctor_f) superclass->ctor.method)(_self, ap);

}

void* super_dtor(const void *_class, void *_self)
{
	const struct Class *superclass = super(_class);

	if (superclass->dtor.method == NULL) 
	{
		fprintf(stderr, "super_dtor: Error: Superclass '%s' doesn't have 'dtor' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}	

	typedef void *(*dtor_f)(void *self);

	return ((dtor_f) superclass->dtor.method)(_self);
}


void* super_cpy(const void *_class, const void *_self, void *object)
{
	const struct Class *superclass = super(_class);

	if (superclass->cpy.method == NULL) 
	{
		fprintf(stderr, "super_cpy: Error: Superclass '%s' doesn't have 'cpy' method!\n",
				superclass->name);
		exit(EXIT_FAILURE);
	}		

	typedef void *(*cpy_f)(const void *self, void *object);

	return ((cpy_f) superclass->cpy.method)(_self, object);
}
