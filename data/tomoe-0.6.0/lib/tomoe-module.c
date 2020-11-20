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
 *  $Id: tomoe-module.c 1206 2007-04-20 10:29:28Z ktou $
 */

#include <stdlib.h>
#include <string.h>

#include <gmodule.h>

#include "tomoe-module.h"
#include "tomoe-module-impl.h"

#define TOMOE_MODULE_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_MODULE, TomoeModulePrivate))

typedef struct _TomoeModulePrivate  TomoeModulePrivate;
struct _TomoeModulePrivate
{
    GModule      *library;
    gchar        *mod_path;
    GList        *registered_types;

    TomoeModuleInitFunc         init;
    TomoeModuleExitFunc         exit;
    TomoeModuleInstantiateFunc  instantiate;
    TomoeModuleGetLogDomainFunc get_log_domain;
};

G_DEFINE_TYPE (TomoeModule, tomoe_module, G_TYPE_TYPE_MODULE)

static void     finalize        (GObject     *object);
static gboolean load            (GTypeModule *module);
static void     unload          (GTypeModule *module);

static void     _tomoe_module_show_error   (GModule     *module);
static GModule *_tomoe_module_open         (const gchar *mod_path);
static void     _tomoe_module_close        (GModule     *module);
static gboolean _tomoe_module_load_func    (GModule     *module,
                                            const gchar *func_name,
                                            gpointer    *symbol);
static gboolean _tomoe_module_match_name   (const gchar *mod_path,
                                            const gchar *name);

static void
tomoe_module_class_init (TomoeModuleClass *klass)
{
    GObjectClass *gobject_class;
    GTypeModuleClass *type_module_class;

    gobject_class = G_OBJECT_CLASS (klass);
    gobject_class->finalize     = finalize;

    type_module_class = G_TYPE_MODULE_CLASS (klass);
    type_module_class->load     = load;
    type_module_class->unload   = unload;

    g_type_class_add_private (gobject_class, sizeof (TomoeModulePrivate));
}

static void
tomoe_module_init (TomoeModule *module)
{
    TomoeModulePrivate *priv = TOMOE_MODULE_GET_PRIVATE (module);

    priv->library          = NULL;
    priv->mod_path         = NULL;
    priv->registered_types = NULL;
}

static void
finalize (GObject *object)
{
    TomoeModulePrivate *priv = TOMOE_MODULE_GET_PRIVATE (object);

    g_free (priv->mod_path);
    priv->mod_path = NULL;
    g_list_free (priv->registered_types);
    priv->registered_types = NULL;

    G_OBJECT_CLASS (tomoe_module_parent_class)->finalize (object);
}

static gboolean
load (GTypeModule *module)
{
    TomoeModulePrivate *priv = TOMOE_MODULE_GET_PRIVATE (module);

    priv->library = _tomoe_module_open (priv->mod_path);
    if (!priv->library)
        return FALSE;

    if (!_tomoe_module_load_func (priv->library,
                                  G_STRINGIFY (TOMOE_MODULE_IMPL_INIT),
                                  (gpointer )&priv->init) ||
        !_tomoe_module_load_func (priv->library,
                                  G_STRINGIFY (TOMOE_MODULE_IMPL_EXIT),
                                  (gpointer )&priv->exit) ||
        !_tomoe_module_load_func (priv->library,
                                  G_STRINGIFY (TOMOE_MODULE_IMPL_INSTANTIATE),
                                  (gpointer )&priv->instantiate) ||
        !_tomoe_module_load_func (priv->library,
                                  G_STRINGIFY (TOMOE_MODULE_IMPL_GET_LOG_DOMAIN),
                                  (gpointer )&priv->get_log_domain)) {
        _tomoe_module_close (priv->library);
        priv->library = NULL;
        return FALSE;
    }

    g_list_free (priv->registered_types);
    priv->registered_types = priv->init (module);

    return TRUE;
}

static void
unload (GTypeModule *module)
{
    TomoeModulePrivate *priv = TOMOE_MODULE_GET_PRIVATE (module);

    priv->exit ();

    _tomoe_module_close (priv->library);
    priv->library  = NULL;

    priv->init = NULL;
    priv->exit = NULL;
    priv->instantiate = NULL;
    priv->get_log_domain = NULL;

    g_list_free (priv->registered_types);
    priv->registered_types = NULL;
}

GList *
tomoe_module_collect_registered_types (GList *modules)
{
    GList *results = NULL;
    GList *node;

    for (node = modules; node; node = g_list_next (node)) {
        TomoeModule *module = node->data;
        GTypeModule *g_type_module;

        g_type_module = G_TYPE_MODULE (module);
        if (g_type_module_use (g_type_module)) {
            TomoeModulePrivate *priv;
            GList *node;

            priv = TOMOE_MODULE_GET_PRIVATE (module);
            for (node = priv->registered_types;
                 node;
                 node = g_list_next (node)) {
                results = g_list_prepend (results, node->data);
            }

            g_type_module_unuse (g_type_module);
        }
    }

    return results;
}

GList *
tomoe_module_collect_log_domains (GList *modules)
{
    GList *results = NULL;
    GList *node;

    for (node = modules; node; node = g_list_next (node)) {
        TomoeModule *module = node->data;
        GTypeModule *g_type_module;

        g_type_module = G_TYPE_MODULE (module);
        if (g_type_module_use (g_type_module)) {
            gchar *log_domain;
            TomoeModulePrivate *priv;

            priv = TOMOE_MODULE_GET_PRIVATE (module);
            log_domain = priv->get_log_domain ();
            if (log_domain) {
                results = g_list_prepend (results, log_domain);
            }

            g_type_module_unuse (g_type_module);
        }
    }

    return results;
}

static void
_tomoe_module_show_error (GModule *module)
{
    gchar *message;

    if (!g_module_error ()) return;
    message = g_locale_to_utf8 (g_module_error(), -1, NULL, NULL, NULL);

    if (module) {
        gchar *name;
        name = g_strdup (g_module_name (module));
        g_warning ("%s: %s", name, message);
        g_free (name);
    } else {
        g_warning ("%s", message);
    }

    g_free (message);
}

TomoeModule *
tomoe_module_find (GList *modules, const gchar *name)
{
    GList *node;

    for (node = modules; node; node = g_list_next(node)) {
        TomoeModule *module = node->data;
        TomoeModulePrivate *priv;

        priv = TOMOE_MODULE_GET_PRIVATE(module);
        if (_tomoe_module_match_name(priv->mod_path, name))
            return module;
    }

    return NULL;
}

GObject *
tomoe_module_instantiate (TomoeModule *module,
                          const gchar *first_property, va_list var_args)
{
    GObject *object = NULL;
    TomoeModulePrivate *priv;

    priv = TOMOE_MODULE_GET_PRIVATE (module);
    if (g_type_module_use (G_TYPE_MODULE(module))) {
        object = priv->instantiate (first_property, var_args);
        g_type_module_unuse (G_TYPE_MODULE(module));
    }

    return object;
}


static GModule *
_tomoe_module_open (const gchar *mod_path)
{
    GModule *module;

    module = g_module_open (mod_path, G_MODULE_BIND_LAZY);
    if (!module) {
        _tomoe_module_show_error (NULL);
    }

    return module;
}

static void
_tomoe_module_close (GModule *module)
{
    if (module && g_module_close (module)) {
        _tomoe_module_show_error (NULL);
    }
}

static gchar *
_tomoe_module_module_file_name (const gchar *name)
{
    return g_strconcat(name, "." G_MODULE_SUFFIX, NULL);
}

static gboolean
_tomoe_module_load_func (GModule *module, const gchar *func_name,
                         gpointer *symbol)
{
    g_return_val_if_fail (module, FALSE);

    if (g_module_symbol (module, func_name, symbol)) {
        return TRUE;
    } else {
        _tomoe_module_show_error (module);
        return FALSE;
    }
}

TomoeModule *
tomoe_module_load_module (const gchar *base_dir, const gchar *name)
{
    gchar *mod_base_name, *mod_path;
    TomoeModule *module = NULL;

    mod_base_name = g_build_filename(base_dir, name, NULL);
    if (g_str_has_suffix(mod_base_name, G_MODULE_SUFFIX)) {
        mod_path = mod_base_name;
    } else {
        mod_path = _tomoe_module_module_file_name(mod_base_name);
        g_free(mod_base_name);
    }

    if (g_file_test(mod_path, G_FILE_TEST_EXISTS)) {
        TomoeModulePrivate *priv;
        gchar *mod_name;

        module = g_object_new(TOMOE_TYPE_MODULE, NULL);
        priv = TOMOE_MODULE_GET_PRIVATE(module);
        priv->mod_path = g_strdup(mod_path);

        mod_name = g_strdup(name);
        if (g_str_has_suffix(mod_name, "."G_MODULE_SUFFIX)) {
            guint last_index;
            last_index =
                strlen(mod_name) - strlen("."G_MODULE_SUFFIX);
            mod_name[last_index] = '\0';
        }
        g_type_module_set_name(G_TYPE_MODULE(module), mod_name);
        g_free(mod_name);
    }
    g_free(mod_path);

    return module;
}

GList *
tomoe_module_load_modules (const gchar *base_dir)
{
    return tomoe_module_load_modules_unique(base_dir, NULL);
}

GList *
tomoe_module_load_modules_unique (const gchar *base_dir, GList *exist_modules)
{
    GDir *dir;
    GList *modules = NULL;
    const gchar *entry;

    dir = g_dir_open(base_dir, 0, NULL);
    if (!dir)
        return modules;

    while ((entry = g_dir_read_name(dir))) {
        TomoeModule *module;

        module = tomoe_module_load_module(base_dir, entry);
        if (module)
        {
            GTypeModule *g_module;

            g_module = G_TYPE_MODULE(module);
            if (tomoe_module_find(exist_modules, g_module->name))
                tomoe_module_unload(module);
            else
                modules = g_list_prepend(modules, module);
        }
    }
    g_dir_close(dir);

    return modules;
}

static gboolean
_tomoe_module_match_name (const gchar *mod_path, const gchar *name)
{
    gboolean matched;
    gchar *module_base_name, *normalized_matched_name;

    module_base_name = g_path_get_basename (mod_path);
    normalized_matched_name = g_strconcat (name, "." G_MODULE_SUFFIX, NULL);

    matched = (0 == strcmp (module_base_name, normalized_matched_name));

    g_free (module_base_name);
    g_free (normalized_matched_name);

    return matched;
}

void
tomoe_module_unload (TomoeModule *module)
{
    GTypeModule *type_module;

    g_return_if_fail (TOMOE_IS_MODULE (module));

    type_module = G_TYPE_MODULE (module);

    if (type_module->type_infos || type_module->interface_infos)
        return;

    g_object_unref (module);
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
