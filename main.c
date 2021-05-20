#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#include "Init.h"

struct Test
{
	int first;
	char second[4];
};

int main(int argc, char *argv[])
{
	var matr = new(Matrix(), Int(), 3, 3);

	oscanf("%m", matr);

	oprintf("%v", matr);

	double time;

	var det;
	measure_time(time, slow_determinant(matr, &det));

	oprintf("Determinant: %v\n", det);
	printf("Time: %lf seconds\n", time);
}
