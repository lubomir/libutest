ACLOCAL_AMFLAGS = $$ACLOCAL_ARGS -I m4

lib_LTLIBRARIES = src/libutest.la

src_libutest_la_SOURCES = src/utest.h \
			  src/utest.c
noinst_PROGRAMS =
AM_CFLAGS = -I$(srcdir)/src
include examples/Makefile.include
