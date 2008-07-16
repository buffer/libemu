AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I../include -I ../.. -Werror -Wall -g
AM_LDFLAGS = -lemu -L../src 

bin_PROGRAMS = scprofiler
noinst_PROGRAMS = testsuite cpurun instrtest instrtree hashtest memtest 

testsuite_LDADD = ../src/libemu.la

testsuite_SOURCES = main.c

cpurun_LDADD = ../src/libemu.la
cpurun_SOURCES = cpu_run.c

instrtest_LDADD = ../src/libemu.la
instrtest_SOURCES = instrtest.c

#instrtree_LDADD = ../src/libemu.la
instrtree_SOURCES = instrtree.c

hashtest_LDADD = ../src/libemu.la
hashtest_SOURCES = hashtest.c

memtest_LDADD = ../src/libemu.la
memtest_SOURCES = memtest.c

scprofiler_LDADD = ../src/libemu.la
scprofiler_SOURCES = scprofiler.c

EXTRA_DIST = emunids.c
