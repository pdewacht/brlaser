ACLOCAL_AMFLAGS = -I m4

dist_doc_DATA = README.md

filter_PROGRAMS = rastertobrlaser
filterdir = $(CUPS_SERVERBIN)/filter

drv_DATA = brlaser.drv
drvdir = $(CUPS_DATADIR)/drv

noinst_PROGRAMS = brdecode


rastertobrlaser_SOURCES = \
	src/main.cc \
	src/debug.h \
	src/debug.cc \
	src/job.h \
	src/job.cc \
	src/block.h \
	src/line.h \
	src/line.cc
rastertobrlaser_CPPFLAGS = $(CUPS_CFLAGS)
rastertobrlaser_LDADD = $(CUPS_LIBS)

brdecode_SOURCES = \
	src/brdecode.cc


TESTS = test_lest test_line test_block
check_PROGRAMS = $(TESTS)
test_lest_SOURCES = \
	test/test_lest.cc \
	test/lest.h
test_line_SOURCES = \
	test/test_line.cc \
	src/line.h \
	src/line.cc \
	test/lest.h
test_block_SOURCES = \
	test/test_block.cc \
	src/block.h \
	test/tempfile.h \
	test/lest.h
