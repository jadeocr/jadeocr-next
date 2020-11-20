# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-

ruby_lib_DATA = tomoe.rb

EXTRA_DIST = $(ruby_lib_DATA)

ruby_binding_HEADERS = rbtomoe.h
ruby_binding_LTLIBRARIES = tomoe.la

INCLUDES =		\
	$(TOMOE_CFLAGS)	\
	$(RUBY_CFLAGS)

tomoe_la_SOURCES =		\
	tomoe-rb.c tomoe-rb.h	\
	tomoe-rb-candidate.c	\
	tomoe-rb-char.c		\
	tomoe-rb-config.c	\
	tomoe-rb-context.c	\
	tomoe-rb-dict.c		\
	tomoe-rb-query.c	\
	tomoe-rb-reading.c	\
	tomoe-rb-recognizer.c	\
	tomoe-rb-writing.c

tomoe_la_LDFLAGS =				\
	-module					\
	-avoid-version				\
	$(RUBY_LDFLAGS)				\
	$(RUBY_EXT_LDFLAGS)

tomoe_la_LIBADD =			\
	$(TOMOE_LIBS)			\
	$(RUBY_LIBS)			\
	$(top_builddir)/lib/libtomoe.la
