#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([utest], [0.1], [lubomir.sedlar@gmail.com])
AC_CONFIG_SRCDIR([src/utest.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign subdir-objects silent-rules -Wall])

# Checks for programs.
AC_PROG_CC_C99
LT_INIT

# Checks for libraries.
AC_CHECK_LIB([utest], [ut_assert_func])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit memset strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
