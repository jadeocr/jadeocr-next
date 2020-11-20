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

SUBDIRS = data

if WITH_PYTHON
SUBDIRS += python
endif

EXTRA_DIST =			\
	run-test.rb		\
	run-tests.sh		\
	tomoe-test-utils.rb	\
	test_char.rb		\
	test_config.rb		\
	test_context.rb		\
	test_dict.rb		\
	test_writing.rb

dict_modules = xml
if USE_EST
dict_modules += est
endif
# if USE_DICT_RUBY
# dict_modules += ruby
# endif
if USE_UNIHAN
dict_modules += unihan
endif
# if USE_SVN
# dict_modules += svn
# endif
if USE_MYSQL
dict_modules += mysql
endif

TESTS = run-tests.sh
TESTS_ENVIRONMENT = DICT_MODULES="$(dict_modules)"

output_dict_modules:
	@echo $(dict_modules)

output_RUBY:
	@echo $(RUBY)
