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
libemu_la_SOURCES += functions/aaa.c
libemu_la_SOURCES += functions/adc.c
libemu_la_SOURCES += functions/add.c
libemu_la_SOURCES += functions/and.c
libemu_la_SOURCES += functions/call.c
libemu_la_SOURCES += functions/cmp.c
libemu_la_SOURCES += functions/dec.c
libemu_la_SOURCES += functions/group_1.c
libemu_la_SOURCES += functions/group_5.c
libemu_la_SOURCES += functions/group_10.c
libemu_la_SOURCES += functions/inc.c
libemu_la_SOURCES += functions/jcc.c
libemu_la_SOURCES += functions/jmp.c
libemu_la_SOURCES += functions/lodscc.c
libemu_la_SOURCES += functions/loopcc.c
libemu_la_SOURCES += functions/misc.c
libemu_la_SOURCES += functions/mov.c
libemu_la_SOURCES += functions/or.c
libemu_la_SOURCES += functions/pop.c
libemu_la_SOURCES += functions/push.c
libemu_la_SOURCES += functions/ret.c
libemu_la_SOURCES += functions/sbb.c
libemu_la_SOURCES += functions/stoscc.c
libemu_la_SOURCES += functions/sub.c
libemu_la_SOURCES += functions/xor.c


libemu_la_LDFLAGS = -no-undefined -version-info @libemu_soname@ -export-symbols-regex "^emu_"
