#ifndef _SSTAT_H_
#define _SSTAT_H_
#include <vector>

int loop_test(const int);
int percentile(double array[], const double target_percentile);
float sum(const std::vector<float>& values);
double calculate_sui_tax(const double amount, const double rate);
#endif