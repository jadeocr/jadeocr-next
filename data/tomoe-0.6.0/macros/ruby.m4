# -*- autoconf -*-
AC_DEFUN([RUBY_CHECK],
[
ruby_required_headers=$1
ruby_additional_cflags=$2

ruby_available="no"
AC_ARG_WITH([ruby],
            AS_HELP_STRING([--with-ruby=PATH],
                           [Ruby interpreter path (default: auto-detect)]),
            [RUBY="$withval"])

if test "$RUBY" = "no"; then
  : # ignore
elif test "$RUBY" = "" -o "$RUBY" = "yes"; then
  AC_PATH_PROG(RUBY, ruby, none)
  if test "$RUBY" != "none"; then
    ruby_available="yes"
  fi
else
  AC_CHECK_FILE([$RUBY],
                [ruby_available="yes"],
                [AC_MSG_WARN([$RUBY is not found. Disable Ruby binding.])])
fi

if test "$ruby_available" = "yes"; then
  rbconfig="$RUBY -rrbconfig -e "

  changequote(<<, >>)
  for var_name in archdir sitearchdir CFLAGS LIBRUBYARG libdir \
                  sitelibdir sitearchdir; do
    rbconfig_tmp=`$rbconfig "print Config::CONFIG['$var_name']"`
    eval "rbconfig_$var_name=\"$rbconfig_tmp\""
  done
  changequote([, ])

  RUBY_CFLAGS="$RUBY_CFLAGS -I$rbconfig_archdir "
  RUBY_CFLAGS="$RUBY_CFLAGS -I$rbconfig_sitearchdir "
  RUBY_CFLAGS="$RUBY_CFLAGS $rbconfig_CFLAGS "
  RUBY_LIBS="$rbconfig_LIBRUBYARG"
  RUBY_LDFLAGS="-L$rbconfig_libdir"

  AC_SUBST(RUBY_CFLAGS)
  AC_SUBST(RUBY_LIBS)
  AC_SUBST(RUBY_LDFLAGS)

  _SAVE_CFLAGS=$CFLAGS
  ruby_undef_package_macros="
#undef PACKAGE_NAME
#undef PACKAGE_TARNAME
#undef PACKAGE_STRING
#undef PACKAGE_VERSION
"
  ruby_disable_message="Disable Ruby binding."
  CFLAGS="$CFLAGS $RUBY_CFLAGS"
  AC_CHECK_HEADERS(ruby.h, [],
                   [ruby_available="no"
                    AC_MSG_WARN([$ruby_disable_message])],
                   [$ruby_undef_package_macros])
  if test "x$ruby_available" = "xyes" -a -n $ruby_required_headers; then
    CFLAGS="$CFLAGS $ruby_additional_cflags"
    AC_CHECK_HEADERS($ruby_required_headers, [],
    		     [ruby_available="no"
		      AC_MSG_WARN([$ruby_disable_message])],
		     [$ruby_undef_package_macros])
  fi
  CFLAGS=$_SAVE_CFLAGS

  AC_MSG_CHECKING([where to install Ruby binding])
  AC_CACHE_VAL([cv_ruby_archdir],
               [cv_ruby_archdir="$rbconfig_sitearchdir"])
  AC_ARG_WITH([ruby-bindingdir],
                AS_HELP_STRING([--with-ruby-bindingdir=EXTDIR],
                               [install Ruby bindings in EXTDIR
                                (default is same as ruby's one)]),
                [ruby_bindingdir="$withval"],
                [ruby_bindingdir="$cv_ruby_archdir"])
  AC_MSG_RESULT([$ruby_bindingdir])

  AC_MSG_CHECKING([where to install Ruby scripts])
  AC_CACHE_VAL([cv_ruby_sitedir],
               [cv_ruby_sitedir="$rbconfig_sitelibdir"])
  AC_ARG_WITH([ruby-libdir],
               AS_HELP_STRING([--with-ruby-libdir=LIBDIR],
                              [install Ruby scripts for bindings in LIBDIR
                               (default is same as ruby's one)]),
               [ruby_libdir="$withval"],
               [ruby_libdir="$cv_ruby_sitedir"])
  AC_MSG_RESULT([$ruby_libdir])

  AC_SUBST(ruby_bindingdir)
  AC_SUBST(ruby_libdir)
fi

AM_CONDITIONAL([WITH_RUBY], [test "$ruby_available" = "yes"])
])
