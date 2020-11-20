# -*- Mode: Makefile; tab-width: 8; indent-tabs-mode: t; -*-

.PHONY: benchamrk

benchmark_files =		\
	load-dict.rb		\
	search-by-recognizer.rb	\
	search-by-dict.rb	\
	convert-char.rb

benchmark: $(top_builddir)/bindings/ruby/tomoe.la
	for bench in $(benchmark_files);	\
	do					\
	  echo "running benchmark: $$bench";	\
	  $(RUBY) $$bench;			\
	done

EXTRA_DIST = $(benchmark_files)
