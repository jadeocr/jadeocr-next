#include <stdlib.h>

#include "tomoe-rb.h"

static ID set_log_domain;
static VALUE mGLibLog;

#ifndef HAVE_GOBJ2RVALU
VALUE
_tomoe_ruby_object_from_instance_with_unref(gpointer instance)
{
    VALUE result = rbgobj_ruby_object_from_instance(instance);
    if (!NIL_P(result))
        g_object_unref(instance);
    return result;
}
#endif

void
_tomoe_rb_set_log_domain (const gchar *domain)
{
    rb_funcall(mGLibLog, set_log_domain, 1, rb_str_new2(domain));
}

void
Init_tomoe(void)
{
    VALUE mTomoe;

    mTomoe = rb_define_module("Tomoe");

    _tomoe_rb_init_tomoe_candidate(mTomoe);
    _tomoe_rb_init_tomoe_char(mTomoe);
    _tomoe_rb_init_tomoe_config(mTomoe);
    _tomoe_rb_init_tomoe_context(mTomoe);
    _tomoe_rb_init_tomoe_dict(mTomoe);
    _tomoe_rb_init_tomoe_query(mTomoe);
    _tomoe_rb_init_tomoe_reading(mTomoe);
    _tomoe_rb_init_tomoe_recognizer(mTomoe);
    _tomoe_rb_init_tomoe_writing(mTomoe);


    tomoe_init();
    atexit(tomoe_quit);

    mGLibLog = rb_const_get(mGLib, rb_intern("Log"));
    set_log_domain = rb_intern("set_log_domain");
}
