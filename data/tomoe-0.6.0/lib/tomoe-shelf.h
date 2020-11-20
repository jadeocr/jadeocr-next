/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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
 *  $Id: tomoe-shelf.h 957 2006-12-05 07:11:02Z kous $
 */

#ifndef __TOMOE_SHELF_H__
#define __TOMOE_SHELF_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-dict.h"

#define TOMOE_TYPE_SHELF            (tomoe_shelf_get_type ())
#define TOMOE_SHELF(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_SHELF, TomoeShelf))
#define TOMOE_SHELF_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_SHELF, TomoeShelfClass))
#define TOMOE_IS_SHELF(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_SHELF))
#define TOMOE_IS_SHELF_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_SHELF))
#define TOMOE_SHELF_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_SHELF, TomoeShelfClass))

typedef struct _TomoeShelf TomoeShelf;
typedef struct _TomoeShelfClass TomoeShelfClass;

struct _TomoeShelf
{
    GObject object;
};

struct _TomoeShelfClass
{
    GObjectClass parent_class;
};

GType            tomoe_shelf_get_type (void) G_GNUC_CONST;

TomoeShelf      *tomoe_shelf_new      (void);

TomoeDict       *tomoe_shelf_get_dict       (TomoeShelf  *shelf,
                                             const gchar *name);
void             tomoe_shelf_register_dict  (TomoeShelf  *shelf,
                                             const gchar *name,
                                             TomoeDict   *dict);
gboolean         tomoe_shelf_unregister_dict(TomoeShelf  *shelf,
                                             const gchar *name);
GList           *tomoe_shelf_get_dict_names (TomoeShelf  *shelf);
gboolean         tomoe_shelf_has_dict       (TomoeShelf  *shelf,
                                             const gchar *name);


G_END_DECLS

#endif /* __TOMOE_SHELF_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
