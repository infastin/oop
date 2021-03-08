#ifndef POINT2D_H_L9K8WQMI
#define POINT2D_H_L9K8WQMI

#include "Selectors.h"

extern const void *Point2D_Class, *Point2D;

typedef void (*draw_f)(const void *self);

struct Point2D_Class
{
	const struct Class _;
	draw_f draw;
};

struct Point2D
{
	const struct Object _;
	int x, y;
};

void draw(const void *self);
void super_draw(const void *class, const void *self);

void initPoint2D(void);

#endif /* end of include guard: POINT2D_H_L9K8WQMI */
