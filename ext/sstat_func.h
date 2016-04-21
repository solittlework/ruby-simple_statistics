#ifndef _SSTAT_FUNC_H_
#define _SSTAT_FUNC_H_
#include "data_type.h"
#include <stdio.h>
#include <stdlib.h>

//store the uncensored and censored at risk within various time range
typedef struct at_risk_array
{
	int * uncensored;
	int * censored;
	double* time;
	int size;
} at_risk_array;

struct at_risk_array count_at_risk(double* time, int* censored, int size)
{
	int i, count_at, num_uncensored_at, num_censored_at;
	double tmp, time_at;
	// sort time and censored based on time together, time can censored array
	struct point* time_censored_array = (struct point*) malloc(size * sizeof(struct point));
	for(i=0; i <size; i++)
	{
		time_censored_array[i].x = time[i];
		//not very fast here, prefer to define another point
		if(censored[i] > 0)
			time_censored_array[i].y = 1;
		else
			time_censored_array[i].y = -1;
	}

	qsort(time_censored_array, size, sizeof(struct point), &point_compare_x);

	//count unique uncensored time point
	int count = 0;
	for(i = 0; i < size; i++)
	{	//uncensored
		if(time_censored_array[i].y < 0)
		{
			if(count == 0)
			{
				count++;
				tmp = time_censored_array[i].x;
			}

			if(count > 0)
			{	//unique
				if (time_censored_array[i].x != tmp)
				{
					count++;
					tmp = time_censored_array[i].x;
				}
			}
		}
	}

	//kaplan_meier time point is the unique uncensored time
	double* unique_uncensored_time = (double *) malloc(count * sizeof(double));

	count = 0;
	for(i = 0; i < size; i++)
	{
		if(time_censored_array[i].y < 0)
		{
			if(count == 0)
			{
				count++;
				unique_uncensored_time[count] = time_censored_array[i].x;
				tmp = time_censored_array[i].x;

			}

			if(count > 0)
			{
				if (time_censored_array[i].x != tmp)
				{
					unique_uncensored_time[count] = time_censored_array[i].x;

					count++;
					tmp = time_censored_array[i].x;
				}
			}
		}
	}

	int* num_uncensored = (int *) malloc(count * sizeof(int));
	int* num_censored = (int *) malloc(count * sizeof(int));

	//record current time point
	time_at = unique_uncensored_time[0];
	count_at = 0;
	num_uncensored_at = 0;
	num_censored_at = 0;

	for(i = 0; i < size; i++)
	{
		if(time_censored_array[i].x <= time_at)
		{
			if (time_censored_array[i].y > 0)
				num_censored_at++;
			else
				num_uncensored_at++;

			if(i == size-1)
			{
				num_uncensored[count_at] = num_uncensored_at;
				num_censored[count_at] = num_censored_at;
			}

		} else {
			num_uncensored[count_at] = num_uncensored_at;
			num_censored[count_at] = num_censored_at;
			count_at++;

			num_uncensored_at = 0;
			num_censored_at = 0;
			time_at = unique_uncensored_time[count_at];
			//we need to update here
			if (time_censored_array[i].y > 0)
				num_censored_at++;
			else
				num_uncensored_at++;
		}
	}

	at_risk_array at_risk_result;
	at_risk_result.uncensored = num_uncensored;
	at_risk_result.censored = num_censored;
	at_risk_result.size = count;
	at_risk_result.time = unique_uncensored_time;
	free(time_censored_array);
	return at_risk_result;
}

double precentile(double* array, int size, double target_percentile)
{
	double percentage_each = 1.0 / size;
	int i;
	int count = 0;
	for(i = 1; i <= size; i++)
	{
		if ( i * percentage_each < target_percentile )
		{
			count++;
		} else
		{
			return array[count];
		}
	}

	return array[size -1];
}

int index_less_equal(double* array, int size, double target)
{
	int i;

	for(i = 0; i < size; i++)
	{
		if(array[i] > target)
			return i;
	}

	return i;
}

double log_rank_test(double* time_1, int* censored_1, double* time_2, int* censored_2, int size)
{	int N1, N2;
	at_risk_array at_risk_1 = count_at_risk(time_1, censored_1, size);
	at_risk_array at_risk_2 = count_at_risk(time_2, censored_2, size);

	free(at_risk_1.uncensored);
	free(at_risk_1.censored);
	free(at_risk_1.time);
	free(at_risk_2.uncensored);
	free(at_risk_2.censored);
	free(at_risk_2.time);

}

struct curve kaplan_meier(double* time, int* censored, int size)
{

	int i, N;

	at_risk_array at_risk = count_at_risk(time, censored, size);

	N = size;

	struct point* KM = (struct point*) malloc(at_risk.size * sizeof(struct point));

	for(i = 0; i < at_risk.size; i++)
	{
		if(i > 0)
		{
			N = (N - at_risk.uncensored[i-1] - at_risk.censored[i-1]);
			KM[i].x = at_risk.time[i];
			KM[i].y = 1.0 * (N - at_risk.uncensored[i] - at_risk.censored[i]) / (N - at_risk.censored[i]) * KM[i-1].y;
		} else {
			KM[0].x = at_risk.time[i];
			KM[0].y = 1.0 * (N - at_risk.uncensored[0] - at_risk.censored[0]) / (N - at_risk.censored[0]);
		}
	}
	
	curve KM_curve;
	KM_curve.point_array = KM;
	KM_curve.size = at_risk.size;

	free(at_risk.uncensored);
	free(at_risk.censored);
	free(at_risk.time);

	return KM_curve;
}

#endif