#include "sstat.h"

static VALUE rb_percentile(VALUE self, VALUE array, VALUE target)
{
  int size = RARRAY_LEN(array);
  int i;
  double _target = NUM2DBL(target);
  double _percentile;
  double* c_array = (double *)malloc(sizeof(double) * size);

  for (i = 0; i < size; i++) {
    c_array[i] = NUM2DBL(rb_ary_entry(array, i));
  }
  _percentile = precentile(c_array, size, _target);
  free(c_array);

  return DBL2NUM(_percentile);
}

static VALUE rb_percentile_index(VALUE self, VALUE array, VALUE target)
{
  int size = RARRAY_LEN(array);
  int i;
  double _target = NUM2DBL(target);
  int _percentile_index;
  double* c_array = (double *)malloc(sizeof(double) * size);

  for (i = 0; i < size; i++) {
    c_array[i] = NUM2DBL(rb_ary_entry(array, i));
  }
  _percentile_index = precentile_index(c_array, size, _target);
  free(c_array);

  return INT2NUM(_percentile_index);
}

static VALUE rb_index_less_equal(VALUE self, VALUE array, VALUE target)
{
  int size = RARRAY_LEN(array);
  int i;
  int _index;
  double _target = NUM2DBL(target);
  double* c_array = (double *)malloc(sizeof(double) * size);
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
  double* _time = (double *)malloc(sizeof(double) * size);
  int* _censored = (int *)malloc(sizeof(int) * size);
  for (i = 0; i < size; i++) {
    _time[i] = NUM2DBL(rb_ary_entry(time, i));

    _censored[i] = NUM2INT(rb_ary_entry(censored, i));
  }
  KM_curve = kaplan_meier(_time, _censored, size);

  VALUE KM_x = rb_ary_new2(KM_curve.size);
  VALUE KM_y = rb_ary_new2(KM_curve.size);

  for (i = 0; i < KM_curve.size; i++)
  {
    rb_ary_store(KM_x, i, DBL2NUM(KM_curve.point_array[i].x));
    rb_ary_store(KM_y, i, DBL2NUM(KM_curve.point_array[i].y));
  }
  result = rb_hash_new();
  rb_hash_aset(result, rb_str_new2("time"), KM_x);
  rb_hash_aset(result, rb_str_new2("prob"), KM_y);

  free(_time);
  free(_censored);
  free(KM_curve.point_array);
  return result;
}

static VALUE rb_log_rank_test(VALUE self, VALUE _time_1, VALUE _cens_1, VALUE _time_2, VALUE _cens_2)
{
  int size_1 = RARRAY_LEN(_time_1);
  int size_2 = RARRAY_LEN(_time_2);

  int i;
  double _Z;

  double* time_1 = (double *)malloc(sizeof(double) * size_1);
  double* time_2 = (double *)malloc(sizeof(double) * size_2);
  int* cens_1 = (int *)malloc(sizeof(int) * size_1);
  int* cens_2 = (int *)malloc(sizeof(int) * size_2);

  for (i = 0; i < size_1; i++) {
    time_1[i] = NUM2DBL(rb_ary_entry(_time_1, i));
    cens_1[i] = NUM2INT(rb_ary_entry(_cens_1, i));
  }

  for (i = 0; i < size_2; i++) {
    time_2[i] = NUM2DBL(rb_ary_entry(_time_2, i));
    cens_2[i] = NUM2INT(rb_ary_entry(_cens_2, i));
  }
  _Z = log_rank_test(time_1, cens_1, time_2, cens_2, size_1, size_2);

  free(time_1);
  free(time_2);
  free(cens_1);
  free(cens_2);

  return DBL2NUM(_Z);
}

static VALUE rb_cdf_unormal_Q(VALUE self, VALUE x)
{
  double Q = cdf_ugaussian_Q(NUM2DBL(x));
  return DBL2NUM(Q);
}