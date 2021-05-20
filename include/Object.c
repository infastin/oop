/* vim: set fdm=marker : */

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "Macro.h"
#include "Object.h"
#include "Selectors.h"
#include "ExceptionObject.h"

/* Object {{{ */

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

/* }}} */

/* Type checking and sizeOf {{{ */

const void* _isObject(char *selfname, char *file, int line, const char *func,
		const void *_self)
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
		fprintf(stderr, "%s:%d: %s: Error: '%s' isn't an object!",
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

void* _cast(char *classname, char *selfname, char *file, int line, const char *func,
		const void *_class, const void *_self)
{
	void (*sigsegv)(int) = signal(SIGSEGV, sigcatch);
#ifdef SIGBUS
	void (*sigbus)(int) = signal(SIGBUS, sigcatch);
#endif

	const struct Object *self = _isObject(selfname, file, line, func, _self);

	int selfclass_len = snprintf(NULL, 0, "%s's class", selfname);
	char *selfclass_name = (char*)calloc(sizeof(char), selfclass_len + 1);
	snprintf(selfclass_name, selfclass_len + 1, "%s's class", selfname);

	const struct Class *myClass = _isObject(selfclass_name, file, line, func, self->class);

	free(selfclass_name);

	if (_class != Object())
	{
		const struct Class *p = myClass;
		const struct Class *class = _isObject(classname, file, line, func, _class);

		while (p != class)
		{
			if (p == Object())
			{
				fprintf(stderr, "%s:%d: %s: Error: '%s' can't be the object of class '%s'!\n", 
						file, line, func, selfname, class->name);
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

const void* _classOf(char *selfname, char *file, int line, const char *func,
		const void *_self)
{
	const struct Object *self = _cast("Object()", selfname, file, line, func, Object(), _self);

	return self->class;
}

size_t _sizeOf(char *selfname, char *file, int line, const char *func,
		const void *_self)
{
	const struct Class *class = _classOf(selfname, file, line, func, _self);

	return class->size;
}

int _isA(char *selfname, char *file, int line, const char *func,
		const void *_self, const void *_class)
{
	if (_self)
	{
		const struct Object *self = _cast("Object()", selfname, file, line, func, Object(), _self);
		const struct Object *class = _cast("Object()", selfname, file, line, func, Object(), _class);

		return _classOf(selfname, file, line, func, self) == class;
	}

	return 0;
}

int _isOf(char *selfname, char *file, int line, const char *func,
		const void *_self, const void *_class) 
{
	if (_self)
	{
		const struct Object *self = _cast("Object()", selfname, file, line, func, Object(), _self);
		const struct Class *myClass = _classOf(selfname, file, line, func, self);
		const struct Class *class = _cast("Class()", selfname, file, line, func, Class(), _class);

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

/* }}} */

/* Class {{{ */

static void* Class_ctor(void *_self, va_list *ap)
{
	struct Class *self = _self;

	self->name = va_arg(*ap, char*);
	self->super = va_arg(*ap, struct Class*);
	self->size = va_arg(*ap, size_t);

	if (self->super == NULL)
	{
		fprintf(stderr, "Class_ctor: Error: Superclass of class '%s' can't be NULL!\n", 
				self->name);
		exit(EXIT_FAILURE);
	}

	const size_t offset = offsetof(struct Class, ctor);
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
			update_method(self, ctor, method);
		else if (selector == (voidf) dtor)
			update_method(self, dtor, method);
		else if (selector == (voidf) cpy)
			update_method(self, cpy, method);
		else if (selector == (voidf) set)
			update_method(self, set, method);
		else if (selector == (voidf) get)
			update_method(self, get, method);
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

/* }}} */

/* Static Initialization {{{ */

static const struct Class _Object;
static const struct Class _Class;

static const struct Class _Object = {
	{ MAGIC_NUM, &_Class, 1 },
	"Object", &_Object, sizeof(struct Object),
	{ "ctor", (voidf) ctor, (voidf) Object_ctor },
	{ "dtor", (voidf) dtor, (voidf) Object_dtor },
	{ "cpy",  (voidf) cpy,  (voidf) Object_cpy },
	{ "set",  (voidf) NULL, (voidf) NULL },
	{ "get",  (voidf) NULL, (voidf) NULL },
};

static const struct Class _Class = {
	{ MAGIC_NUM, &_Class, 1 },
	"Class", &_Object, sizeof(struct Class),
	{ "ctor", (voidf) ctor, (voidf) Class_ctor },
	{ "dtor", (voidf) dtor, (voidf) Class_dtor },
	{ "cpy",  (voidf) cpy,  (voidf) Class_cpy },
	{ "set",  (voidf) NULL, (voidf) NULL },
	{ "get",  (voidf) NULL, (voidf) NULL },
};

const void* const Object(void) {
	return &_Object;
}

const void* const Class(void)
{
	return &_Class;
}

/* }}} */

/* Exceptions {{{ */

ObjectImpl(FormatException)
{
	if (!_FormatException)
	{
		_FormatException = new(ExceptionObject(), "FormatException");
	}

	return _FormatException;
}

/* }}} */
