#include "Utils.h"

char* int_spelling(int value)
{
	char *spelling;

	if (value % 10 == 1 && value != 11)
		spelling = "st";
	else if (value % 10 == 2 && value != 12)
		spelling = "nd";
	else if (value % 10 == 3 && value != 13)
		spelling = "td";
	else
		spelling = "th";

	return spelling;
}
