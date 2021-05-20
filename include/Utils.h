#ifndef UTILS_H_OXNSUB1G
#define UTILS_H_OXNSUB1G

#include <time.h>

#define measure_time(time, exp) 						\
{ 														\
	clock_t start, end; 								\
	start = clock(); 									\
	exp; 												\
	end = clock(); 										\
	time = ((double) (end - start)) / CLOCKS_PER_SEC; 	\
}

char* int_spelling(int value); 

#endif /* end of include guard: UTILS_H_OXNSUB1G */
