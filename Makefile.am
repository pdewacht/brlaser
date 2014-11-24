ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = $(CUPS_CFLAGS)
TESTS = $(check_PROGRAMS)


drv_DATA = brlaser.drv
drvdir = $(CUPS_DATADIR)/drv

filter_PROGRAMS = src/rastertobrlaser
filterdir = $(CUPS_SERVERBIN)/filter

noinst_PROGRAMS = src/brdecode

check_PROGRAMS = \
	test/test_lest \
	test/test_line \
	test/test_block


src_rastertobrlaser_SOURCES = \
	src/main.cc \
	src/debug.h \
	src/debug.cc \
	src/job.h \
	src/job.cc \
	src/block.h \
	src/line.h \
	src/line.cc
src_rastertobrlaser_LDADD = $(CUPS_LIBS)

src_brdecode_SOURCES = \
	src/brdecode.cc

test_test_lest_SOURCES = \
	test/test_lest.cc \
	test/lest.hpp

test_test_line_SOURCES = \
	test/test_line.cc \
	src/line.h \
	src/line.cc \
	test/lest.hpp

test_test_block_SOURCES = \
	test/test_block.cc \
	src/block.h \
	test/tempfile.h \
	test/lest.hpp
