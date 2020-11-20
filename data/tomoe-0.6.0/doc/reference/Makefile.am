MAINTAINERCLEANFILE = Makefile.in

## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = 1.6

# The name of the module.
DOC_MODULE=tomoe

# The top-level SGML file.
DOC_MAIN_SGML_FILE=tomoe-docs.sgml

# Extra options to supply to gtkdoc-scan
SCAN_OPTIONS=

# The directory containing the source code. Relative to $(srcdir)
DOC_SOURCE_DIR=../../lib

# Used for dependencies
HFILE_GLOB=$(top_srcdir)/lib/*.h
CFILE_GLOB=$(top_srcdir)/lib/*.c

# Headers to ignore
IGNORE_HFILES=			\
	glib-utils.h		\
	config.h

INCLUDES = \
	-I$(top_srcdir)/lib 	\
	-I$(top_builddir)/lib	\
	$(GLIB_CFLAGS)
GTKDOC_LIBS = \
	$(top_builddir)/lib/libtomoe.la \
	$(GLIB_LIBS)

# Extra options to supply to gtkdoc-mkdb
MKDB_OPTIONS=--sgml-mode --output-format=xml --ignore-files=trio

# Images to copy into HTML directory
HTML_IMAGES =

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE)
content_files =				\
	version.xml

# Extra options to supply to gtkdoc-fixref
FIXXREF_OPTIONS=

# include common portion ...
include $(top_srcdir)/gtk-doc.make

# Other files to distribute
EXTRA_DIST +=				\
	version.xml.in
