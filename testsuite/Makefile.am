AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I../include -I ../.. -Werror -Wall -g
AM_LDFLAGS = -lemu -L../src 

bin_PROGRAMS = sctest
noinst_PROGRAMS = testsuite cpurun instrtest instrtree hashtest memtest

testsuite_LDADD = ../src/libemu.la

testsuite_SOURCES = main.c

cpurun_LDADD = ../src/libemu.la
cpurun_SOURCES = cpu_run.c

instrtest_LDADD = ../src/libemu.la
instrtest_SOURCES = instrtest.c

sctest_CPPFLAGS = ${LIB_CARGOS_INCDIR}  ${AM_CPPFLAGS}
sctest_LDFLAGS =  ${LIB_CARGOS} ${LIB_CARGOS_LIBDIR} ${AM_LDFLAGS}
sctest_LDADD = ../src/libemu.la
sctest_SOURCES = sctest.c

#instrtree_LDADD = ../src/libemu.la
instrtree_SOURCES = instrtree.c

hashtest_LDADD = ../src/libemu.la
hashtest_SOURCES = hashtest.c

memtest_LDADD = ../src/libemu.la
memtest_SOURCES = memtest.c

EXTRA_DIST = emunids.c
