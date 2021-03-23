#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Init.h"
#include "Matrix.h"
#include "Selectors.h"
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

	FILE *T;

	T = fopen("test.bin", "wb+");

	unsigned int dim1[2];

	matrix_size(matrix1, &dim1[0], &dim1[1]);

	fwrite(dim1, sizeof(dim1), 1, T);
	sfprint(matrix1, T, 1, NULL, 0, -1, -1, -1);

	fclose(T);

	T = fopen("test.bin", "r");

	unsigned int dim2[2];
	int res = fread(dim2, sizeof(dim2), 1, T);

	printf("%d: %dx%d\n", res, dim2[0], dim2[1]);

	smart var matrix2 = new(Matrix(), Float(), dim2[0], dim2[1]);

	for (int i = 0; i < dim2[0]; ++i) 
	{
		for (int j = 0; j < dim2[1]; ++j) 
		{
			double val;
			fread(&val, sizeof(val), 1, T);
			set(matrix2, i, j, val);
		}
	}

	oprint(matrix2);

	fclose(T);

	return 0;
}
