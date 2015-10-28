dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(coremw, 1.0)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE(-Wall -Werror foreign)
AM_PROG_AR

LT_INIT
PKG_CHECK_MODULES(SQLITE, sqlite3 >= 3.8)
AC_PROG_RANLIB
AC_PROG_CXX
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES(Makefile opensaf/Makefile opensaf/imm/Makefile  src/Makefile src/cmwpm/Makefile test/Makefile test/testapp1/Makefile test/javaapp/Makefile)
AC_OUTPUT

