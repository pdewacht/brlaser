ACLOCAL_AMFLAGS = -I m4

filter_PROGRAMS = rastertobrlaser
filterdir = $(CUPS_SERVERBIN)/filter

drv_DATA = brlaser.drv
drvdir = $(CUPS_DATADIR)/drv

rastertobrlaser_SOURCES = \
	src/main.cc \
	src/debug.h \
	src/debug.cc \
	src/job.h \
	src/job.cc \
	src/line.h \
	src/line.cc
rastertobrlaser_CPPFLAGS = $(CUPS_CFLAGS)
rastertobrlaser_LDADD = $(CUPS_LIBS)
