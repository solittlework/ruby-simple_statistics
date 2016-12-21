#ifndef _HISTOGRAM_MEAN_H
#define _HISTOGRAM_MEAN_H
#include "histogram_type.h"

/* The functions here come from GSL libaray */

/*
* Mean of a histogram
* Compute the bin-weighted arithmetic mean M of a histogram using the recurrence relation
* M(n) = M(n-1) + (x[n] - M(n-1)) (w(n)/(W(n-1) + w(n))) 
* W(n) = W(n-1) + w(n)
*/
int histogram_mean(const struct histogram* h, double* hmean)
{
    int i, n;
    double xi, wi;
    (*hmean) = 0;
    double W = 0;
    n = h->n;

    for(i = 0; i < n; i++)
    {
        //make sure size of h->range is n + 1

        xi = (h->range[i+1] + h->range[i]) / 2;
        wi = h->bin[i];
        if(wi > 0)
        {
            W += wi;
            (*hmean) += (xi - (*hmean)) * wi / W;
        }
    }

    return 0;
}

int histogram_bin_sum(const struct histogram* h, double* sum)
{
    size_t i, n;
    (*sum) = 0;
    n = h->n;
    for(i = 0; i < n; i++)
    {
        (*sum) += h->bin[i];
    }

    return 0;
}

int histogram_median(const struct histogram* h, double* hmedian)
{
    size_t i, n;
    double sum, sum_50;
    int proc_flag = histogram_bin_sum(h, &sum);

    n = h->n;

    if(proc_flag != 0)
        return -1;

    sum_50 = sum / 2.0;
    sum = 0;
    for(i = 0; i < n; i++)
    {
        sum += h->bin[i];

        if(sum >= sum_50)
        {
            (*hmedian) = (h->range[i] + h->range[i+1]) / 2;
            break;
        }
    }

    return 0;
}

#endif
