# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-

## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2005 Takuro Ashie <ashie@homa.ne.jp>
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this program; if not, write to the
## Free Software Foundation, Inc., 59 Temple Place, Suite 330,
## Boston, MA  02111-1307  USA

ACLOCAL_AMFLAGS = -I macros

SUBDIRS = po lib bindings module data db test benchmark doc spec

EXTRA_DIST =					\
	autogen.sh				\
	tomoe.pc				\
	intltool-update.in			\
	intltool-merge.in			\
	intltool-extract.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tomoe.pc

CLEANFILES = *~ *.bak
DISTCLEANFILES = tomoe.pc

# build documentation when doing distcheck
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

doc: all
	cd doc && $(MAKE) $(AM_MAKEFLAGS) doc

validate:
	cd data && $(MAKE) $(AM_MAKEFLAGS) validate

benchmark: all
	cd benchmark && $(MAKE) $(AM_MAKEFLAGS) benchmark
