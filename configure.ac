AC_INIT([ei2f], [0.0.1], [TBD@UNKNOWN])

#AC_ARG_ENABLE([leveldb],
#    AS_HELP_STRING([--enable-leveldb], [Use leveldb as dldb backend]))

AC_CANONICAL_TARGET

AC_LANG_CPLUSPLUS

AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
AC_CHECK_PROGS([DOXYGEN], [doxygen])

# Doxygen
if test -z "$DOXYGEN"; then
    AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
else
    AC_CHECK_PROGS([DOT], [dot])
    if test -z "$DOT"; then
        AC_MSG_ERROR([Doxygen needs dot, please install dot first])
    fi
    AC_CHECK_PROGS([PDFLATEX], [pdflatex])
    if test -z "$PDFLATEX"; then
        AC_MSG_ERROR([Doxygen needs pdflatex program, it is part of TeX http://www.tug.org/texlive/acquire-netinstall.html])
    fi
fi

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Makefile])])

AC_ENABLE_SHARED
LT_INIT(libtool)

AC_CONFIG_FILES([Makefile include/Makefile])

AC_OUTPUT

