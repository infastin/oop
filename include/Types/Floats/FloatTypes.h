#ifndef FLOATTYPES_H_QX9MMR1C
#define FLOATTYPES_H_QX9MMR1C

#include "Macro.h"

/*
 * Double
 */

#include "Template/FloatTemplate.h"

#if defined (T) && defined (N)
#undef T
#undef N
#endif

#define T double
#define N Double
#include "Template/FloatTemplate.h"

/*
 * Long Double
 */

#undef T
#undef N

typedef long double ldouble;

#define T ldouble
#define N LDouble
#include "Template/FloatTemplate.h"

#endif /* end of include guard: FLOATTYPES_H_QX9MMR1C */
