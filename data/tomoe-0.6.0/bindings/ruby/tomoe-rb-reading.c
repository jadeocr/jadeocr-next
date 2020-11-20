/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */

#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TRDG(obj)

static VALUE
tr_initialize(VALUE self, VALUE type, VALUE reading)
{
    G_INITIALIZE(self, tomoe_reading_new(RVAL2TRT(type), RVAL2CSTR(reading)));
    return Qnil;
}

static VALUE
tr_to_xml(VALUE self)
{
    VALUE rb_xml = Qnil;
    gchar *xml;

    xml = tomoe_reading_to_xml(_SELF(self));
    if (xml) {
        rb_xml = CSTR2RVAL(xml);
        g_free(xml);
    }

    return rb_xml;
}

static VALUE
tr_compare(VALUE self, VALUE other)
{
    return INT2NUM(tomoe_reading_compare(_SELF(self), RVAL2TRDG(other)));
}

void
_tomoe_rb_init_tomoe_reading(VALUE mTomoe)
{
    VALUE cTomoeReading;

    cTomoeReading = G_DEF_CLASS(TOMOE_TYPE_READING, "Reading", mTomoe);
    G_DEF_CLASS(TOMOE_TYPE_READING_TYPE, "ReadingType", mTomoe);

    G_DEF_CONSTANTS(cTomoeReading, TOMOE_TYPE_READING_TYPE, "TOMOE_READING_");

    rb_define_method(cTomoeReading, "initialize", tr_initialize, 2);

    rb_define_method(cTomoeReading, "<=>", tr_compare, 1);

    rb_define_method(cTomoeReading, "to_xml", tr_to_xml, 0);
}
