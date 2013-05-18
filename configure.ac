#                                               -*- Autoconf -*-
AC_PREREQ([2.69])
AC_INIT([libutest], [0.2], [lubomir.sedlar@gmail.com])
AC_CONFIG_SRCDIR([src/utest.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign subdir-objects silent-rules -Wall])

# Add --disable-examples option
AC_ARG_ENABLE([examples],
              AS_HELP_STRING([--disable-examples], [Do not build examples]))
AM_CONDITIONAL([ENABLE_EXAMPLES], [test "x$enable_examples" != "xno"])

# Add --enable-docs option
AC_ARG_ENABLE([docs],
	      AS_HELP_STRING([--enable-docs], [Build the documentation]))
AM_CONDITIONAL([ENABLE_DOCS], [test "x$enable_docs" = "xyes"])

# Checks for programs.
AC_PROG_CC_C99
AM_PROG_CC_C_O
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

AC_CONFIG_FILES([Doxyfile Makefile data/libutest.pc docs/conf.py])
AC_OUTPUT

echo ""
echo "libutest $VERSION"
echo ""
echo " Prefix......................: ${prefix}"
echo " Compiling examples..........: ${enable_examples:-yes}"
echo " Building docs...............: ${enable_docs:-no}"
