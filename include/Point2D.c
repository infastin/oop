#include <stdarg.h>
#include <stdio.h>

#include "Point2D.h"
#include "new.h"

static void* Point2D_ctor(void *_self, va_list *props)
{
	struct Point2D *self = _self;

	self->x = va_arg(*props, int);
	self->y = va_arg(*props, int);

	return self;
}

static void Point2D_draw(const void *_self)
{
	const struct Point2D *self = _self;

	printf("\".\" at %d,%d\n", self -> x, self -> y);
}

const void *Point2D_Object, *Point2D;

/* void initPoint2D(void)
{
	Point2D_Object = extends(Object, 
			Object, sizeof(struct Point2D_Object));
} */

static const struct Point2D_Object _Point2D = {
	{
		sizeof(struct Point2D),
		Point2D_ctor,
		0
	},
	Point2D_draw,
};

void draw(const void *self)
{
	const struct Point2D_Object* const *obj = self;

	(*obj)->draw(self);
}
