#ifndef _SURVIVAL_STAT_KAPLAN_MEIER_H_
#define _SURVIVAL_STAT_KAPLAN_MEIER_H_

#include "survival_def.h"
#include <math.h>

/**
 * @brief calculate the number of samples censored or uncenosored (die) at each time ragne
 * @param time Event time array
 * @param censored censored information: positive -> censored; zero or negative -> uncensored
 * @return CENS_UC_NUM structure
 */
int censored_uncensred_each_time_range(double* time, int* censored, int size,  struct CENS_UC_NUM** cens_ucens_number)
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
				unique_uncensored_time[count] = time_censored_array[i].x;
				tmp = time_censored_array[i].x;
				count++;
			}

			if (count > 0)
			{
				if (time_censored_array[i].x != tmp)
				{
					unique_uncensored_time[count] = time_censored_array[i].x;
					tmp = time_censored_array[i].x;
					count++;
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

		if (time_censored_array[i].x <= time_at + 1e-5)
		{

			if (time_censored_array[i].y > 0)
				censored_num_at++;
			else
				uncensored_num_at++;

			//if the last sample is censored, follow block stores counting for last time unique uncensored period
			if (i == size - 1)
			{
				count_at++;
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

			/* If the last sample is censored, follow block stores counting for last time unique uncensored period */
			if (i == size - 1)
			{
				uncensored_num[count_at] = uncensored_num_at;
				censored_num[count_at] = censored_num_at;
			}
		}
	}

	alloc_CENS_UC_NUM(cens_ucens_number, count);

	for(i = 0; i < count; i++)
	{
		(*cens_ucens_number)->uncensored[i] = uncensored_num[i];
		(*cens_ucens_number)->censored[i] = censored_num[i];
		(*cens_ucens_number)->time[i] = unique_uncensored_time[i];
	}

	free(time_censored_array);
	free(uncensored_num);
	free(censored_num);
	free(unique_uncensored_time);

	return 0;
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
	struct CENS_UC_NUM* cens_ucens_number;

	censored_uncensred_each_time_range(time, censored, size, &cens_ucens_number);
	
	N = size; //total sample number

	struct point* KM =  alloc_points(size);

	for (i = 0; i < cens_ucens_number->size; i++)
	{
		if (i > 0)
		{
			N = (N - cens_ucens_number->uncensored[i - 1] 
				- cens_ucens_number->censored[i - 1]);

			KM[i].x = cens_ucens_number->time[i];
			KM[i].y = 1.0 * (N - cens_ucens_number->uncensored[i] - cens_ucens_number->censored[i]) / (N - cens_ucens_number->censored[i]) * KM[i - 1].y;
		} else {
			KM[0].x = cens_ucens_number->time[i];
			KM[0].y = 1.0 * (N - cens_ucens_number->uncensored[0] - cens_ucens_number->censored[0]) / (N - cens_ucens_number->censored[0]);
		}
	}

	KM_curve->point_array = KM;
	KM_curve->size = cens_ucens_number->size;
	//free_CENS_UC_NUM(&cens_ucens_number);
	return 0;
}

/**
 * @brief extend the KM curve based on the last 3 points
 */
int KM_3p_extrapolation(struct CENS_UC_NUM* cens_uc_num, struct CENS_UC_NUM** updated_cens_uc_num, int sample_size)
{
	double mean_last_uncensored = 0;
	double mean_last_censored = 0;
	double time_interval_mean = 0;
	int num_left = 0;
	int used_sample_num = 0;
	int extrapolation_size = 0;
	int updated_cens_uc_num_size = 0;
	int i;

	/* calculate the total number (censored and uncensored) already used */
	for(i = 0; i < cens_uc_num->size; i ++)
	{
		used_sample_num += cens_uc_num->censored[i];
		used_sample_num += cens_uc_num->uncensored[i];
	}

	/* TODO should error check here */
	num_left = sample_size - used_sample_num;

	for(i = 0; i < 3; i++)
	{
		/*
		* censored uncensored
		*	x_1		y_1	(should be included)
		*	x_2		y_2	(should be included)
		*	x_3		y_3	(should be included)
		*	x_last	y_last (not used)
		*	why? when we calculate the last 3 time intervals, we need 4 points
		*/
		mean_last_uncensored += cens_uc_num->uncensored[cens_uc_num->size - 2 - i];
		mean_last_censored += cens_uc_num->censored[cens_uc_num->size - 2 - i];
	}

	time_interval_mean = cens_uc_num->time[cens_uc_num->size - 2] - cens_uc_num->time[cens_uc_num->size - 5];

	mean_last_uncensored = mean_last_uncensored / 3;
	mean_last_censored = mean_last_censored / 3;
	time_interval_mean = time_interval_mean / 3;

	/* Calculate how many points we should extrapolate */
	extrapolation_size = ceil((double)num_left / (mean_last_uncensored + mean_last_censored));
	updated_cens_uc_num_size = cens_uc_num->size + extrapolation_size;

	check(alloc_CENS_UC_NUM(updated_cens_uc_num, updated_cens_uc_num_size) == 0, "Failed in allocating CENS_UC_NUM structure");

	for(i = 0; i < cens_uc_num->size; i++)
	{
		(*updated_cens_uc_num)->censored[i] = cens_uc_num->censored[i];
		(*updated_cens_uc_num)->uncensored[i] = cens_uc_num->uncensored[i];
		(*updated_cens_uc_num)->time[i] = cens_uc_num->time[i];
	}

	for(i = cens_uc_num->size; i < (cens_uc_num->size + extrapolation_size); i++)
	{
		(*updated_cens_uc_num)->time[i] = (*updated_cens_uc_num)->time[i-1] + time_interval_mean;

		if (mean_last_uncensored<num_left)
			(*updated_cens_uc_num)->uncensored[i] = mean_last_uncensored;
		else
			(*updated_cens_uc_num)->uncensored[i] = num_left;

		/* We update the num_left to make sure that the total number of samples in the extrapolation group cannot be larger than the total number of samples */
		num_left = num_left - mean_last_uncensored;

		if (mean_last_censored<num_left)
			(*updated_cens_uc_num)->censored[i] =  mean_last_censored;
		else
			(*updated_cens_uc_num)->censored[i] =  num_left;

		num_left = num_left - mean_last_censored;
	}

	return 0;

error:
	free_CENS_UC_NUM((*updated_cens_uc_num));
	return 1;
}

int kaplan_meier_3p_extrapolation(double* time, int* censored, int size, struct curve* KM_curve)
{
	int proc_state = 0;
	int i;
	struct CENS_UC_NUM* cens_ucens_number = NULL;
	censored_uncensred_each_time_range(time, censored, size, &cens_ucens_number);
	struct CENS_UC_NUM* updated_cens_ucens_number = NULL;

	proc_state = KM_3p_extrapolation(cens_ucens_number, &updated_cens_ucens_number, size);
	int N = size;

	struct point* KM =  alloc_points(size);

	for (i = 0; i < updated_cens_ucens_number->size; i++)
	{
		if (i > 0)
		{
			N = (N - updated_cens_ucens_number->uncensored[i - 1] 
				- updated_cens_ucens_number->censored[i - 1]);

			KM[i].x = updated_cens_ucens_number->time[i];
			KM[i].y = 1.0 * (N - updated_cens_ucens_number->uncensored[i] - updated_cens_ucens_number->censored[i]) / (N - updated_cens_ucens_number->censored[i]) * KM[i - 1].y;
		} else {
			KM[0].x = updated_cens_ucens_number->time[i];
			KM[0].y = 1.0 * (N - updated_cens_ucens_number->uncensored[0] - updated_cens_ucens_number->censored[0]) / (N - updated_cens_ucens_number->censored[0]);
		}
	}

	KM_curve->point_array = KM;
	KM_curve->size = updated_cens_ucens_number->size;
	//print_curve(KM_curve);

	free_CENS_UC_NUM(cens_ucens_number);
	free_CENS_UC_NUM(updated_cens_ucens_number);
	return 0;
}
#endif