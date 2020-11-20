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
 *  $Id: tomoe-reading.h 1479 2007-06-15 07:03:06Z ikezoe $
 */

#ifndef __TOMOE_READING_H__
#define __TOMOE_READING_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_READING            (tomoe_reading_get_type ())
#define TOMOE_READING(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_READING, TomoeReading))
#define TOMOE_READING_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_READING, TomoeReadingClass))
#define TOMOE_IS_READING(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_READING))
#define TOMOE_IS_READING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_READING))
#define TOMOE_READING_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_READING, TomoeReadingClass))

typedef struct _TomoeReading      TomoeReading;
typedef struct _TomoeReadingClass TomoeReadingClass;

struct _TomoeReading
{
    GObject object;
};

struct _TomoeReadingClass
{
    GObjectClass parent_class;
};

typedef enum {
    TOMOE_READING_INVALID,
    TOMOE_READING_UNKNOWN,
    TOMOE_READING_JA_ON,
    TOMOE_READING_JA_KUN
} TomoeReadingType;

GType             tomoe_reading_get_type (void) G_GNUC_CONST;

TomoeReading     *tomoe_reading_new              (TomoeReadingType type,
                                                  const gchar      *reading);

TomoeReading     *tomoe_reading_dup              (TomoeReading     *reading);

TomoeReadingType  tomoe_reading_get_reading_type (TomoeReading     *reading);
const gchar      *tomoe_reading_get_reading      (TomoeReading     *reading);

gchar            *tomoe_reading_to_xml           (TomoeReading     *reading);

gint              tomoe_reading_compare          (const TomoeReading *a,
                                                  const TomoeReading *b);

gboolean          tomoe_reading_has_prefix       (TomoeReading       *reading,
                                                  const TomoeReading *prefix_reading);

G_END_DECLS

#endif /* __TOMOE_READING_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
