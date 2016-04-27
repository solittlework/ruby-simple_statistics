#ifndef _SURVIVAL_STAT_KAPLAN_MEIER_H_
#define _SURVIVAL_STAT_KAPLAN_MEIER_H_

#include "survival_def.h"

/**
 * @brief calculate the number of samples censored or uncenosored (die) at each time ragne
 * @param time Event time array
 * @param censored censored information: positive -> censored; zero or negative -> uncensored
 * @return CENS_UC_NUM structure
 */
struct CENS_UC_NUM censored_uncensred_each_time_range(double* time, int* censored, int size)
{
	int i, count_at, uncensored_num_at, censored_num_at;
	double tmp, time_at;

	//sort time and censored based on time together, time can censored array
	struct point* time_censored_array = alloc_points(size);

	//censored, if censored[] is positive
	for (i = 0; i < size; i++)
	{
		time_censored_array[i].x = time[i];
		if (censored[i] > 0)
			time_censored_array[i].y = 1;
		else
			time_censored_array[i].y = -1;
	}

	qsort(time_censored_array, size, sizeof(struct point), &point_compare_x);

	//count number of unique uncensored time point
	int count = 0;
	for (i = 0; i < size; i++)
	{	//uncensored
		if (time_censored_array[i].y < 0)
		{
			if (count == 0)
			{
				count++;
				tmp = time_censored_array[i].x;
			}

			if (count > 0)
			{	//unique
				if (time_censored_array[i].x != tmp)
				{
					count++;
					tmp = time_censored_array[i].x;
				}
			}
		}
	}

	double* unique_uncensored_time = (double *) malloc(count * sizeof(double));

	count = 0;

	for (i = 0; i < size; i++)
	{
		if (time_censored_array[i].y < 0)
		{
			if (count == 0)
			{
				count++;
				unique_uncensored_time[count] = time_censored_array[i].x;
				tmp = time_censored_array[i].x;

			}

			if (count > 0)
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

	int* uncensored_num = (int *) malloc(count * sizeof(int));
	int* censored_num = (int *) malloc(count * sizeof(int));

	//record current time point
	time_at = unique_uncensored_time[0];
	count_at = 0;
	uncensored_num_at = 0;
	censored_num_at = 0;

	for (i = 0; i < size; i++)
	{
		if (time_censored_array[i].x <= time_at)
		{
			if (time_censored_array[i].y > 0)
				censored_num_at++;
			else
				uncensored_num_at++;

			//if the last sample is censored, follow block stores counting for last time unique uncensored period
			if (i == size - 1)
			{
				uncensored_num[count_at] = uncensored_num_at;
				censored_num[count_at] = censored_num_at;
			}

		} else {
			uncensored_num[count_at] = uncensored_num_at;
			censored_num[count_at] = censored_num_at;
			count_at++;

			//reset uncensored_num_at and censored_num_at
			uncensored_num_at = 0;
			censored_num_at = 0;

			//go to next time range
			time_at = unique_uncensored_time[count_at];

			if (time_censored_array[i].y > 0)
				censored_num_at++;
			else
				uncensored_num_at++;
		}
	}

	CENS_UC_NUM cens_ucens_number;
	cens_ucens_number.uncensored = uncensored_num;
	cens_ucens_number.censored = censored_num;
	cens_ucens_number.size = count;
	cens_ucens_number.time = unique_uncensored_time;

	free(time_censored_array);
	return cens_ucens_number;
}

/**
 * @brief calculate the kaplan meier
 * @param time Event time array
 * @param censored censored information: positive -> censored; zero or negative -> uncensored
 * @param size of the time array and censored array
 * @return CENS_UC_NUM structure
 */
int kaplan_meier(double* time, int* censored, int size, curve* KM_curve)
{

	int i, N;

	CENS_UC_NUM cens_ucens_number = censored_uncensred_each_time_range(time, censored, size);

	N = size;

	struct point* KM = (struct point*) malloc(cens_ucens_number.size * sizeof(struct point));

	for (i = 0; i < cens_ucens_number.size; i++)
	{
		if (i > 0)
		{
			N = (N - cens_ucens_number.uncensored[i - 1] - cens_ucens_number.censored[i - 1]);
			KM[i].x = cens_ucens_number.time[i];
			KM[i].y = 1.0 * (N - cens_ucens_number.uncensored[i] - cens_ucens_number.censored[i]) / (N - cens_ucens_number.censored[i]) * KM[i - 1].y;
		} else {
			KM[0].x = cens_ucens_number.time[i];
			KM[0].y = 1.0 * (N - cens_ucens_number.uncensored[0] - cens_ucens_number.censored[0]) / (N - cens_ucens_number.censored[0]);
		}
	}

	KM_curve->point_array = KM;
	KM_curve->size = cens_ucens_number.size;

	free(cens_ucens_number.uncensored);
	free(cens_ucens_number.censored);
	free(cens_ucens_number.time);

	return 0;
}

#endif