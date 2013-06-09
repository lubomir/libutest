ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = src/libutest.la

include_HEADERS = src/utest.h
src_libutest_la_SOURCES = src/utest.h \
			  src/utest.c
noinst_PROGRAMS =
AM_CFLAGS = -I$(srcdir)/src

include data/Makefile.include
if ENABLE_EXAMPLES
include examples/Makefile.include
endif

if ENABLE_DOCS
include docs/Makefile.include
endif
