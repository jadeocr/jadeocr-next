#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TCHR(obj)

static VALUE
tc_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE xml;
    TomoeChar *chr;

    rb_scan_args (argc, argv, "01", &xml);

    if (NIL_P(xml)) {
        chr = tomoe_char_new();
    } else {
        chr = tomoe_char_new_from_xml_data(RVAL2CSTR(xml), RSTRING(xml)->len);
    }

    G_INITIALIZE(self, chr);
    return Qnil;
}

static VALUE
tc_get_readings(VALUE self)
{
    return GLIST2ARY((GList *)tomoe_char_get_readings(_SELF(self)));
}

static VALUE
tc_add_reading(VALUE self, VALUE reading)
{
    tomoe_char_add_reading(_SELF(self), RVAL2TRDG(reading));
    return Qnil;
}

static VALUE
tc_get_radicals(VALUE self)
{
    VALUE radicals;
    const GList *node;

    radicals = rb_ary_new ();
    for (node = tomoe_char_get_radicals(_SELF(self));
         node;
         node = g_list_next (node)) {
        rb_ary_push (radicals, CSTR2RVAL (node->data));
    }
    return radicals;
}

static VALUE
tc_add_radical(VALUE self, VALUE radical)
{
    tomoe_char_add_radical(_SELF(self), RVAL2CSTR(radical));
    return Qnil;
}

static VALUE
tc_get_meta_data(VALUE self, VALUE key)
{
    return CSTR2RVAL(tomoe_char_get_meta_data(_SELF(self), RVAL2CSTR(key)));
}

static VALUE
tc_register_meta_data(VALUE self, VALUE key, VALUE value)
{
    tomoe_char_register_meta_data(_SELF(self),
                                  RVAL2CSTR(key), RVAL2CSTR(value));
    return Qnil;
}

static VALUE
tc_has_meta_data(VALUE self)
{
    return CBOOL2RVAL(tomoe_char_has_meta_data(_SELF(self)));
}

static void
yield_meta_data(gpointer _key, gpointer _value, gpointer user_data)
{
    gchar *key = _key;
    gchar *value = _value;

    rb_yield_values(2, CSTR2RVAL(key), CSTR2RVAL(value));
}

static VALUE
tc_meta_data_foreach(VALUE self)
{
    tomoe_char_meta_data_foreach(_SELF(self), yield_meta_data, NULL);
    return Qnil;
}

static VALUE
tc_to_xml(VALUE self)
{
    gchar *xml;

    xml = tomoe_char_to_xml(_SELF(self));
    if (xml) {
        VALUE result;

        result = CSTR2RVAL(xml);
        g_free(xml);
        return result;
    } else {
        return Qnil;
    }
}

static VALUE
tc_compare(VALUE self, VALUE other)
{
    return INT2NUM(tomoe_char_compare(_SELF(self), RVAL2TCHR(other)));
}

void
_tomoe_rb_init_tomoe_char(VALUE mTomoe)
{
    VALUE cTomoeChar;

    cTomoeChar = G_DEF_CLASS(TOMOE_TYPE_CHAR, "Char", mTomoe);

    rb_define_const(cTomoeChar, "PRIVATE_USE_AREA_START",
                    INT2NUM(TOMOE_CHAR_PRIVATE_USE_AREA_START));
    rb_define_const(cTomoeChar, "PRIVATE_USE_AREA_END",
                    INT2NUM(TOMOE_CHAR_PRIVATE_USE_AREA_END));

    rb_include_module(cTomoeChar, rb_mComparable);
    rb_include_module(cTomoeChar, rb_mEnumerable);

    rb_define_method(cTomoeChar, "initialize", tc_initialize, -1);

    rb_define_method(cTomoeChar, "<=>", tc_compare, 1);

    rb_define_method(cTomoeChar, "readings", tc_get_readings, 0);
    rb_define_method(cTomoeChar, "add_reading", tc_add_reading, 1);
    rb_define_method(cTomoeChar, "radicals", tc_get_radicals, 0);
    rb_define_method(cTomoeChar, "add_radical", tc_add_radical, 1);

    rb_define_method(cTomoeChar, "[]", tc_get_meta_data, 1);
    rb_define_method(cTomoeChar, "[]=", tc_register_meta_data, 2);
    rb_define_method(cTomoeChar, "has_meta_data?", tc_has_meta_data, 0);

    rb_define_method(cTomoeChar, "each", tc_meta_data_foreach, 0);

    rb_define_method(cTomoeChar, "to_xml", tc_to_xml, 0);
}
