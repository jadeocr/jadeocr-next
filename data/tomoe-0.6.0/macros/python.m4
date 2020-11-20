# -*- autoconf -*-
AC_DEFUN([PYTHON_CHECK],
[
python_available="no"
AC_ARG_WITH([python],
            AS_HELP_STRING([--with-python=PATH],
                           [Python interpreter path (default: auto-detect)]),
            [PYTHON="$withval"])

if test "$PYTHON" = "no"; then
  : # ignore
elif test "$PYTHON" = ""; then
  AM_PATH_PYTHON
  if test "$PYTHON" != "none"; then
    python_available="yes"
  fi
else
  AC_CHECK_FILE([$PYTHON],
                [python_available="yes"],
                [AC_MSG_WARN([$PYTHON is not found. Disable PYTHON binding.])])
fi

if test "$python_available" = "yes"; then
  python_disable_message="Disable Python binding."
  PKG_CHECK_MODULES(PYGOBJECT, pygobject-2.0, ,
                    [AC_MSG_WARN([$python_disable_message])
                     python_available="no"])

  if test "$python_available" = "yes"; then
    PY_PREFIX=`$PYTHON -c 'import sys; print sys.prefix'`
    PY_EXEC_PREFIX=`$PYTHON -c 'import sys; print sys.exec_prefix'`
    PYTHON_LIBS="-lpython$PYTHON_VERSION"
    PYTHON_LIB_LOC="-L$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config"
    PYTHON_CFLAGS="-I$PY_PREFIX/include/python$PYTHON_VERSION"

    AC_SUBST(PYTHON_CFLAGS)
    AC_SUBST(PYTHON_LIBS)
    AC_SUBST(PYTHON_LDFLAGS)

    AC_SUBST(PYGOBJECT_CFLAGS)

    _SAVE_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $PYTHON_CFLAGS"
    AC_CHECK_HEADERS(Python.h, [],
		     [python_available="no"
		      AC_MSG_WARN([$python_disable_message])])
    CPPFLAGS=$_SAVE_CPPFLAGS
  fi

  if test "$python_available" = "yes"; then
    AC_PATH_PROG(PYGTK_CODEGEN, pygtk-codegen-2.0)
    if test -z "$PYGTK_CODEGEN"; then
      python_available="no"
      AC_MSG_WARN([$python_disable_message])
    fi
  fi
fi

AM_CONDITIONAL([WITH_PYTHON], [test "$python_available" = "yes"])
])
