#ifndef _SURVIVAL_STAT_DEF_H_
#define _SURVIVAL_STAT_DEF_H_

#include "type_def.h"


/**
 * @brief stores the number of censored and uncensored samples for each time range
 */
typedef struct CENS_UC_NUM
{
	int * uncensored;
	int * censored;
	double* time; //time series
	int size;
} CENS_UC_NUM;

void free_CENS_UC_NUM(struct CENS_UC_NUM instance)
{
	if(instance.uncensored != NULL)
		free(instance.uncensored);

	if(instance.censored != NULL)
		free(instance.censored);

	if(instance.time != NULL)
		free(instance.time);
}

#endif