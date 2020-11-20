/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2000 - 2004 Hiroyuki Komatsu <komatsu@taiyaki.org>
 *  Copyright (C) 2004 Hiroaki Nakamura <hnakamur@good-day.co.jp>
 *  Copyright (C) 2005 Hiroyuki Ikezoe <poincare@ikezoe.net>
 *  Copyright (C) 2005 Takuro Ashie <ashie@homa.ne.jp>
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
 *  $Id: tomoe-dict-xml.c 1533 2007-06-20 11:26:27Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gmodule.h>

#include <tomoe-module-impl.h>
#include <tomoe-dict.h>
#include <tomoe-candidate.h>
#include <tomoe-xml-parser.h>
#include <glib-utils.h>

#include "tomoe-dict-ptr-array.h"

#define TOMOE_TYPE_DICT_XML            tomoe_type_dict_xml
#define TOMOE_DICT_XML(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICT_XML, TomoeDictXML))
#define TOMOE_DICT_XML_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICT_XML, TomoeDictXMLClass))
#define TOMOE_IS_DICT_XML(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICT_XML))
#define TOMOE_IS_DICT_XML_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICT_XML))
#define TOMOE_DICT_XML_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_DICT_XML, TomoeDictXMLClass))

enum {
    PROP_0,
    PROP_FILENAME
};

typedef struct _TomoeDictXML TomoeDictXML;
typedef struct _TomoeDictXMLClass TomoeDictXMLClass;
struct _TomoeDictXML
{
    TomoeDictPtrArray    object;
    gchar               *filename;
    gchar               *name;
};

struct _TomoeDictXMLClass
{
    TomoeDictPtrArrayClass parent_class;
};

static GType tomoe_type_dict_xml = 0;
static TomoeDictPtrArrayClass *parent_class;

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
static gboolean     flush                     (TomoeDict     *dict);
static gboolean     is_available              (TomoeDict     *dict);
static gboolean     tomoe_dict_xml_load       (TomoeDictXML  *dict);
static gboolean     tomoe_dict_xml_save       (TomoeDictXML  *dict);

static void
class_init (TomoeDictXMLClass *klass)
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
    dict_class->flush           = flush;
    dict_class->is_available    = is_available;

    g_object_class_install_property (
        gobject_class,
        PROP_FILENAME,
        g_param_spec_string (
            "filename",
            "Filename",
            "The filename of xml file",
            NULL,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
init (TomoeDictXML *dict)
{
    dict->filename = NULL;
    dict->name     = NULL;
}

static void
register_type (GTypeModule *type_module)
{
    static const GTypeInfo info =
        {
            sizeof (TomoeDictXMLClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDictXML),
            0,
            (GInstanceInitFunc) init,
        };

    tomoe_type_dict_xml = g_type_module_register_type (type_module,
                                                       TOMOE_TYPE_DICT_PTR_ARRAY,
                                                       "TomoeDictXML",
                                                       &info, 0);
}

G_MODULE_EXPORT GList *
TOMOE_MODULE_IMPL_INIT (GTypeModule *type_module)
{
    GList *registered_types = NULL;

    register_type (type_module);
    if (tomoe_type_dict_xml)
        registered_types =
            g_list_prepend (registered_types,
                            (gchar *) g_type_name (tomoe_type_dict_xml));

    return registered_types;
}

G_MODULE_EXPORT void
TOMOE_MODULE_IMPL_EXIT (void)
{
}

G_MODULE_EXPORT GObject *
TOMOE_MODULE_IMPL_INSTANTIATE (const gchar *first_property, va_list args)
{
    return g_object_new_valist (TOMOE_TYPE_DICT_XML, first_property, args);
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
    TomoeDictXML *dict;

    object = klass->constructor (type, n_props, props);
    dict = TOMOE_DICT_XML (object);

    tomoe_dict_xml_load (dict);

    return object;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeDictXML *dict = TOMOE_DICT_XML (object);

    switch (prop_id) {
    case PROP_FILENAME:
        dict->filename = g_value_dup_string (value);
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
    TomoeDictXML *dict = TOMOE_DICT_XML (object);

    switch (prop_id) {
    case PROP_FILENAME:
        g_value_set_string (value, dict->filename);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeDictXML *dict;

    dict = TOMOE_DICT_XML(object);

    flush (TOMOE_DICT (dict));

    if (dict->name)
        g_free (dict->name);
    if (dict->filename)
        g_free (dict->filename);

    dict->name     = NULL;
    dict->filename = NULL;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static const gchar*
get_name (TomoeDict *_dict)
{
    TomoeDictXML *dict = TOMOE_DICT_XML (_dict);
    g_return_val_if_fail (TOMOE_IS_DICT_XML (dict), NULL);
    return dict->name;
}

static gboolean
flush (TomoeDict *_dict)
{
    TomoeDictXML *dict = TOMOE_DICT_XML (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_XML (dict), FALSE);

    if (!dict->filename)
        return FALSE;

    return tomoe_dict_xml_save (dict);
}

static gboolean
is_available (TomoeDict *_dict)
{
    TomoeDictXML *dict = TOMOE_DICT_XML (_dict);

    g_return_val_if_fail (TOMOE_IS_DICT_XML (dict), FALSE);

    if (tomoe_dict_is_editable (_dict) && !dict->filename)
        return FALSE;

    if (!tomoe_dict_is_editable (_dict) && dict->filename &&
        !g_file_test (dict->filename, G_FILE_TEST_EXISTS))
        return FALSE;

    return TRUE;
}

static gboolean
tomoe_dict_xml_load (TomoeDictXML *dict)
{
    gboolean success = TRUE;
    TomoeXMLParsedData result;

    if (!g_file_test (dict->filename, G_FILE_TEST_EXISTS))
        return success;

    result.name = NULL;
    result.chars = _tomoe_dict_ptr_array_get_array (TOMOE_DICT_PTR_ARRAY (dict));
    success = _tomoe_xml_parser_parse_dictionary_file (dict->filename, &result);
    if (result.name) {
        g_free (dict->name);
        dict->name = g_strdup (result.name);
        g_free (result.name);
    }
    _tomoe_dict_ptr_array_sort (TOMOE_DICT_PTR_ARRAY (dict));

    return success;
}

static gboolean
tomoe_dict_xml_save (TomoeDictXML *dict)
{
    GString *xml;
    GError *error = NULL;
    gboolean success;
    gboolean modified = FALSE;
    guint i;
    GPtrArray *chars;

    g_return_val_if_fail (TOMOE_IS_DICT_XML (dict), FALSE);

    if (!tomoe_dict_is_editable (TOMOE_DICT (dict))) return FALSE;

    g_return_val_if_fail (dict->filename, FALSE);

    g_object_get (dict, "modified", &modified, NULL);
    if (!modified) return TRUE;

    xml = g_string_new (
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        "<!DOCTYPE dictionary SYSTEM \"" DATADIR "/dict.dtd\">\n");
    if (dict->name)
        g_string_append_printf (xml,
                                "<dictionary name=\"%s\">\n", dict->name);
    else
        g_string_append (xml, "<dictionary>\n");

    chars = _tomoe_dict_ptr_array_get_array (TOMOE_DICT_PTR_ARRAY (dict));
    for (i = 0; i < chars->len; i++) {
        gchar *chr_xml;
        TomoeChar *chr = g_ptr_array_index (chars, i);

        chr_xml = tomoe_char_to_xml (chr);
        if (chr_xml) {
            g_string_append (xml, chr_xml);
            g_free (chr_xml);
        }
    }

    g_string_append (xml, "</dictionary>\n");

    success = g_file_set_contents (dict->filename, xml->str, xml->len, &error);
    if (success)
        g_object_set (dict, "modified", FALSE, NULL);
    else
        TOMOE_HANDLE_ERROR (error);

    g_string_free (xml, TRUE);
    return success;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
