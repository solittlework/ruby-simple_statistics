#ifndef _SSTAT_H_
#define _SSTAT_H_
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "sstat_func.h"

static VALUE rb_percentile(VALUE self, VALUE arg, VALUE target);
static VALUE rb_index_less_equal(VALUE self, VALUE arg, VALUE target);
static VALUE rb_index_less_equal(VALUE self, VALUE arg, VALUE target);

//rb_ary_storde(unmasked, i, INT2NUM(p ^ m));

void Init_sstat() {
	VALUE sstat_module = rb_define_module("SStat");
	VALUE basic_class = rb_define_class_under(sstat_module, "Basic", rb_cObject);

	rb_define_method(basic_class, "percentile", rb_percentile, 2);
	rb_define_method(basic_class, "index_less_equal", rb_index_less_equal, 2);

}

#endif