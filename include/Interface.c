/* vim: set fdm=marker : */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "Interface.h"
#include "Macro.h"

/* Type checking {{{ */

const void* _isInterface(char *intername, char *file, int line, const char *func,
		const void *_interface)
{
	if (_interface == NULL)
	{
		fprintf(stderr, "%s:%d: %s: Error: '%s' is NULL!",
				file, line, func, intername);
		exit(EXIT_FAILURE);
	}

	const struct Interface *self = _interface;

	if (self->magic != INTERFACE_MAGIC)
	{
		fprintf(stderr, "%s:%d: %s: Error: '%s' isn't an interface!",
				file, line, func, intername);
		exit(EXIT_FAILURE);
	}

	return _interface;
}

static void sigcatch(int signal)
{
	if (signal == SIGBUS)
	{
		fprintf(stderr, "icast: Error: Caught Bus Error, exiting!");
		exit(EXIT_FAILURE);
	}

	if (signal == SIGSEGV)
	{
		fprintf(stderr, "icast: Error: Caught Segmentation Fault, exiting!");
		exit(EXIT_FAILURE);
	}
}

void* _icast(char *intername, char *classname, char *file, int line, const char *func,
		const void *_interface, const void *_class, void *_casted)
{
	void (*sigsegv)(int) = signal(SIGSEGV, sigcatch);
#ifdef SIGBUS
	void (*sigbus)(int) = signal(SIGBUS, sigcatch);
#endif

	if (_casted == NULL)
	{
		fprintf(stderr, "%s:%d: %s: Error: Interface '%s' structure allocation error!",
				file, line, func, intername);
		exit(EXIT_FAILURE);
	}

	const struct Interface *interface = _isInterface(intername, file, line, func, _interface);
	const struct Class *class = _cast("Class()", classname, file, line, func, Class(), _class);

	if (sizeOf(class) <= (offsetof(struct Class, get)) + sizeof(method))
	{
		fprintf(stderr, "%s:%d: %s: Error: Class '%s' doesn't implement interface '%s'!",
				file, line, func, class->name, intername);
		exit(EXIT_FAILURE);
	}

	unsigned int method_sels_nb = interface->method_sels_nb;
	voidf *method_sels = (voidf*)calloc(sizeof(method), method_sels_nb);
	
	if (method_sels == NULL)
	{
		fprintf(stderr, "%s:%d: %s: Error: Method selectors of interface '%s' allocation allocation error!",
				file, line, func, intername);
		exit(EXIT_FAILURE);
	}

	memcpy(method_sels, interface->method_sels, interface->method_sels_nb * sizeof(voidf));

	method* class_method = (method*) ((char*) class + offsetof(struct Class, get) + sizeof(method));
	method* casted = _casted;
	method* c = casted;

	while ((void*) class_method < (void*) ((char*) class + sizeOf(class)))
	{
		for (int i = 0; i < method_sels_nb; ++i) 
		{
			if (method_sels[i] == class_method->selector)
			{
				c->method = class_method->method;
				c->selector = class_method->selector;
				c->tag = class_method->tag;
				c++;

				ARRAY_REMOVE(method_sels, method_sels_nb, i);

				break;
			}
		}

		if (method_sels_nb == 0)
			break;

		class_method++;
	}

	free(method_sels);

	if (method_sels_nb != 0)
	{
		fprintf(stderr, "%s:%d: %s: Error: Class '%s' doesn't implement interface '%s'!",
				file, line, func, class->name, intername);
		exit(EXIT_FAILURE);
	}

#ifdef SIGBUS
	signal(SIGBUS, sigbus);
#endif
	signal(SIGSEGV, sigsegv);

	return (void*) casted;
}

/* }}} */
