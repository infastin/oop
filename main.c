#include <stdio.h>

#include "Circle.h"
#include "Point2D.h"

int main(int argc, char *argv[])
{
	initCircle();

	var a = new(Circle, 1, 2, 3);

	draw(a);

	return 0;
}
