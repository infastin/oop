#ifndef INIT_H_5CYSDZGM
#define INIT_H_5CYSDZGM

#include "Selectors.h"
#include "Object.h"
#include "TypeClass.h"
#include "ReferenceCounter.h"

#include "Types/IntType.h"

#include "Geometry/Circle.h"
#include "Geometry/Point2D.h"

int new_main(int argc, char** argv);

#define main(...) \
	main(int argc, char** argv) { \
		initTypeClass(); \
		initIntType(); \
		initPoint2D(); \
		initCircle(); \
		return new_main(argc, argv); \
	}; \
int new_main(__VA_ARGS__)

#endif /* end of include guard: INIT_H_5CYSDZGM */
