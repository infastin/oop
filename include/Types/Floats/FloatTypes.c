#include "Macro.h"

/*
 * Double
 */

#if defined (FLOAT_FLOAT)
#undef FLOAT_FLOAT
#endif

#include "Template/FloatTemplate.c"

#if defined (T) && defined (N)
#undef T
#undef N
#endif

#define T double
#define N Double
#include "Template/FloatTemplate.c"

/*
 * Long Double
 */

#undef T
#undef N

typedef long double ldouble;

#define T ldouble
#define N LDouble
#include "Template/FloatTemplate.c"
