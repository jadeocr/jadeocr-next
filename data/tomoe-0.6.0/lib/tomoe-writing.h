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
 *  $Id: tomoe-writing.h 1476 2007-06-15 06:39:14Z ikezoe $
 */

#ifndef __TOMOE_WRITING_H__
#define __TOMOE_WRITING_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define TOMOE_WRITING_WIDTH 1000
#define TOMOE_WRITING_HEIGHT 1000

#define TOMOE_TYPE_WRITING            (tomoe_writing_get_type ())
#define TOMOE_WRITING(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_WRITING, TomoeWriting))
#define TOMOE_WRITING_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_WRITING, TomoeWritingClass))
#define TOMOE_IS_WRITING(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_WRITING))
#define TOMOE_IS_WRITING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_WRITING))
#define TOMOE_WRITING_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_WRITING, TomoeWritingClass))

#define TOMOE_TYPE_POINT              (tomoe_point_get_type ())

typedef struct _TomoeWriting      TomoeWriting;
typedef struct _TomoeWritingClass TomoeWritingClass;

typedef struct _TomoePoint        TomoePoint;

struct _TomoeWriting
{
    GObject object;
};

struct _TomoeWritingClass
{
    GObjectClass parent_class;
};

struct _TomoePoint
{
    gint x;
    gint y;
};

GType           tomoe_writing_get_type          (void) G_GNUC_CONST;
TomoeWriting   *tomoe_writing_new               (void);
TomoeWriting   *tomoe_writing_dup               (TomoeWriting  *writing);
void            tomoe_writing_move_to           (TomoeWriting  *writing,
                                                 gint           x,
                                                 gint           y);
void            tomoe_writing_line_to           (TomoeWriting  *writing,
                                                 gint           x,
                                                 gint           y);
void            tomoe_writing_clear             (TomoeWriting  *writing);
guint           tomoe_writing_get_n_strokes     (TomoeWriting  *writing);
void            tomoe_writing_remove_last_stroke(TomoeWriting  *writing);

const GList    *tomoe_writing_get_strokes       (TomoeWriting  *writing);

gchar          *tomoe_writing_to_xml            (TomoeWriting  *writing);


GType           tomoe_point_get_type            (void) G_GNUC_CONST;
TomoePoint     *tomoe_point_new                 (gint x, gint y);
TomoePoint     *tomoe_point_copy                (const TomoePoint *point);
void            tomoe_point_free                (TomoePoint *point);

G_END_DECLS

#endif /* __TOMOE_WRITING_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
