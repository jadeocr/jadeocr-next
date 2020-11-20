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
 *  $Id: glib-compat-file-utilities.h 1081 2006-12-25 03:43:34Z kous $
 */

#ifndef __GLIB_COMPAT_FILE_UTILITIES_H__
#define __GLIB_COMPAT_FILE_UTILITIES_H__

#include <glib.h>

#if GLIB_CHECK_VERSION(2, 6, 0)
#  include <glib/gstdio.h>
#else
#  include "config.h"
#  include <stdio.h>
#  ifdef HAVE_SYS_STAT_H
#    include <sys/stat.h>
#  endif
#  ifdef HAVE_SYS_TYPES_H
#    include <sys/types.h>
#  endif
#  define g_open(filename, flags, mode)	open (filename, flags, mode)
#  define g_filename_display_name(name)	g_strdup (name)
#  define g_unlink(filename)			unlink (filename)
#  define g_mkdir(filename, mode)		mkdir (filename, mode)
#  define g_rename(old_filename, new_filename)	\
										rename (old_filename, new_filename)
#endif

#ifndef O_BINARY
#  define O_BINARY 0
#endif

G_BEGIN_DECLS

G_END_DECLS

#endif /* __GLIB_COMPAT_FILE_UTILITIES_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
