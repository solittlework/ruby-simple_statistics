%module sstat
%{
  #include "sstat.hpp"
%}

%include std_vector.i
namespace std
{
  %template(CppFloatVector) vector<float>;
  %template(CppDoubleVector) vector<double>;
};

extern int loop_test(const int loop_num);
extern float sum(const std::vector<float>& values);

%typemap(in) (double array[]) {
  $1 = (double *)malloc(10 * sizeof(double));
};

%typemap(freearg) (double array[]) {
  free($1);
}

int percentile(double array[], const double target_percentile);

%inline%{
  std::vector<float> CppVectorF;
%}