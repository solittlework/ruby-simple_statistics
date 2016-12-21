#ifndef _HISTOGRAM_TYPE_H
#define _HISTOGRAM_TYPE_H

/* The functions here come from GSL libaray */
struct histogram {
    size_t n ;
    double * range ; /* size of range should be size of bin + 1 */
    double * bin ;
};

void free_histogram(struct histogram* h)
{
    if(h)
    {
        if(h->range)
            free(h->range);

        if(h->bin)
            free(h->bin);

        free(h);
    }
}

#endif
