#include <stdio.h>
#include <time.h>

#include "Init.h"
#include "limits.h"

int main(int argc, char *argv[])
{
	smart var matrix1 = new(Matrix(), Float(), 2, 2);

	srand(time(0));	

	rnd(Matrix(), matrix1, 100);

	oprintf("%8.2m\n", matrix1);

	var det;
	determinant(matrix1, &det);
	oprintf("%f\n", det);

	return 0;
}
