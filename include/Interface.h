#ifndef INTERFACE_H_DBQUJ2HM
#define INTERFACE_H_DBQUJ2HM

#include <stddef.h>
#include <stdlib.h>

/*
 * Interface macro
 */

#define InterfaceHeader(T) extern const void* const T (void)

#define InterfaceImpl(T)  		\
	static const void *_ ## T; 	\
	const void* const T(void)

/*
 * Interface
 */

#define MAGIC_INUM 0xb00ba

struct InterfaceElem
{
	const struct Interface *interface;
	size_t offset;
};

struct Interface
{
	unsigned int magic;
	const char *name;
	struct InterfaceElem *extends;
	unsigned int ext_number;
};

const void* _isInterface(const void *self, 
		char *selfname, char *file, int line, const char *func);

ssize_t _doesExtend(const void *interface, const void *self, 
		char *iname, char *selfname, char *file, int line, const char *func);

void *_icast(const void *interface, const void *self, 
		char *iname, char *selfname, char *file, int line, const char *func);

#define isInterface(self) _isInterface(self, \
	#self, __FILE__, __LINE__, __FUNCTION__)

#define doesExtend(interface, self, offset) _doesExtend(interface, self, \
	#interface, #self, __FILE__, __FILE__, __FUNCTION__)

#define icast(interface, self) _icast(interface, self, \
	#interface, #self, __FILE__, __LINE__, __FUNCTION__)

#endif /* end of include guard: INTERFACE_H_DBQUJ2HM */
