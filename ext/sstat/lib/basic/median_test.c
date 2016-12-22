#include "median.h"

/* Memeory test for median */
int main(int argc, char const *argv[])
{
    double data[14] = { 1.0, 2.1, 3.2, 4.5, 5.5, 6.2, 7.4, 8.5, 9.1, 10.4, 11.3, 12.5, 13.2, 14.7 };
    double m;
    int proc = median(data, 14, &m);
    printf("Median %f \n", m);

    return 0;
}