#ifndef GENERICTYPE_H_I24WTVPN
#define GENERICTYPE_H_I24WTVPN

#include <stdint.h>

#include "Object.h"
#include "Macro.h"

ClassHeader(GenericType);
ObjectHeader(GenericTypeException);

struct GenericType
{
	struct Object _;

	enum
	{
		GT_SHORT,
		GT_USHORT,
		GT_INT,
		GT_UINT,
		GT_LONG,
		GT_ULONG,
		GT_LLONG,
		GT_ULLONG,
		GT_FLOAT,
		GT_DOUBLE,
		GT_LDOUBLE,
		GT_POINTER
	} type;

	union {
		short		   		gt_short;
		unsigned short 		gt_ushort;
		int 	 	   		gt_int;
		unsigned   			gt_uint;
		long 	 	   		gt_long;
		unsigned long   	gt_ulong;
		long long 			gt_llong;
		unsigned long long 	gt_ullong;
		float 				gt_float;
		double 				gt_double;
		long double 		gt_ldouble;
		void* 				gt_pointer;
	} value;
};

#endif /* end of include guard: GENERICTYPE_H_I24WTVPN */
