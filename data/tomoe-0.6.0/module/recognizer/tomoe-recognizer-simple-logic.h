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
 *  $Id: tomoe-recognizer-simple-logic.h 893 2006-11-29 06:49:39Z makeinu $
 */

/**
 *  @file tomoe-recognizer-simple-logic.h
 *  @brief Provide a set of API to recognize handwriting.
 */

#ifndef __TOMOE_RECOGNIZER_SIMPLE_LOGIC_IMPL_H__
#define __TOMOE_RECOGNIZER_SIMPLE_LOGIC_IMPL_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "tomoe-dict.h"
#include "tomoe-recognizer.h"

GList     *_tomoe_recognizer_simple_get_candidates (TomoeRecognizer *recognizer,
                                                    TomoeDict    *dict,
                                                    TomoeWriting *input);

#ifdef	__cplusplus
}
#endif

#endif /* __TOMOE_RECOGNIZER_SIMPLE_LOGIC_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
