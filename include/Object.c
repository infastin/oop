#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "Object.h"
#include "Selectors.h"
#include "ExceptionObject.h"

/*
 * Object
 */

static void* Object_ctor(void *_self, va_list *ap)
{
	return _self;
}

static void* Object_dtor(void *_self)
{
	return _self;
}

static void* Object_cpy(const void *_self, void *_object)
{
	return _object;
}

/*
 * Object selectors
 */

const void* _isObject(const void *_self, char *selfname, char *file, int line, const char *func)
{
	if (_self == NULL)
	{
		fprintf(stderr, "%s:%d: %s: Error: '%s' is NULL!", 
				file, line, func, selfname);
		exit(EXIT_FAILURE);
	}

	const struct Object *self = _self;

	if (self->magic != MAGIC_NUM)
	{
		fprintf(stderr, "%s:%d: %s: Error: '%s' isn't object!",
				file, line, func, selfname);
		exit(EXIT_FAILURE);
	}

	return _self;
}

static void sigcatch(int signal)
{
	if (signal == SIGBUS)
	{
		fprintf(stderr, "cast: Error: Caught Bus Error, exiting!");
		exit(EXIT_FAILURE);
	}
	
	if (signal == SIGSEGV)
	{
		fprintf(stderr, "cast: Error: Caught Segmentation Fault, exiting!");
		exit(EXIT_FAILURE);
	}
}

void* _cast(const void *_class, const void *_self, char *selfname, char *file, int line, const char *func)
{
	void (*sigsegv)(int) = signal(SIGSEGV, sigcatch);
#ifdef SIGBUS
	void (*sigbus)(int) = signal(SIGBUS, sigcatch);
#endif

	const struct Object *self = _isObject(_self, selfname, file, line, func);
	const struct Class  *myClass = _isObject(self->class, selfname, file, line, func);

	if (_class != Object())
	{
		const struct Class *p = myClass;
		const struct Class *class = _isObject(_class, selfname, file, line, func);

		while (p != class)
		{
			if (p == Object())
			{
				fprintf(stderr, "%s:%d: %s: Error: '%s' is object of class '%s' and can't be the object of class '%s'!\n", 
						file, line, func, selfname, myClass->name, class->name);
				exit(EXIT_FAILURE);
			}

			p = p->super;
		}
	}

#ifdef SIGBUS
	signal(SIGBUS, sigbus);
#endif
	signal(SIGSEGV, sigsegv);

	return (void*) self;
}

const void* _classOf(const void *_self, char *selfname, char *file, int line, const char *func)
{
	const struct Object *self = _cast(Object(), _self, selfname, file, line, func);

	return self->class;
}

size_t _sizeOf(const void *_self, char *selfname, char *file, int line, const char *func)
{
	const struct Class *class = _classOf(_self, selfname, file, line, func);

	return class->size;
}

int _isA(const void *_self, const void *_class, char *selfname, char *file, int line, const char *func)
{
	if (_self)
	{
		const struct Object *self = _cast(Object(), _self, selfname, file, line, func);
		const struct Object *class = _cast(Object(), _class, selfname, file, line, func);

		return _classOf(self, selfname, file, line, func) == class;
	}

	return 0;
}

int _isOf(const void *_self, const void *_class, char *selfname, char *file, int line, const char *func)
{
	if (_self)
	{
		const struct Object *self = _cast(Object(), _self, selfname, file, line, func);
		const struct Class *myClass = _classOf(self, selfname, file, line, func);
		const struct Class *class = _cast(Class(), _class, selfname, file, line, func);

		if (class != Object())
		{
			while(myClass != class)
			{
				if (myClass != Object())
					myClass = myClass->super;
				else
					return 0;
			}
		}

		return 1;
	}

	return 0;
}

/*
 * Class
 */

static void* Class_ctor(void *_self, va_list *ap)
{
	struct Class *self = _self;
	const size_t offset = offsetof(struct Class, ctor);

	self->name = va_arg(*ap, char*);
	self->super = va_arg(*ap, struct Class*);
	self->size = va_arg(*ap, size_t);

	if (self->super == NULL)
	{
		fprintf(stderr, "Class_ctor: Error: Superclass of class '%s' can't be NULL!\n", 
				self->name);
		exit(EXIT_FAILURE);
	}

	memcpy((char*) self + offset, 
			(char*) self->super + offset, 
			sizeOf(self->super) - offset);

	voidf selector;

	va_list ap_copy;
	va_copy(ap_copy, *ap);

	while ((selector = va_arg(ap_copy, voidf)))
	{	
		voidf method = va_arg(ap_copy, voidf);

		if (selector == (voidf) ctor)
			self->ctor = (ctor_f) method;
		else if (selector == (voidf) dtor)
			self->dtor = (dtor_f) method;
		else if (selector == (voidf) cpy)
			self->cpy = (cpy_f) method;
		else if (selector == (voidf) set)
			self->set = (set_f) method;
		else if (selector == (voidf) get)
			self->get = (get_f) method;
		else if (selector == (voidf) sfprint)
			self->sfprint = (sfprint_f) method;
		else if (selector == (voidf) sfscan)
			self->sfscan = (sfscan_f) method;
	}

	va_end(ap_copy);
	return self;
}

static void* Class_dtor(void *_self)
{
	struct Class *self = _self;
	fprintf(stderr, "Class_dtor: Error: can't destroy class '%s'!\n", self->name);
	return 0;
}

static void* Class_cpy(const void *_self, void *_object)
{
	const struct Class *self = _self;
	fprintf(stderr, "Class_cpy: Error: can't copy class '%s'!\n", self->name);
	return 0;
}

/*
 *	Initialization
 */

static const struct Class _Object;
static const struct Class _Class;

static const struct Class _Object = {
	{ MAGIC_NUM, &_Class, 1 },
	"Object", &_Object, sizeof(struct Object),
	Object_ctor, Object_dtor, Object_cpy, 
	NULL, NULL, NULL, NULL
};

static const struct Class _Class = {
	{ MAGIC_NUM, &_Class, 1 },
	"Class", &_Object, sizeof(struct Class),
	Class_ctor, Class_dtor, Class_cpy, 
	NULL, NULL, NULL, NULL
};

const void* const Object(void) {
	return &_Object;
}

const void* const Class(void)
{
	return &_Class;
}

/*
 * Exceptions
 */
ObjectImpl(FormatException)
{
	if (!_FormatException)
	{
		_FormatException = new(ExceptionObject(), "FormatException");
	}

	return _FormatException;
}
