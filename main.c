#include <stdio.h>

#include "Circle.h"

int main(int argc, char *argv[])
{
	initCircle();

	void *a = new(Circle, 1, 2, 3);

	draw(a);

	return 0;
}
