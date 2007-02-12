# libemu Makefile
# Paul Baecher, Markus Koetter
# $Id$

AUTOMAKE_OPTIONS = foreign



AM_CFLAGS = -pipe -D _GNU_SOURCE -I../include -Werror -Wall -g

pkglib_LTLIBRARIES = libemu.la

SUBDIRS = functions

libemu_la_SOURCES = emu.c
libemu_la_SOURCES += emu_log.c
libemu_la_SOURCES += emu_memory.c
libemu_la_SOURCES += emu_cpu_data.c
libemu_la_SOURCES += emu_cpu.c
libemu_la_SOURCES += functions/add.c
libemu_la_SOURCES += functions/misc.c

libemu_la_LDFLAGS = -no-undefined -version-info @libemu_soname@ -export-symbols-regex "^emu_"
