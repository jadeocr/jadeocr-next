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
 *  $Id: glib-utils.c 1076 2006-12-25 02:41:48Z kous $
 */

#include <glib.h>
#include "glib-utils.h"

void
g_ptr_array_foreach_reverse (GPtrArray *array,
                             GFunc      func,
                             gpointer   user_data)
{
    gint i;

    i = array->len - 1;
    while (i >= 0) {
        func (g_ptr_array_index (array, i--), user_data);
    }
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
