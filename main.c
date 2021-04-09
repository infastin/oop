#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "Bitset.h"
#include "IO.h"
#include "Init.h"

int main(int argc, char *argv[])
{
	/*smart var matrix1 = new(Matrix(), Float(), 2, 2);

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

	srand(time(0));

	int i1 = 478912;
	int i2 = 10;

	smart struct Bitset *bits1 = new(Bitset(), 32);
	smart struct Bitset *bits2 = new(Bitset(), 32);

	for (int i = 0; i < 32; ++i)
	{
		int bit1 = (i1 & (1 << i)) ? 1 : 0;
		set(bits1, i, bit1);

		int bit2 = (i2 & (1 << i)) ? 1 : 0;
		set(bits2, i, bit2);
	}

	smart struct Bitset *bits3 = divide(bits1, bits2);
	smart struct Bitset *bits4 = modulo(bits1, bits2);

	oprintln(bits1);
	oprintln(bits2);
	oprintln(bits3);
	oprintln(bits4);

	return 0;
}
