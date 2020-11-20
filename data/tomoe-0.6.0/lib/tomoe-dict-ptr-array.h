/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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
 *  $Id: tomoe-dict-ptr-array.h 1486 2007-06-15 12:48:40Z ikezoe $
 */

#ifndef __TOMOE_DICT_PTR_ARRAY_H__
#define __TOMOE_DICT_PTR_ARRAY_H__

#include <glib.h>

G_BEGIN_DECLS

#include "tomoe-dict.h"

#define TOMOE_TYPE_DICT_PTR_ARRAY            (_tomoe_dict_ptr_array_get_type ())
#define TOMOE_DICT_PTR_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICT_PTR_ARRAY, TomoeDictPtrArray))
#define TOMOE_DICT_PTR_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICT_PTR_ARRAY, TomoeDictPtrArrayClass))
#define TOMOE_IS_DICT_PTR_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICT_PTR_ARRAY))
#define TOMOE_IS_DICT_PTR_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICT_PTR_ARRAY))
#define TOMOE_DICT_PTR_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_DICT_PTR_ARRAY, TomoeDictPtrArrayClass))

typedef struct _TomoeDictPtrArray TomoeDictPtrArray;
typedef struct _TomoeDictPtrArrayClass TomoeDictPtrArrayClass;

struct _TomoeDictPtrArray
{
    TomoeDict object;
};

struct _TomoeDictPtrArrayClass
{
    TomoeDictClass parent_class;
};

GType     _tomoe_dict_ptr_array_get_type        (void) G_GNUC_CONST;
void      _tomoe_dict_ptr_array_sort            (TomoeDictPtrArray *dict);
GPtrArray *_tomoe_dict_ptr_array_get_array      (TomoeDictPtrArray *dict);

G_END_DECLS

#endif /* __TOMOE_DICT_PTR_ARRAY_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
