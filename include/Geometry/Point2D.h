#ifndef POINT2D_H_L9K8WQMI
#define POINT2D_H_L9K8WQMI

#include "Selectors.h"

extern const void* const Point2D_Class(void);
extern const void* const Point2D(void);

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

#endif /* end of include guard: POINT2D_H_L9K8WQMI */
