#ifndef REFERENCECOUNTER_H_GPXNQOXC
#define REFERENCECOUNTER_H_GPXNQOXC

extern void release(void *self_ptr);

#define smart __attribute__ ((cleanup(release)))

void* retain(void *self);

#endif /* end of include guard: REFERENCECOUNTER_H_GPXNQOXC */
