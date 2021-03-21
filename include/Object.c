#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "Object.h"
#include "Selectors.h"

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

const void* _isObject(const void *_self, char *file, int line)
{
	if (_self == NULL)
	{
		fprintf(stderr, "%s:%d: isObject error: given variable is NULL!", 
				file, line);
		exit(EXIT_FAILURE);
	}

	const struct Object *self = _self;

	if (self->magic != MAGIC_NUM)
	{
		fprintf(stderr, "%s:%d: isObject error: given variable isn't object!",
				file, line);
		exit(EXIT_FAILURE);
	}

	return _self;
}

static void catch(int signal)
{
	if (signal == SIGBUS)
	{
		fprintf(stderr, "Cast error: Caught Bus Error, exiting!");
		exit(EXIT_FAILURE);
	}
	
	if (signal == SIGSEGV)
	{
		fprintf(stderr, "Cast error: Caught Segmentation Fault, exiting!");
		exit(EXIT_FAILURE);
	}
}

void* _cast(const void *_class, const void *_self, char *file, int line)
{
	void (*sigsegv)(int) = signal(SIGSEGV, catch);
#ifdef SIGBUS
	void (*sigbus)(int) = signal(SIGBUS, catch);
#endif

	const struct Object *self = _isObject(_self, file, line);
	const struct Class  *myClass = _isObject(self->class, file, line);

	if (_class != Object())
	{
		const struct Class *p = myClass;
		const struct Class *class = _isObject(_class, file, line);

		while (p != class)
		{
			if (p == Object())
			{
				fprintf(stderr, "%s:%d: cast error: Object '%s' can't be the object '%s'!\n", 
						file, line, myClass->name, class->name);
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

const void* classOf(const void *_self)
{
	const struct Object *self = cast(Object(), _self);

	return self->class;
}

size_t sizeOf(const void *_self)
{
	const struct Class *class = classOf(_self);

	return class->size;
}

int isA(const void *_self, const void *class)
{
	if (_self)
	{
		const struct Object *self = cast(Object(), _self);
		cast(Object(), class);

		return classOf(self) == class;
	}

	return 0;
}

int isOf(const void *_self, const void *class)
{
	if (_self)
	{
		const struct Object *self = cast(Object(), _self);
		const struct Class *myClass = classOf(self);

		cast(Class, class);

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
		fprintf(stderr, "Class Constructor Error: Superclass of object '%s' can't be NULL!\n", 
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
		else if (selector == (voidf) stringer)
			self->stringer = (stringer_f) method;
	}

	va_end(ap_copy);
	return self;
}

static void* Class_dtor(void *_self)
{
	struct Class *self = _self;
	fprintf(stderr, "Class Destructor Error: can't destroy class '%s'!\n", self->name);
	return 0;
}

static void* Class_cpy(const void *_self, void *_object)
{
	const struct Class *self = _self;
	fprintf(stderr, "Class Copy Error: can't copy class '%s'!\n", self->name);
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
	NULL, NULL, NULL
};

static const struct Class _Class = {
	{ MAGIC_NUM, &_Class, 1 },
	"Class", &_Object, sizeof(struct Class),
	Class_ctor, Class_dtor, Class_cpy, 
	NULL, NULL, NULL
};

const void* const Object(void) {
	return &_Object;
}

const void* const Class(void)
{
	return &_Class;
}
