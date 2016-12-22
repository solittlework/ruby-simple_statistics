#ifndef _SURVIVAL_STAT_KAPLAN_MEIER_H_
#define _SURVIVAL_STAT_KAPLAN_MEIER_H_

#include "survival_def.h"
#include "global_utility.h"
#include <math.h>
#include <stdio.h>
/**
 * @brief calculate the number of samples censored or uncenosored (die) at each time ragne
 * @param time Event time array
 * @param censored censored information: positive -> censored; zero or negative -> uncensored
 * @return CENS_UC_NUM structure
 */
 #define EXP 1e-12

double max_time_point(double* time, int size) {
	double max = 0;
	for(int i = 0; i < size; i++) {
		if(time[i] > max)
			max = time[i];
	}

	return max;
}

int censored_uncensred_each_time_range(double* time, int* censored, int size,  struct CENS_UC_NUM** cens_ucens_number)
{
	int i, count_at, uncensored_num_at, censored_num_at;
	double tmp, time_at;


    /* Sort time and censored based on time together, time can censored array */
	struct point* time_censored_array = alloc_points(size);

    /* Sample i is censored, if censored[i] > 0 */
	for (i = 0; i < size; i++)
	{
		time_censored_array[i].x = time[i];
		if (censored[i] > 0)
			time_censored_array[i].y = 1;
		else
			time_censored_array[i].y = -1;
	}

	qsort(time_censored_array, size, sizeof(struct point), &point_compare_x);

	/* count number of unique uncensored time point */
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

	alloc_CENS_UC_NUM(cens_ucens_number, count);

	count = 0;

	for (i = 0; i < size; i++)
	{
		if (time_censored_array[i].y < 0)
		{
			if (count == 0)
			{
				(*cens_ucens_number)->time[count] = time_censored_array[i].x;
				tmp = time_censored_array[i].x;
				count++;
			}

			if (count > 0)
			{
				if (time_censored_array[i].x != tmp)
				{
					(*cens_ucens_number)->time[count] = time_censored_array[i].x;
					tmp = time_censored_array[i].x;
					count++;
				}
			}
		}
	}

    /* Store a copy of latest event time points */
	time_at = (*cens_ucens_number)->time[0];
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

			/* If the last sample is censored, follow block stores counting for last time unique uncensored period */
			if (i == size - 1)
			{
				(*cens_ucens_number)->uncensored[count_at] = uncensored_num_at;
				(*cens_ucens_number)->censored[count_at] = censored_num_at;
			}

		} else {

			if(count_at < (*cens_ucens_number)-> size)
			{
				(*cens_ucens_number)->uncensored[count_at] = uncensored_num_at;
				(*cens_ucens_number)->censored[count_at] = censored_num_at;
			}

			count_at++;

            /* Reset uncensored_num_at and censored_num_at */
			uncensored_num_at = 0;
			censored_num_at = 0;

            /* Jump to next time range */
			if(count_at < (*cens_ucens_number)-> size)
				time_at = (*cens_ucens_number)->time[count_at];

			if (time_censored_array[i].y > 0)
				censored_num_at++;
			else
				uncensored_num_at++;

			/* If the last sample is censored, follow block stores counting for last time unique uncensored period */
			if ( (i == size - 1) && (count_at < (*cens_ucens_number)-> size))
			{
				(*cens_ucens_number)->uncensored[count_at] = uncensored_num_at;
				(*cens_ucens_number)->censored[count_at] = censored_num_at;
			}
		}
	}

	if(time_censored_array != NULL)
		free(time_censored_array);

	return 0;
error:
	return OUTOF_MEMORY_ERROR;
}

void calculate_kaplan_meier(int size, const struct CENS_UC_NUM* cens_ucens_number, struct point** KM)
{
	int i;
	int N = size; //total sample number

	for (i = 0; i < cens_ucens_number->size; i++)
	{
		if (i > 0)
		{
			N = (N - cens_ucens_number->uncensored[i - 1] 
				- cens_ucens_number->censored[i - 1]);

			(*KM)[i].x = cens_ucens_number->time[i];
			(*KM)[i].y = 1.0 * (N - cens_ucens_number->uncensored[i] - cens_ucens_number->censored[i]) / (N - cens_ucens_number->censored[i]) * (*KM)[i - 1].y;
		} else {
			(*KM)[0].x = cens_ucens_number->time[i];
			(*KM)[0].y = 1.0 * (N - cens_ucens_number->uncensored[0] - cens_ucens_number->censored[0]) / (N - cens_ucens_number->censored[0]);
		}
	}
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
	struct CENS_UC_NUM* cens_ucens_number;

	censored_uncensred_each_time_range(time, censored, size, &cens_ucens_number);
	
	struct point* KM =  alloc_points(size);
	calculate_kaplan_meier(size, cens_ucens_number, &KM);
	KM_curve->point_array = KM;
	KM_curve->size = cens_ucens_number->size;
	return 0;
}

/**
 * @brief extend the KM curve based on the last 3 points
 */
int KM_3p_extrapolation(struct CENS_UC_NUM** cens_uc_num, struct CENS_UC_NUM ** updated_cens_uc_num, int sample_size, double max_time_point)
{
	int mean_last_uncensored = 0;
	int mean_last_censored = 0;
	double time_interval_mean = 0;
	
	int num_left = 0;
	int num_left_copy = 0;

	/*Track break point during pre-mapping */
	int is_break_at_uncensored = 0;

	/*Track break point during pre-mapping */
	int is_break_at_censored = 0;

	int used_sample_num = 0;
	int extrapolation_size = 0;
	int updated_cens_uc_num_size = 0;
	int i;
	int last_index = (*cens_uc_num)->size - 1;
	int last_1_index = (*cens_uc_num)->size - 2;
	int last_4_index = (*cens_uc_num)->size - 5;
	int actual_updated_size = 0;
	int count = 0;
	int extra = 0;

	/* calculate the total number (censored and uncensored) already used */
	for(i = 0; i < (*cens_uc_num)->size; i ++)
	{
		used_sample_num += (*cens_uc_num)->censored[i];
		used_sample_num += (*cens_uc_num)->uncensored[i];
	}

	/* TODO should add error check here */
	num_left = sample_size - used_sample_num;

	/*Calculate the mean for uncensored, censored and time interval */
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
		mean_last_uncensored += (*cens_uc_num)->uncensored[(*cens_uc_num)->size - 2 - i];
		mean_last_censored += (*cens_uc_num)->censored[(*cens_uc_num)->size - 2 - i];
	}

	time_interval_mean = (*cens_uc_num)->time[last_1_index] - (*cens_uc_num)->time[last_4_index];
	mean_last_uncensored = (int)floor(mean_last_uncensored / 3);
	mean_last_censored = (int)floor(mean_last_censored / 3);

	time_interval_mean = time_interval_mean / 3;
	
	/* Calculate how many points we should extrapolate */
	extrapolation_size = ceil((double)num_left / (mean_last_uncensored + mean_last_censored));

	updated_cens_uc_num_size = (*cens_uc_num)->size + extrapolation_size;

	/*If both mean_last_uncensored and mean_last_censored are zero return copy cens_uc_num to updated_cens_uc_num */
	if(mean_last_uncensored < EXP && mean_last_censored < EXP) {
		check(alloc_CENS_UC_NUM(updated_cens_uc_num, (*cens_uc_num)->size) == 0, "Failed in allocating CENS_UC_NUM structure");
		/* Create a copy for the original cens_uc_num struct */
		for(i = 0; i < (*cens_uc_num)->size; i++)
		{
			(*updated_cens_uc_num)->censored[i] = (*cens_uc_num)->censored[i];
			(*updated_cens_uc_num)->uncensored[i] = (*cens_uc_num)->uncensored[i];
			(*updated_cens_uc_num)->time[i] = (*cens_uc_num)->time[i];
		}
		return 0;
	}

	num_left_copy = num_left;


	/* Pre-mapping to find the actual extrapolation number */
	while(num_left_copy >= 0) {
		num_left_copy = num_left_copy - mean_last_uncensored;
		count++;
		if(num_left_copy >= 0) {
			is_break_at_uncensored = 0;
			extra = num_left_copy;
		} else {
			is_break_at_uncensored = 1;
			break;
		}

		num_left_copy = num_left_copy - mean_last_censored;

		if(num_left_copy > 0) {
			is_break_at_censored = 0;
			extra = num_left_copy;
		} else {
			is_break_at_censored = 1;
			break;
		}

		if(num_left_copy == 0) {
			is_break_at_uncensored = 0;
			is_break_at_censored = 0;
			break;
		}
	}

	actual_updated_size = (*cens_uc_num)->size + count;

	check(alloc_CENS_UC_NUM(updated_cens_uc_num, actual_updated_size) == 0, "Failed in allocating CENS_UC_NUM structure");
	
	/* Create a copy for the original cens_uc_num struct */
	for(i = 0; i < (*cens_uc_num)->size; i++)
	{
		(*updated_cens_uc_num)->censored[i] = (*cens_uc_num)->censored[i];
		(*updated_cens_uc_num)->uncensored[i] = (*cens_uc_num)->uncensored[i];
		(*updated_cens_uc_num)->time[i] = (*cens_uc_num)->time[i];
	}

	time_interval_mean = (max_time_point - (*cens_uc_num)->time[last_index]) / count;
	printf("Time interval: %f \n", time_interval_mean);

	for(i = (*cens_uc_num)->size; i < actual_updated_size; i++) {
		(*updated_cens_uc_num)->time[i] = (*updated_cens_uc_num)->time[i-1] + time_interval_mean;
		if(i == actual_updated_size -1) {

			if(is_break_at_uncensored) {
				(*updated_cens_uc_num)->uncensored[i] = extra;
				(*updated_cens_uc_num)->censored[i] = 0;
			} else if(is_break_at_censored) {
				(*updated_cens_uc_num)->uncensored[i] = mean_last_uncensored;
				(*updated_cens_uc_num)->censored[i] = extra;
			} else {
				(*updated_cens_uc_num)->uncensored[i] = mean_last_censored;
				(*updated_cens_uc_num)->censored[i] = mean_last_uncensored;
			}

		} else {
			(*updated_cens_uc_num)->uncensored[i] = mean_last_censored;
			(*updated_cens_uc_num)->censored[i] =  mean_last_uncensored;
		}
	}

	return 0;

error:
	free_CENS_UC_NUM(updated_cens_uc_num);
	return 1;
}

int kaplan_meier_3p_extrapolation(double* time, int* censored, int size, struct curve* KM_curve)
{
	int proc_state = 0;
	struct CENS_UC_NUM* cens_ucens_number;
	double max_time = max_time_point(time, size);
	proc_state = censored_uncensred_each_time_range(time, censored, size, &cens_ucens_number);
	struct point* KM =  alloc_points(size);

	/* If the length of the inital KM curve is less than or equal to 7, we will not apply extrapolation */
	if(cens_ucens_number->size > 6)
	{
		struct CENS_UC_NUM* updated_cens_ucens_number;
		proc_state = KM_3p_extrapolation(&cens_ucens_number, &updated_cens_ucens_number, size, max_time);
		calculate_kaplan_meier(size, updated_cens_ucens_number, &KM);
		KM_curve->point_array = KM;
		KM_curve->size = updated_cens_ucens_number->size;
		KM_curve->point_array = KM;
		free_CENS_UC_NUM(&updated_cens_ucens_number);
	} else {
		calculate_kaplan_meier(size, cens_ucens_number, &KM);
		KM_curve->point_array = KM;
		KM_curve->size = cens_ucens_number->size;
		KM_curve->point_array = KM;
	}

	free_CENS_UC_NUM(&cens_ucens_number);

	return proc_state;
}

#endif