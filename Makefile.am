ACLOCAL_AMFLAGS = -I m4

NULL=

lib_LTLIBRARIES = src/libutest.la

include_HEADERS = src/utest.h
src_libutest_la_SOURCES = \
			  src/main.c \
			  src/timer.c \
			  src/timer.h \
			  src/utest.h \
			  src/utest.c \
			  src/utils.c \
			  src/utils.h \
			  $(NULL)

noinst_PROGRAMS = $(NULL)
AM_CFLAGS = -I$(srcdir)/src

CLEANFILES = $(NULL)
include data/Makefile.include
include docs/Makefile.include
if ENABLE_EXAMPLES
include examples/Makefile.include
endif
