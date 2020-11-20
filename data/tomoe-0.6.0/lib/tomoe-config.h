/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2006 Juernjakob Harder <juernjakob.harder@gmail.com>
 *  Copyright (C) 2007 Kouhei Sutou <kou@cozmixng.org>
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
 *  $Id: tomoe-config.h 1442 2007-06-13 01:42:19Z makeinu $
 */

#ifndef __TOMOE_CONFIG_H__
#define __TOMOE_CONFIG_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-shelf.h"

#define TOMOE_TYPE_CONFIG            (tomoe_config_get_type ())
#define TOMOE_CONFIG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CONFIG, TomoeConfig))
#define TOMOE_CONFIG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CONFIG, TomoeConfigClass))
#define TOMOE_IS_CONFIG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CONFIG))
#define TOMOE_IS_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CONFIG))
#define TOMOE_CONFIG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_CONFIG, TomoeConfigClass))

typedef struct _TomoeConfig TomoeConfig;
typedef struct _TomoeConfigClass TomoeConfigClass;

struct _TomoeConfig
{
    GObject object;
};

struct _TomoeConfigClass
{
    GObjectClass parent_class;
};

GType            tomoe_config_get_type (void) G_GNUC_CONST;

TomoeConfig     *tomoe_config_new                 (const gchar *config_file);
const gchar     *tomoe_config_get_filename        (TomoeConfig *config);
const gchar     *tomoe_config_get_user_dict_name  (TomoeConfig *config);
TomoeShelf      *tomoe_config_make_shelf          (TomoeConfig *config,
                                                   const gchar *language);
const gchar *const *tomoe_config_get_languages    (TomoeConfig *config);

G_END_DECLS

#endif /* __TOMOE_CONFIG_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
