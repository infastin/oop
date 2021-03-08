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

	while ((selector = va_arg(*props, voidf)))
	{	voidf method = va_arg(*props, voidf);

		if (selector == (voidf) draw)
			self->draw = (draw_f) method;
	}

	return self;
}

void Point2D_Class_method(void *_self, voidf _replace, voidf _method)
{
	struct Point2D_Class *self = _self;

	if (_replace == (voidf) draw)
		self->draw = (draw_f) _method;
	else
		super_method(Point2D_Class, _self, _replace, _method);
}


/*
 * Initializtion
 */

const void *Point2D_Class, *Point2D;

void initPoint2D(void)
{
	if(!Point2D_Class)
	{
		void *pointclass = new(Class, "Point2D_Class", 
				Class, sizeof(struct Point2D_Class));

		method(pointclass, (voidf) ctor, (voidf) Point2D_Class_ctor);
		method(pointclass, (voidf) method, (voidf) Point2D_Class_method);

		Point2D_Class = pointclass;
	}

	if (!Point2D)
	{
		void *point = new(Point2D_Class, "Point",
				Object, sizeof(struct Point2D));

		method(point, (voidf) ctor, (voidf) Point2D_ctor);
		method(point, (voidf) draw, (voidf) Point2D_draw);

		Point2D = point;
	}
}
