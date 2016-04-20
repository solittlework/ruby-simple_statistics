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

static VALUE rb_kaplan_meier(VALUE self, VALUE time, VALUE censored)
{
  int size = RARRAY_LEN(time);
  int i;
  struct curve KM_curve;
  VALUE result;
  double* _time = (double *)malloc(sizeof(double)*size);
  int* _censored = (int *)malloc(sizeof(int)*size);
  for (i = 0; i < size; i++) {
    _time[i] = NUM2DBL(rb_ary_entry(time, i));

    _censored[i] = NUM2INT(rb_ary_entry(censored, i));
  }
  KM_curve = kaplan_meier(_time, _censored, size);

  VALUE KM_x = rb_ary_new2(KM_curve.size);
  VALUE KM_y = rb_ary_new2(KM_curve.size);

  for(i = 0; i< KM_curve.size; i++)
  {
    rb_ary_store(KM_x, i, DBL2NUM(KM_curve.point_arr[i].x));
    rb_ary_store(KM_y, i, DBL2NUM(KM_curve.point_arr[i].y));
  }
  result = rb_hash_new();
  rb_hash_aset(result, rb_str_new2("time"), KM_x);
  rb_hash_aset(result, rb_str_new2("prob"), KM_y);

  free(_time);
  free(_censored);
  free(KM_curve.point_arr);
  return result;
}