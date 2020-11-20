# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-

## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2006 Kouhei Sutou <kou@cozmixng.org>
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

CLEANFILES = *~ *.bak

AM_CPPFLAGS =							\
	-DRUBY_EXTDIR=\""$(ruby_extdir)"\"			\
	-DRUBY_LIBDIR=\""$(ruby_libdir)"\"

INCLUDES =				\
	-I$(top_srcdir)/bindings/ruby	\
	$(TOMOE_CFLAGS)			\
	$(EST_CFLAGS)			\
	$(SVN_CFLAGS)			\
	$(MYSQL_CFLAGS)			\
	$(RUBY_CFLAGS)

LIBADD = $(TOMOE_LIBS)			\
	$(top_builddir)/lib/libtomoe.la
LDFLAGS =							\
  -rpath $(dict_moduledir) -avoid-version -module		\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)

dict_module_LTLIBRARIES = xml.la

xml_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe/Dict:XML\" $(AM_CPPFLAGS)
xml_la_SOURCES = tomoe-dict-xml.c
xml_la_LIBADD = $(LIBADD) 

if USE_EST
dict_module_LTLIBRARIES += est.la

est_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe/Dict:Est\" $(AM_CPPFLAGS)
est_la_SOURCES = tomoe-dict-est.c
est_la_LIBADD = $(EST_LIBS) $(LIBADD)
endif

if USE_DICT_RUBY
dict_module_LTLIBRARIES += ruby.la

ruby_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe/Dict:Ruby\" $(AM_CPPFLAGS)
ruby_la_SOURCES = tomoe-dict-ruby.c
ruby_la_LIBADD = $(RUBY_LIBS) $(LIBADD)
endif

if USE_UNIHAN
dict_module_LTLIBRARIES += unihan.la

unihan_built_sources = tomoe-unihan-data.h

CLEANFILES += Unihan.txt.cache
MAINTAINERCLEANFILES = Unihan.stamp Unihan.txt
DISTCLEANFILES = Unihan.zip
BUILT_SOURCES = $(unihan_built_sources)

EXTRA_DIST = unihan-compiler.rb download.rb $(BUILT_SOURCES) Unihan.stamp

unihan_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe/Dict:Unihan\" $(AM_CPPFLAGS)
unihan_la_SOURCES =		\
	tomoe-dict-unihan.c	\
	tomoe-unihan.c		\
	tomoe-unihan.h		\
	$(unihan_built_sources)
unihan_la_LIBADD = $(LIBADD) 

unihan-compiler.rb: xml.la

$(top_builddir)/data/kanjidic2.xml:
	cd $(top_builddir)/data && $(MAKE) $(AM_MAKEFLAGS) kanjidic2.xml

tomoe-unihan-data.h: unihan-compiler.rb Unihan.stamp $(top_builddir)/data/kanjidic2.xml
	$(RUBY) -I$(top_srcdir)/bindings/ruby -I$(top_builddir)/bindings/ruby/.libs \
	  -I$(top_srcdir)/test unihan-compiler.rb \
	  Unihan.txt $(top_srcdir)/data/kanjidic2.xml > $@.tmp || exit 1;
	mv $@.tmp $@

Unihan_URL = ftp://ftp.unicode.org/Public/UNIDATA/Unihan.zip
Unihan.stamp:
	if test ! -f Unihan.txt; then					\
	  $(RUBY) $(srcdir)/download.rb $(Unihan_URL) || exit 1;	\
	  $(UNZIP) -o Unihan.zip || exit 1;				\
	fi
	touch $@
endif

if USE_SVN
dict_module_LTLIBRARIES += svn.la

svn_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe/Dict:Svn\" $(AM_CPPFLAGS)
svn_la_SOURCES = tomoe-dict-svn.c
svn_la_LIBADD = $(SVN_LIBS) $(LIBADD)
endif

if USE_MYSQL
dict_module_LTLIBRARIES += mysql.la

mysql_la_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe/Dict:MySQL\" $(AM_CPPFLAGS)
mysql_la_SOURCES = tomoe-dict-mysql.c
mysql_la_LIBADD = $(MYSQL_LIBS) $(LIBADD)
endif
