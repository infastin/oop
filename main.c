#include <stdio.h>
#include <time.h>

#include "Init.h"

int main(int argc, char *argv[])
{
	smart var matrix1 = new(Matrix(), Int(), 3, 3);
	
	srand(time(0));	

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int ran1 = rand() % 100;
			set(matrix1, i, j, ran1);
			printf("%d ", ran1);
		}

		printf("\n");
	}

	int det;
	determinant(matrix1, &det);
	printf("%d\n", det);

	var tmp = new(Float(), 45.00);

	oprintf("%v\n%07m", tmp, matrix1);

	return 0;
}
