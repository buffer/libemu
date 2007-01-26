AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I../include -I ../.. -Wall
AM_LDFLAGS = -lemu -L../src

bin_PROGRAMS = testsuite cpurun

testsuite_LDADD = ../src/libemu.la

testsuite_SOURCES = main.c

cpurun_LDADD = ../src/libemu.la
cpurun_SOURCES = cpu_run.c
