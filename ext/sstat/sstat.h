#ifndef _SSTAT_H_
#define _SSTAT_H_

/* Include ruby macron */
#include <ruby.h>

/* Include C STL */
#include <stdio.h>
#include <stdlib.h>

/* Include simple statiscs files */
#include "lib/survival/survival.h"
#include "lib/survival/distribution.h"
#include "lib/global_utility.h"
#include "lib/histogram/histogram.h"

/* Define C to Ruby bridge functions */
static VALUE rb_percentile(VALUE self, VALUE arg, VALUE target);
static VALUE rb_percentile_index(VALUE self, VALUE arg, VALUE target);
static VALUE rb_index_less_equal(VALUE self, VALUE arg, VALUE target);
static VALUE rb_kaplan_meier(VALUE self, VALUE time, VALUE censor);

static VALUE rb_kaplan_meier_3p_extraploation(VALUE self, VALUE time, VALUE censor);

static VALUE rb_log_rank_test(VALUE self, VALUE time_1, VALUE censor_1,VALUE time_2, VALUE censor_2);

static VALUE rb_cdf_unormal_Q(VALUE self, VALUE x);

static VALUE rb_dist_chi_square_Q(VALUE self, VALUE index, VALUE degree);
static VALUE rb_dist_chi_square_P(VALUE self, VALUE index, VALUE degree);

static VALUE rb_hist_mean(VALUE self, VALUE bin, VALUE range);
static VALUE rb_hist_median(VALUE self, VALUE bin, VALUE range);

static VALUE rb_basic_(VALUE self, VALUE bin, VALUE range);

/* Transfer Ruby functions to Ruby module */
void Init_sstat() {
    VALUE sstat_module = rb_define_module("SStat");
    VALUE surv_class = rb_define_class_under(sstat_module, "Surv", rb_cObject);
    VALUE dist_class = rb_define_class_under(sstat_module, "Dist", rb_cObject);
    VALUE hist_class = rb_define_class_under(sstat_module, "Hist", rb_cObject);

    /* Functions for basic */
    rb_define_method(surv_class, "percentile_index", rb_percentile_index, 2);
    rb_define_method(surv_class, "percentile", rb_percentile, 2);
    rb_define_method(surv_class, "index_less_equal", rb_index_less_equal, 2);
    rb_define_method(surv_class, "kaplan_meier", rb_kaplan_meier, 2);
    rb_define_method(surv_class, "log_rank_test", rb_log_rank_test, 4);
    rb_define_method(surv_class, "kaplan_meier_3p_extraploation", rb_kaplan_meier_3p_extraploation, 2);

    /* Functions for distribution */
    rb_define_method(dist_class, "cdf_unormal_Q", rb_cdf_unormal_Q, 1);

    /* Functions for histogram */
    rb_define_method(hist_class, "hist_mean", rb_hist_mean, 2);
    rb_define_method(hist_class, "hist_median", rb_hist_median, 2);
}

#endif
