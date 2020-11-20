/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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
 *  $Id: tomoe-char.h 1480 2007-06-15 07:20:33Z ikezoe $
 */

#ifndef __TOMOE_CHAR_H__
#define __TOMOE_CHAR_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-reading.h"
#include "tomoe-writing.h"

#define TOMOE_CHAR_PRIVATE_USE_AREA_START 0xE000
#define TOMOE_CHAR_PRIVATE_USE_AREA_END   0xF8FF

#define TOMOE_TYPE_CHAR            (tomoe_char_get_type ())
#define TOMOE_CHAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CHAR, TomoeChar))
#define TOMOE_CHAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CHAR, TomoeCharClass))
#define TOMOE_IS_CHAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CHAR))
#define TOMOE_IS_CHAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CHAR))
#define TOMOE_CHAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_CHAR, TomoeCharClass))

typedef struct _TomoeChar      TomoeChar;
typedef struct _TomoeCharClass TomoeCharClass;

struct _TomoeChar
{
    GObject object;
};

struct _TomoeCharClass
{
    GObjectClass parent_class;
};

GType           tomoe_char_get_type (void) G_GNUC_CONST;

TomoeChar      *tomoe_char_new                  (void);
TomoeChar      *tomoe_char_new_from_xml_data    (const gchar   *data,
                                                 gssize         len);
TomoeChar      *tomoe_char_dup                  (TomoeChar     *chr);

const gchar    *tomoe_char_get_utf8             (TomoeChar     *chr);
void            tomoe_char_set_utf8             (TomoeChar     *chr,
                                                 const char    *utf8);
gint            tomoe_char_get_n_strokes        (TomoeChar     *chr);
void            tomoe_char_set_n_strokes        (TomoeChar     *chr,
                                                 gint           n_strokes);
const GList    *tomoe_char_get_readings         (TomoeChar     *chr);
void            tomoe_char_add_reading          (TomoeChar     *chr,
                                                 TomoeReading  *reading);
const GList    *tomoe_char_get_radicals         (TomoeChar     *chr);
void            tomoe_char_add_radical          (TomoeChar     *chr,
                                                 const gchar   *radical);
TomoeWriting   *tomoe_char_get_writing          (TomoeChar     *chr);
void            tomoe_char_set_writing          (TomoeChar     *chr,
                                                 TomoeWriting  *writing);
const gchar    *tomoe_char_get_variant          (TomoeChar     *chr);
void            tomoe_char_set_variant          (TomoeChar     *chr,
                                                 const gchar   *variant);
const gchar    *tomoe_char_get_meta_data        (TomoeChar     *chr,
                                                 const gchar   *key);
void            tomoe_char_register_meta_data   (TomoeChar     *chr,
                                                 const gchar   *key,
                                                 const gchar   *value);
gboolean        tomoe_char_has_meta_data        (TomoeChar     *chr);
void            tomoe_char_meta_data_foreach    (TomoeChar     *chr,
                                                 GHFunc         func,
                                                 gpointer       user_data);

gint            tomoe_char_compare              (const TomoeChar *a,
                                                 const TomoeChar *b);

gchar          *tomoe_char_to_xml               (TomoeChar* chr);

G_END_DECLS

#endif /* __TOMOE_CHAR_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
