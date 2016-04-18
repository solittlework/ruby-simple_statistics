#include "sstat.hpp"

double calculate_sui_tax(const double amount, const double rate) {
  double tax = amount * rate;
  return tax;
}
float sum(const std::vector<float>& values){
  return values.size() * 1.0;
}

int loop_test(const int loop_num)
{
  int i;
  int j;
  int count = 0;
  for( i = 0; i < loop_num; i++ )
  {
    for( j = 0; j < loop_num; j++ )
    {
      count++;
    }
  }

  return count;
}

int percentile(double array[], const double target_percentile)
{
  int array_length = sizeof(array) /sizeof(double);
  return array_length;
  /*
  double percentage = 1.0 / array_length;
  int i = 0;
  double updated_percentile = 0;
  for (i = 0; i < array_length; ++i)
  {
    updated_percentile = i * percentage;
    if (updated_percentile > target_percentile)
    {
      if(i > 0)
        return array[i -1];
      else
        return array[0];
    }
  }

  return array[array_length-1];*/
}