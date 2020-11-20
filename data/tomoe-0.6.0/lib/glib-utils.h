/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2005 Takuro Ashie <ashie@homa.ne.jp>
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
 *  $Id: glib-utils.h 1212 2007-04-25 07:35:56Z makeinu $
 */


#ifndef __GLIB_UTILS_H__
#define __GLIB_UTILS_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <glib/garray.h>
#include <glib/gi18n-lib.h>
#include "glib-compat-file-utilities.h"
#include "glib-compat-key-file.h"

G_BEGIN_DECLS

#define TOMOE_PTR_ARRAY_FREE_ALL(p_array, free_func)            \
{                                                               \
    g_ptr_array_foreach (p_array, (GFunc) (free_func), NULL);   \
    g_ptr_array_free (p_array, TRUE);                           \
}

#define TOMOE_HANDLE_ERROR(error) do {              \
    g_warning ("%s: %d: %s",                        \
               g_quark_to_string (error->domain),   \
               error->code,                         \
               error->message);                     \
    g_error_free (error);                           \
    error = NULL;                                   \
} while (FALSE)

void g_ptr_array_foreach_reverse (GPtrArray *array,
                                  GFunc      func,
                                  gpointer   user_data);

#if !GLIB_CHECK_VERSION(2, 8, 0)
gboolean g_file_set_contents (const gchar *filename,
                              const gchar *contents,
                              gssize	     length,
                              GError	   **error);
#endif

G_END_DECLS

#endif /* __GLIB_UTILS_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
