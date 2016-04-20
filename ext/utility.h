#ifndef _SSTAT_UTILITY_H_
#define _SSTAT_UTILITY_H_

#include <stdio.h>
#include <stdlib.h>
#include "data_type.h"

int cmp_func_double (const void * a, const void * b)
{
   return ( *(double*)a - *(double*)b );
}

int point_compare_x (const void * a, const void * b)
{
	return ((struct point*)a)->x - ((struct point*)b)->x;
}

#endif
