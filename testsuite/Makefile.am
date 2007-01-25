AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I../include -I ../.. -Wall
AM_LDFLAGS = -lemu -L../src

bin_PROGRAMS = testsuite

testsuite_LDADD = ../src/libemu.la

testsuite_SOURCES = main.c

