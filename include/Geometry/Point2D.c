#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "Object.h"
#include "Point2D.h"
#include "Selectors.h"

/*
 * Point2D
 */

static void* Point2D_ctor(void *_self, va_list *props)
{
	struct Point2D *self = super_ctor(Point2D, _self, props);

	self->x = va_arg(*props, int);
	self->y = va_arg(*props, int);

	return self;
}

static void Point2D_draw(const void *_self)
{
	const struct Point2D *self = _self;

	printf("\".\" at %d,%d\n", self->x, self->y);
}

/*
 * New selectors for Point2D
 */

void draw(const void *_self)
{
	const struct Point2D_Class *class = classOf(_self);

	assert(class->draw);
	class->draw(_self);
}

void super_draw(const void *_class, const void *_self)
{
	const struct Point2D_Class *superclass = super(_class);

	assert(superclass->draw);
	superclass->draw(_self);
}

/*
 * Point2D_Class
 */

static void* Point2D_Class_ctor(void *_self, va_list *props)
{
	struct Point2D_Class *self = super_ctor(Point2D_Class, _self, props);

	voidf selector;
	va_list ap;
	va_copy(ap, *props);

	while ((selector = va_arg(ap, voidf)))
	{	
		voidf method = va_arg(ap, voidf);

		if (selector == (voidf) draw)
			self->draw = (draw_f) method;
	}

	return self;
}

/*
 * Initializtion
 */

const void *Point2D_Class, *Point2D;

void initPoint2D(void)
{
	if(!Point2D_Class)
	{
		Point2D_Class = new(Class, "Point2D_Class", 
				Class, sizeof(struct Point2D_Class),
				ctor, Point2D_Class_ctor,
				0);
	}

	if (!Point2D)
	{
		Point2D = new(Point2D_Class, "Point",
				Object, sizeof(struct Point2D),
				ctor, Point2D_ctor,
				draw, Point2D_draw,
				0);
	}
}
