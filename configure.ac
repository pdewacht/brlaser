# This file is part of the brlaser printer driver.
#
# Copyright 2013 Peter De Wachter
#
# brlaser is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# brlaser is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with brlaser.  If not, see <http:#www.gnu.org/licenses/>.

AC_PREREQ(2.68)
AC_INIT([brlaser], [3], [pdewacht@gmail.com], [brlaser],
  [https://github.com/pdewacht/brlaser])

AC_CONFIG_SRCDIR([src/line.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_LANG([C++])

AM_INIT_AUTOMAKE([1.11.1 foreign subdir-objects dist-xz -Wall -Werror])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
AX_CXXFLAGS_WARN_ALL

dnl Use cups-config to detect the CUPS configuration.
AC_PATH_PROG(CUPS_CONFIG, cups-config)
AS_IF([test -z "$CUPS_CONFIG"],
  [AC_MSG_ERROR(["cups-config" command not found. Please install the CUPS development package.])])
CUPS_CFLAGS=`"$CUPS_CONFIG" --cflags`
CUPS_LIBS=`"$CUPS_CONFIG" --image --libs`
CUPS_SERVERBIN=`"$CUPS_CONFIG" --serverbin`
CUPS_DATADIR=`"$CUPS_CONFIG" --datadir`
AC_SUBST(CUPS_CFLAGS)
AC_SUBST(CUPS_LIBS)
AC_SUBST(CUPS_SERVERBIN)
AC_SUBST(CUPS_DATADIR)

dnl 'cups-config --libs' lists a lot of libs we don't need/want,
dnl try to figure out whether the linker knows about --as-needed.
AC_ARG_ENABLE([as-needed],
  AC_HELP_STRING([--disable-as-needed], [disable overlinking protection]))
AS_IF([test "x$enable_as_needed" != "xno"],
  [AX_APPEND_LINK_FLAGS([-Wl,--as-needed])])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile brlaser.drv])
AC_OUTPUT
