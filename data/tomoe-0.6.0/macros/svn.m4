# -*- autoconf -*-
AC_DEFUN([SVN_CHECK],
[
PKG_CHECK_MODULES(APR, apr-1, [use_svn=yes], [use_svn=no])

svn_includes="/usr/local/include /usr/include"
AC_ARG_WITH(svn-include,
            AS_HELP_STRING([--with-svn-include=DIR],
                           [Use the given path to the subversion headers.]),
            [if test "$withval" != "yes" -a "$withval" != ""; then
               svn_includes=$withval
             fi])
AC_MSG_CHECKING([for Subversion headers])
svn_include=""
for value in $svn_includes ; do
    if test -f $value/subversion-1/svn_types.h ; then
        svn_include=$value
        break
    fi
done
if test "x$svn_include" != "x" ; then
    AC_MSG_RESULT([$svn_include])
else
    AC_MSG_RESULT([not found])
    use_svn="no"
fi
SVN_CFLAGS="\$(APR_CFLAGS) -I$svn_include/subversion-1"

svn_libs="/usr/local/lib /usr/lib"
AC_ARG_WITH(svn-lib,
            AS_HELP_STRING([--with-svn-lib=DIR],
                           [Use the given path to the subversion libraries.]),
            [if test "$withval" != "yes" -a "$withval" != ""; then
               SVN_LIBS=$withval
             fi])
AC_MSG_CHECKING([for Subversion libraries])
svn_lib=""
for value in $svn_libs ; do
  if ls $value/libsvn_client-1.* >/dev/null 2>&1; then
    svn_lib=$value
    break
  fi
done
if test "x$svn_lib" != "x" ; then
    AC_MSG_RESULT([$svn_lib])
else
    AC_MSG_RESULT([not found])
    use_svn="no"
fi
SVN_LIBS="\$(APR_LIBS) -L$svn_lib -lsvn_client-1 -lsvn_subr-1"

AC_SUBST(SVN_CFLAGS)
AC_SUBST(SVN_LIBS)

AM_CONDITIONAL([USE_SVN], [test "$use_svn" = "yes"])
])
