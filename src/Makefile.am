# libemu Makefile
# Paul Baecher, Markus Koetter
# $Id$

AUTOMAKE_OPTIONS = foreign



AM_CFLAGS = -pipe -D _GNU_SOURCE -I../include -Wall -Werror

pkglib_LTLIBRARIES = libemu.la



libemu_la_SOURCES = emu.c
libemu_la_SOURCES += log.c
libemu_la_SOURCES += emu_memory.c

libemu_la_LDFLAGS = -no-undefined -version-info @libemu_soname@ -export-symbols-regex "^emu_"
