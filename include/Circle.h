#ifndef CIRCLE_H_VZMN5SHJ
#define CIRCLE_H_VZMN5SHJ

#include "Point2D.h"
extern const void* Circle;

struct Circle_Object
{
	struct Point2D_Object;
	const struct Point2D_Object *super;
};

struct Circle
{
	struct Point2D;
	int rad;
};

void initCircle();

#endif /* end of include guard: CIRCLE_H_VZMN5SHJ */
