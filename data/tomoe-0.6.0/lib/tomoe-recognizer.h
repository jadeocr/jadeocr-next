/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2006-2007 Kouhei Sutou <kou@cozmixng.org>
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
 *  $Id: tomoe-recognizer.h 1206 2007-04-20 10:29:28Z ktou $
 */

#ifndef __TOMOE_RECOGNIZER_H__
#define __TOMOE_RECOGNIZER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-dict.h"
#include "tomoe-writing.h"

#define TOMOE_TYPE_RECOGNIZER            (tomoe_recognizer_get_type ())
#define TOMOE_RECOGNIZER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_RECOGNIZER, TomoeRecognizer))
#define TOMOE_RECOGNIZER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_RECOGNIZER, TomoeRecognizerClass))
#define TOMOE_IS_RECOGNIZER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_RECOGNIZER))
#define TOMOE_IS_RECOGNIZER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_RECOGNIZER))
#define TOMOE_RECOGNIZER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_RECOGNIZER, TomoeRecognizerClass))

typedef struct _TomoeRecognizer TomoeRecognizer;
typedef struct _TomoeRecognizerClass TomoeRecognizerClass;

struct _TomoeRecognizer
{
    GObject object;
};

struct _TomoeRecognizerClass
{
    GObjectClass parent_class;

    GList       *(*search)          (TomoeRecognizer *recognizer,
                                     TomoeWriting    *input);
    gboolean     (*is_available)    (TomoeRecognizer *recognizer);
};

GType            tomoe_recognizer_get_type (void) G_GNUC_CONST;


void             tomoe_recognizer_init        (void);
void             tomoe_recognizer_quit        (void);

const gchar     *tomoe_recognizer_get_default_module_dir   (void);
void             tomoe_recognizer_set_default_module_dir   (const gchar *dir);

void             tomoe_recognizer_load        (const gchar *base_dir);
TomoeModule     *tomoe_recognizer_load_module (const gchar *name);
void             tomoe_recognizer_unload      (void);
GList           *tomoe_recognizer_get_registered_types (void);
GList           *tomoe_recognizer_get_log_domains      (void);

TomoeRecognizer *tomoe_recognizer_new         (const gchar *name,
                                               const gchar *first_property,
                                               ...);

GList           *tomoe_recognizer_search   (TomoeRecognizer *recognizer,
                                            TomoeWriting    *input);

const gchar     *tomoe_recognizer_get_language  (TomoeRecognizer *recognizer);

gboolean         tomoe_recognizer_is_available  (TomoeRecognizer *recognizer);

G_END_DECLS

#endif /* __TOMOE_RECOGNIZER_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
