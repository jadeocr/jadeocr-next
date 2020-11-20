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
 *  $Id: tomoe-query.h 1065 2006-12-22 05:01:02Z kous $
 */

#ifndef __TOMOE_QUERY_H__
#define __TOMOE_QUERY_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-char.h"
#include "tomoe-reading.h"
#include "tomoe-writing.h"

#define TOMOE_TYPE_QUERY            (tomoe_query_get_type ())
#define TOMOE_QUERY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_QUERY, TomoeQuery))
#define TOMOE_QUERY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_QUERY, TomoeQueryClass))
#define TOMOE_IS_QUERY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_QUERY))
#define TOMOE_IS_QUERY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_QUERY))
#define TOMOE_QUERY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_QUERY, TomoeQueryClass))

typedef struct _TomoeQuery TomoeQuery;
typedef struct _TomoeQueryClass TomoeQueryClass;

struct _TomoeQuery
{
    GObject object;
};

struct _TomoeQueryClass
{
    GObjectClass parent_class;
};

GType            tomoe_query_get_type (void) G_GNUC_CONST;

TomoeQuery      *tomoe_query_new      (void);

const gchar     *tomoe_query_get_utf8              (TomoeQuery   *query);
void             tomoe_query_set_utf8              (TomoeQuery   *query,
                                                    const gchar  *utf8);

void             tomoe_query_add_reading           (TomoeQuery   *query,
                                                    TomoeReading *reading);
const GList     *tomoe_query_get_readings          (TomoeQuery   *query);

void             tomoe_query_add_radical           (TomoeQuery   *query,
                                                    const gchar  *radical);
const GList     *tomoe_query_get_radicals          (TomoeQuery   *query);

void             tomoe_query_set_variant           (TomoeQuery   *query,
                                                    const gchar  *variant);
const gchar     *tomoe_query_get_variant           (TomoeQuery   *query);

void             tomoe_query_set_min_n_strokes     (TomoeQuery   *query,
                                                    gint          n_strokes);
gint             tomoe_query_get_min_n_strokes     (TomoeQuery   *query);

void             tomoe_query_set_max_n_strokes     (TomoeQuery   *query,
                                                    gint          n_strokes);
gint             tomoe_query_get_max_n_strokes     (TomoeQuery   *query);

void             tomoe_query_set_writing           (TomoeQuery   *query,
                                                    TomoeWriting *writing);
TomoeWriting    *tomoe_query_get_writing           (TomoeQuery   *query);

gboolean         tomoe_query_is_empty              (TomoeQuery   *query);

G_END_DECLS

#endif /* __TOMOE_QUERY_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
