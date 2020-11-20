/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2006 Kouhei Sutou <kou@cozmixng.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *  Boston, MA  02111-1307  USA
 *
 *  $Id: tomoe-dict-ruby.c 1493 2007-06-16 10:59:43Z ikezoe $
 */

#include <ruby.h>

#undef _
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gmodule.h>

#include <tomoe-module-impl.h>
#include <tomoe-dict.h>
#include <tomoe-candidate.h>
#include <glib-utils.h>

#include <rbtomoe.h>

#define INIT_FILE "dict-init"

#define TOMOE_TYPE_DICT_RUBY            tomoe_type_dict_ruby
#define TOMOE_DICT_RUBY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICT_RUBY, TomoeDictRuby))
#define TOMOE_DICT_RUBY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICT_RUBY, TomoeDictRubyClass))
#define TOMOE_IS_DICT_RUBY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICT_RUBY))
#define TOMOE_IS_DICT_RUBY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICT_RUBY))
#define TOMOE_DICT_RUBY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_DICT_RUBY, TomoeDictRubyClass))

enum {
    PROP_0,
    PROP_FILENAME,
    PROP_EDITABLE,
    PROP_LOAD_PATH
};

typedef struct _TomoeDictRuby TomoeDictRuby;
typedef struct _TomoeDictRubyClass TomoeDictRubyClass;
struct _TomoeDictRuby
{
    TomoeDict            object;
    gchar               *filename;
    gchar               *name;
    gchar               *load_path;

    gboolean             editable;

    VALUE                rb_dict;
};

struct _TomoeDictRubyClass
{
    TomoeDictClass parent_class;
};

typedef struct _TomoeDictSearchContext {
    TomoeQuery *query;
    GList *results;
} TomoeDictSearchContext;

extern VALUE rb_load_path;

static GType tomoe_type_dict_ruby = 0;
static TomoeDictClass *parent_class;

static GObject     *constructor               (GType                  type,
                                               guint                  n_props,
                                               GObjectConstructParam *props);
static void         dispose                   (GObject       *object);
static void         set_property              (GObject       *object,
                                               guint         prop_id,
                                               const GValue  *value,
                                               GParamSpec    *pspec);
static void         get_property              (GObject       *object,
                                               guint          prop_id,
                                               GValue        *value,
                                               GParamSpec    *pspec);
static const gchar *get_name                  (TomoeDict     *dict);
static gboolean     register_char             (TomoeDict     *dict,
                                               TomoeChar     *chr);
static gboolean     unregister_char           (TomoeDict     *dict,
                                               const gchar   *utf8);
static TomoeChar    *get_char                 (TomoeDict     *dict,
                                               const gchar   *utf8);
static GList        *search                   (TomoeDict     *dict,
                                               TomoeQuery    *query);

static void
class_init (TomoeDictRubyClass *klass)
{
    GObjectClass *gobject_class;
    TomoeDictClass *dict_class;

    parent_class = g_type_class_peek_parent (klass);

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->constructor  = constructor;
    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    dict_class = TOMOE_DICT_CLASS (klass);
    dict_class->get_name        = get_name;
    dict_class->register_char   = register_char;
    dict_class->unregister_char = unregister_char;
    dict_class->get_char        = get_char;
    dict_class->search          = search;

    g_object_class_install_property (
        gobject_class,
        PROP_FILENAME,
        g_param_spec_string (
            "filename",
            "Filename",
            "The filename of xml file",
            NULL,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(
        gobject_class,
        PROP_EDITABLE,
        g_param_spec_boolean(
            "editable",
            "Editable",
            "Editable flag",
            TRUE,
            G_PARAM_READWRITE));
}

static void
init (TomoeDictRuby *dict)
{
    dict->filename  = NULL;
    dict->name      = NULL;
    dict->load_path = NULL;
    dict->editable  = FALSE;
    dict->rb_dict   = Qnil;
}

static void
register_type (GTypeModule *type_module)
{
    static const GTypeInfo info =
        {
            sizeof (TomoeDictRubyClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDictRuby),
            0,
            (GInstanceInitFunc) init,
        };

    tomoe_type_dict_ruby = g_type_module_register_type (type_module,
                                                       TOMOE_TYPE_DICT,
                                                       "TomoeDictRuby",
                                                       &info, 0);
}

static void
tomoe_dict_ruby_atexit(void)
{
    ruby_cleanup (0);
}

G_MODULE_EXPORT GList *
TOMOE_MODULE_IMPL_INIT (GTypeModule *type_module)
{
    GList *registered_types = NULL;
    static gchar *argv[] = {G_STRINGIFY (PACKAGE)};
    static gboolean initialized = FALSE;

    register_type (type_module);
    if (tomoe_type_dict_ruby)
        registered_types =
            g_list_prepend (registered_types,
                            (gchar *) g_type_name (tomoe_type_dict_ruby));

    if (!rb_load_path) {
        ruby_init ();
        atexit(tomoe_dict_ruby_atexit);
        ruby_script (PACKAGE);
        ruby_set_argv (1, argv);

        if (RARRAY(rb_load_path)->len == 0) {
            ruby_init_loadpath ();
        }
    }

    if (!initialized) {
        initialized = TRUE;
        rb_ary_unshift (rb_load_path, rb_str_new2 (RUBY_EXTDIR));
        rb_ary_unshift (rb_load_path, rb_str_new2 (RUBY_LIBDIR));
    }

    return registered_types;
}

G_MODULE_EXPORT void
TOMOE_MODULE_IMPL_EXIT (void)
{
}

G_MODULE_EXPORT GObject *
TOMOE_MODULE_IMPL_INSTANTIATE (const gchar *first_property, va_list args)
{
    GObject *object;

    object = g_object_new_valist (TOMOE_TYPE_DICT_RUBY,
                                  first_property, args);
    return object;
}

G_MODULE_EXPORT gchar *
TOMOE_MODULE_IMPL_GET_LOG_DOMAIN (void)
{
    return g_strdup (G_LOG_DOMAIN);
}

static GObject *
constructor (GType type, guint n_props, GObjectConstructParam *props)
{
    GObject *object;
    GObjectClass *klass = G_OBJECT_CLASS (parent_class);
    TomoeDictRuby *dict;
    VALUE mTomoe, cTomoeDictRuby;

    object = klass->constructor (type, n_props, props);
    dict = TOMOE_DICT_RUBY (object);

    if (dict->load_path)
        rb_ary_unshift (rb_load_path, CSTR2RVAL (dict->load_path));

    rb_funcall (Qnil, rb_intern ("require"), 1, rb_str_new2 (INIT_FILE));

    mTomoe = rb_const_get (rb_cObject, rb_intern ("Tomoe"));
    cTomoeDictRuby = rb_const_get (mTomoe, rb_intern ("DictRuby"));
    dict->rb_dict = rb_funcall (cTomoeDictRuby, rb_intern ("find"),
                                1, GOBJ2RVAL (dict));
    if (NIL_P (dict->rb_dict)) {
        g_object_unref (object);
        return NULL;
    }
    rb_gc_register_address (&dict->rb_dict);

    return object;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (object);

    switch (prop_id) {
      case PROP_FILENAME:
        dict->filename = g_value_dup_string (value);
        break;
      case PROP_EDITABLE:
        dict->editable = g_value_get_boolean (value);
        break;
      case PROP_LOAD_PATH:
        dict->load_path = g_value_dup_string (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}


static void
get_property (GObject *object,
              guint prop_id,
              GValue *value,
              GParamSpec *pspec)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (object);

    switch (prop_id) {
      case PROP_FILENAME:
        g_value_set_string (value, dict->filename);
        break;
      case PROP_EDITABLE:
        g_value_set_boolean (value, dict->editable);
        break;
      case PROP_LOAD_PATH:
        g_value_set_string (value, dict->load_path);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeDictRuby *dict;

    dict = TOMOE_DICT_RUBY(object);

    if (dict->name)
        g_free (dict->name);
    if (dict->filename)
        g_free (dict->filename);
    if (dict->load_path)
        g_free (dict->load_path);
    if (!NIL_P (dict->rb_dict))
        rb_gc_unregister_address (&dict->rb_dict);

    dict->name     = NULL;
    dict->filename = NULL;
    dict->load_path = NULL;
    dict->rb_dict  = Qnil;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static const gchar*
get_name (TomoeDict *_dict)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (_dict);
    VALUE name;

    g_return_val_if_fail (TOMOE_IS_DICT_RUBY (dict), NULL);

    name = rb_funcall (dict->rb_dict, rb_intern ("name"), 0);
    return RVAL2CSTR (name);
}

static gboolean
register_char (TomoeDict *_dict, TomoeChar *chr)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (_dict);
    g_return_val_if_fail (TOMOE_IS_DICT_RUBY (dict), FALSE);
    g_return_val_if_fail (chr, FALSE);

    return RVAL2CBOOL (rb_funcall (dict->rb_dict, rb_intern ("register"),
                                   1, GOBJ2RVAL (chr)));
}

static gboolean
unregister_char (TomoeDict *_dict, const gchar *utf8)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_RUBY (dict), FALSE);
    g_return_val_if_fail (utf8 && *utf8 != '\0', FALSE);

    return RVAL2CBOOL (rb_funcall (dict->rb_dict, rb_intern ("unregister"),
                                   1, CSTR2RVAL (utf8)));
}

static TomoeChar *
get_char (TomoeDict *_dict, const gchar *utf8)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_RUBY (dict), NULL);
    g_return_val_if_fail (utf8 && *utf8 != '\0', NULL);

    return RVAL2TCHR (rb_funcall (dict->rb_dict, rb_intern ("[]"),
                                  1, CSTR2RVAL (utf8)));
}

static GList *
search (TomoeDict *_dict, TomoeQuery *query)
{
    TomoeDictRuby *dict = TOMOE_DICT_RUBY (_dict);
    GList *results = NULL;
    VALUE rb_results;
    int i, len;

    g_return_val_if_fail (TOMOE_IS_DICT_RUBY (dict), results);

    rb_results = rb_funcall (dict->rb_dict, rb_intern ("search"),
                             1, GOBJ2RVAL (query));
    len = RARRAY (rb_results)->len;
    for (i = len; i; i--) {
        results = g_list_prepend (results,
                                  RVAL2TCND (RARRAY (rb_results)->ptr[i]));
    }
    return results;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
