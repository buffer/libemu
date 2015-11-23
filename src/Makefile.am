# libemu Makefile
# Paul Baecher, Markus Koetter
# $Id$

AUTOMAKE_OPTIONS = foreign subdir-objects



AM_CFLAGS = -pipe -D _GNU_SOURCE -I../include -Werror -Wall -g

lib_LTLIBRARIES = libemu.la

SUBDIRS = functions

libemu_la_SOURCES = emu.c
libemu_la_SOURCES += emu_log.c
libemu_la_SOURCES += emu_memory.c
libemu_la_SOURCES += emu_cpu_data.c
libemu_la_SOURCES += emu_cpu.c
libemu_la_SOURCES += emu_string.c
libemu_la_SOURCES += emu_getpc.c
libemu_la_SOURCES += emu_graph.c
libemu_la_SOURCES += emu_hashtable.c
libemu_la_SOURCES += emu_list.c
libemu_la_SOURCES += emu_queue.c
libemu_la_SOURCES += emu_stack.c
libemu_la_SOURCES += emu_shellcode.c
libemu_la_SOURCES += emu_source.c
libemu_la_SOURCES += emu_track.c
libemu_la_SOURCES += emu_breakpoint.c
libemu_la_SOURCES += functions/aaa.c
libemu_la_SOURCES += functions/adc.c
libemu_la_SOURCES += functions/add.c
libemu_la_SOURCES += functions/and.c
libemu_la_SOURCES += functions/call.c
libemu_la_SOURCES += functions/cmp.c
libemu_la_SOURCES += functions/cmps.c
libemu_la_SOURCES += functions/dec.c
libemu_la_SOURCES += functions/div.c
libemu_la_SOURCES += functions/group_1.c
libemu_la_SOURCES += functions/group_2.c
libemu_la_SOURCES += functions/group_3.c
libemu_la_SOURCES += functions/group_4.c
libemu_la_SOURCES += functions/group_5.c
libemu_la_SOURCES += functions/group_10.c
libemu_la_SOURCES += functions/idiv.c
libemu_la_SOURCES += functions/imul.c
libemu_la_SOURCES += functions/inc.c
libemu_la_SOURCES += functions/int.c
libemu_la_SOURCES += functions/jcc.c
libemu_la_SOURCES += functions/jmp.c
libemu_la_SOURCES += functions/lodscc.c
libemu_la_SOURCES += functions/loopcc.c
libemu_la_SOURCES += functions/misc.c
libemu_la_SOURCES += functions/mov.c
libemu_la_SOURCES += functions/movsx.c
libemu_la_SOURCES += functions/movzx.c
libemu_la_SOURCES += functions/mul.c
libemu_la_SOURCES += functions/neg.c
libemu_la_SOURCES += functions/not.c
libemu_la_SOURCES += functions/or.c
libemu_la_SOURCES += functions/pop.c
libemu_la_SOURCES += functions/push.c
libemu_la_SOURCES += functions/rcl.c
libemu_la_SOURCES += functions/rcr.c
libemu_la_SOURCES += functions/repcc.c
libemu_la_SOURCES += functions/ret.c
libemu_la_SOURCES += functions/rol.c
libemu_la_SOURCES += functions/ror.c
libemu_la_SOURCES += functions/sal.c
libemu_la_SOURCES += functions/sar.c
libemu_la_SOURCES += functions/sbb.c
libemu_la_SOURCES += functions/scas.c
libemu_la_SOURCES += functions/shr.c
libemu_la_SOURCES += functions/stoscc.c
libemu_la_SOURCES += functions/sub.c
libemu_la_SOURCES += functions/test.c
libemu_la_SOURCES += functions/xchg.c
libemu_la_SOURCES += functions/xor.c

libemu_la_SOURCES += libdasm.c libdasm.h opcode_tables.h

libemu_la_SOURCES += environment/emu_env.c
libemu_la_SOURCES += environment/emu_profile.c
libemu_la_SOURCES += environment/win32/emu_env_w32.c
libemu_la_SOURCES += environment/win32/emu_env_w32_dll.c
libemu_la_SOURCES += environment/win32/emu_env_w32_dll_export.c
libemu_la_SOURCES += environment/win32/env_w32_dll_export_kernel32_hooks.c
libemu_la_SOURCES += environment/win32/env_w32_dll_export_urlmon_hooks.c
libemu_la_SOURCES += environment/win32/env_w32_dll_export_ws2_32_hooks.c
libemu_la_SOURCES += environment/win32/env_w32_dll_export_msvcrt_hooks.c
libemu_la_SOURCES += environment/win32/env_w32_dll_export_shell32_hooks.c
libemu_la_SOURCES += environment/win32/env_w32_dll_export_shdocvw_hooks.c

libemu_la_SOURCES += environment/linux/emu_env_linux.c
libemu_la_SOURCES += environment/linux/env_linux_syscall_hooks.c


libemu_la_LDFLAGS = -no-undefined -version-info @libemu_soname@ -export-symbols-regex "^emu_"
