#ifndef _SSTAT_GLOBAL_UTILITY_H_
#define _SSTAT_GLOBAL_UTILITY_H_
#include <stdio.h>
#include <stdlib.h>
#include "type_def.h"

int compare_double (const void * a, const void * b)
{
   return *((double*)a) > *((double*)b)? 1 : 0;
}

int point_compare_x (const void * a, const void * b)
{
	return ((struct point*)a)->x - ((struct point*)b)->x;
}

#endif