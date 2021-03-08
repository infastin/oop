#ifndef POINT2D_H_L9K8WQMI
#define POINT2D_H_L9K8WQMI

#include "new.h"

extern const void* Point2D;

struct Point2D_Object
{
	struct Object;
	void (*draw)(const void *self);
};

struct Point2D
{
	const void *object;
	int x, y;
};

void draw(const void *self);

#endif /* end of include guard: POINT2D_H_L9K8WQMI */
