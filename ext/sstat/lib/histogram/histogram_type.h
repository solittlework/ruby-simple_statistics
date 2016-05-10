#ifndef _HISTOGRAM_TYPE_H
#define _HISTOGRAM_TYPE_H
/*
 * The contain of this file is modified based on GSL lib
 * */
typedef double f_T;

typedef struct {
    size_t n ;
    double * range ;
    double * bin ;
} histogram;

#endif
