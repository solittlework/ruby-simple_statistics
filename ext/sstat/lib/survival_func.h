#ifndef _SURVIVAL_STAT_FUNC_H_
#define _SURVIVAL_STAT_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include "survival_def.h"
#include "survival_utility.h"

int precentile_index(double* array, int size, double target_percentile)
{
	double percentage_each = 1.0 / size;
	int i;
	int count = 0;

	for (i = 1; i <= size; i++)
	{
		if ( i * percentage_each <= target_percentile )
		{
			count++;
		} else
		{
			return count;
		}
	}

	return size -1;
}

double precentile(double* array, int size, double target_percentile)
{
	int index;
	index = precentile_index(array, size, target_percentile);

	return array[index];
}

int index_less_equal(double* array, int size, double target)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (array[i] > target)
			return i;
	}

	return i;
}

double log_rank_test(double* time_1, int* censored_1, double* time_2, int* censored_2, int size_1, int size_2)
{
	int i, time_pnt_size;
	int index;

	array merged_time_pnts = merge_two_array(time_1, size_1, time_2, size_2);
	array merged_uniq_time_pnts = create_sorted_unique_array(merged_time_pnts.D_ptr, merged_time_pnts.size);

	//The lengths of Group_N_1 and Group_N_2 are not expected to be same. Step 1. create unique time array which inlcude time points for both
	CENS_UC_NUM Group_N_1 = group_N_given_range(time_1, censored_1, size_1, merged_uniq_time_pnts.D_ptr, merged_uniq_time_pnts.size);
	CENS_UC_NUM Group_N_2 = group_N_given_range(time_2, censored_2, size_2, merged_uniq_time_pnts.D_ptr, merged_uniq_time_pnts.size);

	double Z = 0;
	double V_i_sum = 0;

	CENS_UC_NUM combined_Group_N_1;
	CENS_UC_NUM combined_Group_N_2;

	combined_Group_N_1.uncensored = (int*) malloc(merged_uniq_time_pnts.size * sizeof(int));
	combined_Group_N_1.censored = (int*) malloc(merged_uniq_time_pnts.size * sizeof(int));

	combined_Group_N_2.uncensored = (int*) malloc(merged_uniq_time_pnts.size * sizeof(int));
	combined_Group_N_2.censored = (int*) malloc(merged_uniq_time_pnts.size * sizeof(int));

	//CAUTION: DO NOT SET TIME ARRAY HEERE, USE merged_uniq_time_pnts INSTEAD
	combined_Group_N_1.time = NULL;
	combined_Group_N_2.time = NULL;

	time_pnt_size = merged_uniq_time_pnts.size;

	int* N1_at_risk = (int*) malloc(time_pnt_size * sizeof(int));
	int* N2_at_risk = (int*) malloc(time_pnt_size * sizeof(int));
	int* N_i = (int*) malloc(time_pnt_size * sizeof(int));
	int* O_i = (int*) malloc(time_pnt_size * sizeof(int));

	double* E_i = (double*) malloc(time_pnt_size * sizeof(double));
	double* V_i = (double*) malloc(time_pnt_size * sizeof(double));

	//update at_risk for 1 and 2 based on combined_time_arr.
	for (i = 0; i < time_pnt_size; ++i)
	{
		index = find_first_index_has(Group_N_1.time, Group_N_1.size, merged_uniq_time_pnts.D_ptr[i]);
		if (index >= 0)
		{
			combined_Group_N_1.uncensored[i] = Group_N_1.uncensored[index];
			combined_Group_N_1.censored[i] = Group_N_1.censored[index];
		} else {

			combined_Group_N_1.uncensored[i] = 0;
			combined_Group_N_1.censored[i] = 0;
		}
	}

	for (i = 0; i < time_pnt_size; ++i)
	{
		index = find_first_index_has(Group_N_2.time, Group_N_2.size, merged_uniq_time_pnts.D_ptr[i]);

		if (index >= 0)
		{
			combined_Group_N_2.uncensored[i] = Group_N_2.uncensored[index];
			combined_Group_N_2.censored[i] = Group_N_2.censored[index];
		} else {
			combined_Group_N_2.uncensored[i] = 0;
			combined_Group_N_2.censored[i] = 0;
		}
	}

	N1_at_risk[0] =  size_1 - combined_Group_N_1.censored[0];
	N2_at_risk[0] =  size_2 - combined_Group_N_2.censored[0];

	for (i = 1; i < time_pnt_size; ++i)
	{
		N1_at_risk[i] = N1_at_risk[i - 1] - combined_Group_N_1.uncensored[i - 1] - combined_Group_N_1.censored[i];
		N2_at_risk[i] = N2_at_risk[i - 1] - combined_Group_N_2.uncensored[i - 1] - combined_Group_N_2.censored[i];

	}

	for (i = 0; i < time_pnt_size; ++i)
	{
		N_i[i] = N1_at_risk[i] + N2_at_risk[i];
		O_i[i] = (combined_Group_N_1.uncensored[i] + combined_Group_N_2.uncensored[i]);
		E_i[i] = 1.0 * N1_at_risk[i] * O_i[i] / N_i[i];

		if (N_i[i] > 1)
		{
			V_i[i] = 1.0 * O_i[i]
			         * (1.0 * N1_at_risk[i] / N_i[i])
			         * (1 - 1.0 * N1_at_risk[i] / N_i[i])
			         * (N_i[i] - O_i[i]) / (N_i[i] - 1);
			         
			V_i_sum += V_i[i];
		}

		Z += (combined_Group_N_1.uncensored[i] - E_i[i]);
	}

	Z = Z / sqrt(V_i_sum);
	free(Group_N_1.uncensored);
	free(Group_N_1.censored);
	free(Group_N_2.uncensored);
	free(Group_N_2.censored);
	free(merged_time_pnts.D_ptr);
	free(merged_uniq_time_pnts.D_ptr);
	free(N1_at_risk);
	free(N2_at_risk);
	free(N_i);
	free(O_i);
	free(E_i);
	free(V_i);

	return Z;
}

#endif