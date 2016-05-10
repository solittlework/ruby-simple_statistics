#ifndef _SURVIVAL_STAT_DEF_H_
#define _SURVIVAL_STAT_DEF_H_

#include "type_def.h"
//define survival stat data
typedef struct Group_N
{
	int * uncensored;
	int * censored;
	double* time;
	int size;
} Group_N;

#endif
