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
	double diff = (double)(((struct point*)a)->x) - (double)(((struct point*)b)->x);
	if(diff > 0)
		return 1;
	else
		return 0;
}

#endif