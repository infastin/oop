#include <setjmp.h>
#include <stdio.h>
#include <time.h>

#include "Exception.h"
#include "ExceptionObject.h"
#include "Init.h"
#include "Matrix.h"
#include "Object.h"
#include "Selectors.h"
#include "TypeClass.h"

int main(int argc, char *argv[])
{
	smart var matrix1 = new(Matrix(), Int(), 3, 3);
	
	srand(time(0));	

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int ran1 = rand() % 10;
			set(matrix1, i, j, ran1);
			printf("%d ", ran1);
		}

		printf("\n");
	}

	int det;
	determinant(matrix1, &det);
	printf("%d\n", det);

	return 0;
}
