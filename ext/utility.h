#ifndef _SSTAT_UTILITY_H_
#define _SSTAT_UTILITY_H_

#include <stdio.h>
#include <stdlib.h>
#include "data_type.h"

int compare_double (const void * a, const void * b)
{
   return *((double*)a) > *((double*)b)? 1 : 0;
}

int point_compare_x (const void * a, const void * b)
{
	return ((struct point*)a)->x - ((struct point*)b)->x;
}

struct array create_sorted_unique_array(double* array, int size)
{
	struct array arr;
	int i;
	//To speed up, choose to do not memset arr.
	arr.D_ptr = NULL;
	int count;

	qsort(array, size, sizeof(double), &compare_double);

	count = 1;
	//calcualte number of unique
	for(i = 1; i < size; ++i)
	{
		if(array[i] != array[i-1])
		{
			count++;
		}
	}

	double * unique_arr = (double *) malloc(count * sizeof(double));

	//assign unique elements
	count = 1;
	unique_arr[0] = array[0];
	for(i = 1; i < size; ++i)
	{
		if(array[i] != array[i-1])
		{
			unique_arr[count] = array[i];
			count++;
		}
	}

	arr.D_ptr = unique_arr;
	arr.size = count;

	return arr;
}

array merge_two_array(double* array_1, int size_1, double* array_2, int size_2)
{
	int i;
	struct array arr;

	//To speed up, choose to do not memset arr.
	arr.D_ptr = NULL;

	int total_size = size_1 + size_2;
	double * merged_array = (double *) malloc(total_size * sizeof(double));
	for(i = 0; i < size_1; i++)
	{
		merged_array[i] = array_1[i];
	}

	for( i = 0; i < size_2; i++)
	{
		merged_array[i + size_1] = array_2[i];
	}

	arr.D_ptr = merged_array;
	arr.size = total_size;

	return arr;
}

int find_first_index_has(double* arr, int size, double value)
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(fabs(arr[i] - value) < EPS)
		{
			return i;
		}
	}

	//no value in the array
	return -1;
}

#endif