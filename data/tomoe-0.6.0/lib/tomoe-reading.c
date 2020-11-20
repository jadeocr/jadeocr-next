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
 *  $Id: tomoe-reading.c 1479 2007-06-15 07:03:06Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n-lib.h>

#include "tomoe-enum-types.h"
#include "tomoe-reading.h"
#include "glib-utils.h"

#define TOMOE_READING_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_READING, TomoeReadingPrivate))

typedef struct _TomoeReadingPrivate	TomoeReadingPrivate;
struct _TomoeReadingPrivate
{
    TomoeReadingType  type;
    gchar            *reading;
};

enum
{
    PROP_0,
    PROP_TYPE,
    PROP_READING
};

G_DEFINE_TYPE (TomoeReading, tomoe_reading, G_TYPE_OBJECT)

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
tomoe_reading_class_init (TomoeReadingClass *klass)
{
    GObjectClass *gobject_class;
    GParamSpec *spec;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    spec = g_param_spec_enum ("type",
                              N_("Type"),
                              N_("A type of the reading."),
                              TOMOE_TYPE_READING_TYPE,
                              TOMOE_READING_INVALID,
                              G_PARAM_READABLE | G_PARAM_WRITABLE |
                              G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (gobject_class, PROP_TYPE, spec);

    spec = g_param_spec_string ("reading",
                                N_("Reading"),
                                N_("Reading of the reading."),
                                NULL,
                                G_PARAM_READABLE | G_PARAM_WRITABLE |
                                G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (gobject_class, PROP_READING, spec);

    g_type_class_add_private (gobject_class, sizeof (TomoeReadingPrivate));
}

static void
tomoe_reading_init (TomoeReading *reading)
{
    TomoeReadingPrivate *priv = TOMOE_READING_GET_PRIVATE (reading);
    priv->type = TOMOE_READING_INVALID;
    priv->reading = NULL;
}

TomoeReading *
tomoe_reading_new (TomoeReadingType type, const gchar *reading)
{
    return g_object_new(TOMOE_TYPE_READING,
                        "type", type,
                        "reading", reading,
                        NULL);
}

TomoeReading *
tomoe_reading_dup (TomoeReading *reading)
{
    TomoeReadingPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_READING (reading), NULL);

    priv = TOMOE_READING_GET_PRIVATE (reading);

    return tomoe_reading_new (priv->type, priv->reading);
}

static void
dispose (GObject *object)
{
    TomoeReadingPrivate *priv = TOMOE_READING_GET_PRIVATE (object);

    if (priv->reading)
        g_free(priv->reading);
    priv->reading = NULL;

    G_OBJECT_CLASS (tomoe_reading_parent_class)->dispose (object);
}
static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
    TomoeReading *reading;
    TomoeReadingPrivate *priv;

    reading = TOMOE_READING(object);
    priv = TOMOE_READING_GET_PRIVATE (reading);

    switch (prop_id) {
      case PROP_TYPE:
        priv->type = g_value_get_enum(value);
        break;
      case PROP_READING:
        g_free(priv->reading);
        priv->reading = g_value_dup_string(value);
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
    TomoeReading *reading;
    TomoeReadingPrivate *priv;

    reading = TOMOE_READING (object);
    priv = TOMOE_READING_GET_PRIVATE (reading);

    switch (prop_id) {
      case PROP_TYPE:
        g_value_set_enum(value, priv->type);
        break;
      case PROP_READING:
        g_value_set_string(value, priv->reading);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

TomoeReadingType
tomoe_reading_get_reading_type (TomoeReading* reading)
{
    TomoeReadingPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_READING (reading), TOMOE_READING_INVALID);

    priv = TOMOE_READING_GET_PRIVATE (reading);
    return priv->type;
}

const gchar *
tomoe_reading_get_reading (TomoeReading* reading)
{
    TomoeReadingPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_READING (reading), NULL);

    priv = TOMOE_READING_GET_PRIVATE (reading);
    return priv->reading;
}

gchar *
tomoe_reading_to_xml (TomoeReading *reading)
{
    TomoeReadingPrivate *priv;
    gchar *format, *type_name;

    g_return_val_if_fail (TOMOE_IS_READING (reading), NULL);

    priv = TOMOE_READING_GET_PRIVATE (reading);

    type_name = NULL;
    if (priv->type == TOMOE_READING_INVALID)
        format = "      <reading>%s</reading>\n";
    else
        format = "      <reading type=\"%s\">%s</reading>\n";

    switch (priv->type) {
      case TOMOE_READING_INVALID:
        break;
      case TOMOE_READING_UNKNOWN:
        type_name = "unknown";
        break;
      case TOMOE_READING_JA_ON:
        type_name = "ja_on";
        break;
      case TOMOE_READING_JA_KUN:
        type_name = "ja_kun";
        break;
    }

    if (type_name)
        return g_markup_printf_escaped (format, type_name, priv->reading);
    else
        return g_markup_printf_escaped (format, priv->reading);
}

/**
 * tomoe_reading_compare:
 * @a: a TomoeReading object.
 * @b: a TomoeReading object to compare with.
 *
 * Compare to TomoeReading objects with its own utf8 character.
 *
 * Return value: -1 a != b, 0 a= b, 
 */
gint
tomoe_reading_compare (const TomoeReading *a, const TomoeReading *b)
{
    TomoeReadingPrivate *priv_a, *priv_b;

    if (!a || !b) return 0;

    priv_a = TOMOE_READING_GET_PRIVATE (a);
    priv_b = TOMOE_READING_GET_PRIVATE (b);
    if (!priv_a || !priv_b) return 0;

    if (!priv_a->reading || !priv_b->reading) return 0;

    if (priv_a->type != priv_b->type &&
        (priv_a->type != TOMOE_READING_UNKNOWN &&
         priv_b->type != TOMOE_READING_UNKNOWN)) {
        return -1;
    }

    return strcmp (priv_a->reading, priv_b->reading);
}

gboolean
tomoe_reading_has_prefix (TomoeReading *reading, const TomoeReading *prefix_reading)
{
    TomoeReadingPrivate *priv_a, *priv_b;

    if (!reading || !prefix_reading) return TRUE;

    priv_a = TOMOE_READING_GET_PRIVATE (reading);
    priv_b = TOMOE_READING_GET_PRIVATE (prefix_reading);
    if (!priv_a || !priv_b) return 0;

    if (!priv_a->reading || !priv_b->reading) return TRUE;

    if (priv_a->type != priv_b->type &&
        (priv_a->type != TOMOE_READING_UNKNOWN &&
         priv_b->type != TOMOE_READING_UNKNOWN)) {
        return FALSE;
    }

    return g_str_has_prefix (priv_a->reading, priv_b->reading);
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
