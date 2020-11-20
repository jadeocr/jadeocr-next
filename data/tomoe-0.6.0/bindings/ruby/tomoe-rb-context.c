/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */

#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TCTX(obj)

static VALUE
tc_load_config(int argc, VALUE *argv, VALUE self)
{
    VALUE filename;

    rb_scan_args(argc, argv, "01", &filename);

    tomoe_context_load_config(_SELF(self),
                              NIL_P(filename) ? NULL : RVAL2CSTR(filename));
    return Qnil;
}

static VALUE
tc_search(VALUE self, VALUE query)
{
    return GLIST2ARYF(tomoe_context_search(_SELF(self), RVAL2TQRY(query)));
}

/* static VALUE */
/* tc_multi_search(VALUE self, VALUE rb_queries) */
/* { */
/*     return GLIST2ARYF(tomoe_context_multi_search(_SELF(self), */
/*                                                  RVAL2GLIST(queries))); */
/* } */

static VALUE
tc_register(VALUE self, VALUE chr)
{
    return CBOOL2RVAL(tomoe_context_register(_SELF(self), RVAL2TCHR(chr)));
}

static VALUE
tc_unregister(VALUE self, VALUE utf8)
{
    return CBOOL2RVAL(tomoe_context_unregister(_SELF(self), RVAL2CSTR(utf8)));
}

static VALUE
tc_get_char(VALUE self, VALUE utf8)
{
    return GOBJ2RVALU(tomoe_context_get_char(_SELF(self), RVAL2CSTR(utf8)));
}

void
_tomoe_rb_init_tomoe_context(VALUE mTomoe)
{
    VALUE cTomoeContext;

    cTomoeContext = G_DEF_CLASS(TOMOE_TYPE_CONTEXT, "Context", mTomoe);

    rb_define_method(cTomoeContext, "load_config", tc_load_config, -1);
    rb_define_method(cTomoeContext, "search", tc_search, 1);
/*     rb_define_method(cTomoeContext, "multi_search", tc_multi_search, 1); */

    rb_define_method(cTomoeContext, "register", tc_register, 1);
    rb_define_method(cTomoeContext, "unregister", tc_unregister, 1);

    rb_define_method(cTomoeContext, "[]", tc_get_char, 1);
}
