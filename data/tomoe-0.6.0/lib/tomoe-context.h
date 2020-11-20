/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
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
 *  $Id: tomoe-context.h 969 2006-12-06 06:28:37Z kous $
 */

#ifndef __TOMOE_CONTEXT_H__
#define __TOMOE_CONTEXT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-candidate.h"
#include "tomoe-dict.h"
#include "tomoe-query.h"

#define TOMOE_TYPE_CONTEXT            (tomoe_context_get_type ())
#define TOMOE_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CONTEXT, TomoeContext))
#define TOMOE_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CONTEXT, TomoeContextClass))
#define TOMOE_IS_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CONTEXT))
#define TOMOE_IS_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CONTEXT))
#define TOMOE_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_CONTEXT, TomoeContextClass))

typedef struct _TomoeContext TomoeContext;
typedef struct _TomoeContextClass TomoeContextClass;

struct _TomoeContext
{
    GObject object;
};

struct _TomoeContextClass
{
    GObjectClass parent_class;
};

GType           tomoe_context_get_type (void) G_GNUC_CONST;

TomoeContext   *tomoe_context_new      (void);

void            tomoe_context_load_config     (TomoeContext       *ctx,
                                               const gchar        *config_file);

GList          *tomoe_context_search          (TomoeContext       *ctx,
                                               TomoeQuery         *query);
GList          *tomoe_context_multi_search    (TomoeContext       *ctx,
                                               GList              *queries);

TomoeChar      *tomoe_context_get_char        (TomoeContext       *ctx,
                                               const gchar        *utf8);

gboolean        tomoe_context_register        (TomoeContext       *ctx,
                                               TomoeChar          *chr);
gboolean        tomoe_context_unregister      (TomoeContext       *ctx,
                                               const gchar        *utf8);

G_END_DECLS

#endif /* __TOMOE_CONTEXT_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
