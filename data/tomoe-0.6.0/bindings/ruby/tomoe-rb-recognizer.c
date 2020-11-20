#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TREC(obj)

static VALUE mTomoe;

static VALUE
td_s_load(VALUE self, VALUE name)
{
    VALUE loaded = Qfalse;
    VALUE normalized_name;
    TomoeModule *module;

    normalized_name = rb_funcall(name, rb_intern("downcase"), 0);
    module = tomoe_recognizer_load_module(RVAL2CSTR(normalized_name));
    if (module)
    {
        gchar *type_name, *class_name;
        GType type;

        class_name = g_strconcat("Recognizer", RVAL2CSTR(name), NULL);
        type_name = g_strconcat("Tomoe", class_name, NULL);
        type = g_type_from_name(type_name);
        if (type) {
            if (!rb_const_defined(mTomoe, rb_intern(class_name)))
                G_DEF_CLASS3(type_name, class_name, mTomoe);

            if (rbgobj_lookup_class_by_gtype(type, Qnil))
                loaded = Qtrue;
        }
        g_free(class_name);
        g_free(type_name);
    }

    return loaded;
}

static VALUE
td_s_default_module_dir(VALUE self)
{
    return CSTR2RVAL(tomoe_recognizer_get_default_module_dir());
}

static VALUE
td_s_set_default_module_dir(VALUE self, VALUE dir)
{
    tomoe_recognizer_set_default_module_dir(RVAL2CSTR2(dir));
    return Qnil;
}

static VALUE
tr_language(VALUE self)
{
    const gchar *language;

    language = tomoe_recognizer_get_language(_SELF(self));
    if (language)
        return rb_str_new2(language);
    else
        return Qnil;
}

void
_tomoe_rb_init_tomoe_recognizer(VALUE _mTomoe)
{
    VALUE cTomoeRecognizer;

    mTomoe = _mTomoe;
    cTomoeRecognizer = G_DEF_CLASS(TOMOE_TYPE_RECOGNIZER, "Recognizer", mTomoe);

    rb_define_singleton_method(cTomoeRecognizer, "load", td_s_load, 1);
    rb_define_singleton_method(cTomoeRecognizer, "default_module_dir",
                               td_s_default_module_dir, 0);
    rb_define_singleton_method(cTomoeRecognizer, "set_default_module_dir",
                               td_s_set_default_module_dir, 1);
    rb_define_singleton_method(cTomoeRecognizer, "default_module_dir=",
                               td_s_set_default_module_dir, 1);

    rb_define_method(cTomoeRecognizer, "language", tr_language, 0);
}
