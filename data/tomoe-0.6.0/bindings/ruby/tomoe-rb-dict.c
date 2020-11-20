#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TDIC(obj)

static VALUE mTomoe;

static VALUE
td_s_load(VALUE self, VALUE name)
{
    VALUE loaded = Qfalse;
    VALUE normalized_name;
    TomoeModule *module;

    normalized_name = rb_funcall(name, rb_intern("downcase"), 0);
    module = tomoe_dict_load_module(RVAL2CSTR(normalized_name));
    if (module)
    {
        gchar *type_name, *class_name;
        GType type;

        class_name = g_strconcat("Dict", RVAL2CSTR(name), NULL);
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
    return CSTR2RVAL(tomoe_dict_get_default_module_dir());
}

static VALUE
td_s_set_default_module_dir(VALUE self, VALUE dir)
{
    tomoe_dict_set_default_module_dir(RVAL2CSTR2(dir));
    return Qnil;
}

static VALUE
td_get_char(VALUE self, VALUE utf8)
{
    return GOBJ2RVALU(tomoe_dict_get_char(_SELF(self), RVAL2CSTR(utf8)));
}

static VALUE
td_register_char(VALUE self, VALUE chr)
{
    return CBOOL2RVAL(tomoe_dict_register_char(_SELF(self), RVAL2TCHR(chr)));
}

static VALUE
td_unregister_char(VALUE self, VALUE utf8)
{
    return CBOOL2RVAL(tomoe_dict_unregister_char(_SELF(self), RVAL2CSTR(utf8)));
}

static VALUE
td_search(VALUE self, VALUE query)
{
    return GLIST2ARYF(tomoe_dict_search(_SELF(self), RVAL2TQRY(query)));
}

static VALUE
td_get_available_private_utf8(VALUE self)
{
    return CSTR2RVAL2(tomoe_dict_get_available_private_utf8(_SELF(self)));
}

static VALUE
td_flush(VALUE self)
{
    return CBOOL2RVAL(tomoe_dict_flush(_SELF(self)));
}

static VALUE
td_copy(VALUE self, VALUE dest_dict)
{
    return CBOOL2RVAL(tomoe_dict_copy(_SELF(self), RVAL2TDIC(dest_dict)));
}

void
_tomoe_rb_init_tomoe_dict(VALUE _mTomoe)
{
    VALUE cTomoeDict;

    mTomoe = _mTomoe;
    cTomoeDict = G_DEF_CLASS(TOMOE_TYPE_DICT, "Dict", mTomoe);

    rb_define_singleton_method(cTomoeDict, "load", td_s_load, 1);
    rb_define_singleton_method(cTomoeDict, "default_module_dir",
                               td_s_default_module_dir, 0);
    rb_define_singleton_method(cTomoeDict, "set_default_module_dir",
                               td_s_set_default_module_dir, 1);
    rb_define_singleton_method(cTomoeDict, "default_module_dir=",
                               td_s_set_default_module_dir, 1);

    rb_define_method(cTomoeDict, "[]", td_get_char, 1);
    rb_define_method(cTomoeDict, "register", td_register_char, 1);
    rb_define_method(cTomoeDict, "unregister", td_unregister_char, 1);

    rb_define_method(cTomoeDict, "search", td_search, 1);

    rb_define_method(cTomoeDict, "available_private_utf8",
                     td_get_available_private_utf8, 0);
    rb_define_method(cTomoeDict, "flush", td_flush, 0);
    rb_define_method(cTomoeDict, "copy", td_copy, 1);
}
