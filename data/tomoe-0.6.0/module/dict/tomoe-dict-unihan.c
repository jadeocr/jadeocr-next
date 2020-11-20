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
 *  $Id: tomoe-dict-unihan.c 1517 2007-06-18 22:36:31Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gmodule.h>

#include <tomoe-module-impl.h>
#include <tomoe-dict.h>
#include <tomoe-candidate.h>
#include <tomoe-xml-parser.h>
#include <glib-utils.h>

#include "tomoe-unihan.h"
#include "tomoe-dict-ptr-array.h"

#define TOMOE_TYPE_DICT_UNIHAN            tomoe_type_dict_unihan
#define TOMOE_DICT_UNIHAN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICT_UNIHAN, TomoeDictUnihan))
#define TOMOE_DICT_UNIHAN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICT_UNIHAN, TomoeDictUnihanClass))
#define TOMOE_IS_DICT_UNIHAN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICT_UNIHAN))
#define TOMOE_IS_DICT_UNIHAN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICT_UNIHAN))
#define TOMOE_DICT_UNIHAN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_DICT_UNIHAN, TomoeDictUnihanClass))

enum {
    PROP_0,
    PROP_NAME
};

#define DEFAULT_NAME "Unihan"

typedef struct _TomoeDictUnihan TomoeDictUnihan;
typedef struct _TomoeDictUnihanClass TomoeDictUnihanClass;
struct _TomoeDictUnihan
{
    TomoeDictPtrArray    object;
    gchar               *name;
};

struct _TomoeDictUnihanClass
{
    TomoeDictPtrArrayClass parent_class;
};

static GType tomoe_type_dict_unihan = 0;
static TomoeDictPtrArray *parent_class;
static TomoeDictUnihan *the_singleton = NULL;

static GObject     *constructor               (GType                  type,
                                               guint                  n_props,
                                               GObjectConstructParam *props);
static void         dispose                   (GObject       *object);
static void         set_property              (GObject       *object,
                                               guint         prop_id,
                                               const GValue  *value,
                                               GParamSpec    *pspec);
static void         get_property              (GObject       *object,
                                               guint          prop_id,
                                               GValue        *value,
                                               GParamSpec    *pspec);
static const gchar *get_name                  (TomoeDict     *dict);
static gboolean     flush                     (TomoeDict     *dict);
static gboolean     is_editable               (TomoeDict     *dict);
static gboolean     is_available              (TomoeDict     *dict);
static gchar       *get_available_private_utf8 (TomoeDict    *dict);

static void
class_init (TomoeDictUnihanClass *klass)
{
    GObjectClass *gobject_class;
    TomoeDictClass *dict_class;

    parent_class = g_type_class_peek_parent (klass);

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->constructor  = constructor;
    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    dict_class = TOMOE_DICT_CLASS (klass);
    dict_class->get_name        = get_name;
    dict_class->flush           = flush;
    dict_class->is_editable     = is_editable;
    dict_class->is_available    = is_available;
    dict_class->get_available_private_utf8 = get_available_private_utf8;

    g_object_class_install_property (
        gobject_class,
        PROP_NAME,
        g_param_spec_string (
            "name",
            "Name",
            "The name of the dictionary",
            NULL,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
init (TomoeDictUnihan *dict)
{
    dict->name     = NULL;
}

static void
register_type (GTypeModule *type_module)
{
    static const GTypeInfo info =
        {
            sizeof (TomoeDictUnihanClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDictUnihan),
            0,
            (GInstanceInitFunc) init,
        };

    tomoe_type_dict_unihan = g_type_module_register_type (type_module,
                                                          TOMOE_TYPE_DICT_PTR_ARRAY,
                                                          "TomoeDictUnihan",
                                                          &info, 0);
}

G_MODULE_EXPORT GList *
TOMOE_MODULE_IMPL_INIT (GTypeModule *type_module)
{
    GList *registered_types = NULL;

    register_type (type_module);
    if (tomoe_type_dict_unihan)
        registered_types =
            g_list_prepend (registered_types,
                            (gchar *) g_type_name (tomoe_type_dict_unihan));

    return registered_types;
}

G_MODULE_EXPORT void
TOMOE_MODULE_IMPL_EXIT (void)
{
}

G_MODULE_EXPORT GObject *
TOMOE_MODULE_IMPL_INSTANTIATE (const gchar *first_property, va_list args)
{
    return g_object_new_valist (TOMOE_TYPE_DICT_UNIHAN, first_property, args);
}

G_MODULE_EXPORT gchar *
TOMOE_MODULE_IMPL_GET_LOG_DOMAIN (void)
{
    return g_strdup (G_LOG_DOMAIN);
}

static GObject *
constructor (GType type, guint n_props,
             GObjectConstructParam *props)
{
    GObject *object;
    GObjectClass *klass = G_OBJECT_CLASS (parent_class);

    if (!the_singleton) {
        GPtrArray *chars;
        object = klass->constructor (type, n_props, props);
        the_singleton = TOMOE_DICT_UNIHAN (object);

        chars = _tomoe_dict_ptr_array_get_array (TOMOE_DICT_PTR_ARRAY (object));
        _tomoe_unihan_create (chars);
    } else {
        object = g_object_ref (G_OBJECT (the_singleton));
    }

    return object;
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeDictUnihan *dict = TOMOE_DICT_UNIHAN (object);

    switch (prop_id) {
      case PROP_NAME:
        g_free (dict->name);
        dict->name = g_value_dup_string (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}


static void
get_property (GObject *object,
              guint prop_id,
              GValue *value,
              GParamSpec *pspec)
{
    TomoeDict *_dict = TOMOE_DICT (object);

    switch (prop_id) {
      case PROP_NAME:
        g_value_set_string (value, get_name (_dict));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
dispose (GObject *object)
{
    TomoeDictUnihan *dict;

    dict = TOMOE_DICT_UNIHAN (object);

    if (dict->name)
        g_free (dict->name);

    dict->name     = NULL;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static const gchar*
get_name (TomoeDict *_dict)
{
    TomoeDictUnihan *dict = TOMOE_DICT_UNIHAN (_dict);
    g_return_val_if_fail (TOMOE_IS_DICT_UNIHAN (dict), NULL);
    return dict->name ? dict->name : DEFAULT_NAME;
}


static gboolean
flush (TomoeDict *_dict)
{
    return TRUE;
}

static gboolean
is_editable (TomoeDict *_dict)
{
    return FALSE;
}

static gboolean
is_available (TomoeDict *_dict)
{
    return TRUE;
}

static gchar *
get_available_private_utf8 (TomoeDict *_dict)
{
    return NULL;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
