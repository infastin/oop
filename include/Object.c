#include <assert.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "ExceptionObject.h"
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


static void catch(int signal)
{
	assert(signal == 0);
}

void* cast(const void* class, const void *_self)
{
	void (*sigsegv)(int) = signal(SIGSEGV, catch);
#ifdef SIGBUS
	void (*sigbus)(int) = signal(SIGBUS, catch);
#endif

	const struct Object *self = isObject(_self);
	const struct Class  *myClass = isObject(self->class);

	if (class != Object())
	{
		isObject(class);

		while(myClass != class)
		{
			assert(myClass != Object());
			myClass = myClass->super;
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

	assert(self->super);

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
	}

	va_end(ap_copy);
	return self;
}

static void* Class_dtor(void *_self)
{
	struct Class *self = _self;
	fprintf(stderr, "%s: cannot destroy class\n", self->name);
	return 0;
}

static void* Class_cpy(const void *_self, void *_object)
{
	const struct Class *self = _self;
	fprintf(stderr, "%s: cannot copy class\n", self->name);
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
	Object_ctor, Object_dtor, Object_cpy, NULL, NULL
};

static const struct Class _Class = {
	{ MAGIC_NUM, &_Class, 1 },
	"Class", &_Object, sizeof(struct Class),
	Class_ctor, Class_dtor, Class_cpy, NULL, NULL 
};

const void* const Object(void) {
	return &_Object;
}

const void* const Class(void)
{
	return &_Class;
}

/*
 * Exception Initialization
 */

ObjectImpl(ObjectException)
{
	if (_ObjectException)
	{
		_ObjectException = new(ExceptionObject(), "ObjectException");
	}

	return _ObjectException;
}
