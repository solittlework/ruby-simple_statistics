#ifndef _SSTAT_H_
#define _SSTAT_H_
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/survival.h"
#include "lib/global_utility.h"
#include "lib/distribution.h"

static VALUE rb_percentile(VALUE self, VALUE arg, VALUE target);
static VALUE rb_percentile_index(VALUE self, VALUE arg, VALUE target);
static VALUE rb_index_less_equal(VALUE self, VALUE arg, VALUE target);
static VALUE rb_kaplan_meier(VALUE self, VALUE time, VALUE censor);
static VALUE rb_log_rank_test(VALUE self, VALUE time_1, VALUE censor_1,VALUE time_2, VALUE censor_2);
static VALUE rb_cdf_unormal_Q(VALUE self, VALUE x);

//rb_ary_storde(unmasked, i, INT2NUM(p ^ m));
void Init_sstat() {
	VALUE sstat_module = rb_define_module("SStat");
	VALUE surv_class = rb_define_class_under(sstat_module, "Surv", rb_cObject);
	VALUE dist_class = rb_define_class_under(sstat_module, "Dist", rb_cObject);
	rb_define_method(surv_class, "percentile_index", rb_percentile_index, 2);
	rb_define_method(surv_class, "percentile", rb_percentile, 2);
	rb_define_method(surv_class, "index_less_equal", rb_index_less_equal, 2);
	rb_define_method(surv_class, "kaplan_meier", rb_kaplan_meier, 2);
	rb_define_method(surv_class, "log_rank_test", rb_log_rank_test, 4);
	rb_define_method(dist_class, "cdf_unormal_Q", rb_cdf_unormal_Q, 1);

}

#endif