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
 *  $Id: tomoe-dict-ptr-array.c 1486 2007-06-15 12:48:40Z ikezoe $
 */

#include <string.h>

#include "tomoe-dict-ptr-array.h"

#include "tomoe-candidate.h"
#include <glib-utils.h>

typedef struct _TomoeDictSearchContext {
    TomoeQuery *query;
    GList *results;
} TomoeDictSearchContext;

#define TOMOE_DICT_PTR_ARRAY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_DICT_PTR_ARRAY, TomoeDictPtrArrayPrivate))

enum {
    PROP_0,
    PROP_EDITABLE,
    PROP_MODIFIED
};

typedef struct _TomoeDictPtrArrayPrivate	TomoeDictPtrArrayPrivate;
struct _TomoeDictPtrArrayPrivate
{
    GPtrArray           *chars;
    gboolean             editable;
    gboolean             modified;
};

static TomoeDictClass *parent_class;

G_DEFINE_ABSTRACT_TYPE (TomoeDictPtrArray, _tomoe_dict_ptr_array, TOMOE_TYPE_DICT)

static void         dispose                    (GObject       *object);
static void         set_property               (GObject       *object,
                                                guint         prop_id,
                                                const GValue  *value,
                                                GParamSpec    *pspec);
static void         get_property               (GObject       *object,
                                                guint          prop_id,
                                                GValue        *value,
                                                GParamSpec    *pspec);
static gboolean     register_char              (TomoeDict     *dict,
                                                TomoeChar     *chr);
static gboolean     unregister_char            (TomoeDict     *dict,
                                                const gchar   *utf8);
static TomoeChar   *get_char                   (TomoeDict     *dict,
                                                const gchar   *utf8);
static GList       *search                     (TomoeDict     *dict,
                                                TomoeQuery    *query);
static gboolean     copy                       (TomoeDict     *src_dict,
                                                TomoeDict     *dest_dict);
static gboolean     is_editable                (TomoeDict     *dict);
static gchar       *get_available_private_utf8 (TomoeDict     *dict);

static void
_tomoe_dict_ptr_array_class_init (TomoeDictPtrArrayClass *klass)
{
    GObjectClass *gobject_class;
    TomoeDictClass *dict_class;

    gobject_class = G_OBJECT_CLASS (klass);
    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    parent_class = g_type_class_peek_parent (klass);

    dict_class = TOMOE_DICT_CLASS (klass);
    dict_class->get_name        = NULL;
    dict_class->register_char   = register_char;
    dict_class->unregister_char = unregister_char;
    dict_class->get_char        = get_char;
    dict_class->search          = search;
    dict_class->flush           = NULL;
    dict_class->copy            = copy;
    dict_class->is_editable     = is_editable;
    dict_class->is_available    = NULL;
    dict_class->get_available_private_utf8 = get_available_private_utf8;

    g_object_class_install_property(
        gobject_class,
        PROP_EDITABLE,
        g_param_spec_boolean(
            "editable",
            "Editable",
            "Editable flag",
            FALSE,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(
        gobject_class,
        PROP_MODIFIED,
        g_param_spec_boolean(
            "modified",
            "Modified",
            "Modified flag",
            FALSE,
            G_PARAM_READWRITE));

    g_type_class_add_private (gobject_class, sizeof (TomoeDictPtrArrayPrivate));
}

static void
_tomoe_dict_ptr_array_init (TomoeDictPtrArray *dict)
{
    TomoeDictPtrArrayPrivate *priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);
    priv->chars    = g_ptr_array_new();
    priv->editable = FALSE;
    priv->modified = FALSE;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeDictPtrArrayPrivate *priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (object);

    switch (prop_id) {
    case PROP_EDITABLE:
        priv->editable = g_value_get_boolean (value);
        break;
    case PROP_MODIFIED:
        priv->modified = g_value_get_boolean (value);
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
    TomoeDictPtrArrayPrivate *priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (object);

    switch (prop_id) {
    case PROP_EDITABLE:
        g_value_set_boolean (value, priv->editable);
        break;
    case PROP_MODIFIED:
        g_value_set_boolean (value, priv->modified);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeDictPtrArrayPrivate *priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (object);

    if (priv->chars)
        TOMOE_PTR_ARRAY_FREE_ALL(priv->chars, g_object_unref);

    priv->chars    = NULL;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static gint
char_compare_func (gconstpointer a, gconstpointer b)
{
    TomoeChar *ca = *(TomoeChar **) a;
    TomoeChar *cb = *(TomoeChar **) b;
    return tomoe_char_compare (ca, cb);
}

void
_tomoe_dict_ptr_array_sort (TomoeDictPtrArray *dict)
{
    TomoeDictPtrArrayPrivate *priv;

    g_return_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict));

    priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);

    g_ptr_array_sort (priv->chars, char_compare_func);
}

static gboolean
register_char (TomoeDict *dict, TomoeChar *chr)
{
    TomoeDictPtrArrayPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_CHAR (chr), FALSE);

    priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);

    unregister_char (dict, tomoe_char_get_utf8 (chr));
    g_ptr_array_add (priv->chars, g_object_ref (G_OBJECT (chr)));
    _tomoe_dict_ptr_array_sort (TOMOE_DICT_PTR_ARRAY (dict));

    priv->modified = TRUE;

    return TRUE;
}

static gboolean
unregister_char (TomoeDict *dict, const gchar *utf8)
{
    TomoeDictPtrArrayPrivate *priv;
    TomoeChar *removed = NULL;
    gint i, len, index = -1;

    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), FALSE);
    g_return_val_if_fail (utf8 && *utf8 != '\0', FALSE);

    priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);

    len = priv->chars->len;
    for (i = 0; i < len; i++) {
        TomoeChar *chr = g_ptr_array_index (priv->chars, i);
        if (g_str_equal (tomoe_char_get_utf8(chr), utf8)) {
            index = i;
            removed = chr;
            break;
        }
    }

    if (index >= 0) {
        g_ptr_array_remove_index (priv->chars, index);
        g_object_unref (removed);
        priv->modified = TRUE;
        return TRUE;
    } else {
        return FALSE;
    }
}

static TomoeChar *
get_char (TomoeDict *dict, const gchar *utf8)
{
    TomoeDictPtrArrayPrivate *priv;
    gint i, len;

    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), NULL);
    g_return_val_if_fail (utf8 && *utf8 != '\0', NULL);

    priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);

    len = priv->chars->len;
    for (i = 0; i < len; i++) {
        TomoeChar *chr = g_ptr_array_index (priv->chars, i);
        if (g_str_equal (tomoe_char_get_utf8(chr), utf8)) {
            return g_object_ref (chr);
        }
    }

    return NULL;
}

static gboolean
does_match_char_with_utf8 (TomoeChar *chr, const gchar *utf8)
{
    if (!utf8)
        return TRUE;

    return g_utf8_collate (tomoe_char_get_utf8 (chr), utf8) == 0;
}

static gboolean
does_match_char_with_variant (TomoeChar *chr, const gchar *variant)
{
    const gchar *chr_variant;

    if (!variant)
        return TRUE;

    chr_variant = tomoe_char_get_variant (chr);
    if (!chr_variant)
        return FALSE;

    return g_utf8_collate (chr_variant, variant) == 0;
}

static gboolean
does_match_char_with_n_strokes (TomoeChar *chr, gint min, gint max)
{
    TomoeWriting *writing;
    gint n_strokes;

    if (min < 0 && max < 0)
        return TRUE;

    n_strokes = tomoe_char_get_n_strokes (chr);
    if (n_strokes < 0) {
        writing = tomoe_char_get_writing (chr);
        if (!writing)
            return FALSE;
        n_strokes = tomoe_writing_get_n_strokes (writing);
    }

    return ((min < 0 || min <= n_strokes) &&
            (max < 0 || max >= n_strokes));
}

static gint
reading_compare_func (gconstpointer a, gconstpointer b)
{
    return tomoe_reading_has_prefix (TOMOE_READING (a), TOMOE_READING (b)) ? 0 : -1;
}

static gboolean
does_match_char_with_reading (TomoeChar *chr, TomoeReading *reading)
{
    if (!reading)
        return TRUE;

    if (g_list_find_custom ((GList *)tomoe_char_get_readings (chr),
                            reading, reading_compare_func))
        return TRUE;
    else
        return FALSE;
}

static gboolean
does_match_char_with_readings (TomoeChar *chr, const GList *readings)
{
    GList *node;

    for (node = (GList *)readings; node; node = g_list_next (node)) {
        TomoeReading *reading = node->data;
        if (!does_match_char_with_reading (chr, reading))
            return FALSE;
    }

    return TRUE;
}

static gboolean
does_match_char_with_radical (TomoeChar *chr, const gchar *radical)
{
    if (!radical)
        return TRUE;

    if (g_list_find_custom ((GList *)tomoe_char_get_radicals (chr),
                            radical, (GCompareFunc)g_utf8_collate))
        return TRUE;
    else
        return FALSE;
}

static gboolean
does_match_char_with_radicals (TomoeChar *chr, const GList *radicals)
{
    GList *node;

    for (node = (GList *)radicals; node; node = g_list_next (node)) {
        const gchar *radical = node->data;
        if (!does_match_char_with_radical (chr, radical))
            return FALSE;
    }

    return TRUE;
}

static void
collect_chars_by_query (gpointer data, gpointer user_data)
{
    TomoeChar *chr = data;
    TomoeDictSearchContext *context = user_data;
    TomoeQuery *q;

    q = context->query;
    if (does_match_char_with_utf8 (chr, tomoe_query_get_utf8 (q)) &&
        does_match_char_with_variant (chr, tomoe_query_get_variant (q)) &&
        does_match_char_with_n_strokes (chr,
                                        tomoe_query_get_min_n_strokes (q),
                                        tomoe_query_get_max_n_strokes (q)) &&
        does_match_char_with_readings (chr, tomoe_query_get_readings (q)) &&
        does_match_char_with_radicals (chr, tomoe_query_get_radicals (q))) {
        context->results = g_list_prepend (context->results,
                                           tomoe_candidate_new (chr));
    }
}

static void
collect_all_chars (gpointer data, gpointer user_data)
{
    TomoeChar *chr = data;
    TomoeDictSearchContext *context = user_data;

    context->results = g_list_prepend (context->results,
                                       tomoe_candidate_new (chr));
}

static GList *
search (TomoeDict *dict, TomoeQuery *query)
{
    TomoeDictSearchContext search_context;
    TomoeDictPtrArrayPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), NULL);

    priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);

    search_context.query = g_object_ref (query);
    search_context.results = NULL;

    if (tomoe_query_is_empty (query)) {
        g_ptr_array_foreach_reverse (priv->chars, collect_all_chars,
                                     &search_context);
    } else {
        g_ptr_array_foreach_reverse (priv->chars, collect_chars_by_query,
                                     &search_context);
    }
    g_object_unref (search_context.query);

    return search_context.results;
}

static void
copy_all_chars (gpointer data, gpointer user_data)
{
    TomoeChar *chr = data;
    GPtrArray **dest_chars = user_data;

    g_ptr_array_add (*dest_chars, tomoe_char_dup (chr));
}

gboolean
copy (TomoeDict *src_dict, TomoeDict *dest_dict)
{
    TomoeDictPtrArrayPrivate *src_priv, *dest_priv;
    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (src_dict), FALSE);

    if (!tomoe_dict_is_editable (dest_dict))
        return FALSE;

    if (!TOMOE_IS_DICT_PTR_ARRAY (dest_dict)) {
        tomoe_dict_plain_copy (src_dict, dest_dict);
        return TRUE;
    }

    src_priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (src_dict);
    dest_priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dest_dict);

    /* remove all chars from destination */
    if (dest_priv->chars->len > 0) {
        g_ptr_array_foreach (dest_priv->chars, (GFunc) g_object_unref, NULL);
        g_ptr_array_remove_range (dest_priv->chars, 0, dest_priv->chars->len);
    }

    g_ptr_array_foreach_reverse (src_priv->chars, copy_all_chars, &dest_priv->chars);

    dest_priv->modified = TRUE;

    return TRUE;
}

static gboolean
is_editable (TomoeDict *dict)
{
    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), FALSE);

    return TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict)->editable;
}

gchar *
get_available_private_utf8 (TomoeDict *dict)
{
    TomoeDictPtrArrayPrivate *priv;
    gint i, len, result_len;
    gchar *result;
    gunichar result_ucs = TOMOE_CHAR_PRIVATE_USE_AREA_START;

    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), NULL);

    priv = TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict);

    len = priv->chars->len;
    for (i = 0; i < len; i++) {
        TomoeChar *chr;
        gunichar ucs;

        chr = priv->chars->pdata[i];
        ucs = g_utf8_get_char (tomoe_char_get_utf8 (chr));
        if (ucs >= TOMOE_CHAR_PRIVATE_USE_AREA_START) {
            if (ucs >= TOMOE_CHAR_PRIVATE_USE_AREA_END) {
                return NULL;
            } else {
                result_ucs = ucs + 1;
            }
        }
    }

    result_len = g_unichar_to_utf8 (result_ucs, NULL);
    result = g_new (gchar, result_len + 1);
    g_unichar_to_utf8 (result_ucs, result);
    result[result_len] = '\0';
    return result;
}

GPtrArray *
_tomoe_dict_ptr_array_get_array (TomoeDictPtrArray *dict)
{
    g_return_val_if_fail (TOMOE_IS_DICT_PTR_ARRAY (dict), NULL);

    return TOMOE_DICT_PTR_ARRAY_GET_PRIVATE (dict)->chars;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
