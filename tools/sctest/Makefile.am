AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I../../include -I ../../.. -Werror -Wall -g
AM_LDFLAGS = -lemu -L../../src 


# if you want to compile without tests, uncomment the following line:
#AM_CPPFLAGS += -D_NO_TESTS


bin_PROGRAMS = sctest

sctest_CPPFLAGS = ${LIB_CARGOS_INCDIR}  ${AM_CPPFLAGS}
sctest_LDFLAGS =  ${LIB_CARGOS} ${LIB_CARGOS_LIBDIR} ${AM_LDFLAGS}
sctest_LDADD = ../../src/libemu.la
sctest_SOURCES = dot.c
sctest_SOURCES += dot.h
sctest_SOURCES += sctestmain.c
sctest_SOURCES += tests.c
sctest_SOURCES += tests.h
sctest_SOURCES += userhooks.c
sctest_SOURCES += userhooks.h
sctest_SOURCES += nanny.c
sctest_SOURCES += nanny.h
sctest_SOURCES += options.h

