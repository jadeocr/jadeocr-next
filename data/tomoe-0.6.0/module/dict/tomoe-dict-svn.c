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
 *  $Id: tomoe-dict-svn.c 1493 2007-06-16 10:59:43Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gmodule.h>

#include <svn_pools.h>
#include <svn_client.h>
#include <svn_utf.h>

#include <tomoe-module-impl.h>
#include <tomoe-dict.h>
#include <glib-utils.h>

#include "tomoe-dict-ptr-array.h"

#define DIRTY_BORDER 10

#define TOMOE_TYPE_DICT_SVN            tomoe_type_dict_svn
#define TOMOE_DICT_SVN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICT_SVN, TomoeDictSvn))
#define TOMOE_DICT_SVN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICT_SVN, TomoeDictSvnClass))
#define TOMOE_IS_DICT_SVN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICT_SVN))
#define TOMOE_IS_DICT_SVN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICT_SVN))
#define TOMOE_DICT_SVN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_DICT_SVN, TomoeDictSvnClass))

enum {
    PROP_0,
    PROP_DICTIONARY,
    PROP_WORKING_COPY
};

typedef struct _TomoeDictSvn TomoeDictSvn;
typedef struct _TomoeDictSvnClass TomoeDictSvnClass;
struct _TomoeDictSvn
{
    TomoeDict         object;
    gchar            *working_copy;

    gint              dirty;

    TomoeDict        *sub_dict;

    apr_pool_t       *pool;
    svn_client_ctx_t *ctx;
};

struct _TomoeDictSvnClass
{
    TomoeDictClass parent_class;
};

static GType tomoe_type_dict_svn = 0;
static TomoeDictClass *parent_class;

static apr_pool_t *pool;

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
static TomoeChar   *get_char                  (TomoeDict     *dict,
                                               const gchar   *utf8);
static GList       *search                    (TomoeDict     *dict,
                                               TomoeQuery    *query);
static gboolean     flush                     (TomoeDict     *dict);
static gboolean     is_editable               (TomoeDict     *dict);
static gboolean     is_available              (TomoeDict     *dict);
static gchar       *get_available_private_utf8 (TomoeDict    *dict);

static gboolean     tomoe_dict_svn_update      (TomoeDictSvn *dict,
                                                gboolean      force);
static gboolean     tomoe_dict_svn_commit      (TomoeDictSvn *dict);

static void
class_init (TomoeDictSvnClass *klass)
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
    dict_class->flush           = flush;
    dict_class->is_editable     = is_editable;
    dict_class->is_available    = is_available;
    dict_class->get_available_private_utf8 = get_available_private_utf8;

    g_object_class_install_property (
        gobject_class,
        PROP_DICTIONARY,
        g_param_spec_object (
            "dictionary",
            "Dictionary",
            "The dictionary to delegate of the dictionary",
            TOMOE_TYPE_DICT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property (
        gobject_class,
        PROP_WORKING_COPY,
        g_param_spec_string (
            "working-copy",
            "Working copy",
            "The working copy directory of the dictionary",
            NULL,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
init (TomoeDictSvn *dict)
{
    svn_error_t *err;

    dict->working_copy  = NULL;
    dict->dirty         = 0;
    dict->sub_dict      = NULL;
    dict->pool          = svn_pool_create (NULL);
    err = svn_client_create_context (&dict->ctx, dict->pool);

    if (err) {
        g_warning ("create context: %s", err->message);
        svn_error_clear (err);
    }
}

static void
register_type (GTypeModule *type_module)
{
    static const GTypeInfo info =
        {
            sizeof (TomoeDictSvnClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDictSvn),
            0,
            (GInstanceInitFunc) init,
        };

    tomoe_type_dict_svn = g_type_module_register_type (type_module,
                                                       TOMOE_TYPE_DICT,
                                                       "TomoeDictSvn",
                                                       &info, 0);
}

G_MODULE_EXPORT GList *
TOMOE_MODULE_IMPL_INIT (GTypeModule *type_module)
{
    GList *registered_types = NULL;
    apr_status_t status;

    status = apr_initialize ();
    if (status) {
        char buf[1024];
        apr_strerror (status, buf, sizeof(buf) - 1);
        g_warning ("cannot initialize APR: %s", buf);
        return registered_types;
    }

    pool = svn_pool_create (NULL);
    svn_utf_initialize (pool);

    register_type (type_module);
    if (tomoe_type_dict_svn)
        registered_types =
            g_list_prepend (registered_types,
                            (gchar *) g_type_name (tomoe_type_dict_svn));

    return registered_types;
}

G_MODULE_EXPORT void
TOMOE_MODULE_IMPL_EXIT (void)
{
    apr_pool_destroy (pool);
}

G_MODULE_EXPORT GObject *
TOMOE_MODULE_IMPL_INSTANTIATE (const gchar *first_property, va_list var_args)
{
    return g_object_new_valist (TOMOE_TYPE_DICT_SVN, first_property, var_args);
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
    TomoeDictSvn *dict;

    object = klass->constructor (type, n_props, props);
    dict = TOMOE_DICT_SVN (object);

    return object;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (object);

    switch (prop_id) {
      case PROP_DICTIONARY:
        if (dict->sub_dict)
            g_object_unref (dict->sub_dict);
        dict->sub_dict = g_value_get_object (value);
        if (dict->sub_dict)
            g_object_ref (dict->sub_dict);
        break;
      case PROP_WORKING_COPY:
        g_free (dict->working_copy);
        dict->working_copy = g_value_dup_string (value);
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
    TomoeDictSvn *dict = TOMOE_DICT_SVN (object);

    switch (prop_id) {
      case PROP_DICTIONARY:
        g_value_set_object (value, dict->sub_dict);
        break;
      case PROP_WORKING_COPY:
        g_value_set_string (value, dict->working_copy);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeDictSvn *dict;

    dict = TOMOE_DICT_SVN (object);

    if (dict->working_copy) {
        if (g_file_test (dict->working_copy, G_FILE_TEST_EXISTS))
            flush (TOMOE_DICT (object));
        g_free (dict->working_copy);
    }

    if (dict->sub_dict)
        g_object_unref (dict->sub_dict);
    if (dict->pool)
        apr_pool_destroy (dict->pool);

    dict->working_copy  = NULL;
    dict->dirty         = 0;
    dict->sub_dict      = NULL;
    dict->pool          = NULL;
    dict->ctx           = NULL;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static const gchar*
get_name (TomoeDict *_dict)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), NULL);

    return tomoe_dict_get_name (dict->sub_dict);
}

static gboolean
register_char (TomoeDict *_dict, TomoeChar *chr)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), FALSE);

    return (tomoe_dict_svn_update (dict, TRUE) &&
            tomoe_dict_register_char (dict->sub_dict, chr) &&
            tomoe_dict_svn_commit (dict));
}

static gboolean
unregister_char (TomoeDict *_dict, const gchar *utf8)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), FALSE);

    return (tomoe_dict_svn_update (dict, TRUE) &&
            tomoe_dict_unregister_char (dict->sub_dict, utf8) &&
            tomoe_dict_svn_commit (dict));
}

static TomoeChar *
get_char (TomoeDict *_dict, const gchar *utf8)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), NULL);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), NULL);

    if (tomoe_dict_svn_update (dict, FALSE))
        return tomoe_dict_get_char (dict->sub_dict, utf8);
    else
        return NULL;
}

static GList *
search (TomoeDict *_dict, TomoeQuery *query)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), NULL);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), NULL);

    if (tomoe_dict_svn_update (dict, FALSE))
        return tomoe_dict_search (dict->sub_dict, query);
    else
        return NULL;
}

static gboolean
flush (TomoeDict *_dict)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), FALSE);

    return (tomoe_dict_svn_update (dict, TRUE) &&
            tomoe_dict_flush (dict->sub_dict) &&
            tomoe_dict_svn_commit (dict));
}

static gboolean
is_editable (TomoeDict *_dict)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), FALSE);

    return tomoe_dict_is_editable (dict->sub_dict);
}

static gboolean
is_available (TomoeDict *_dict)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), FALSE);

    if (!dict->working_copy)
        return FALSE;

    if (!g_file_test (dict->working_copy, G_FILE_TEST_EXISTS))
        return FALSE;

    return tomoe_dict_is_available (dict->sub_dict);
}

static gchar *
get_available_private_utf8 (TomoeDict *_dict)
{
    TomoeDictSvn *dict = TOMOE_DICT_SVN (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_SVN (dict), NULL);
    g_return_val_if_fail (TOMOE_IS_DICT (dict->sub_dict), FALSE);

    if (tomoe_dict_svn_update (dict, TRUE))
        return tomoe_dict_get_available_private_utf8 (dict->sub_dict);
    else
        return NULL;
}


static gboolean
tomoe_dict_svn_update (TomoeDictSvn *dict, gboolean force)
{
    svn_error_t *err;
    svn_opt_revision_t revision;
    apr_pool_t *sub_pool;

    g_return_val_if_fail (dict->working_copy, FALSE);
    g_return_val_if_fail (g_file_test (dict->working_copy, G_FILE_TEST_EXISTS),
                          FALSE);

    g_return_val_if_fail (dict->ctx, FALSE);

    if (!force && dict->dirty++ < DIRTY_BORDER)
        return TRUE;

    sub_pool = svn_pool_create (dict->pool);
    revision.kind = svn_opt_revision_head;
    err = svn_client_update (NULL, dict->working_copy, &revision, TRUE,
                             dict->ctx, sub_pool);
    apr_pool_destroy (sub_pool);

    if (err) {
        g_warning ("failed to update: %s", err->message);
        svn_error_clear (err);
        return FALSE;
    } else {
        dict->dirty = 0;
        return TRUE;
    }
}

static gboolean
tomoe_dict_svn_commit (TomoeDictSvn *dict)
{
    svn_error_t *err;
    svn_client_commit_info_t *info;
    apr_array_header_t *targets;
    apr_pool_t *sub_pool;

    g_return_val_if_fail (dict->working_copy, FALSE);
    g_return_val_if_fail (g_file_test (dict->working_copy, G_FILE_TEST_EXISTS),
                          FALSE);

    g_return_val_if_fail (dict->ctx, FALSE);

    sub_pool = svn_pool_create (dict->pool);
    targets = apr_array_make (sub_pool, 1, sizeof(gchar));
    APR_ARRAY_PUSH(targets, gchar *) = dict->working_copy;
    err = svn_client_commit (&info, targets, FALSE, dict->ctx, sub_pool);
    apr_pool_destroy (sub_pool);

    if (err) {
        g_warning ("failed to commit: %s", err->message);
        svn_error_clear (err);
        return FALSE;
    } else {
        return TRUE;
    }
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
