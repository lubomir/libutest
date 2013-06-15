ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = src/libutest.la

include_HEADERS = src/utest.h
src_libutest_la_SOURCES = src/main.c \
			  src/timer.c \
			  src/utest.h \
			  src/utest.c \
			  src/utils.c
noinst_PROGRAMS =
AM_CFLAGS = -I$(srcdir)/src

CLEANFILES=
include data/Makefile.include
include docs/Makefile.include
if ENABLE_EXAMPLES
include examples/Makefile.include
endif
