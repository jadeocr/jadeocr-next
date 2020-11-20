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
 *  $Id: tomoe-module-impl.h 1097 2006-12-27 06:50:46Z kous $
 */

#ifndef __TOMOE_MODULE_IMPL_H__
#define __TOMOE_MODULE_IMPL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-module.h"

typedef GList   *(*TomoeModuleInitFunc)         (GTypeModule *module);
typedef void     (*TomoeModuleExitFunc)         (void);
typedef GObject *(*TomoeModuleInstantiateFunc)  (const gchar *first_property,
                                                 va_list      var_args);
typedef gchar   *(*TomoeModuleGetLogDomainFunc) (void);

#define TOMOE_MODULE_IMPL_INIT           tomoe_module_impl_init
#define TOMOE_MODULE_IMPL_EXIT           tomoe_module_impl_exit
#define TOMOE_MODULE_IMPL_INSTANTIATE    tomoe_module_impl_instantiate
#define TOMOE_MODULE_IMPL_GET_LOG_DOMAIN tomoe_module_impl_get_log_domain


GList   *TOMOE_MODULE_IMPL_INIT           (GTypeModule  *module);
void     TOMOE_MODULE_IMPL_EXIT           (void);
GObject *TOMOE_MODULE_IMPL_INSTANTIATE    (const gchar *first_property,
                                           va_list      var_args);
gchar   *TOMOE_MODULE_IMPL_GET_LOG_DOMAIN (void);

G_END_DECLS

#endif /* __TOMOE_MODULE_IMPL_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
