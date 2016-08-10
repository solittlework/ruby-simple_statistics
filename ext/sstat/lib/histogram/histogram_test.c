#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"

int main(int argc, char const *argv[])
{
    int n = 5;
    int i = 0;
    double hmean = 0;
    struct histogram* h = (struct histogram*)malloc( sizeof(struct histogram));

    h->range = malloc(sizeof(double) * (n + 1));
    h->bin = malloc(sizeof(double) * (n));

    for(i = 0; i < (n+1); i++)
        h->range[i] = i;

    h->bin[0] = 2;
    h->bin[1] = 3;
    h->bin[2] = 5;
    h->bin[3] = 2;
    h->bin[4] = 1;

    h->n = n;
    histogram_mean(h, &hmean);
    printf("histogram mean %f \n", hmean);
    free_histogram(h);
    return 0;
}