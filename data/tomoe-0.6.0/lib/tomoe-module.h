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
 *  $Id: tomoe-module.h 1206 2007-04-20 10:29:28Z ktou $
 */

#ifndef __TOMOE_MODULE_H__
#define __TOMOE_MODULE_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_MODULE            (tomoe_module_get_type ())
#define TOMOE_MODULE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_MODULE, TomoeModule))
#define TOMOE_MODULE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_MODULE, TomoeModuleClass))
#define TOMOE_IS_MODULE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_MODULE))
#define TOMOE_IS_MODULE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_MODULE))
#define TOMOE_MODULE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_MODULE, TomoeModuleClass))

typedef struct _TomoeModule TomoeModule;
typedef struct _TomoeModuleClass TomoeModuleClass;

struct _TomoeModule
{
    GTypeModule object;
};

struct _TomoeModuleClass
{
    GTypeModuleClass parent_class;
};

GType     tomoe_module_get_type (void) G_GNUC_CONST;


TomoeModule *tomoe_module_load_module       (const gchar    *base_dir,
                                             const gchar    *name);
GList       *tomoe_module_load_modules (const gchar    *base_dir);
GList       *tomoe_module_load_modules_unique  (const gchar    *base_dir,
                                                GList      *modules);

TomoeModule *tomoe_module_find      (GList          *modules,
                                     const gchar    *name);

GObject  *tomoe_module_instantiate  (TomoeModule *module,
                                     const gchar *first_property,
                                     va_list      var_args);

GList    *tomoe_module_collect_registered_types (GList *modules);
GList    *tomoe_module_collect_log_domains      (GList *modules);

void      tomoe_module_unload (TomoeModule *module);

G_END_DECLS

#endif /* __TOMOE_MODULE_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
