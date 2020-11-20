/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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
 *  $Id: tomoe-writing.c 1476 2007-06-15 06:39:14Z ikezoe $
 */

#include <stdlib.h>
#include "tomoe-writing.h"

#define TOMOE_WRITING_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_WRITING, TomoeWritingPrivate))

typedef struct _TomoeWritingPrivate TomoeWritingPrivate;
typedef struct _TomoeStroke       TomoeStroke;

struct _TomoeWritingPrivate
{
    GList *stroke_first;
    GList *stroke_last;
    guint  n_strokes;
};

G_DEFINE_TYPE (TomoeWriting, tomoe_writing, G_TYPE_OBJECT)

static void          dispose      (GObject *object);
static GList       *_stroke_new   (gint x, gint y);
static void         _stroke_free  (GList *stroke);

static void
tomoe_writing_class_init (TomoeWritingClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = dispose;

    g_type_class_add_private (gobject_class, sizeof (TomoeWritingPrivate));
}

static void
tomoe_writing_init (TomoeWriting *writing)
{
    TomoeWritingPrivate *priv = TOMOE_WRITING_GET_PRIVATE(writing);
    priv->stroke_first = NULL;
    priv->stroke_last  = NULL;
}

static void
dispose (GObject *object)
{
    TomoeWriting *writing = TOMOE_WRITING (object);

    tomoe_writing_clear (writing);

    G_OBJECT_CLASS (tomoe_writing_parent_class)->dispose (object);
}

TomoeWriting *
tomoe_writing_new (void)
{
    TomoeWriting *writing = g_object_new(TOMOE_TYPE_WRITING, NULL);
    return writing;
}

TomoeWriting *
tomoe_writing_dup (TomoeWriting *writing)
{
    TomoeWriting *new_writing;
    TomoeWritingPrivate *priv;
    GList *stroke_list;

    g_return_val_if_fail (TOMOE_IS_WRITING (writing), NULL);

    new_writing = tomoe_writing_new ();

    priv = TOMOE_WRITING_GET_PRIVATE(writing);
    if (!priv->stroke_first) return new_writing;

    for (stroke_list = priv->stroke_first;
         stroke_list;
         stroke_list = g_list_next (stroke_list)) {
        GList *point_list = stroke_list->data;
        GList *first_point = point_list;

        if (!point_list) continue;

        for (; point_list; point_list = g_list_next (point_list)) {
            TomoePoint *p = point_list->data;

            if (!p) continue;
 
            if (point_list == first_point)
                tomoe_writing_move_to (new_writing, p->x, p->y);
            else
                tomoe_writing_line_to (new_writing, p->x, p->y);
        }
    }

    return new_writing;
}

void
tomoe_writing_move_to (TomoeWriting *writing, gint x, gint y)
{
    TomoeWritingPrivate *priv;

    g_return_if_fail (TOMOE_IS_WRITING (writing));

    priv = TOMOE_WRITING_GET_PRIVATE(writing);

    priv->stroke_last = g_list_append (priv->stroke_last, _stroke_new (x, y));
    if (!priv->stroke_first)
        priv->stroke_first = priv->stroke_last;
    priv->stroke_last = g_list_last (priv->stroke_last);
    priv->n_strokes++;
}

void
tomoe_writing_line_to (TomoeWriting *writing, gint x, gint y)
{
    TomoeWritingPrivate *priv;
    GList *stroke;

    g_return_if_fail (TOMOE_IS_WRITING (writing));

    priv = TOMOE_WRITING_GET_PRIVATE(writing);
    g_return_if_fail (priv->stroke_last);

    stroke = priv->stroke_last->data;
    g_return_if_fail (stroke);

    priv->stroke_last->data = g_list_append (stroke, tomoe_point_new (x, y));
}

void
tomoe_writing_clear (TomoeWriting *writing)
{
    TomoeWritingPrivate *priv;

    priv = TOMOE_WRITING_GET_PRIVATE(writing);
    g_return_if_fail (priv);

    g_list_foreach (priv->stroke_first, (GFunc)_stroke_free, NULL);
    g_list_free (priv->stroke_first);

    priv->stroke_first = NULL;
    priv->stroke_last  = NULL;
    priv->n_strokes = 0;
}

guint
tomoe_writing_get_n_strokes (TomoeWriting *writing)
{
    TomoeWritingPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_WRITING (writing), 0);

    priv = TOMOE_WRITING_GET_PRIVATE(writing);
    g_return_val_if_fail (priv, 0);

    return priv->n_strokes;
}

void
tomoe_writing_remove_last_stroke (TomoeWriting *writing)
{
    TomoeWritingPrivate *priv;
    GList *stroke;

    g_return_if_fail (TOMOE_IS_WRITING (writing));

    priv = TOMOE_WRITING_GET_PRIVATE(writing);
    g_return_if_fail (priv);
    if (!priv->stroke_last) return;

    stroke = priv->stroke_last->data;
    g_return_if_fail (stroke);

    priv->stroke_first = g_list_remove (priv->stroke_first, stroke);
    priv->stroke_last = g_list_last (priv->stroke_first);

    priv->n_strokes--;

    _stroke_free (stroke);
}

const GList *
tomoe_writing_get_strokes (TomoeWriting  *writing)
{
    g_return_val_if_fail (TOMOE_IS_WRITING (writing), NULL);

    return TOMOE_WRITING_GET_PRIVATE(writing)->stroke_first;
}

gchar *
tomoe_writing_to_xml (TomoeWriting *writing)
{
    TomoeWritingPrivate *priv;
    GList *stroke_list;
    GString *output;

    g_return_val_if_fail (TOMOE_IS_WRITING (writing), NULL);

    priv = TOMOE_WRITING_GET_PRIVATE(writing);
    if (!priv->stroke_first) return g_strdup ("");

    output = g_string_new ("    <strokes>\n");
    for (stroke_list = priv->stroke_first;
         stroke_list;
         stroke_list = g_list_next (stroke_list)) {
        GList *point_list = stroke_list->data;

        if (!point_list) continue;
        g_string_append (output, "      <stroke>\n");

        for (; point_list; point_list = g_list_next (point_list)) {
            TomoePoint *p = point_list->data;

            if (!p) continue;

            g_string_append_printf (output,
                                    "        <point x=\"%d\" y=\"%d\"/>\n",
                                    p->x, p->y);
        }

        g_string_append (output, "      </stroke>\n");
    }

    g_string_append (output, "    </strokes>\n");

    return g_string_free (output, FALSE);
}

TomoePoint *
tomoe_point_new (gint x, gint y)
{
    TomoePoint *p = g_new (TomoePoint, 1);

    g_return_val_if_fail (p, NULL);

    p->x = x;
    p->y = y;

    return p;
}

TomoePoint *
tomoe_point_copy (const TomoePoint *point)
{
    TomoePoint *new_point;

    g_return_val_if_fail (point, NULL);

    new_point = g_new (TomoePoint, 1);
    *new_point = *point;
    return new_point;
}

void
tomoe_point_free (TomoePoint *point)
{
    g_return_if_fail (point);

    g_free (point);
}

GType
tomoe_point_get_type (void)
{
    static GType our_type = 0;

    if (our_type == 0) {
        const gchar *str;
#if GLIB_CHECK_VERSION(2, 10, 0)
        str = g_intern_static_string ("TomoePoint");
#else
        str = "TomoePoint";
#endif
        our_type =
            g_boxed_type_register_static (str,
                                          (GBoxedCopyFunc)tomoe_point_copy,
                                          (GBoxedFreeFunc)tomoe_point_free);
    }

    return our_type;
}

static GList *
_stroke_new (gint x, gint y)
{
    return g_list_append (NULL, tomoe_point_new (x, y));
}

static void
_stroke_free (GList *stroke)
{
    g_list_foreach (stroke, (GFunc) g_free, NULL);
    g_list_free (stroke);
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
