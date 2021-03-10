#include <stdio.h>

#include "Init.h"

int main(int argc, char *argv[])
{
	{
		smart var a = new(Int, 10);
		smart var b = a;

		smart struct Object *A = retain(a);
		smart struct Object *B = retain(b);

		set(a, 12);

		int val = get(A, int);

		printf("%d %lu %lu\n", val, A->ref_count, B->ref_count);
	}

	return 0;
}
