#ifndef _SURVIVAL_STAT_DEF_H_
#define _SURVIVAL_STAT_DEF_H_

#include "type_def.h"

/* Define survival stat data structure */
typedef struct Group_N
{
	/* Time stands for the event time */
	double* time;

	/* How many people died at different event times */
	int * uncensored;

	/* How many people censored at different event times */
	int * censored;

	int size;

} Group_N;

void free_Group_N(struct Group_N* grp_n)
{
	if(grp_n->time != NULL)
		free(grp_n->time);

	if(grp_n->uncensored != NULL )
		free(grp_n->uncensored);

	if(grp_n->censored != NULL )
		free(grp_n->censored);
}

void put_Group_N(struct Group_N * grp_n)
{
	int i;
	puts("Start to print out Group N");
	for( i = 0; i < grp_n->size; i++ )
	{
		printf("Time : %f -- Uncensored : %f -- Censored : %f \n", 
			grp_n-> time[i],
			grp_n-> uncensored[i],
			grp_n-> censored[i]);
	}
}

#endif
