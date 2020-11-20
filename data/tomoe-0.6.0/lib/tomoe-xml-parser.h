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
 *  $Id: tomoe-xml-parser.h 921 2006-12-01 01:37:38Z ikezoe $
 */

#ifndef __TOMOE_XML_PARSER_H__
#define __TOMOE_XML_PARSER_H__

#include <glib.h>

G_BEGIN_DECLS

#include <tomoe-char.h>

typedef struct _TomoeXMLParsedData TomoeXMLParsedData;

struct _TomoeXMLParsedData
{
    gchar     *name;
    GPtrArray *chars;
};

gboolean   _tomoe_xml_parser_parse_dictionary_file (const gchar *filename,
                                                    TomoeXMLParsedData *result);
TomoeChar *_tomoe_xml_parser_parse_char_data       (const gchar *xml,
                                                    gssize      len);


G_END_DECLS

#endif /* __TOMOE_XML_PARSER_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
