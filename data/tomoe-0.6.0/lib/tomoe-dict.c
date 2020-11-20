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
 *  $Id: tomoe-dict.c 1534 2007-06-20 11:53:14Z ikezoe $
 */

#include "tomoe-dict.h"
#include "tomoe-module.h"
#include "tomoe-candidate.h"

static GList *dicts = NULL;
static gchar *module_dir = NULL;

void tomoe_dict_init (void)
{
}

void tomoe_dict_quit (void)
{
    tomoe_dict_unload ();
    tomoe_dict_set_default_module_dir (NULL);
}

const gchar *
tomoe_dict_get_default_module_dir (void)
{
    return module_dir;
}

void
tomoe_dict_set_default_module_dir (const gchar *dir)
{
    if (module_dir)
        g_free (module_dir);
    module_dir = NULL;

    if (dir)
        module_dir = g_strdup (dir);
}

static const gchar *
_tomoe_dict_module_dir (void)
{
    const gchar *dir;

    if (module_dir)
        return module_dir;

    dir = g_getenv ("TOMOE_DICT_MODULE_DIR");
    if (dir)
        return dir;

    return DICT_MODULEDIR;
}

void
tomoe_dict_load (const gchar *base_dir)
{
    if (!base_dir)
        base_dir = _tomoe_dict_module_dir ();

    dicts = g_list_concat (tomoe_module_load_modules (base_dir), dicts);
}

TomoeModule *
tomoe_dict_load_module (const gchar *name)
{
    TomoeModule *module;

    module = tomoe_module_find (dicts, name);
    if (module)
        return module;

    module = tomoe_module_load_module (_tomoe_dict_module_dir (), name);
    if (module) {
        if (g_type_module_use (G_TYPE_MODULE (module))) {
            dicts = g_list_prepend (dicts, module);
            g_type_module_unuse (G_TYPE_MODULE (module));
        }
    }

    return module;
}

void
tomoe_dict_unload (void)
{
    g_list_foreach (dicts, (GFunc) tomoe_module_unload, NULL);
    g_list_free (dicts);
    dicts = NULL;
}

GList *
tomoe_dict_get_registered_types (void)
{
    return tomoe_module_collect_registered_types (dicts);
}

GList *
tomoe_dict_get_log_domains (void)
{
    return tomoe_module_collect_log_domains (dicts);
}

#define tomoe_dict_init init
G_DEFINE_ABSTRACT_TYPE (TomoeDict, tomoe_dict, G_TYPE_OBJECT)
#undef tomoe_dict_init

static void
tomoe_dict_class_init (TomoeDictClass *klass)
{
    klass->get_name        = NULL;
    klass->register_char   = NULL;
    klass->unregister_char = NULL;
    klass->get_char        = NULL;
    klass->search          = NULL;
    klass->flush           = NULL;
    klass->is_editable     = NULL;
    klass->is_available    = NULL;
    klass->get_available_private_utf8 = NULL;
}

static void
init (TomoeDict *dict)
{
}

/**
 * tomoe_dict_new:
 * @name: The name of dictionary type.
 * @first_property: the name of the first property.
 * @... :  the value of the first property, followed optionally by more name/value pairs, followed by NULL
 *
 * Create a new #TomoeDict.
 *
 * Return value: a new #TomoeDict.
 */
TomoeDict *
tomoe_dict_new (const gchar *name, const gchar *first_property, ...)
{
    TomoeModule *module;
    GObject *dict;
    va_list var_args;

    module = tomoe_dict_load_module (name);
    g_return_val_if_fail (module != NULL, NULL);

    va_start (var_args, first_property);
    dict = tomoe_module_instantiate (module, first_property, var_args);
    va_end (var_args);

    return TOMOE_DICT (dict);
}

/**
 * tomoe_dict_get_name:
 * @dict: a #TomoeDict.
 *
 * Get the dictionary name.
 *
 * Return value: the name of the dictionary.
 */
const gchar *
tomoe_dict_get_name (TomoeDict *dict)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), NULL);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->get_name)
        return klass->get_name (dict);
    else
        return NULL;
}

/**
 * tomoe_dict_register_char:
 * @dict: a TomoeDict object.
 * @chr: a #TomoeChar object to register.
 *
 * Register a #TomoeChar object.
 *
 * Return value: TRUE if success.
 */
gboolean
tomoe_dict_register_char (TomoeDict *dict, TomoeChar *chr)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), FALSE);

    if (!tomoe_dict_is_editable (dict)) {
        g_warning ("the dictionary isn't editable.");
        return FALSE;
    }

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->register_char) {
        gboolean need_to_set_utf8, success;

        need_to_set_utf8 = !tomoe_char_get_utf8 (chr);
        if (need_to_set_utf8) {
            gchar *utf8;
            utf8 = tomoe_dict_get_available_private_utf8 (dict);
            if (!utf8) {
                g_warning ("there is no available PUA(Private Use Area)");
                return FALSE;
            }
            tomoe_char_set_utf8 (chr, utf8);
            g_free (utf8);
        }
        success = klass->register_char (dict, chr);
        if (!success && need_to_set_utf8)
            tomoe_char_set_utf8 (chr, NULL);
        return success;
    } else {
        return FALSE;
    }
}

/**
 * tomoe_dict_unregister_char:
 * @dict: a TomoeDict object.
 * @utf8: UTF-8 encoded value of the character.
 *
 * Unregister a #TomoeChar object which has utf8 code point.
 *
 * Return value: TRUE if success.
 */
gboolean
tomoe_dict_unregister_char (TomoeDict *dict, const gchar *utf8)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), FALSE);

    if (!tomoe_dict_is_editable (dict)) {
        g_warning ("the dictionary isn't editable.");
        return FALSE;
    }

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->unregister_char)
        return klass->unregister_char (dict, utf8);
    else
        return FALSE;
}

/**
 * tomoe_dict_get_char:
 * @dict: a TomoeDict object.
 * @utf8: UTF-8 encoded value of the character.
 *
 * Get a #TomoeChar object which has utf8 code point
 *
 * Return value: a #TomoeChar object.
 */
TomoeChar *
tomoe_dict_get_char (TomoeDict *dict, const gchar *utf8)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), NULL);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->get_char)
        return klass->get_char (dict, utf8);
    else
        return FALSE;
}

GList *
tomoe_dict_search (TomoeDict *dict, TomoeQuery *query)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), NULL);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->search)
        return klass->search (dict, query);
    else
        return NULL;
}

gboolean
tomoe_dict_flush (TomoeDict *dict)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), FALSE);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->flush)
        return klass->flush (dict);
    else
        return FALSE;
}

gboolean
tomoe_dict_copy (TomoeDict *src_dict, TomoeDict *dest_dict)
{
    g_return_val_if_fail (TOMOE_IS_DICT (src_dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dest_dict), FALSE);

    if (!tomoe_dict_is_editable (dest_dict)) {
        g_warning ("destination dictionary isn't editable.");
        return FALSE;
    }

    if (TOMOE_DICT_GET_CLASS (src_dict)->copy)
        return TOMOE_DICT_GET_CLASS (src_dict)->copy (src_dict, dest_dict);

    return tomoe_dict_plain_copy (src_dict, dest_dict);
}

gboolean
tomoe_dict_plain_copy (TomoeDict *src_dict, TomoeDict *dest_dict)
{
    TomoeQuery *query;
    GList *cands = NULL, *node;

    g_return_val_if_fail (TOMOE_IS_DICT (src_dict), FALSE);
    g_return_val_if_fail (TOMOE_IS_DICT (dest_dict), FALSE);

    if (!tomoe_dict_is_available (src_dict)) {
        g_warning ("source dictionary isn't available.");
        return FALSE;
    }
    if (!tomoe_dict_is_editable (dest_dict)) {
        g_warning ("destination dictionary isn't editable.");
        return FALSE;
    }

    query = tomoe_query_new ();

    cands = tomoe_dict_search (dest_dict, query);
    for (node = cands; node; node = g_list_next (node)) {
        TomoeChar *chr = tomoe_candidate_get_char (TOMOE_CANDIDATE (node->data));
        tomoe_dict_unregister_char (dest_dict, tomoe_char_get_utf8 (chr));
    }
    
    if (cands) {
        g_list_foreach (cands, (GFunc) g_object_unref, NULL);
        g_list_free (cands);
    }
    
    cands = tomoe_dict_search (src_dict, query);
    for (node = cands; node; node = g_list_next (node)) {
        TomoeChar *chr = tomoe_candidate_get_char (TOMOE_CANDIDATE (node->data));
        TomoeChar *new_chr = tomoe_char_dup (chr);
        tomoe_dict_register_char (dest_dict, new_chr);
        g_object_unref (new_chr);
    }
    
    if (cands) {
        g_list_foreach (cands, (GFunc) g_object_unref, NULL);
        g_list_free (cands);
    }

    g_object_unref (query);

    return TRUE;
}

gboolean
tomoe_dict_is_editable (TomoeDict *dict)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), FALSE);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->is_editable)
        return klass->is_editable (dict);
    else
        return FALSE;
}

gboolean
tomoe_dict_is_available (TomoeDict *dict)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), FALSE);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->is_available)
        return klass->is_available (dict);
    else
        return FALSE;
}

gchar *
tomoe_dict_get_available_private_utf8 (TomoeDict *dict)
{
    TomoeDictClass *klass;

    g_return_val_if_fail (TOMOE_IS_DICT (dict), NULL);

    klass = TOMOE_DICT_GET_CLASS (dict);
    if (klass->get_available_private_utf8) {
        return klass->get_available_private_utf8 (dict);
    } else {
        g_warning ("%s backend doesn't support PUA(Private Use Area)",
                   tomoe_dict_get_name (dict));
        return NULL;
    }
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
