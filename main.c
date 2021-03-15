#include <setjmp.h>
#include <stdio.h>

#include "Exception.h"
#include "ExceptionObject.h"
#include "Init.h"
#include "Object.h"
#include "Selectors.h"

int main(int argc, char *argv[])
{
	try 
	{
		throw(SomeError(), "Bruh");
	}
	catch(e, SomeError())
	{
		printf("Caught exception: '%s: %s'", exception_name(e), exception_message(e));
	}

	return 0;
}
