#ifndef _SSTAT_UTILITY_H_
#define _SSTAT_UTILITY_H_
#include <stdio.h>
#include <stdlib.h>

inline int cmp_func_double (const void * a, const void * b)
{
   return ( *(double*)a - *(double*)b );
}

void quick_sort_simultaneous (double *a, bool* b, int n) {
    int i, j;
    double p, t_a;
    bool t_b;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        //swap together
        t_a = a[i];
        a[i] = a[j];
        a[j] = t_a;

        t_b = b[i];
        b[i] = b[j];
        b[j] = t_b;
    }

    quick_sort(a, b, i);
    quick_sort(a + i, b + i, n - i);
}

#endif
