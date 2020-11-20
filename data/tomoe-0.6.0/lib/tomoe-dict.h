/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2000 - 2004 Hiroyuki Komatsu <komatsu@taiyaki.org>
 *  Copyright (C) 2004 Hiroaki Nakamura <hnakamur@good-day.co.jp>
 *  Copyright (C) 2005 Hiroyuki Ikezoe <poincare@ikezoe.net>
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
 *  $Id: tomoe-dict.h 1475 2007-06-15 06:17:55Z ikezoe $
 */

#ifndef __TOMOE_DICT_H__
#define __TOMOE_DICT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "tomoe-module.h"
#include "tomoe-char.h"
#include "tomoe-query.h"

#define TOMOE_TYPE_DICT            (tomoe_dict_get_type ())
#define TOMOE_DICT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICT, TomoeDict))
#define TOMOE_DICT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICT, TomoeDictClass))
#define TOMOE_IS_DICT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICT))
#define TOMOE_IS_DICT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICT))
#define TOMOE_DICT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_DICT, TomoeDictClass))

typedef struct _TomoeDict TomoeDict;
typedef struct _TomoeDictClass TomoeDictClass;

struct _TomoeDict
{
    GObject object;
};

struct _TomoeDictClass
{
    GObjectClass parent_class;

    const gchar    *(*get_name)            (TomoeDict     *dict);
    gboolean        (*register_char)       (TomoeDict     *dict,
                                            TomoeChar     *chr);
    gboolean        (*unregister_char)     (TomoeDict     *dict,
                                            const gchar   *utf8);
    TomoeChar      *(*get_char)            (TomoeDict     *dict,
                                            const gchar   *utf8);
    GList          *(*search)              (TomoeDict     *dict,
                                            TomoeQuery    *query);
    gboolean        (*flush)               (TomoeDict     *dict);
    gboolean        (*copy)                (TomoeDict     *src_dict,
                                            TomoeDict     *dest_dict);
    gboolean        (*is_editable)         (TomoeDict     *dict);
    gboolean        (*is_available)        (TomoeDict     *dict);
    gchar          *(*get_available_private_utf8) (TomoeDict *dict);
};

GType           tomoe_dict_get_type (void) G_GNUC_CONST;

void            tomoe_dict_init        (void);
void            tomoe_dict_quit        (void);

const gchar    *tomoe_dict_get_default_module_dir   (void);
void            tomoe_dict_set_default_module_dir   (const gchar *dir);

void            tomoe_dict_load        (const gchar *base_dir);
TomoeModule    *tomoe_dict_load_module (const gchar *name);
void            tomoe_dict_unload      (void);
GList          *tomoe_dict_get_registered_types (void);
GList          *tomoe_dict_get_log_domains      (void);

TomoeDict      *tomoe_dict_new         (const gchar *name,
                                        const gchar *first_property,
                                        ...);

const gchar    *tomoe_dict_get_name             (TomoeDict     *dict);

/* Editable methods */
gboolean        tomoe_dict_register_char        (TomoeDict     *dict,
                                                 TomoeChar     *chr);

gboolean        tomoe_dict_unregister_char      (TomoeDict     *dict,
                                                 const gchar   *utf8);

TomoeChar      *tomoe_dict_get_char             (TomoeDict     *dict,
                                                 const gchar   *utf8);

/* search method */
GList          *tomoe_dict_search               (TomoeDict     *dict,
                                                 TomoeQuery    *query);

gboolean        tomoe_dict_flush                (TomoeDict     *dict);

gboolean        tomoe_dict_copy                 (TomoeDict     *src_dict,
                                                 TomoeDict     *dest_dict);

gboolean        tomoe_dict_plain_copy           (TomoeDict     *src_dict,
                                                 TomoeDict     *dest_dict);

gboolean        tomoe_dict_is_editable          (TomoeDict *dict);
gboolean        tomoe_dict_is_available         (TomoeDict *dict);

gchar          *tomoe_dict_get_available_private_utf8 (TomoeDict *dict);

G_END_DECLS

#endif /* __TOMOE_DICT_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
