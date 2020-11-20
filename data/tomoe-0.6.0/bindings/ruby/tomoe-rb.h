/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
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
 *  $Id: tomoe-rb.h 1206 2007-04-20 10:29:28Z ktou $
 */

#ifndef __TOMOE_RB_H__
#define __TOMOE_RB_H__

#include "rbtomoe.h"

G_BEGIN_DECLS

#ifdef GOBJ2RVALU
#  define HAVE_GOBJ2RVALU
#endif

#ifndef HAVE_GOBJ2RVALU
#  define GOBJ2RVALU(gobj) (_tomoe_ruby_object_from_instance_with_unref(gobj))
VALUE _tomoe_ruby_object_from_instance_with_unref(gpointer instance);
#endif

#ifndef RVAL2CSTR2
#  define RVAL2CSTR2(v) (NIL_P(v) ? NULL : RVAL2CSTR(v))
#endif

void Init_tomoe(void);

void _tomoe_rb_init_tomoe_candidate(VALUE mTomoe);
void _tomoe_rb_init_tomoe_char(VALUE mTomoe);
void _tomoe_rb_init_tomoe_config(VALUE mTomoe);
void _tomoe_rb_init_tomoe_context(VALUE mTomoe);
void _tomoe_rb_init_tomoe_dict(VALUE mTomoe);
void _tomoe_rb_init_tomoe_query(VALUE mTomoe);
void _tomoe_rb_init_tomoe_reading(VALUE mTomoe);
void _tomoe_rb_init_tomoe_recognizer(VALUE mTomoe);
void _tomoe_rb_init_tomoe_writing(VALUE mTomoe);

void _tomoe_rb_set_log_domain(const gchar *domain);

G_END_DECLS

#endif /* __TOMOE_RB_H__ */

/*
vi:expandtab
*/
