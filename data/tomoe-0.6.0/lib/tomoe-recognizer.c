/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 *  Copyright (C) 2006-2007 Kouhei Sutou <kou@cozmixng.org>
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
 *  $Id: tomoe-recognizer.c 1212 2007-04-25 07:35:56Z makeinu $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <glib/gi18n-lib.h>

#include "tomoe-module.h"
#include "tomoe-recognizer.h"

static GList *recognizers = NULL;
static gchar *module_dir = NULL;

void tomoe_recognizer_init (void)
{
}

void tomoe_recognizer_quit (void)
{
    tomoe_recognizer_unload ();
    if (module_dir)
        g_free (module_dir);
    module_dir = NULL;
}

const gchar *
tomoe_recognizer_get_default_module_dir (void)
{
    return module_dir;
}

void
tomoe_recognizer_set_default_module_dir (const gchar *dir)
{
    if (module_dir)
        g_free (module_dir);
    module_dir = NULL;

    if (dir)
        module_dir = g_strdup (dir);
}

static const gchar *
_tomoe_recognizer_module_dir (void)
{
    const gchar *dir;
    if (module_dir)
        return module_dir;

    dir = g_getenv ("TOMOE_RECOGNIZER_MODULE_DIR");
    if (dir)
        return dir;

    return RECOGNIZER_MODULEDIR;
}

void
tomoe_recognizer_load (const gchar *base_dir)
{
    if (!base_dir)
        base_dir = _tomoe_recognizer_module_dir ();

    recognizers = g_list_concat (tomoe_module_load_modules (base_dir),
                                 recognizers);
}

TomoeModule *
tomoe_recognizer_load_module (const gchar *name)
{
    TomoeModule *module;

    module = tomoe_module_find (recognizers, name);
    if (module)
        return module;

    module = tomoe_module_load_module (_tomoe_recognizer_module_dir (), name);
    if (module) {
        if (g_type_module_use (G_TYPE_MODULE (module))) {
            recognizers = g_list_prepend (recognizers, module);
            g_type_module_unuse (G_TYPE_MODULE (module));
        }
    }

    return module;
}

void
tomoe_recognizer_unload (void)
{
    g_list_foreach (recognizers, (GFunc) tomoe_module_unload, NULL);
    g_list_free (recognizers);
    recognizers = NULL;
}

GList *
tomoe_recognizer_get_registered_types (void)
{
    return tomoe_module_collect_registered_types (recognizers);
}

GList *
tomoe_recognizer_get_log_domains (void)
{
    return tomoe_module_collect_log_domains (recognizers);
}


#define TOMOE_RECOGNIZER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_RECOGNIZER, TomoeRecognizerPrivate))

typedef struct _TomoeRecognizerPrivate  TomoeRecognizerPrivate;
struct _TomoeRecognizerPrivate
{
    gchar   *language;
};

enum {
    PROP_0,
    PROP_LANGUAGE
};

#define tomoe_recognizer_init init
G_DEFINE_ABSTRACT_TYPE (TomoeRecognizer, tomoe_recognizer, G_TYPE_OBJECT)
#undef tomoe_recognizer_init

static void         dispose                 (GObject       *object);
static void         set_property            (GObject       *object,
                                             guint         prop_id,
                                             const GValue  *value,
                                             GParamSpec    *pspec);
static void         get_property            (GObject       *object,
                                             guint          prop_id,
                                             GValue        *value,
                                             GParamSpec    *pspec);

static void
tomoe_recognizer_class_init (TomoeRecognizerClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);
    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    klass->search = NULL;
    klass->is_available = NULL;

    g_object_class_install_property (
        gobject_class,
        PROP_LANGUAGE,
        g_param_spec_string (
            "language",
            _("Language"),
            _("The language of the recognizer"),
            NULL,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_type_class_add_private (gobject_class, sizeof (TomoeRecognizerPrivate));
}

static void
init (TomoeRecognizer *recognizer)
{
    TomoeRecognizerPrivate *priv = TOMOE_RECOGNIZER_GET_PRIVATE (recognizer);

    priv->language = NULL;
}

static void
dispose (GObject *object)
{
    TomoeRecognizerPrivate *priv = TOMOE_RECOGNIZER_GET_PRIVATE (object);

    if (priv->language)
        g_free (priv->language);

    priv->language  = NULL;

    G_OBJECT_CLASS (tomoe_recognizer_parent_class)->dispose (object);
}

static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
    TomoeRecognizer *recognizer;
    TomoeRecognizerPrivate *priv;

    recognizer = TOMOE_RECOGNIZER(object);
    priv = TOMOE_RECOGNIZER_GET_PRIVATE (recognizer);

    switch (prop_id) {
      case PROP_LANGUAGE:
        if (priv->language)
            g_free(priv->language);
        priv->language = g_value_dup_string (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
get_property (GObject    *object,
              guint       prop_id,
              GValue     *value,
              GParamSpec *pspec)
{
    TomoeRecognizer *recognizer;
    TomoeRecognizerPrivate *priv;

    recognizer = TOMOE_RECOGNIZER (object);
    priv = TOMOE_RECOGNIZER_GET_PRIVATE (recognizer);

    switch (prop_id) {
      case PROP_LANGUAGE:
        g_value_set_string (value, priv->language);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

/**
 * tomoe_recognizer_new:
 * @name: The name of recognizer type.
 * @first_property: the name of the first property.
 * @... :  the value of the first property, followed optionally by more name/value pairs, followed by NULL
 *
 * Create a new #TomoeRecognizer.
 *
 * Return value: a new #TomoeRecognizer.
 */
TomoeRecognizer *
tomoe_recognizer_new (const gchar *name, const gchar *first_property, ...)
{
    TomoeModule *module;
    GObject *recognizer;
    va_list var_args;

    module = tomoe_recognizer_load_module (name);
    g_return_val_if_fail (module != NULL, NULL);

    va_start (var_args, first_property);
    recognizer = tomoe_module_instantiate (module, first_property, var_args);
    va_end (var_args);

    return TOMOE_RECOGNIZER (recognizer);
}

/**
 * tomoe_recognizer_search:
 * @recognizer: a TomoeRecognizer object.
 * @input: a #TomoeWriting object for matching.
 *
 * Match strokes of TomoeChar with TomoeWriting.
 *
 * Return value: A newly-allocated list of
 * #TomoeCandidate. The each #TomoeCandidate should be also
 * freed with g_object_unref.
  */
GList *
tomoe_recognizer_search (TomoeRecognizer *recognizer, TomoeWriting *input)
{
    TomoeRecognizerClass *klass;

    g_return_val_if_fail (TOMOE_IS_RECOGNIZER (recognizer), NULL);

    klass = TOMOE_RECOGNIZER_GET_CLASS (recognizer);
    if (klass->search)
        return klass->search (recognizer, input);
    else
        return NULL;
}

/**
 * tomoe_recognizer_get_lanaugage:
 * @recognizer: a TomoeRecognizer object.
 *
 * Return value: A language used by the recognizer or %NULL. The
 * value should not free.
  */
const gchar *
tomoe_recognizer_get_language (TomoeRecognizer *recognizer)
{
    g_return_val_if_fail (TOMOE_IS_RECOGNIZER (recognizer), NULL);

    return TOMOE_RECOGNIZER_GET_PRIVATE (recognizer)->language;
}

/**
 * tomoe_recognizer_is_available:
 * @recognizer: a TomoeRecognizer object.
 *
 * Return value: %TRUE if the recognizer is available,
 * %FALSE otherwise.
  */
gboolean
tomoe_recognizer_is_available (TomoeRecognizer *recognizer)
{
    TomoeRecognizerClass *klass;

    g_return_val_if_fail (TOMOE_IS_RECOGNIZER (recognizer), FALSE);

    klass = TOMOE_RECOGNIZER_GET_CLASS (recognizer);
    if (klass->is_available)
        return klass->is_available (recognizer);
    else
        return FALSE;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
