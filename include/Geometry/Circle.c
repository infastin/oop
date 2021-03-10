#include <stdio.h>

#include "Circle.h"
#include "Object.h"
#include "Point2D.h"
#include "Selectors.h"

static void* Circle_ctor(void *_self, va_list *props)
{
	struct Circle *self = super_ctor(Circle, _self, props);	

	self->rad = va_arg(*props, int);
	return self;
}

void Circle_draw(const void *_self)
{
	const struct Circle *self = _self;

	printf("circle at %d,%d rad %d\n", self->x, self->y, self->rad);
}

const void* Circle;

void initCircle()
{
	if (!Circle)
	{
		initPoint2D();

		Circle = new(Point2D_Class, "Circle",
				Point2D, sizeof(struct Circle),
				ctor, Circle_ctor,
				draw, Circle_draw,
				0);
	}
}
