#ifndef _SSTAT_FUNC_H_
#define _SSTAT_FUNC_H_
#include "data_type.h"
#include <stdio.h>
#include <stdlib.h>

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

struct curve kaplan_meier(double* time, int* censored, int size)
{

	int i, count, KM_size,
		KM_count_at, num_uncensored_at, num_censored_at,
		N;
	double KM_tp_at;
	double tmp;
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

	//count unique uncensored time point for KM
	count = 0;
	for(i = 0; i < size; i++)
	{
		if(time_censored_array[i].y < 0)
		{
			if(count == 0)
			{
				count++;
				tmp = time_censored_array[i].x;
			}

			if(count > 0)
			{
				if (time_censored_array[i].x != tmp)
				{
					count++;
					tmp = time_censored_array[i].x;
				}
			}
		}
	}

	KM_size = count;
	//kaplan_meier time point is the unique uncensored time
	double* KM_tp = (double *) malloc(KM_size * sizeof(double));

	count = 0;
	for(i = 0; i < size; i++)
	{
		if(time_censored_array[i].y < 0)
		{
			if(count == 0)
			{
				count++;
				KM_tp[count] = time_censored_array[i].x;
				tmp = time_censored_array[i].x;

			}

			if(count > 0)
			{
				if (time_censored_array[i].x != tmp)
				{
					KM_tp[count] = time_censored_array[i].x;

					count++;
					tmp = time_censored_array[i].x;
				}
			}
		}
	}

	//number of uncensored in KM for corresponding time pont
	int* num_uncensored_KM = (int *) malloc(count * sizeof(int));

	//number of censored in KM for corresponding time pont
	int* num_censored_KM = (int *) malloc(count * sizeof(int));

	//record current KM time point
	KM_tp_at = KM_tp[0];
	KM_count_at = 0;
	num_uncensored_at = 0;
	num_censored_at = 0;

	for(i = 0; i < size; i++)
	{
		if(time_censored_array[i].x <= KM_tp_at)
		{
			if (time_censored_array[i].y > 0)
				num_censored_at++;
			else
				num_uncensored_at++;

			if(i == size-1)
			{
				num_uncensored_KM[KM_count_at] = num_uncensored_at;
				num_censored_KM[KM_count_at] = num_censored_at;
			}

		} else {
			num_uncensored_KM[KM_count_at] = num_uncensored_at;
			num_censored_KM[KM_count_at] = num_censored_at;
			KM_count_at++;

			num_uncensored_at = 0;
			num_censored_at = 0;
			KM_tp_at = KM_tp[KM_count_at];
			//Notice we still need to update here
			if (time_censored_array[i].y > 0)
				num_censored_at++;
			else
				num_uncensored_at++;
		}
	}

	struct point* KM = (struct point*) malloc(size * sizeof(struct point));
	count = sizeof time_censored_array;

	//Total size of sample
	N = size;

	for(i = 0; i < KM_size; i++)
	{
		if(i > 0)
		{
			N = (N - num_uncensored_KM[i-1] - num_censored_KM[i-1]);
			KM[i].x = KM_tp[i];
			KM[i].y = 1.0 * (N - num_uncensored_KM[i] - num_censored_KM[i]) / (N - num_censored_KM[i]) * KM[i-1].y;
		} else {
			KM[0].x = KM_tp[0];
			KM[0].y = 1.0 * (N - num_uncensored_KM[0] - num_censored_KM[0]) / (N - num_censored_KM[0]);
		}
	}
	
	curve KM_curve;
	KM_curve.point_arr = KM;
	KM_curve.size = KM_size;
	free(time_censored_array);
	free(KM_tp);
	free(num_uncensored_KM);
	free(num_censored_KM);
	return KM_curve;
}

#endif
