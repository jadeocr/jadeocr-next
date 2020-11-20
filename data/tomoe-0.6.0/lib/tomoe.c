/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2000 - 2004 Hiroyuki Komatsu <komatsu@taiyaki.org>
 *  Copyright (C) 2004 Hiroaki Nakamura <hnakamur@good-day.co.jp>
 *  Copyright (C) 2005 Hiroyuki Ikezoe <poincare@ikezoe.net>
 *  Copyright (C) 2006 Juernjakob Harder <juernjakob.harder@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your opion) any later version.
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
 *  $Id: tomoe.c 1296 2007-06-06 07:35:27Z ktou $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/garray.h>
#include "tomoe.h"

static gboolean initialized = FALSE;

/**
 * tomoe_init:
 *
 * Initialize tomoe library.
 *
 */
void
tomoe_init (void)
{
    if (!initialized) {
        initialized = TRUE;
        GTypeDebugFlags debug_flag = G_TYPE_DEBUG_NONE;
        g_type_init_with_debug_flags (debug_flag);
        tomoe_dict_init ();
        tomoe_recognizer_init ();
    }
}

/**
 * tomoe_quit:
 *
 * Terminate tomoe library.
 *
 */
void
tomoe_quit (void)
{
    if (initialized) {
        initialized = FALSE;
        tomoe_dict_quit ();
        tomoe_recognizer_quit ();
    }
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
