#include "Macro.h"

/*
 * Signed and unsigned ints
 */

#if defined(INT_UNSIGNED)
#undef INT_UNSIGNED
#endif

#if defined(INT_SHORT)
#undef INT_SHORT 
#endif

#include "Template/IntTemplate.c"

#if defined(T) && defined(N)
#undef T
#undef N
#endif

#if !defined(INT_UNSIGNED)
#define INT_UNSIGNED
#endif

#define T unsigned
#define N UInt


/*
 * Signed and unsigned shorts
 */

#undef T
#undef N

#undef INT_UNSIGNED
#define INT_SHORT 

#define T short
#define N ShortInt
#include "Template/IntTemplate.c"

#undef T
#undef N

#define INT_UNSIGNED

typedef unsigned short ushort;

#define T ushort
#define N UShortInt
#include "Template/IntTemplate.c"

/*
 * Signed and unsigned longs
 */

#undef T
#undef N

#undef INT_UNSIGNED
#undef INT_SHORT

#define T long
#define N LongInt
#include "Template/IntTemplate.c"

#undef T
#undef N

#define INT_UNSIGNED

typedef unsigned long ulong;

#define T ulong
#define N ULongInt
#include "Template/IntTemplate.c"

/*
 * Signed and unsigned long longs
 */

#undef T
#undef N

#undef INT_UNSIGNED

typedef long long llong;

#define T llong
#define N LLongInt
#include "Template/IntTemplate.c"

#undef T
#undef N

#define INT_UNSIGNED

typedef unsigned long long ullong;

#define T ullong
#define N ULLongInt
#include "Template/IntTemplate.c"
