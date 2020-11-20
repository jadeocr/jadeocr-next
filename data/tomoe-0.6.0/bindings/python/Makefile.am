pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pytomoe.pc

pytomoe_public_h_sources =		\
	pytomoe-utils.h

pkginclude_HEADERS =			\
	$(pytomoe_public_h_sources)

pyexec_LTLIBRARIES = tomoe.la
defsdir = $(pkgdatadir)/python

INCLUDES =			\
	$(PYTHON_CFLAGS)	\
	$(PYGOBJECT_CFLAGS)	\
	$(TOMOE_CFLAGS)

nodist_tomoe_la_SOURCES =	\
	tomoe.c

tomoe_la_SOURCES =		\
	tomoe-module.c

tomoe_la_LDFLAGS =			\
	-module				\
	-avoid-version			\
	$(TOMOE_LDFLAGS)		\
	$(PYTHON_LDFLAGS)

tomoe_la_LIBADD =			\
	$(TOMOE_LIBS)			\
	$(PYTHON_LIBS)			\
	$(top_builddir)/lib/libtomoe.la

OVERRIDES =					\
	tomoe.override				\
	tomoe-char.override			\
	tomoe-config.override			\
	tomoe-context.override			\
	tomoe-dict.override			\
	tomoe-query.override			\
	tomoe-reading.override			\
	tomoe-recognizer.override		\
	tomoe-shelf.override			\
	tomoe-writing.override

defs_DATA = tomoe.defs

tomoe.c: $(defs_DATA) $(OVERRIDES)

CLEANFILES = tomoe.c
EXTRA_DIST = $(defs_DATA) $(OVERRIDES) 

.defs.c:
	(cd $(srcdir)				\
	 && $(PYGTK_CODEGEN)			\
	    --override $*.override		\
	    --prefix py$* $*.defs) > gen-$*.c	\
	 && cp gen-$*.c $*.c			\
	 && rm -f gen-$*.c
