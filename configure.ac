#                                               -*- Autoconf -*-
AC_PREREQ([2.69])
AC_INIT([libutest], [0.4], [lubomir.sedlar@gmail.com],
        [utest], [http://lubomir.github.io/libutest/])
AC_CONFIG_SRCDIR([src/utest.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])
AM_SILENT_RULES([yes])

# Enable feature test macros
AC_DEFINE([_POSIX_C_SOURCE], [200809L], [We want some POSIX features])

# Add --disable-examples option
AC_ARG_ENABLE([examples],
              AS_HELP_STRING([--disable-examples], [Do not build examples]))
AM_CONDITIONAL([ENABLE_EXAMPLES], [test "x$enable_examples" != "xno"])

# Checks for programs.
AC_PROG_CC_C99
AM_PROG_CC_C_O
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit clock_gettime memset strstr])
AC_FUNC_FORK

AC_CONFIG_FILES([Makefile data/libutest.pc docs/Constants.hs])
AC_OUTPUT

echo ""
echo "libutest $VERSION"
echo ""
echo " Prefix......................: ${prefix}"
echo " CFLAGS......................: ${CFLAGS}"
echo " Compiling examples..........: ${enable_examples:-yes}"
