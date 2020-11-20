#!/bin/sh

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

run()
{
    $@
    if test $? -ne 0; then
	echo "Failed $@"
	exit 1
    fi
}

run libtoolize --copy --force
run glib-gettextize --force --copy
run intltoolize --force --copy --automake
run gtkdocize --copy
run aclocal -I macros
run autoheader
run automake --add-missing --foreign --copy
run autoconf
