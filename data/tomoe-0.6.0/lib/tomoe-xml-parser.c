/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2006 Takuro Ashie <ashie@homa.ne.jp>
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
 *  $Id: tomoe-xml-parser.c 1067 2006-12-22 05:26:03Z kous $
 */

#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include <tomoe-xml-parser.h>
#include <glib-utils.h>

typedef enum {
    STATE_NONE,
    STATE_UTF8,
    STATE_VARIANT,
    STATE_N_STROKES,
    STATE_STROKES,
    STATE_READINGS,
    STATE_READING,
    STATE_RADICALS,
    STATE_RADICAL,
    STATE_WRITING,
    STATE_STROKE,
    STATE_POINT,
    STATE_META
} TomoeCharState;

typedef struct _ParseData
{
    TomoeXMLParsedData *result;
    gboolean            in_dict;
    TomoeCharState      state;

    TomoeChar          *chr;
    TomoeWriting       *writing;
    gint                n_points;
    TomoeReadingType    reading_type;

    const gchar        *filename;

    /* meta data */
    gchar *key;
    gchar *value;
} ParseData;

static void
set_parse_error (GMarkupParseContext *context, GError **error,
                 ParseData *data)
{
    gchar buf[1024];
    gint line = 0, chr = 0;

    if (!error) return;

    g_markup_parse_context_get_position (context, &line, &chr);

    g_snprintf (buf, G_N_ELEMENTS (buf),
                "Invalid content at line %d char %d of %s.",
                line, chr, data->filename);

    *error = g_error_new (G_MARKUP_ERROR,
                          G_MARKUP_ERROR_INVALID_CONTENT,
                          buf);
}

static void
start_element_handler (GMarkupParseContext *context,
                       const gchar         *element_name,
                       const gchar        **attr_names,
                       const gchar        **attr_values,
                       gpointer             user_data,
                       GError             **error)
{
    ParseData *data = user_data;

    if (!strcmp ("dictionary", element_name)) {
        gint idx;

        for (idx = 0; attr_names && attr_names[idx]; idx++) {
            if (!strcmp ("name", attr_names[idx])) {
                g_free (data->result->name);
                data->result->name = g_strdup (attr_values[idx]);
            }
        }
        data->in_dict = TRUE;
        return;
    }

    if (!data->in_dict) {
        set_parse_error (context, error, data);
        return;
    }

    if (!strcmp ("character", element_name)) {
        data->chr = tomoe_char_new ();
        return;
    }

    if (!data->chr) {
        set_parse_error (context, error, data);
        return;
    }

    if (!strcmp ("utf8", element_name)) {
        data->state = STATE_UTF8;
        return;
    }

    if (!strcmp ("variant", element_name)) {
        data->state = STATE_VARIANT;
        return;
    }

    if (!strcmp ("number-of-strokes", element_name)) {
        data->state = STATE_N_STROKES;
        return;
    }

    if (!strcmp ("strokes", element_name)) {
        data->state = STATE_WRITING;
        data->writing = tomoe_writing_new ();
        return;
    }

    if (!strcmp ("stroke", element_name)) {
        if (!data->writing) {
            set_parse_error (context, error, data);
            return;
        }
        data->state = STATE_STROKE;
        data->n_points = 0;
        return;
    }

    if (!strcmp ("point", element_name)) {
        gint idx, x = -1, y = -1;

        if (data->state != STATE_STROKE) {
            set_parse_error (context, error, data);
            return;
        }

        data->state = STATE_POINT;

        for (idx = 0; attr_names && attr_names[idx]; idx++) {
            if (!strcmp ("x", attr_names[idx])) {
                x = atoi (attr_values[idx]);
            } else if (!strcmp ("y", attr_names[idx])) {
                y = atoi (attr_values[idx]);
            }
        }

        if (x < 0 || y < 0 || x >= 1000 || y >= 1000) {
            g_warning ("Invalid writing data: %s: x = %d, y = %d\n",
                       tomoe_char_get_utf8 (data->chr), x, y);
        }

        if (data->n_points == 0)
            tomoe_writing_move_to (data->writing, x, y);
        else
            tomoe_writing_line_to (data->writing, x, y);

        data->n_points++;
        return;
    }

    if (!strcmp ("readings", element_name)) {
        data->state = STATE_READINGS;
        return;
    }

    if (!strcmp ("reading", element_name)) {
        gint idx;

        if (data->state != STATE_READINGS) {
            set_parse_error (context, error, data);
            return;
        }

        data->state = STATE_READING;
        data->reading_type = TOMOE_READING_UNKNOWN;

        for (idx = 0; attr_names && attr_names[idx]; idx++) {
            if (!strcmp ("type", attr_names[idx])) {
                if (!strcmp ("ja_on", attr_values[idx]))
                    data->reading_type = TOMOE_READING_JA_ON;
                else if (!strcmp ("ja_kun", attr_values[idx]))
                    data->reading_type = TOMOE_READING_JA_KUN;
            }
        }

        return;
    }

    if (!strcmp ("radicals", element_name)) {
        data->state = STATE_RADICALS;
        return;
    }

    if (!strcmp ("radical", element_name)) {
        if (data->state != STATE_RADICALS) {
            set_parse_error (context, error, data);
            return;
        }

        data->state = STATE_RADICAL;
        return;
    }

    if (!strcmp ("meta", element_name)) {
        data->state = STATE_META;
        return;
    }

    if (data->state == STATE_META) {
        g_free (data->key);
        g_free (data->value);
        data->key   = g_strdup (element_name);
        data->value = NULL;
    }

    /* throw error? */
}

static void
end_element_handler (GMarkupParseContext *context,
                     const gchar         *element_name,
                     gpointer             user_data,
                     GError             **error)
{
    ParseData *data = user_data;

    if (!strcmp ("dictionary", element_name)) {
        data->in_dict = FALSE;
        return;
    }

    if (!strcmp ("character", element_name)) {
        if (tomoe_char_get_utf8 (data->chr))
            g_ptr_array_add (data->result->chars, data->chr);
        else
            g_object_unref (G_OBJECT (data->chr));
        data->chr = NULL;
        return;
    }

    if (!strcmp("utf8", element_name)) {
        data->state = STATE_NONE;
        return;
    }

    if (!strcmp("variant", element_name)) {
        data->state = STATE_NONE;
        return;
    }

    if (!strcmp ("number-of-strokes", element_name)) {
        data->state = STATE_NONE;
        return;
    }

    if (!strcmp ("strokes", element_name)) {
        if (data->chr && data->writing)
            tomoe_char_set_writing (data->chr, data->writing);
        data->writing = NULL;
        data->state = STATE_NONE;
        return;
    }

    if (!strcmp ("stroke", element_name)) {
        data->state = STATE_WRITING;
        data->n_points = 0;
        return;
    }

    if (!strcmp ("point", element_name)) {
        data->state = STATE_STROKE;
        return;
    }

    if (!strcmp ("readings", element_name)) {
        data->state = STATE_NONE;
        data->reading_type = TOMOE_READING_INVALID;
        return;
    }

    if (!strcmp ("reading", element_name)) {
        data->state = STATE_READINGS;
        return;
    }

    if (!strcmp ("radicals", element_name)) {
        data->state = STATE_NONE;
        return;
    }

    if (!strcmp ("radical", element_name)) {
        data->state = STATE_RADICALS;
        return;
    }

    if (!strcmp ("meta", element_name)) {
        data->state = STATE_NONE;
        return;
    }

    if (data->state == STATE_META) {
        if (data->chr && data->key && data->value)
            tomoe_char_register_meta_data (data->chr, data->key, data->value);
        g_free (data->key);
        g_free (data->value);
        data->key   = NULL;
        data->value = NULL;
    }
}

static void
text_handler (GMarkupParseContext *context,
              const gchar         *text,
              gsize                text_len,
              gpointer             user_data,
              GError             **error)
{
    ParseData *data = user_data;

    switch (data->state) {
    case STATE_UTF8:
    {
        tomoe_char_set_utf8 (data->chr, text);
        return;
    }
    case STATE_VARIANT:
    {
        tomoe_char_set_variant (data->chr, text);
        return;
    }
    case STATE_N_STROKES:
    {
        tomoe_char_set_n_strokes (data->chr, atoi (text));
        return;
    }
    case STATE_READING:
    {
        TomoeReading *reading;

        reading = tomoe_reading_new (data->reading_type, text);
        tomoe_char_add_reading (data->chr, reading);
        g_object_unref (reading);
        return;
    }
    case STATE_RADICAL:
    {
        tomoe_char_add_radical (data->chr, text);
        return;
    }
    case STATE_META:
    {
        g_free (data->value);
        data->value = g_strdup (text);
        return;
    }
    default:
        break;
    }
}

static void
passthrough_handler (GMarkupParseContext *context,
                     const gchar         *text,
                     gsize                text_len,
                     gpointer             user_data,
                     GError             **error)
{
}

static void
error_handler (GMarkupParseContext *context,
               GError              *error,
               gpointer             user_data)
{
}

static GMarkupParser parser = {
    start_element_handler,
    end_element_handler,
    text_handler,
    passthrough_handler,
    error_handler,
};


static void
init_parse_data (ParseData *data, TomoeXMLParsedData *result,
                 const gchar *filename)
{
    data->result       = result;
    data->in_dict      = FALSE;
    data->state        = STATE_NONE;
    data->chr          = NULL;
    data->writing      = NULL;
    data->filename     = filename;
    data->key          = NULL;
    data->value        = NULL;
    data->n_points     = 0;
    data->reading_type = TOMOE_READING_INVALID;
}

gboolean
_tomoe_xml_parser_parse_dictionary_file (const gchar *filename,
                                         TomoeXMLParsedData *result)
{
    GMarkupParseContext *context;
    FILE *f;
    gint bytes;
    gchar buf[4096];
    ParseData data;
    gboolean retval = TRUE;

    f = fopen (filename, "rb");
    if (!f) {
        g_warning ("failed to open dictionary file %s: %s", filename,
                   sys_errlist[errno]);
    }
    g_return_val_if_fail (f, FALSE);

    init_parse_data (&data, result, filename);

    context = g_markup_parse_context_new (&parser, 0, &data, NULL);

    while ((bytes = fread (buf, sizeof (gchar), 4096, f)) > 0) {
        GError *error = NULL;
        gboolean success;

        success = g_markup_parse_context_parse(context, buf, bytes, &error);
        if (!success) {
            g_warning("Tomoe XML Dictionary: %s", error->message);
            g_error_free(error);
            retval = FALSE;
            break;
        }
    }

    fclose (f);
    g_markup_parse_context_free (context);

    return retval;
}

TomoeChar *
_tomoe_xml_parser_parse_char_data (const gchar *xml, gssize len)
{
    GMarkupParseContext *context;
    TomoeXMLParsedData result;
    TomoeChar *chr = NULL;
    ParseData data;
    gboolean success;
    GError *error = NULL;

    result.chars = g_ptr_array_new ();

    init_parse_data (&data, &result, NULL);
    data.in_dict = TRUE;

    context = g_markup_parse_context_new (&parser, 0, &data, NULL);

    if (len == -1)
        len = strlen (xml);

    success = g_markup_parse_context_parse(context, xml, len, &error);
    if (!success) {
        g_warning("Tomoe XML Dictionary: %s", error->message);
        g_error_free(error);
    }

    g_markup_parse_context_free (context);

    if (success && result.chars->len > 0) {
        chr = g_ptr_array_remove_index (result.chars, 0);
    }
    TOMOE_PTR_ARRAY_FREE_ALL (result.chars, (GFunc) g_object_unref);

    return chr;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
