#ifndef INTTEMPLATE_H_2SKYVGWR
#define INTTEMPLATE_H_2SKYVGWR

#include "Macro.h"

/*
 * Signed and unsigned ints
 */

#include "Template/IntTemplate.h"

#if defined(T) && defined(N)
#undef T
#undef N
#endif

#define T unsigned
#define N UInt
#include "Template/IntTemplate.h"

/*
 * Signed and unsigned shorts
 */

#undef T
#undef N

#define T short
#define N ShortInt
#include "Template/IntTemplate.h"

#undef T
#undef N

typedef unsigned short ushort;

#define T ushort
#define N UShortInt
#include "Template/IntTemplate.h"

/*
 * Signed and unsigned longs
 */

#undef T
#undef N

#define T long
#define N LongInt
#include "Template/IntTemplate.h"

#undef T
#undef N

typedef unsigned long ulong;

#define T ulong
#define N ULongInt
#include "Template/IntTemplate.h"

/*
 * Signed and unsigned long longs
 */

#undef T
#undef N

typedef long long llong;

#define T llong
#define N LLongInt
#include "Template/IntTemplate.h"

#undef T
#undef N

typedef unsigned long long ullong;

#define T ullong
#define N ULLongInt
#include "Template/IntTemplate.h"

#endif /* end of include guard: INTTEMPLATE_H_2SKYVGWR */
