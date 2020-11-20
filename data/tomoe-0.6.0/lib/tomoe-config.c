/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
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
 *  $Id: tomoe-config.c 1307 2007-06-07 03:31:25Z ikezoe $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <glib.h>
#include <glib/gi18n-lib.h>

#include "tomoe-config.h"
#include "tomoe-dict.h"
#include "tomoe-config.h"
#include "glib-utils.h"

#define DEFAULT_USER_DICT_NAME "user"

#define TOMOE_CONFIG_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CONFIG, TomoeConfigPrivate))

typedef struct _TomoeConfigPrivate  TomoeConfigPrivate;
struct _TomoeConfigPrivate
{
    gchar       *filename;
    gchar       *user_dict_name;
    GKeyFile    *key_file;
    gchar		**languages;
};

enum
{
    PROP_0,
    PROP_FILENAME
};


G_DEFINE_TYPE (TomoeConfig, tomoe_config, G_TYPE_OBJECT)

static const gchar *system_config_file = CONFDIR "/config";

static void     dispose      (GObject       *object);
static GObject *constructor  (GType                  type,
                              guint                  n_props,
                              GObjectConstructParam *props);
static void     set_property (GObject       *object,
                              guint          prop_id,
                              const GValue  *value,
                              GParamSpec    *pspec);
static void     get_property (GObject       *object,
                              guint          prop_id,
                              GValue        *value,
                              GParamSpec    *pspec);

static void     tomoe_config_load         (TomoeConfig  *config);
static void     tomoe_config_save         (TomoeConfig *config);

static gboolean _tomoe_config_key_file_get_boolean (GKeyFile *key_file,
                                                    const gchar *group,
                                                    const gchar *key,
                                                    gboolean default_value);
static gchar   *_tomoe_config_key_file_get_string  (GKeyFile *key_file,
                                                    const gchar *group,
                                                    const gchar *key,
                                                    const gchar *default_value);
static gchar  **_tomoe_config_key_file_get_string_list(GKeyFile *key_file,
                                                       const gchar *group,
                                                       const gchar *key,
                                                       gchar **default_value);
static gint     _tomoe_config_key_file_get_integer (GKeyFile *key_file,
                                                    const gchar *group,
                                                    const gchar *key,
                                                    gint default_value);
static void     _tomoe_config_load_system_dictionaries   (TomoeConfig *config,
                                                          TomoeShelf *shelf);
static TomoeDict *_tomoe_config_load_dictionary (GKeyFile    *key_file,
                                                 const gchar *dict_name,
                                                 const gchar *type);

static void
tomoe_config_class_init (TomoeConfigClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose      = dispose;
    gobject_class->constructor  = constructor;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    g_object_class_install_property (gobject_class,
                                     PROP_FILENAME,
                                     g_param_spec_string ("filename",
                                         N_("Filename"),
                                         N_("The filename for storing user settings"),
                                         NULL,
                                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_type_class_add_private (gobject_class, sizeof (TomoeConfigPrivate));
}

static GObject *
constructor (GType type, guint n_props, GObjectConstructParam *props)
{

    GObject *object;
    GObjectClass *klass = G_OBJECT_CLASS (tomoe_config_parent_class);

    object = klass->constructor (type, n_props, props);

    tomoe_config_load (TOMOE_CONFIG (object));

    return object;
}

static void
tomoe_config_init (TomoeConfig *config)
{
    TomoeConfigPrivate *priv = TOMOE_CONFIG_GET_PRIVATE (config);

    priv->filename   = NULL;
    priv->user_dict_name = NULL;
    priv->key_file   = NULL;
    priv->languages  = NULL;
}

TomoeConfig *
tomoe_config_new (const gchar *config_file)
{
    TomoeConfig *config;

    config = g_object_new(TOMOE_TYPE_CONFIG,
                          "filename", config_file,
                          NULL);

    return config;
}

static void
dispose (GObject *object)
{
    TomoeConfig *config;
    TomoeConfigPrivate *priv;

    config = TOMOE_CONFIG (object);
    priv = TOMOE_CONFIG_GET_PRIVATE (config);

    tomoe_config_save (config);

    if (priv->filename)
        g_free (priv->filename);
    if (priv->user_dict_name)
        g_free (priv->user_dict_name);
    if (priv->key_file)
        g_key_file_free (priv->key_file);
    if (priv->languages)
        g_strfreev (priv->languages);

    priv->filename  = NULL;
    priv->user_dict_name = NULL;
    priv->key_file = NULL;
    priv->languages = NULL;

    if (G_OBJECT_CLASS (tomoe_config_parent_class)->dispose)
        G_OBJECT_CLASS (tomoe_config_parent_class)->dispose (object);
}

static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
    TomoeConfigPrivate *priv = TOMOE_CONFIG_GET_PRIVATE (object);

    switch (prop_id) {
      case PROP_FILENAME:
        if (priv->filename)
            g_free (priv->filename);
        priv->filename = g_value_dup_string (value);
        break;

      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
get_property (GObject    *object,
              guint       prop_id,
              GValue     *value,
              GParamSpec *pspec)
{
    TomoeConfigPrivate *priv = TOMOE_CONFIG_GET_PRIVATE (object);

    switch (prop_id) {
      case PROP_FILENAME:
        g_value_set_string (value, priv->filename);
        break;

      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
tomoe_config_load (TomoeConfig *config)
{
    GKeyFile *key_file;
    GError *error = NULL;
    TomoeConfigPrivate *priv;
    const gchar *config_file;
    gchar *language;

    g_return_if_fail (config);

    priv = TOMOE_CONFIG_GET_PRIVATE (config);

    if (priv->key_file) {
        g_key_file_free (priv->key_file);
        priv->key_file = NULL;
    }

    key_file = g_key_file_new ();
    config_file = priv->filename ? priv->filename : system_config_file;
    if (!g_key_file_load_from_file (key_file, config_file,
                                    G_KEY_FILE_KEEP_COMMENTS |
                                    G_KEY_FILE_KEEP_TRANSLATIONS,
                                    &error)) {
        g_key_file_free (key_file);
        TOMOE_HANDLE_ERROR (error);
        return;
    }

    priv->key_file = key_file;

    if (priv->user_dict_name)
        g_free (priv->user_dict_name);
    priv->user_dict_name =
        _tomoe_config_key_file_get_string (key_file,
                                           "config", "user-dictionary",
                                           DEFAULT_USER_DICT_NAME);

    if (priv->languages)
        g_strfreev (priv->languages);
    language = _tomoe_config_key_file_get_string (key_file,
                                                  "config", "language",
                                                  NULL);
    if (language) {
        priv->languages = g_new0(gchar *, 2);
        priv->languages[0] = language;
    } else {
        priv->languages =
            _tomoe_config_key_file_get_string_list (key_file,
                                                    "config", "languages",
                                                    NULL);
    }
}

static void
tomoe_config_save (TomoeConfig *config)
{
    TomoeConfigPrivate *priv;

    g_return_if_fail (config);

    priv = TOMOE_CONFIG_GET_PRIVATE (config);
    if (priv->filename && priv->key_file) {
        gchar *data;
        gsize length;
        GError *error = NULL;

        data = g_key_file_to_data (priv->key_file, &length, &error);
        if (error) {
            TOMOE_HANDLE_ERROR (error);
            return;
        }

        if (!g_file_set_contents (priv->filename, data, length, &error)) {
            TOMOE_HANDLE_ERROR (error);
            return;
        }
    }
}

const gchar *
tomoe_config_get_filename (TomoeConfig *config)
{
    TomoeConfigPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CONFIG (config), NULL);

    priv = TOMOE_CONFIG_GET_PRIVATE(config);
    return priv->filename;
}

const gchar *
tomoe_config_get_user_dict_name (TomoeConfig *config)
{
    TomoeConfigPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CONFIG (config), NULL);

    priv = TOMOE_CONFIG_GET_PRIVATE(config);
    return priv->user_dict_name;
}

TomoeShelf *
tomoe_config_make_shelf (TomoeConfig *config, const gchar *language)
{
    TomoeConfigPrivate *priv;
    TomoeShelf *shelf;
    GKeyFile *key_file;
    guint i;
    gchar **dicts;
    gsize dicts_size;

    g_return_val_if_fail (config, NULL);

    priv = TOMOE_CONFIG_GET_PRIVATE(config);
    key_file = priv->key_file;
    g_return_val_if_fail (key_file, NULL);

    shelf = tomoe_shelf_new ();
    dicts = g_key_file_get_groups (key_file, &dicts_size);
    for (i = 0; i < dicts_size; i++) {
        gchar *dict_name, *type;
        TomoeDict *dict;

        dict_name = dicts[i];
        if (!g_str_has_suffix (dict_name, "-dictionary"))
            continue;

        if (!_tomoe_config_key_file_get_boolean (key_file, dict_name,
                                                 "use", TRUE))
            continue;

        type = _tomoe_config_key_file_get_string (key_file, dict_name,
                                                  "type", "xml");
        dict = _tomoe_config_load_dictionary (key_file, dict_name, type);
        if (dict) {
            gchar tmp, *dictionary_suffix;
            dictionary_suffix = g_strrstr (dict_name, "-dictionary");
            tmp = dict_name[dictionary_suffix - dict_name];
            dict_name[dictionary_suffix - dict_name] = '\0';
            tomoe_shelf_register_dict (shelf, dict_name, dict);
            dict_name[dictionary_suffix - dict_name] = tmp;
            g_object_unref (dict);
        }
        g_free (type);
    }
    g_strfreev(dicts);

    if (_tomoe_config_key_file_get_boolean (key_file,
                                            "config", "use-system-dictionaries",
                                            TRUE)) {
        _tomoe_config_load_system_dictionaries (config, shelf);
    }

    return shelf;
}

const gchar *const *
tomoe_config_get_languages (TomoeConfig *config)
{
    TomoeConfigPrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CONFIG (config), NULL);

    priv = TOMOE_CONFIG_GET_PRIVATE (config);
    return (const gchar *const *)(priv->languages);
}


static gboolean
_tomoe_config_key_file_get_boolean (GKeyFile *key_file,
                                    const gchar *group,
                                    const gchar *key,
                                    gboolean default_value)
{
    gboolean result;
    GError *error = NULL;

    result = g_key_file_get_boolean (key_file, group, key, &error);
    if (error) {
        switch (error->code) {
          case G_KEY_FILE_ERROR_NOT_FOUND:
            g_error_free (error);
            break;
          case G_KEY_FILE_ERROR_INVALID_VALUE:
            TOMOE_HANDLE_ERROR (error);
            break;
        }
        result = default_value;
    }

    return result;
}

static gchar *
_tomoe_config_key_file_get_string (GKeyFile *key_file,
                                   const gchar *group,
                                   const gchar *key,
                                   const gchar *default_value)
{
    gchar *result = NULL;
    GError *error = NULL;

    result = g_key_file_get_string (key_file, group, key, &error);
    if (error) {
        switch (error->code) {
          case G_KEY_FILE_ERROR_NOT_FOUND:
            g_error_free (error);
            break;
          case G_KEY_FILE_ERROR_INVALID_VALUE:
            TOMOE_HANDLE_ERROR (error);
            break;
        }
        if (default_value)
            result = g_strdup (default_value);
    }

    return result;
}

static gchar **
_tomoe_config_key_file_get_string_list (GKeyFile *key_file,
                                        const gchar *group,
                                        const gchar *key,
                                        gchar **default_value)
{
    gchar **result = NULL;
    gsize length = 0;
    GError *error = NULL;

    result = g_key_file_get_string_list (key_file, group, key, &length, &error);
    if (error) {
        switch (error->code) {
          case G_KEY_FILE_ERROR_NOT_FOUND:
            g_error_free (error);
            break;
          case G_KEY_FILE_ERROR_INVALID_VALUE:
            TOMOE_HANDLE_ERROR (error);
            break;
        }
        if (default_value)
            result = g_strdupv (default_value);
    }

    return result;
}

static gint
_tomoe_config_key_file_get_integer (GKeyFile *key_file,
                                    const gchar *group,
                                    const gchar *key,
                                    gint default_value)
{
    gint result;
    GError *error = NULL;

    result = g_key_file_get_integer (key_file, group, key, &error);
    if (error) {
        switch (error->code) {
          case G_KEY_FILE_ERROR_NOT_FOUND:
            g_error_free (error);
            break;
          case G_KEY_FILE_ERROR_INVALID_VALUE:
            TOMOE_HANDLE_ERROR (error);
            break;
        }
        result = default_value;
    }

    return result;
}

static void
_tomoe_config_load_system_dictionaries (TomoeConfig *config, TomoeShelf *shelf)
{
    const gchar *filename;
    GDir *gdir;
    TomoeDict *dict;

    dict = tomoe_dict_new ("unihan", NULL);
    if (dict) {
        tomoe_shelf_register_dict (shelf, "Unihan", dict);
        g_object_unref (dict);
    }

    gdir = g_dir_open (DICT_DATADIR, 0, NULL);
    if (!gdir)
        return;

    while ((filename = g_dir_read_name (gdir))) {
        gchar *path;

        if (!g_str_has_suffix (filename, ".xml"))
            continue;
        path = g_build_filename(DICT_DATADIR, filename, NULL);
        if (tomoe_shelf_has_dict (shelf, path)) {
            g_free (path);
            continue;
        }

        dict = tomoe_dict_new ("xml", "filename", path, "editable", FALSE,
                               NULL);
        if (dict) {
            tomoe_shelf_register_dict (shelf, tomoe_dict_get_name (dict), dict);
            g_object_unref (dict);
        }

        g_free (path);
    }
    g_dir_close (gdir);
}

static TomoeDict *
load_xml_dictionary (GKeyFile *key_file, const gchar *dict_name)
{
    TomoeDict *dict;
    GError *error = NULL;
    gchar *filename;
    gboolean user_dict, editable;

    filename = g_key_file_get_string (key_file, dict_name, "file", &error);
    if (error) {
        TOMOE_HANDLE_ERROR (error);
        return NULL;
    }

    editable = _tomoe_config_key_file_get_boolean (key_file, dict_name,
                                                   "editable", TRUE);
    user_dict = _tomoe_config_key_file_get_boolean (key_file, dict_name,
                                                    "user", TRUE);
    if (!user_dict) {
        gchar *tmp;
        tmp = g_build_filename (DICT_DATADIR, filename, NULL);
        g_free (filename);
        filename = tmp;
    }

    dict = tomoe_dict_new ("xml",
                           "filename", filename,
                           "editable", editable,
                           NULL);

    g_free (filename);

    return dict;
}

static TomoeDict *
load_est_dictionary (GKeyFile *key_file, const gchar *dict_name)
{
    TomoeDict *dict;
    GError *error = NULL;
    gchar *name, *database;
    gboolean user_dict, editable;

    name = g_key_file_get_string (key_file, dict_name, "name", &error);
    if (error) {
        TOMOE_HANDLE_ERROR (error);
        return NULL;
    }

    database = g_key_file_get_string (key_file, dict_name, "database", &error);
    if (error) {
        TOMOE_HANDLE_ERROR (error);
        return NULL;
    }

    editable = _tomoe_config_key_file_get_boolean (key_file, dict_name,
                                                   "editable", TRUE);
    user_dict = _tomoe_config_key_file_get_boolean (key_file, dict_name,
                                                    "user", TRUE);
    if (!user_dict) {
        gchar *tmp;
        tmp = g_build_filename (DICT_DATADIR, database, NULL);
        g_free (database);
        database = tmp;
    }

    dict = tomoe_dict_new ("est",
                           "name", name,
                           "database", database,
                           "editable", editable,
                           NULL);

    g_free (name);
    g_free (database);

    return dict;
}

static TomoeDict *
load_unihan_dictionary (GKeyFile *key_file, const gchar *dict_name)
{
    TomoeDict *dict;
    gchar *name;

    name = _tomoe_config_key_file_get_string (key_file, dict_name,
                                              "name", NULL);
    dict = tomoe_dict_new ("unihan",
                           "name", name,
                           NULL);
    g_free (name);

    return dict;
}

static TomoeDict *
load_mysql_dictionary (GKeyFile *key_file, const gchar *dict_name)
{
    TomoeDict *dict;
    gchar *name, *user, *password, *host, *socket, *database;
    gint port;
    gboolean editable;

    name = _tomoe_config_key_file_get_string (key_file, dict_name,
                                              "name", NULL);
    editable = _tomoe_config_key_file_get_boolean (key_file, dict_name,
                                                   "editable", TRUE);
    user = _tomoe_config_key_file_get_string (key_file, dict_name,
                                              "user", NULL);
    password = _tomoe_config_key_file_get_string (key_file, dict_name,
                                                  "password", NULL);
    host = _tomoe_config_key_file_get_string (key_file, dict_name,
                                              "host", NULL);
    port = _tomoe_config_key_file_get_integer (key_file, dict_name,
                                               "port", 0);
    socket = _tomoe_config_key_file_get_string (key_file, dict_name,
                                                "socket", NULL);
    database = _tomoe_config_key_file_get_string (key_file, dict_name,
                                                  "database", NULL);

    dict = tomoe_dict_new ("mysql",
                           "name", name,
                           "editable", editable,
                           "user", user,
                           "password", password,
                           "host", host,
                           "port", port,
                           "socket", socket,
                           "database", database,
                           NULL);
    g_free (name);

    return dict;
}

static TomoeDict *
_tomoe_config_load_dictionary (GKeyFile *key_file,
                               const gchar *dict_name,
                               const gchar *type)
{
    if (strcmp (type, "xml") == 0) {
        return load_xml_dictionary (key_file, dict_name);
    } else if (strcmp (type, "est") == 0) {
        return load_est_dictionary (key_file, dict_name);
    } else if (strcmp (type, "unihan") == 0) {
        return load_unihan_dictionary (key_file, dict_name);
    } else if (strcmp (type, "mysql") == 0) {
        return load_mysql_dictionary (key_file, dict_name);
    } else {
        return NULL;
    }
}

/*
 * vi:ts=4:nowrap:ai:expandtab
 */
