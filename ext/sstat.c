#include "sstat.h"

static VALUE rb_percentile(VALUE self, VALUE array, VALUE target)
{
  int size = RARRAY_LEN(array);
  int i;
  double _target = NUM2DBL(target);
  double _percentile;
  double* c_array = (double *)malloc(sizeof(double)*size);

  for (i = 0; i < size; i++) {
    c_array[i] = NUM2DBL(rb_ary_entry(array, i));
  }
  _percentile = precentile(c_array, size, _target);
  free(c_array);

  return DBL2NUM(_percentile);
}

static VALUE rb_index_less_equal(VALUE self, VALUE array, VALUE target)
{
  int size = RARRAY_LEN(array);
  int i;
  int _index;
  double _target = NUM2DBL(target);
  double* c_array = (double *)malloc(sizeof(double)*size);

  for (i = 0; i < size; i++) {
    c_array[i] = NUM2DBL(rb_ary_entry(array, i));
  }

  _index = index_less_equal(c_array, size, _target);

  free(c_array);

  return INT2NUM(_index);
}