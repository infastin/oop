#include <stdio.h>
#include <stdlib.h>

#include "Exception.h"
#include "Interface.h"
#include "Object.h"
#include "Utils.h"

const void* _isInterface(const void *_self, 
		char *selfname, char *file, int line, const char *func)
{
	if (_self == NULL)
	{
		fprintf(stderr, "%s:%d: %s: Error '%s' is NULL!",
				file, line, func, selfname);
		exit(EXIT_FAILURE);
	}

	const struct Interface *self = _self;

	if (self->magic != MAGIC_INUM)
	{
		fprintf(stderr, "%s:%d: %s: Error: '%s' isn't an interface!",
				file, line, func, selfname);
		exit(EXIT_FAILURE);
	}

	return _self;
}

ssize_t _doesExtend(const void *_interface, const void *_self, 
		char *iname, char *selfname, char *file, int line, const char *func)
{
	const struct Interface *self = _isInterface(_self, selfname, file, line, func);
	const struct Interface *interface = _isInterface(_interface, iname, file, line, func);

	ssize_t offset = -1;

	for (unsigned int i = 0; i < self->ext_number; ++i)
	{
		if (self->extends[i].interface == interface)
		{
			offset = self->extends[i].offset;
			break;
		}

		if (self->extends[i].interface->ext_number != 0 && self->extends[i].interface->extends != NULL)
		{
			int ext_iname_len = snprintf(NULL, 0, "%s's extended interface '%s'", 
					selfname, self->extends[i].interface->name);
			char *ext_iname = (char*)calloc(sizeof(char), ext_iname_len);
			snprintf(ext_iname, ext_iname_len, "%s's extended interface '%s'", 
					selfname, self->extends[i].interface->name);

			ssize_t res = _doesExtend(_interface, self->extends[i].interface, ext_iname, selfname, file, line, func);
			free(ext_iname);

			if (res != -1)
			{
				offset = self->extends[i].offset + res;
				break;
			}
		}
	}

	return offset;
}

void* _icast(const void *_interface, const void *_self, 
		char *iname, char *selfname, char *file, int line, const char *func)
{
	const struct Class *self = _cast(Class(), _self, "Class()", selfname, file, line, func);
	const struct Interface *interface = _isInterface(_interface, iname, file, line, func);

	ssize_t offset = -1;

	for (unsigned int i = 0; i < self->impl_number; i++)
	{
		if (self->implements[i].interface == interface)
		{
			offset = self->implements[i].offset;
			break;
		}

		if (self->implements[i].interface->ext_number != 0 && self->implements[i].interface->extends != NULL)
		{
			int ext_iname_len = snprintf(NULL, 0, "%s's extended interface '%s'", 
					iname, self->implements[i].interface->name);
			char *ext_iname = (char*)calloc(sizeof(char), ext_iname_len);
			snprintf(ext_iname, ext_iname_len, "%s's extended interface '%s'", 
					iname, self->implements[i].interface->name);

			ssize_t res = _doesExtend(_interface, self->implements[i].interface, iname, ext_iname, file, line, func);
			free(ext_iname);

			if (res != -1)
			{
				offset = self->implements[i].offset + res;
				break;
			}
		}
	}

	if (offset == -1)
	{
		fprintf(stderr, "%s:%d: %s: Error: Class '%s' doesn't implement interface '%s'!\n", 
				file, line, func, self->name, interface->name);
		exit(EXIT_FAILURE);
	}

	return (void*) (((char*) self) + offset);
}
