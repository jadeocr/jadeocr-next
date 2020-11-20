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
 *  $Id: tomoe-query.c 1307 2007-06-07 03:31:25Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n-lib.h>

#include "tomoe-enum-types.h"
#include "tomoe-query.h"
#include "glib-utils.h"

#define TOMOE_QUERY_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_QUERY, TomoeQueryPrivate))

typedef struct _TomoeQueryPrivate	TomoeQueryPrivate;
struct _TomoeQueryPrivate
{
    gchar        *utf8;
    gint          min_n_strokes;
    gint          max_n_strokes;
    GList        *readings;
    GList        *radicals;
    TomoeWriting *writing;
    gchar        *variant;
};

enum
{
    PROP_0,
    PROP_UTF8,
    PROP_MIN_N_STROKES,
    PROP_MAX_N_STROKES,
    PROP_READINGS,
    PROP_RADICALS,
    PROP_WRITING,
    PROP_VARIANTS
};

G_DEFINE_TYPE (TomoeQuery, tomoe_query, G_TYPE_OBJECT)

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
tomoe_query_class_init (TomoeQueryClass *klass)
{
    GObjectClass *gobject_class;
    GParamSpec *spec;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    spec = g_param_spec_string ("utf8",
                                N_("UTF8"),
                                N_("UTF8 encoding of searched character."),
                                NULL,
                                G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_UTF8, spec);

    spec = g_param_spec_int ("min_n_strokes",
                             N_("Minimum number of strokes"),
                             N_("Minimum number of strokes of searched "
                                "character."),
                             -1, G_MAXINT32, -1,
                             G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_MIN_N_STROKES, spec);

    spec = g_param_spec_int ("max_n_strokes",
                             N_("Maximum number of strokes"),
                             N_("Maximum number of strokes of searched "
                                "character."),
                             -1, G_MAXINT32, -1,
                             G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_MAX_N_STROKES, spec);

    spec = g_param_spec_object ("writing",
                                N_("Writing"),
                                N_("Writing of searched character."),
                                TOMOE_TYPE_WRITING,
                                G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_WRITING, spec);

    g_type_class_add_private (gobject_class, sizeof (TomoeQueryPrivate));
}

static void
tomoe_query_init (TomoeQuery *query)
{
    TomoeQueryPrivate *priv = TOMOE_QUERY_GET_PRIVATE (query);

    priv->utf8     = NULL;
    priv->min_n_strokes = -1;
    priv->max_n_strokes = -1;
    priv->readings = NULL;
    priv->radicals = NULL;
    priv->writing  = NULL;
    priv->variant  = NULL;
}

TomoeQuery *
tomoe_query_new (void)
{
    return g_object_new(TOMOE_TYPE_QUERY, NULL);
}

static void
dispose (GObject *object)
{
    TomoeQueryPrivate *priv = TOMOE_QUERY_GET_PRIVATE (object);

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
    if (priv->variant)
        g_free (priv->variant);
    if (priv->writing)
        g_object_unref (priv->writing);

    priv->utf8     = NULL;
    priv->min_n_strokes = -1;
    priv->max_n_strokes = -1;
    priv->readings = NULL;
    priv->radicals = NULL;
    priv->writing  = NULL;
    priv->variant  = NULL;

    G_OBJECT_CLASS (tomoe_query_parent_class)->dispose (object);
}
static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
    TomoeQuery *query;
    TomoeQueryPrivate *priv;

    query = TOMOE_QUERY(object);
    priv = TOMOE_QUERY_GET_PRIVATE (query);

    switch (prop_id) {
      case PROP_UTF8:
        tomoe_query_set_utf8 (query, g_value_get_string (value));
        break;
      case PROP_MIN_N_STROKES:
        tomoe_query_set_min_n_strokes (query, g_value_get_int (value));
        break;
      case PROP_MAX_N_STROKES:
        tomoe_query_set_max_n_strokes (query, g_value_get_int (value));
        break;
      case PROP_WRITING:
        tomoe_query_set_writing (query, g_value_get_object (value));
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
    TomoeQuery *query;
    TomoeQueryPrivate *priv;

    query = TOMOE_QUERY (object);
    priv = TOMOE_QUERY_GET_PRIVATE (query);

    switch (prop_id) {
      case PROP_UTF8:
        g_value_set_string (value, priv->utf8);
        break;
      case PROP_MIN_N_STROKES:
        g_value_set_int (value, priv->min_n_strokes);
        break;
      case PROP_MAX_N_STROKES:
        g_value_set_int (value, priv->max_n_strokes);
        break;
      case PROP_WRITING:
        g_value_set_object (value, priv->writing);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

const gchar *
tomoe_query_get_utf8 (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), NULL);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return priv->utf8;
}

void
tomoe_query_set_utf8 (TomoeQuery *query, const gchar *utf8)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    if (priv->utf8)
        g_free (priv->utf8);

    priv->utf8 = utf8 ? g_strdup (utf8) : NULL;
}

const GList *
tomoe_query_get_readings (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), NULL);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return priv->readings;
}

void
tomoe_query_add_reading (TomoeQuery *query, TomoeReading *reading)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    priv->readings = g_list_append (priv->readings, g_object_ref (reading));
}

const GList *
tomoe_query_get_radicals (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), NULL);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return priv->radicals;
}

void
tomoe_query_add_radical (TomoeQuery *query, const gchar *radical)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));
    g_return_if_fail (radical && radical[0] != '\0');

    priv = TOMOE_QUERY_GET_PRIVATE (query);

    priv->radicals = g_list_prepend (priv->radicals, g_strdup (radical));
}

const gchar *
tomoe_query_get_variant (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), NULL);

    priv = TOMOE_QUERY_GET_PRIVATE (query);

    return priv->variant;
}

void
tomoe_query_set_variant (TomoeQuery *query, const gchar *variant)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));

    priv = TOMOE_QUERY_GET_PRIVATE (query);

    if (priv->variant)
        g_free (priv->variant);
    priv->variant = variant ? g_strdup (variant) : NULL;
}

void
tomoe_query_set_min_n_strokes (TomoeQuery *query, gint n_strokes)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    priv->min_n_strokes = n_strokes;
}

gint
tomoe_query_get_min_n_strokes (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), -1);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return priv->min_n_strokes;
}

void
tomoe_query_set_max_n_strokes (TomoeQuery *query, gint n_strokes)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    priv->max_n_strokes = n_strokes;
}

gint
tomoe_query_get_max_n_strokes (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), -1);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return priv->max_n_strokes;
}

void
tomoe_query_set_writing (TomoeQuery *query, TomoeWriting *writing)
{
    TomoeQueryPrivate *priv;

    g_return_if_fail (TOMOE_IS_QUERY (query));

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    if (priv->writing)
        g_object_unref (priv->writing);

    priv->writing = g_object_ref (writing);
}

TomoeWriting *
tomoe_query_get_writing (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), NULL);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return priv->writing;
}

gboolean
tomoe_query_is_empty (TomoeQuery *query)
{
    TomoeQueryPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_QUERY (query), TRUE);

    priv = TOMOE_QUERY_GET_PRIVATE (query);
    return !(priv->utf8 ||
             priv->min_n_strokes > 0 ||
             priv->max_n_strokes > 0 ||
             priv->readings ||
             priv->radicals ||
             priv->writing ||
             priv->variant);
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
