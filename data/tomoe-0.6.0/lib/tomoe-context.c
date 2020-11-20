/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2006 Juernjakob Harder <juernjakob.harder@gmail.com>
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
 *  $Id: tomoe-context.c 1183 2007-04-12 05:37:20Z ktou $
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tomoe-dict.h"
#include "tomoe-recognizer.h"
#include "tomoe-context.h"
#include "tomoe-config.h"
#include "tomoe-shelf.h"
#include "tomoe-candidate.h"
#include "glib-utils.h"

#define DEFAULT_USER_DICT_CONTENT									\
"<?xml version =\"1.0\" encoding=\"UTF-8\"?>\n"						\
"<!DOCTYPE dictionary SYSTEM \"" DATADIR "/dict.dtd\">\n"			\
"<dictionary name=\"User dictionary\">\n"							\
"</dictionary>\n"

#define TOMOE_CONTEXT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CONTEXT, TomoeContextPrivate))

enum {
    PROP_0,
    PROP_RECOGNIZER
};

typedef struct _TomoeContextPrivate	TomoeContextPrivate;
struct _TomoeContextPrivate
{
    TomoeShelf *shelf;
    TomoeRecognizer *recognizer;
    TomoeDict *user_dict;
    gchar **languages;
};

G_DEFINE_TYPE (TomoeContext, tomoe_context, G_TYPE_OBJECT)

static void         dispose                   (GObject       *object);
static void         set_property              (GObject       *object,
                                               guint         prop_id,
                                               const GValue  *value,
                                               GParamSpec    *pspec);
static void         get_property              (GObject       *object,
                                               guint          prop_id,
                                               GValue        *value,
                                               GParamSpec    *pspec);

static void
tomoe_context_class_init (TomoeContextClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    g_object_class_install_property (
        gobject_class,
        PROP_RECOGNIZER,
        g_param_spec_object (
            "recognizer",
            "Recognizer",
            "The recognizer of the context",
            TOMOE_TYPE_RECOGNIZER,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_type_class_add_private (gobject_class, sizeof (TomoeContextPrivate));
}

static void
tomoe_context_init (TomoeContext *context)
{
    TomoeContextPrivate *priv = TOMOE_CONTEXT_GET_PRIVATE (context);

    priv->shelf      = NULL;
    priv->recognizer = NULL;
    priv->user_dict  = NULL;
    priv->languages = g_strdupv ((gchar **)g_get_language_names ());
}

/**
 * tomoe_context_new:
 * 
 * Create a new #TomoeContext.
 *
 * Return value: a new #TomoeContext.
 */
TomoeContext*
tomoe_context_new(void)
{
    TomoeContext *context;

    context = g_object_new(TOMOE_TYPE_CONTEXT, NULL);

    return context;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeContextPrivate *priv = TOMOE_CONTEXT_GET_PRIVATE (object);

    switch (prop_id) {
      case PROP_RECOGNIZER:
        if (priv->recognizer)
            g_object_unref (priv->recognizer);
        priv->recognizer = g_value_get_object (value);
        if (priv->recognizer)
            g_object_ref (priv->recognizer);
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
    TomoeContextPrivate *priv = TOMOE_CONTEXT_GET_PRIVATE (object);

    switch (prop_id) {
      case PROP_RECOGNIZER:
        g_value_set_object (value, priv->recognizer);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeContextPrivate *priv = TOMOE_CONTEXT_GET_PRIVATE (object);

    if (priv->shelf)
        g_object_unref (priv->shelf);
    if (priv->recognizer)
        g_object_unref (priv->recognizer);
    if (priv->user_dict)
        g_object_unref (priv->user_dict);
    if (priv->languages)
        g_strfreev (priv->languages);

    priv->shelf      = NULL;
    priv->recognizer = NULL;
    priv->user_dict  = NULL;
    priv->languages  = NULL;

    G_OBJECT_CLASS (tomoe_context_parent_class)->dispose (object);
}

static gboolean
ensure_user_dict_file_content (const gchar *user_dict_filename)
{
    if (!g_file_test (user_dict_filename, G_FILE_TEST_EXISTS)) {
        const gchar *content;
        GError *error = NULL;

        content = DEFAULT_USER_DICT_CONTENT;
        if (!g_file_set_contents (user_dict_filename,
                                  content, strlen (content), &error)) {
            TOMOE_HANDLE_ERROR (error);
            return FALSE;
        }
    }

    return TRUE;
}

static gchar *
ensure_user_dict_file (void)
{
    gchar *tomoe_dir_name, *user_dict_filename;

    tomoe_dir_name = g_build_filename (g_get_home_dir (), ".tomoe", NULL);

    if (!g_file_test (tomoe_dir_name, G_FILE_TEST_EXISTS)) {
        if (g_mkdir (tomoe_dir_name, 0700) == -1) {
            g_warning ("can't create %s: %s", tomoe_dir_name, strerror (errno));
        }
    }

    if (!g_file_test (tomoe_dir_name, G_FILE_TEST_IS_DIR)) {
        g_warning ("%s isn't directory: %s", tomoe_dir_name, strerror (errno));
    }

    user_dict_filename = g_build_filename (tomoe_dir_name, "dict.xml", NULL);
    g_free (tomoe_dir_name);

    if (!ensure_user_dict_file_content (user_dict_filename)) {
        g_free (user_dict_filename);
        return NULL;
    }

    return user_dict_filename;
}

static TomoeDict *
ensure_user_dict (TomoeShelf *shelf, const gchar *name)
{
    TomoeDict *user_dict;

    g_return_val_if_fail (TOMOE_IS_SHELF (shelf), NULL);

    user_dict = tomoe_shelf_get_dict (shelf, name);
    if (user_dict) {
        g_object_ref (user_dict);
    } else {
        gchar *user_dict_filename;

        user_dict_filename = ensure_user_dict_file ();
        if (user_dict_filename) {
            user_dict = tomoe_dict_new ("xml",
                                        "filename", user_dict_filename,
                                        "editable", TRUE,
                                        NULL);
            g_free (user_dict_filename);
        }

        tomoe_shelf_register_dict (shelf, name, user_dict);
    }

    return user_dict;
}

/**
 * tomoe_context_load_config:
 * @ctx: a #TomoeContext.
 * @config_file: the filename of configuration file to load.
 * 
 * Load dictionaries into configuration file.
 */
void
tomoe_context_load_config (TomoeContext *ctx, const gchar *config_file)
{
    TomoeContextPrivate *priv;
    TomoeConfig* cfg;

    g_return_if_fail (ctx);

    priv = TOMOE_CONTEXT_GET_PRIVATE(ctx);
    cfg = tomoe_config_new (config_file);

    if (priv->shelf)
        g_object_unref (priv->shelf);
    priv->shelf = tomoe_config_make_shelf (cfg, NULL);

    if (priv->user_dict)
        g_object_unref (priv->user_dict);
    priv->user_dict = ensure_user_dict (priv->shelf,
                                        tomoe_config_get_user_dict_name (cfg));

    g_strfreev (priv->languages);
    priv->languages = g_strdupv ((gchar **)tomoe_config_get_languages (cfg));
    if (!priv->languages)
        priv->languages = g_strdupv ((gchar **)g_get_language_names ());

    g_object_unref (cfg);
}

static gint
_candidate_compare_func (gconstpointer a, gconstpointer b)
{
    TomoeCandidate *ca = *(TomoeCandidate **) a;
    TomoeCandidate *cb = *(TomoeCandidate **) b;
    return tomoe_candidate_compare (ca, cb);
}


static GList *
tomoe_context_search_by_strokes (TomoeContext *context, TomoeWriting *input)
{
    TomoeContextPrivate *priv;
    GList *matched = NULL;

    g_return_val_if_fail (context, matched);
    if (!input) return matched;

    priv = TOMOE_CONTEXT_GET_PRIVATE (context);
    if (!priv->recognizer) {
        gchar **languages;

        languages = priv->languages;
        while (*languages && priv->recognizer == NULL) {
            priv->recognizer = tomoe_recognizer_new ("simple",
                                                     "language", *languages,
                                                     NULL);
            languages++;

            if (!priv->recognizer)
                continue;

            if (!tomoe_recognizer_is_available (priv->recognizer)) {
                g_object_unref (priv->recognizer);
                priv->recognizer = NULL;
            }
        }

        if (!priv->recognizer)
            priv->recognizer = tomoe_recognizer_new ("simple", NULL);
        g_return_val_if_fail (TOMOE_IS_RECOGNIZER (priv->recognizer), matched);
    }

    if (tomoe_recognizer_is_available (priv->recognizer))
        matched = g_list_sort (tomoe_recognizer_search (priv->recognizer, input),
                               _candidate_compare_func);

    return matched;
}

static GList *
tomoe_context_search_by_dict (TomoeContext *context, TomoeQuery *query)
{
    TomoeContextPrivate *priv;
    TomoeShelf *shelf;
    GList *names, *name;
    GList *results = NULL;

    if (!context) return results;

    priv = TOMOE_CONTEXT_GET_PRIVATE (context);
    shelf = priv->shelf;
    if (!shelf) return results;

    names = tomoe_shelf_get_dict_names(shelf);
    if (!names) return results;

    for (name = names; name; name = name->next) {
        TomoeDict *dict;
        dict = tomoe_shelf_get_dict(shelf, name->data);
        results = g_list_concat (tomoe_dict_search (dict, query), results);
    }
    results = g_list_sort (results, _candidate_compare_func);

    return results;
}

GList *
tomoe_context_search (TomoeContext *context, TomoeQuery *query)
{
    TomoeWriting *writing;

    writing = tomoe_query_get_writing (query);
    if (writing)
        return tomoe_context_search_by_strokes (context, writing);
    else
        return tomoe_context_search_by_dict (context, query);
}

gboolean
tomoe_context_register (TomoeContext *context, TomoeChar *chr)
{
    TomoeContextPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CONTEXT (context), FALSE);

    priv = TOMOE_CONTEXT_GET_PRIVATE (context);
    if (!priv->user_dict) {
        g_warning (_("user dictionary doesn't exist"));
        return FALSE;
    }

    return tomoe_dict_register_char (priv->user_dict, chr);
}

gboolean
tomoe_context_unregister (TomoeContext *context, const gchar *utf8)
{
    TomoeContextPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CONTEXT (context), FALSE);

    priv = TOMOE_CONTEXT_GET_PRIVATE (context);
    if (!priv->user_dict) {
        g_warning (_("user dictionary doesn't exist"));
        return FALSE;
    }

    return tomoe_dict_unregister_char (priv->user_dict, utf8);
}

TomoeChar *
tomoe_context_get_char (TomoeContext *context, const gchar *utf8)
{
    TomoeContextPrivate *priv;
    TomoeShelf *shelf;
    TomoeChar *chr = NULL;
    GList *names, *node;

    g_return_val_if_fail (TOMOE_IS_CONTEXT (context), chr);

    priv = TOMOE_CONTEXT_GET_PRIVATE (context);

    shelf = priv->shelf;
    if (!shelf) return chr;

    names = tomoe_shelf_get_dict_names (shelf);
    if (!names) return chr;

    for (node = names; node; node = g_list_next (node)) {
        const gchar *name = node->data;
        TomoeDict *dict;

        dict = tomoe_shelf_get_dict (shelf, name);
        chr = tomoe_dict_get_char (dict, utf8);
        if (chr)
            break;
    }

    return chr;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
