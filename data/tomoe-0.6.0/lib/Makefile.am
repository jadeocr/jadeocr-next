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

CLEANFILES = *~ *.bak

AM_CPPFLAGS = -DG_LOG_DOMAIN=\"Tomoe\"
INCLUDES = $(TOMOE_CFLAGS)

tomoe_public_h_sources =		\
	tomoe.h				\
	tomoe-candidate.h		\
	tomoe-char.h			\
	tomoe-context.h			\
	tomoe-config.h			\
	tomoe-dict.h			\
	tomoe-module.h			\
	tomoe-module-impl.h		\
	tomoe-query.h			\
	tomoe-reading.h			\
	tomoe-recognizer.h		\
	tomoe-shelf.h			\
	tomoe-writing.h

enum_source_prefix = tomoe-enum-types

enum_sources_h = \
	$(tomoe_public_h_sources)

enum_sources =			\
	$(enum_source_prefix).c	\
	$(enum_source_prefix).h

BUILT_SOURCES = $(enum_sources)

pkginclude_HEADERS =			\
	$(tomoe_public_h_sources)	\
	$(enum_source_prefix).h

lib_LTLIBRARIES = libtomoe.la

libtomoe_la_SOURCES =		\
	glib-utils.c		\
	glib-utils.h		\
	glib-compat-file-utilities.c	\
	glib-compat-file-utilities.h	\
	glib-compat-key-file.c	\
	glib-compat-key-file.h	\
	tomoe-enum-types.c	\
	tomoe.c			\
	tomoe-candidate.c	\
	tomoe-char.c		\
	tomoe-context.c		\
	tomoe-config.c		\
	tomoe-dict.c		\
	tomoe-dict-ptr-array.c	\
	tomoe-dict-ptr-array.h	\
	tomoe-module.c		\
	tomoe-query.c		\
	tomoe-reading.c		\
	tomoe-recognizer.c	\
	tomoe-shelf.c		\
	tomoe-writing.c         \
	tomoe-xml-parser.c	\
	tomoe-xml-parser.h

libtomoe_la_LDFLAGS =						\
  -version-info $(LT_VERSION_INFO)				\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)

libtomoe_la_LD_ADD = \
	-lm

libtomoe_la_LIBADD = $(TOMOE_LIBS)


MAINTAINERCLEANFILES =			\
	stamp-$(enum_source_prefix)-c	\
	stamp-$(enum_source_prefix)-h

$(enum_source_prefix).c: stamp-$(enum_source_prefix)-c $(enum_source_prefix).h
	@true

stamp-$(enum_source_prefix)-c: $(enum_sources_h) Makefile
	(cd $(srcdir) && \
	  include_headers="" && \
	  for h in $(enum_sources_h); do \
	    include_headers="$${include_headers}#include \"$${h}\"\n"; \
	  done && \
	  $(GLIB_MKENUMS) \
	    --fhead "#include \"$(enum_source_prefix).h\"\n$${include_headers}" \
	    --fprod "\n/* enumerations from \"@filename@\" */" \
	    --vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" 	\
	    --vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
	    --vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (\"@EnumName@\", values);\n  }\n  return etype;\n}\n" \
	    $(enum_sources_h)) > tmp-$(enum_source_prefix).c && \
	(cmp -s tmp-$(enum_source_prefix).c $(enum_source_prefix).c || \
	  cp tmp-$(enum_source_prefix).c $(enum_source_prefix).c ) && \
	rm -f tmp-$(enum_source_prefix).c && \
	echo timestamp > $(@F)

$(enum_source_prefix).h: stamp-$(enum_source_prefix)-h
	@true

stamp-$(enum_source_prefix)-h: $(enum_sources_h) Makefile
	(cd $(srcdir) && \
	  mark="__`echo $(enum_source_prefix) | sed -e 's/-/_/g' | tr a-z A-Z`_H__" && \
	  $(GLIB_MKENUMS) \
	    --fhead "#ifndef $${mark}\n#define $${mark}\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS\n" \
	    --fprod "/* enumerations from \"@filename@\" */\n" \
	    --vhead "GType @enum_name@_get_type (void);\n#define TOMOE_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n" 	\
	    --ftail "G_END_DECLS\n\n#endif /* $${mark} */" \
	    $(enum_sources_h)) > tmp-$(enum_source_prefix).h && \
	(cmp -s tmp-$(enum_source_prefix).h $(enum_source_prefix).h || \
	  cp tmp-$(enum_source_prefix).h $(enum_source_prefix).h) && \
	rm -f tmp-$(enum_source_prefix).h && \
	echo timestamp > $(@F)
