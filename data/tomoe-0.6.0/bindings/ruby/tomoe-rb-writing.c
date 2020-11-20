/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */

#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TWTG(obj)

static VALUE
tw_move_to(VALUE self, VALUE x, VALUE y)
{
    tomoe_writing_move_to(_SELF(self), NUM2INT(x), NUM2INT(y));
    return Qnil;
}

static VALUE
tw_line_to(VALUE self, VALUE x, VALUE y)
{
    tomoe_writing_line_to(_SELF(self), NUM2INT(x), NUM2INT(y));
    return Qnil;
}

static VALUE
tw_clear(VALUE self)
{
    tomoe_writing_clear(_SELF(self));
    return Qnil;
}

static VALUE
tw_get_n_strokes(VALUE self)
{
    return UINT2NUM(tomoe_writing_get_n_strokes(_SELF(self)));
}

static VALUE
tw_remove_last_stroke(VALUE self)
{
    tomoe_writing_remove_last_stroke(_SELF(self));
    return Qnil;
}

static VALUE
tw_stroke_to_array(GList *stroke)
{
    VALUE array;
    GList *points;

    array = rb_ary_new();
    for (points = stroke; points; points = points->next) {
        TomoePoint *point;

        point = points->data;
        rb_ary_push(array,
                    rb_ary_new3(2, INT2NUM(point->x), INT2NUM(point->y)));
    }

    return array;
}

static VALUE
tw_get_strokes(VALUE self)
{
    VALUE array;
    TomoeWriting *writing;
    const GList *strokes, *stroke;

    writing = _SELF(self);
    strokes = tomoe_writing_get_strokes(writing);
    array = rb_ary_new();
    for (stroke = strokes; stroke; stroke = g_list_next(stroke)) {
        rb_ary_push(array, tw_stroke_to_array(stroke->data));
    }

    return array;
}

static VALUE
tw_each(VALUE self)
{
    return rb_ary_each(tw_get_strokes(self));
}

static VALUE
tw_to_xml(VALUE self)
{
    gchar *xml;

    xml = tomoe_writing_to_xml(_SELF(self));
    if (xml) {
        VALUE result;
        result = CSTR2RVAL(xml);
        g_free(xml);
        return result;
    } else {
        return Qnil;
    }
}

void
_tomoe_rb_init_tomoe_writing(VALUE mTomoe)
{
    VALUE cTomoeWriting;

    cTomoeWriting = G_DEF_CLASS(TOMOE_TYPE_WRITING, "Writing", mTomoe);

    rb_define_const(cTomoeWriting, "WIDTH", INT2NUM(TOMOE_WRITING_WIDTH));
    rb_define_const(cTomoeWriting, "HEIGHT", INT2NUM(TOMOE_WRITING_HEIGHT));

    rb_include_module(cTomoeWriting, rb_mEnumerable);

    rb_define_method(cTomoeWriting, "move_to", tw_move_to, 2);
    rb_define_method(cTomoeWriting, "line_to", tw_line_to, 2);
    rb_define_method(cTomoeWriting, "clear", tw_clear, 0);
    rb_define_method(cTomoeWriting, "n_strokes", tw_get_n_strokes, 0);
    rb_define_method(cTomoeWriting, "remove_last_stroke",
                     tw_remove_last_stroke, 0);

    rb_define_method(cTomoeWriting, "strokes", tw_get_strokes, 0);
    rb_define_method(cTomoeWriting, "each", tw_each, 0);

    rb_define_method(cTomoeWriting, "to_xml", tw_to_xml, 0);
}
