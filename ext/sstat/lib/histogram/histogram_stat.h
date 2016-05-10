#ifndef _HISTOGRAM_MEAN_H
#define _HISTOGRAM_MEAN_H
#include "histogram_type.h"

/*
*
*/
int histogram_mean(const histogram* h, f_T* res)
{
	int i, n;
	f_T xi, wi;
	f_T hmean = 0;
	f_T W = 0;
	n = h->n;

	for(i = 0; i < n; i++)
	{
		//make sure size of h->range is n + 1
		xi = (h->range[i+1] + h->range[i]) / 2;
		wi = h->bin[i];
		if(wi > 0)
		{
			W += wi;
			hmean += (hmean - xi) * wi / W;
		}
	}

	(*res) = hmean;
	return 0;
}

int histogram_bin_sum(const histogram* h, f_T* res)
{
	size_t i, n;
	f_T sum = 0;
	n = h->n;
	for(i = 0; i < n; i++)
	{
		sum += h->bin[i];
	}

	(*res) = sum;
	return 0;
}

int histogram_median(const histogram* h, f_T* res)
{
	size_t i, n;
	f_T sum, sum_50;
	int proc_flag = histogram_bin_sum(h, &sum);

	n = h->n;

	if(proc_flag != 0)
		return -1; //unexpected error

	sum_50 = sum / 2.0;
	sum = 0;
	for(i = 0; i < n; i++)
	{
		sum += h->bin[i];

		if(sum >= sum_50)
		{
			(*res) = (h->range[i] + h->range[i+1]) / 2;
			break;
		}
	}

	return 0;
}

#endif
