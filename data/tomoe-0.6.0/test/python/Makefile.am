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

EXTRA_DIST =			\
	runtests.py		\
	test_common.py		\
	test_config.py		\
	test_context.py		\
	test_candidate.py	\
	test_char.py		\
	test_dict.py		\
	test_dict_est.py	\
	test_dict_mysql.py	\
	test_dict_unihan.py	\
	test_dict_xml.py	\
	test_query.py		\
	test_reading.py		\
	test_shelf.py		\
	test_writing.py

dict_modules = xml
if USE_EST
dict_modules += est
endif
if USE_UNIHAN
dict_modules += unihan
endif
if USE_MYSQL
dict_modules += mysql
endif

TESTS = runtests.py
TESTS_ENVIRONMENT = DICT_MODULES="$(dict_modules)"
