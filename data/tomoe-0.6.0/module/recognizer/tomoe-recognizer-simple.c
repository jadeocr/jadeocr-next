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
 *  $Id: tomoe-recognizer-simple.c 1212 2007-04-25 07:35:56Z makeinu $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <gmodule.h>

#include <glib/gi18n-lib.h>

#include <tomoe-module-impl.h>
#include <tomoe-recognizer.h>
#include "tomoe-recognizer-simple-logic.h"

#define TOMOE_TYPE_RECOGNIZER_SIMPLE            tomoe_type_recognizer_simple
#define TOMOE_RECOGNIZER_SIMPLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_RECOGNIZER_SIMPLE, TomoeRecognizerSimple))
#define TOMOE_RECOGNIZER_SIMPLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_RECOGNIZER_SIMPLE, TomoeRecognizerSimpleClass))
#define TOMOE_IS_RECOGNIZER_SIMPLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_RECOGNIZER_SIMPLE))
#define TOMOE_IS_RECOGNIZER_SIMPLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_RECOGNIZER_SIMPLE))
#define TOMOE_RECOGNIZER_SIMPLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_RECOGNIZER_SIMPLE, TomoeRecognizerSimpleClass))

enum {
    PROP_0,
    PROP_DICTIONARY
};

typedef struct _TomoeRecognizerSimple TomoeRecognizerSimple;
typedef struct _TomoeRecognizerSimpleClass TomoeRecognizerSimpleClass;

struct _TomoeRecognizerSimple
{
    TomoeRecognizer  object;
    TomoeDict       *dict;
};

struct _TomoeRecognizerSimpleClass
{
    TomoeRecognizerClass parent_class;
};

static GType tomoe_type_recognizer_simple = 0;
static GObjectClass *parent_class;

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
static GList       *search                    (TomoeRecognizer *recognizer,
                                               TomoeWriting    *input);
static gboolean     is_available              (TomoeRecognizer *recognizer);

static void
class_init (TomoeRecognizerSimpleClass *klass)
{
    GObjectClass *gobject_class;
    TomoeRecognizerClass *recognizer_class;

    parent_class = g_type_class_peek_parent (klass);

    gobject_class = G_OBJECT_CLASS (klass);
    gobject_class->constructor  = constructor;
    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    recognizer_class = TOMOE_RECOGNIZER_CLASS (klass);
    recognizer_class->search = search;
    recognizer_class->is_available = is_available;

    g_object_class_install_property (
        gobject_class,
        PROP_DICTIONARY,
        g_param_spec_object (
            "dictionary",
            _("Dictionary"),
            _("The dictionary of the recognizer"),
            TOMOE_TYPE_DICT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
init (TomoeRecognizerSimple *recognizer)
{
}

static void
register_type (GTypeModule *type_module)
{
    static const GTypeInfo info =
        {
            sizeof (TomoeRecognizerSimpleClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeRecognizerSimple),
            0,
            (GInstanceInitFunc) init,
        };

    tomoe_type_recognizer_simple =
        g_type_module_register_type (type_module,
                                     TOMOE_TYPE_RECOGNIZER,
                                     "TomoeRecognizerSimple",
                                     &info, 0);
}

G_MODULE_EXPORT GList *
TOMOE_MODULE_IMPL_INIT (GTypeModule *type_module)
{
    GList *registered_types = NULL;

    register_type (type_module);
    if (tomoe_type_recognizer_simple) {
        gchar *name = (gchar *) g_type_name (tomoe_type_recognizer_simple);
        registered_types = g_list_prepend (registered_types, name);
    }

    return registered_types;
}

G_MODULE_EXPORT void
TOMOE_MODULE_IMPL_EXIT (void)
{
}

G_MODULE_EXPORT GObject *
TOMOE_MODULE_IMPL_INSTANTIATE (const gchar *first_property, va_list var_args)
{
    return g_object_new_valist (TOMOE_TYPE_RECOGNIZER_SIMPLE,
                                first_property, var_args);
}

G_MODULE_EXPORT gchar *
TOMOE_MODULE_IMPL_GET_LOG_DOMAIN (void)
{
    return g_strdup (G_LOG_DOMAIN);
}


static GObject *
constructor (GType type, guint n_props,
             GObjectConstructParam *props)
{
    GObject *object;
    GObjectClass *klass = G_OBJECT_CLASS (parent_class);
    TomoeRecognizerSimple *recognizer;

    object = klass->constructor (type, n_props, props);
    recognizer = TOMOE_RECOGNIZER_SIMPLE (object);

    if (!recognizer->dict) {
        const gchar *language;
        gchar *dict_name;
        gchar *filename;

        language = tomoe_recognizer_get_language (TOMOE_RECOGNIZER (object));
        if (language)
            dict_name = g_strconcat ("handwriting-", language, ".xml", NULL);
        else
            dict_name = g_strdup ("handwriting.xml");

        filename = g_build_filename (RECOGNIZER_DATADIR, dict_name, NULL);
        recognizer->dict = tomoe_dict_new ("xml",
                                           "filename", filename,
                                           NULL);
        if (recognizer->dict && !tomoe_dict_is_available (recognizer->dict)) {
            g_object_unref (recognizer->dict);
            recognizer->dict = NULL;
        }

        g_free(dict_name);
        g_free(filename);
    }

    return object;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeRecognizerSimple *recognizer = TOMOE_RECOGNIZER_SIMPLE (object);

    switch (prop_id) {
      case PROP_DICTIONARY:
        if (recognizer->dict)
            g_object_unref (recognizer->dict);
        recognizer->dict = g_value_get_object (value);
        if (recognizer->dict)
            g_object_ref (recognizer->dict);
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
    TomoeRecognizerSimple *recognizer = TOMOE_RECOGNIZER_SIMPLE (object);

    switch (prop_id) {
      case PROP_DICTIONARY:
        g_value_set_object (value, recognizer->dict);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeRecognizerSimple *recognizer;

    recognizer = TOMOE_RECOGNIZER_SIMPLE (object);

    if (recognizer->dict)
        g_object_unref (recognizer->dict);

    recognizer->dict = NULL;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static GList *
search (TomoeRecognizer *_recognizer, TomoeWriting *input)
{
    TomoeRecognizerSimple *recognizer;

    recognizer = TOMOE_RECOGNIZER_SIMPLE (_recognizer);
    return _tomoe_recognizer_simple_get_candidates (_recognizer,
                                                    recognizer->dict,
                                                    input);
}

static gboolean
is_available (TomoeRecognizer *_recognizer)
{
    TomoeRecognizerSimple *recognizer;

    recognizer = TOMOE_RECOGNIZER_SIMPLE (_recognizer);
    return recognizer->dict != NULL;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
