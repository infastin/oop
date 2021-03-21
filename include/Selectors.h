#ifndef NEW_H_0FEJN6R1
#define NEW_H_0FEJN6R1

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

typedef void* var;

void*  _new(const void *class, char *file, int line, ...);
void*  _vnew(const void *class, char *file, int line, va_list *ap);
void   _delete(void *self, char *file, int line);
void*  _copy(const void *self, char *file, int line);

void*  ctor(void *self, va_list *ap);
void*  dtor(void *self);
void*  cpy(const void *self, void *object);

const void* super(const void *self);
void*  super_ctor(const void *class, void *self, va_list *ap);
void*  super_dtor(const void *class, void *self);
void*  super_cpy(const void *class, const void *self, void *object);

char*  stringer(const void *self, ...);
int    reader(const void *_self, const char *str, ...);

void   set(void *self, ...);
void   get(const void *self, ...);

void   vset(void *self, va_list *ap);
void   vget(const void *self, va_list *ap);
char*  vstringer(const void *self, va_list *ap);
int    vreader(const void *_self, const char *str, va_list *ap);

#define new(class, ...) _new(class, __FILE__, __LINE__, ## __VA_ARGS__)
#define vnew(class, ap) _vnew(class, __FILE__, __LINE__, ap)
#define copy(self) _copy(self, __FILE__, __LINE__)
#define delete(self) _delete(self, __FILE__, __LINE__)

#endif /* end of include guard: NEW_H_0FEJN6R1 */
