#ifndef INTERFACE_H_RAPMB3IV
#define INTERFACE_H_RAPMB3IV

#include "Object.h"

#define INTERFACE_MAGIC 0xAb0bA

struct Interface
{
	unsigned int  magic;
	const char 	 *name;
	unsigned int  method_sels_nb;
	voidf 		 *method_sels;
};

void* _icast(char *intername, char *selfname, char *file, int line, const char *func,
		const void *_interface, const void *_self, void *casted);

const void* _isInterface(char *intername, char *file, int line, const char *func,
		const void *interface);

#define isInterface(self) _isInterface(TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		self)

#define _icast_stack(interface, self, casted) _icast(TOSTR(interface), TOSTR(self), __FILE__, __LINE__, __FUNCTION__, \
		interface, self, casted)

#define icast(interface, self) _icast_stack(interface(), self, (char[sizeof(struct interface)]){0})

#define interface_sel(r, m, i, v, b, ...) \
	r m(__VA_ARGS__) \
{ \
	const struct Class *class = classOf(_self); \
	const struct i ## Interface *v = icast(i ## Interface, class); \
	\
	if (v->m.method == NULL) \
	throw(i ## Exception(), "Error: Class '%s' doesn't implement '" #m "' method of '" #i "Interface'!", \
			class->name); \
	b; \
}\
r super_ ## m(const void *_class, __VA_ARGS__) \
{ \
	const struct Class *superclass = super(_class); \
	const struct i ## Interface *v = icast(i ## Interface, superclass); \
	\
	if (v->m.method == NULL) \
	throw(i ## Exception(), "Error: Class '%s' doesn't implement '" #m "' method of '" #i "Interface'!", \
			superclass->name); \
	b; \
}

#define interface_sel_header(r, m, ...) \
r m(__VA_ARGS__); \
r super_ ## m(const void *class, __VA_ARGS__);

#endif /* end of include guard: INTERFACE_H_RAPMB3IV */
