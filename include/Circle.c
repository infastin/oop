#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "Circle.h"
#include "Point2D.h"

static void* Circle_ctor(void *_self, va_list *props)
{
	struct Circle *self = ((const struct Object *) Point2D)->ctor(_self, props);

	self->rad = va_arg(*props, int);
	return self;
}

void Circle_draw(const void *_self)
{
	const struct Circle *self = _self;

	printf("circle at %d,%d rad %d\n", self->x, self->y, self->rad);
}

const void *Circle;

void initCircle()
{
	struct Circle_Object *_Circle = (struct Circle_Object*)calloc(1, sizeof(struct Circle_Object));
	const size_t offset = offsetof(struct Circle_Object, ctor);

	_Circle->super = (const struct Point2D_Object*)Point2D;
	_Circle->size = sizeof(struct Circle);

	memcpy((char*)_Circle + offset, 
		   (char*)_Circle->super + offset,
			sizeof(struct Point2D_Object) - offset);

	_Circle->ctor = Circle_ctor;
	_Circle->draw = Circle_draw;

	Circle = _Circle;
}
