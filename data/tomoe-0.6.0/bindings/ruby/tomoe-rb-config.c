#include "tomoe-rb.h"

#define _SELF(obj) RVAL2TCFG(obj)

static VALUE
tc_initialize(VALUE self, VALUE file_name)
{
    TomoeConfig *config;

    config = tomoe_config_new(StringValueCStr(file_name));
    G_INITIALIZE(self, config);

    return Qnil;
}

static VALUE
tc_languages(VALUE self)
{
    VALUE rb_languages = Qnil;
    const gchar *const *languages;

    languages = tomoe_config_get_languages(_SELF(self));
    if (languages) {
        rb_languages = rb_ary_new();
        while (*languages) {
            rb_ary_push(rb_languages, rb_str_new2(*languages));
            languages++;
        }
    }

    return rb_languages;
}

void
_tomoe_rb_init_tomoe_config(VALUE mTomoe)
{
    VALUE cTomoeConfig;

    cTomoeConfig = G_DEF_CLASS(TOMOE_TYPE_CONFIG, "Config", mTomoe);

    rb_define_method(cTomoeConfig, "initialize", tc_initialize, 1);

    rb_define_method(cTomoeConfig, "languages", tc_languages, 0);
}

