#include "FloatTemplate_default.h"

#if defined (T) && defined (N)

#include "TypeClass.h"

ClassHeader(N);

struct N
{
	const struct Object _;
	T value;
};

#endif
