#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Init.h"

int main(int argc, char *argv[])
{
	/* smart var matrix1 = new(Matrix(), Float(), 2, 2);

	srand(time(0));	

	rnd(Matrix(), matrix1, 100);

	oprintf("%4.2m\n", matrix1);

	{
		smart var det;
		determinant(matrix1, &det);
		oprintf("%f\n", det);
	}

	FILE *T;

	T = fopen("test.bin", "wb+");

	unsigned int dim1[2];

	matrix_size(matrix1, &dim1[0], &dim1[1]);

	fwrite(dim1, sizeof(dim1), 1, T);
	ofwrite(matrix1, T);

	fclose(T);

	T = fopen("test.bin", "r");

	unsigned int dim2[2];
	fread(dim2, sizeof(dim2), 1, T);

	printf("%dx%d\n", dim2[0], dim2[1]);

	smart var matrix2 = new(Matrix(), Float(), dim2[0], dim2[1]);

	ofread(matrix2, T);

	oprintf("%4.2m\n", matrix2);

	fclose(T); */

	int bruh = 45678923;

	var bits = new(Bitset(), 32);

	for (int i = 0; i < 32; ++i)
	{
		int bit = (bruh & (1 << i)) ? 1 : 0;
		set(bits, i, bit);
	}

	oprint(bits);

	return 0;
}
