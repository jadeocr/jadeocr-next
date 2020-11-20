/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
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
 *  $Id: tomoe-char.c 1488 2007-06-16 01:16:23Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n-lib.h>

#include "tomoe-char.h"
#include "tomoe-dict.h"
#include "tomoe-xml-parser.h"
#include "glib-utils.h"

#define TOMOE_CHAR_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CHAR, TomoeCharPrivate))

typedef struct _TomoeCharPrivate	TomoeCharPrivate;
struct _TomoeCharPrivate
{
    gchar                *utf8;
    gint                  n_strokes;
    GList                *readings;
    GList                *radicals;
    TomoeWriting         *writing;
    gchar                *variant;
    GHashTable           *meta_data;
};

enum
{
    PROP_0,
    PROP_UTF8,
    PROP_N_STROKES,
    PROP_WRITING,
    PROP_VARIANT
};

G_DEFINE_TYPE (TomoeChar, tomoe_char, G_TYPE_OBJECT)

static void dispose        (GObject         *object);
static void set_property   (GObject         *object,
                            guint            prop_id,
                            const GValue    *value,
                            GParamSpec      *pspec);
static void get_property   (GObject         *object,
                            guint            prop_id,
                            GValue          *value,
                            GParamSpec      *pspec);

static void
tomoe_char_class_init (TomoeCharClass *klass)
{
    GObjectClass *gobject_class;
    GParamSpec *spec;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    spec = g_param_spec_string ("utf8",
                                N_("UTF8"),
                                N_("UTF8 encoding of the character."),
                                NULL,
                                G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (gobject_class, PROP_UTF8, spec);

    spec = g_param_spec_int ("n_strokes",
                             N_("Number of strokes"),
                             N_("Number of strokes of the character."),
                             -1, G_MAXINT32, -1,
                             G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (gobject_class, PROP_N_STROKES, spec);

    spec = g_param_spec_object ("writing",
                                N_("Writing"),
                                N_("Writing of the character."),
                                TOMOE_TYPE_WRITING,
                                G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (gobject_class, PROP_WRITING, spec);

    spec = g_param_spec_string ("variant",
                                N_("Variant"),
                                N_("Variant of the character."),
                                NULL,
                                G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (gobject_class, PROP_VARIANT, spec);

    g_type_class_add_private (gobject_class, sizeof (TomoeCharPrivate));
}

static void
tomoe_char_init (TomoeChar *chr)
{
    TomoeCharPrivate *priv = TOMOE_CHAR_GET_PRIVATE (chr);
    priv->utf8       = NULL;
    priv->n_strokes  = -1;
    priv->readings   = NULL;
    priv->radicals   = NULL;
    priv->writing    = NULL;
    priv->variant    = NULL;
    priv->meta_data  = g_hash_table_new_full(g_str_hash, g_str_equal,
                                             g_free, g_free);
}

/**
 * tomoe_char_new:
 * 
 * Create a new #TomoeChar.
 *
 * Return value: a new #TomoeChar
 */
TomoeChar*
tomoe_char_new (void)
{
    return g_object_new(TOMOE_TYPE_CHAR, NULL);
}

TomoeChar*
tomoe_char_new_from_xml_data (const gchar *data, gssize len)
{
    return _tomoe_xml_parser_parse_char_data (data, len);
}

static void
copy_meta_data (gpointer key, gpointer value, gpointer user_data)
{
    TomoeChar *chr = user_data;
    gchar *meta_key = key;
    gchar *meta_value = value;

    tomoe_char_register_meta_data (chr, meta_key, meta_value);
}

TomoeChar*
tomoe_char_dup (TomoeChar *chr)
{
    TomoeChar *new_chr;
    TomoeCharPrivate *priv;

    new_chr = tomoe_char_new ();
    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    tomoe_char_set_utf8 (new_chr, priv->utf8);
    tomoe_char_set_n_strokes (new_chr, priv->n_strokes);

    if (priv->writing) {
        TomoeWriting *new_writing = tomoe_writing_dup (priv->writing);
        tomoe_char_set_writing (new_chr, new_writing);
        g_object_unref (new_writing);
    }

    if (priv->variant)
        tomoe_char_set_variant (new_chr, priv->variant);

    if (priv->readings) {
        GList *node; 
        for (node = g_list_last(priv->readings); node; node = g_list_previous (node)) {
            TomoeReading *new_reading = tomoe_reading_dup (TOMOE_READING(node->data));
            tomoe_char_add_reading (new_chr, new_reading);
            g_object_unref (new_reading);
        }
    }

    if (priv->radicals) {
        GList *node; 
        for (node = g_list_last(priv->radicals); node; node = g_list_previous (node)) {
            tomoe_char_add_radical (new_chr, node->data);
        }
    }

    if (priv->meta_data) {
        tomoe_char_meta_data_foreach (chr, (GHFunc) copy_meta_data, new_chr);
    }

    return new_chr;
}

static void
dispose (GObject *object)
{
    TomoeCharPrivate *priv = TOMOE_CHAR_GET_PRIVATE (object);

    if (priv->utf8)
        g_free (priv->utf8);
    if (priv->readings) {
        g_list_foreach (priv->readings, (GFunc)g_object_unref, NULL);
        g_list_free (priv->readings);
    }
    if (priv->radicals) {
        g_list_foreach (priv->radicals, (GFunc)g_free, NULL);
        g_list_free (priv->radicals);
    }
    if (priv->writing)
        g_object_unref (G_OBJECT (priv->writing));
    if (priv->variant)
        g_free (priv->variant);
    if (priv->meta_data)
        g_hash_table_destroy (priv->meta_data);

    priv->utf8      = NULL;
    priv->n_strokes = -1;
    priv->readings  = NULL;
    priv->radicals  = NULL;
    priv->writing   = NULL;
    priv->variant   = NULL;
    priv->meta_data = NULL;

    G_OBJECT_CLASS (tomoe_char_parent_class)->dispose (object);
}

static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
    TomoeChar *chr;

    chr = TOMOE_CHAR(object);
    switch (prop_id) {
      case PROP_UTF8:
        tomoe_char_set_utf8 (chr, g_value_get_string (value));
        break;
      case PROP_N_STROKES:
        tomoe_char_set_n_strokes (chr, g_value_get_int (value));
        break;
      case PROP_WRITING:
        tomoe_char_set_writing (chr, g_value_get_object (value));
        break;
      case PROP_VARIANT:
        tomoe_char_set_variant (chr, g_value_get_string (value));
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
    TomoeChar *chr;
    TomoeCharPrivate *priv;

    chr = TOMOE_CHAR (object);
    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    switch (prop_id) {
      case PROP_UTF8:
        g_value_set_string (value, priv->utf8);
        break;
      case PROP_N_STROKES:
        g_value_set_int (value, priv->n_strokes);
        break;
      case PROP_WRITING:
        g_value_set_object (value, priv->writing);
        break;
      case PROP_VARIANT:
        g_value_set_string (value, priv->variant);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

const gchar *
tomoe_char_get_utf8 (TomoeChar *chr)
{
    TomoeCharPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    return priv->utf8;
}

void
tomoe_char_set_utf8 (TomoeChar *chr, const gchar *utf8)
{
    TomoeCharPrivate *priv;

    g_return_if_fail (TOMOE_IS_CHAR (chr));

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    if (priv->utf8)
        g_free (priv->utf8);
    priv->utf8 = utf8 ? g_strdup (utf8) : NULL;
}

gint
tomoe_char_get_n_strokes (TomoeChar *chr)
{
    TomoeCharPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), 0);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    return priv->n_strokes;
}

void
tomoe_char_set_n_strokes (TomoeChar *chr, gint n_strokes)
{
    TomoeCharPrivate *priv;

    g_return_if_fail (TOMOE_IS_CHAR (chr));

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    priv->n_strokes = n_strokes;
}

const GList *
tomoe_char_get_readings (TomoeChar* chr)
{
    TomoeCharPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    return priv->readings;
}

void
tomoe_char_add_reading (TomoeChar* chr, TomoeReading *reading)
{
    TomoeCharPrivate *priv;

    g_return_if_fail (TOMOE_IS_CHAR (chr));

    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    priv->readings = g_list_prepend (priv->readings, g_object_ref (reading));
}

const GList *
tomoe_char_get_radicals (TomoeChar* chr)
{
    TomoeCharPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    return priv->radicals;
}

void
tomoe_char_add_radical (TomoeChar* chr, const gchar *radical)
{
    TomoeCharPrivate *priv;

    g_return_if_fail (TOMOE_IS_CHAR (chr));
    g_return_if_fail (radical && radical[0] != '\0');

    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    priv->radicals = g_list_prepend (priv->radicals, g_strdup (radical));
}

TomoeWriting *
tomoe_char_get_writing (TomoeChar *chr)
{
    TomoeCharPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    return priv->writing;
}

void
tomoe_char_set_writing (TomoeChar *chr, TomoeWriting *writing)
{
    TomoeCharPrivate *priv;

    g_return_if_fail (TOMOE_IS_CHAR (chr));

    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    if (priv->writing)
        g_object_unref (G_OBJECT (priv->writing));
    priv->writing = g_object_ref (writing);
}

const gchar *
tomoe_char_get_variant (TomoeChar *chr)
{
    TomoeCharPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    return priv->variant;
}

void
tomoe_char_set_variant (TomoeChar *chr, const gchar *variant)
{
    TomoeCharPrivate *priv;

    g_return_if_fail (TOMOE_IS_CHAR (chr));

    priv = TOMOE_CHAR_GET_PRIVATE (chr);

    if (priv->variant)
        g_free (priv->variant);
    priv->variant = variant ? g_strdup (variant) : NULL;
}

void
tomoe_char_register_meta_data (TomoeChar *chr, const gchar *key,
                               const gchar *value)
{
    TomoeCharPrivate *priv;
    g_return_if_fail (chr);
    g_return_if_fail (key);
    g_return_if_fail (value);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    g_hash_table_insert (priv->meta_data, g_strdup (key), g_strdup (value));
}

const gchar*
tomoe_char_get_meta_data (TomoeChar* chr, const gchar *key)
{
    TomoeCharPrivate *priv;
    g_return_val_if_fail (chr, NULL);
    g_return_val_if_fail (key, NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    return g_hash_table_lookup (priv->meta_data, key);
}

gboolean
tomoe_char_has_meta_data (TomoeChar *chr)
{
    TomoeCharPrivate *priv;
    g_return_val_if_fail (chr, FALSE);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    return g_hash_table_size (priv->meta_data) > 0;
}

void
tomoe_char_meta_data_foreach (TomoeChar* chr, GHFunc func, gpointer user_data)
{
    TomoeCharPrivate *priv;
    g_return_if_fail (chr);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    g_hash_table_foreach (priv->meta_data, func, user_data);
}

/**
 * tomoe_char_compare:
 * @a: a TomoeChar object.
 * @b: a TomoeChar object to compare with.
 *
 * Compare to TomoeChar objects with its own utf8 character.
 *
 * Return value: -1 a < b, 0 a= b, 1 a > b
 */
gint
tomoe_char_compare (const TomoeChar *a, const TomoeChar *b)
{
    TomoeCharPrivate *priv_a, *priv_b;

    if (!a || !b) return 0;

    priv_a = TOMOE_CHAR_GET_PRIVATE (a);
    priv_b = TOMOE_CHAR_GET_PRIVATE (b);
    if (!priv_a || !priv_b) return 0;

    if (!priv_a->utf8 || !priv_b->utf8) return 0;
    return strcmp (priv_a->utf8, priv_b->utf8);
}


static void
tomoe_char_to_xml_utf8 (TomoeChar *chr, TomoeCharPrivate *priv,
                        GString *output)
{
    gchar *xml;

    if (!priv->utf8) return;

    xml = g_markup_printf_escaped ("    <utf8>%s</utf8>\n", priv->utf8);
    g_string_append (output, xml);
    g_free (xml);
}

static void
tomoe_char_to_xml_variant (TomoeChar *chr, TomoeCharPrivate *priv,
                           GString *output)
{
    gchar *xml;

    if (!priv->variant) return;

    xml = g_markup_printf_escaped ("    <variant>%s</variant>\n",
                                   priv->variant);
    g_string_append (output, xml);
    g_free (xml);
}

static void
tomoe_char_to_xml_readings (TomoeChar *chr, TomoeCharPrivate *priv,
                            GString *output)
{
    GList *node;

    if (!priv->readings) return;

    g_string_append (output, "    <readings>\n");
    for (node = g_list_last (priv->readings); node; node = g_list_previous (node)) {
        TomoeReading *reading = node->data;
        gchar *xml;

        xml = tomoe_reading_to_xml (reading);
        if (xml) {
            g_string_append (output, xml);
            g_free (xml);
        }
    }
    g_string_append (output, "    </readings>\n");
}

static void
tomoe_char_to_xml_radicals (TomoeChar *chr, TomoeCharPrivate *priv,
                            GString *output)
{
    GList *node;

    if (!priv->radicals) return;

    g_string_append (output, "    <radicals>\n");
    for (node = priv->radicals; node; node = g_list_next (node)) {
        const gchar *radical = node->data;
        gchar *xml;

        xml = g_markup_printf_escaped (radical, -1);
        g_string_append_printf (output, "      <radical>%s</radical>\n", xml);
        g_free (xml);
    }
    g_string_append (output, "    </radicals>\n");
}

static void
tomoe_char_to_xml_writing (TomoeChar *chr, TomoeCharPrivate *priv,
                           GString *output)
{
    gchar *xml;

    if (!priv->writing) return;

    xml = tomoe_writing_to_xml (priv->writing);

    if (xml && xml[0] != '\0') {
        g_string_append (output, xml);
        g_free (xml);
    }
}

static void
tomoe_char_to_xml_n_strokes (TomoeChar *chr, TomoeCharPrivate *priv,
                             GString *output)
{
    if (priv->n_strokes < 0) return;

    g_string_append_printf (
        output,
        "    <number-of-strokes>%d</number-of-strokes>\n",
        priv->n_strokes);
}


static void
tomoe_char_to_xml_meta_datum (gpointer key, gpointer value, gpointer user_data)
{
    GString *output = user_data;
    gchar *meta_key = key;
    gchar *meta_value = value;
    gchar *result;

    result = g_markup_printf_escaped ("      <%s>%s</%s>\n",
                                      meta_key, meta_value, meta_key);
    g_string_append (output, result);
    g_free (result);
}

static void
tomoe_char_to_xml_meta (TomoeChar *chr, TomoeCharPrivate *priv, GString *output)
{
    if (!tomoe_char_has_meta_data (chr)) return;

    g_string_append (output, "    <meta>\n");
    tomoe_char_meta_data_foreach (chr, tomoe_char_to_xml_meta_datum, output);
    g_string_append (output, "    </meta>\n");
}

gchar *
tomoe_char_to_xml (TomoeChar* chr)
{
    TomoeCharPrivate *priv;
    GString *output;

    g_return_val_if_fail (TOMOE_IS_CHAR (chr), NULL);

    priv = TOMOE_CHAR_GET_PRIVATE (chr);
    output = g_string_new ("");

    tomoe_char_to_xml_utf8 (chr, priv, output);
    tomoe_char_to_xml_variant (chr, priv, output);
    tomoe_char_to_xml_readings (chr, priv, output);
    tomoe_char_to_xml_radicals (chr, priv, output);
    tomoe_char_to_xml_n_strokes (chr, priv, output);
    tomoe_char_to_xml_writing (chr, priv, output);
    tomoe_char_to_xml_meta (chr, priv, output);

    if (output->len > 0) {
        g_string_prepend (output, "  <character>\n");
        g_string_append (output, "  </character>\n");
    }

    return g_string_free (output, FALSE);
}


/*
vi:ts=4:nowrap:ai:expandtab
*/
