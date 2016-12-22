#ifndef _BASIC_MEDIAN_H_
#define _BASIC_MEDIAN_H_

#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Calculate the median for ASC sorted array 
 */
int median_for_sorted_data(const double* sorted_data, int n, double* median)
{
    const int lhs = (n - 1) / 2 ;
    const int rhs = n / 2 ;

    if(n == 0)
    {
        (*median) = 0;
        return 0;
    }

    if (lhs == rhs)
    {
        (*median) = sorted_data[lhs] ;
    }
    else 
    {
        (*median) = (sorted_data[lhs] + sorted_data[rhs]) / 2.0 ;
    }

    return 0;
}

/**
 * @brief Calculate the median for given array
 */
int median(const double* data, int n, double* median)
{
    int i;
    double* data_cpy = (double *) malloc(sizeof(double) * n);
    for(i = 0; i < n; i++)
    {
        data_cpy[i] = data[i];
    }

    qsort(data_cpy, n, sizeof(double), &compare_double);

    median_for_sorted_data(data_cpy, n, median);
    free(data_cpy);
    return 0;
}

#endif