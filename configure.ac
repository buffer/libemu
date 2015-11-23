# -*- mode: m4; -*-
# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# $Id$

AC_PREREQ(2.59)
AC_INIT([libemu], [0.2.0], [nepenthesdev@gmail.com])
AM_INIT_AUTOMAKE([libemu], [0.2.0])
AC_REVISION([$Id$])

AC_PREFIX_DEFAULT(/opt/libemu)
AC_CONFIG_SRCDIR([include/emu/emu.h])
AM_CONFIG_HEADER([config.h])
# AM_MAINTAINER_MODE

AC_CANONICAL_HOST
AC_C_CONST
AC_C_BIGENDIAN

dnl Check for some target-specific stuff
case "$host" in
*-*-freebsd*)
  CPPFLAGS="$CPPFLAGS -I/usr/local/include -I/usr/src/contrib/file/ -Wno-unused-local-typedefs"
  LDFLAGS="$LDFLAGS -L/usr/local/lib -Wl,-rpath,/usr/local/lib"
  ;;
*-*-linux*)
  CPPFLAGS="$CPPFLAGS -D _GNU_SOURCE -I/usr/local/include -Wno-unused-local-typedefs"
  LDFLAGS="$LDFLAGS -L/usr/local/lib -Wl,-rpath,/usr/local/lib"
  ;;
*-*-darwin*)
  CPPFLAGS="$CPPFLAGS -I/opt/local/include -Wno-unused-local-typedefs"
  LDFLAGS="$LDFLAGS -L/opt/local/lib"
  if test "$GCC" = "yes"; then
          CFLAGS="$CFLAGS -no-cpp-precomp"
  fi
  ;;
esac

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_CHECK_HEADERS([stdint.h stdlib.h string.h strings.h unistd.h])

dnl   We ought not to have to worry about getopt.h, getopt{,1}.c,
dnl   since they can always be unconditionally compiled and linked.  See
dnl   http://mail.gnu.org/archive/html/autoconf/2000-12/msg00049.html
dnl   and surrounding thread for discussion.  However that doesn't
dnl   appear to work in fact, and the function in getopt_long.c has
dnl   different magic to avoid getting horribly tangled up.
AC_CHECK_FUNCS(getopt)
AC_CHECK_DECLS([getopt,getsubopt,getopt_long,setenv,putenv])
AC_REPLACE_FUNCS(getopt_long getsubopt)

dnl   getopt_long.c uses (and provides a replacement for)
dnl   this glibc utility function
AC_CHECK_FUNCS(__strchrnul)


# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_UID_T
AC_STRUCT_TM
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL


AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(off_t)

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([strndup inet_ntoa  memmove memset strdup strerror])

# library soname
# check http://sources.redhat.com/autobook/autobook/autobook_91.html#SEC91
libemu_current=2
libemu_revision=0
libemu_age=0
libemu_soname=$libemu_current:$libemu_revision:$libemu_age
AC_SUBST(libemu_soname)

AM_PROG_CC_C_O

dnl **************************************************
dnl * large filesystem Support                       *
dnl **************************************************

AC_MSG_CHECKING(for Large File System support)
AC_ARG_ENABLE(lfs,
 AC_HELP_STRING([--enable-lfs],[Turn on Large File System (default)]),
 [case "$host" in
 *-*-linux*)
 case "${enableval}" in
    yes) CPPFLAGS="${CPPFLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES" ;;
     no) ;;
      *) AC_MSG_ERROR(bad value ${enableval} for --enable-lfs) ;;
   esac
   ;;
 esac],[
 CPPFLAGS="${CPPFLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES"
 enable_lfs=yes]
 ,enable_lfs=no,)
AC_MSG_RESULT($enableval)


dnl **************************************************
dnl * debug code & debug messages                    *
dnl **************************************************

AC_MSG_CHECKING(whether debug code generation should be enabled)
AC_ARG_ENABLE([debug],
		[AS_HELP_STRING(--enable-debug, enable debug code generation [[default=yes]])],
		[enable_debug=${enableval}], [enable_debug="yes"])
if test x"$enable_debug" = "xyes"; then
	AC_DEFINE([DEBUG], 1, [enable debug code generation])
fi
AC_MSG_RESULT($enable_debug)



#dnl **************************************************
#dnl * interactive hooks                              *
#dnl **************************************************
#
#AC_MSG_CHECKING(if hooks should be interactive)
#AC_ARG_ENABLE([interactive-hooks],
#		[AS_HELP_STRING(--enable-interactive-hooks, enable interactive hooks (dangerous!)[[default=no]])],
#		[enable_interactive_hooks=${enableval}], [enable_interactive_hooks="yes"])
#if test x"$enable_interactive_hooks" = "xyes"; then
#	AC_DEFINE([HAVE_INTERACTIVE_HOOKS], 1, [enable interactive hooks])
#fi
#AC_MSG_RESULT($enable_interactive_hooks)


dnl **************************************************
dnl * python bindings                                *
dnl **************************************************

AC_ARG_ENABLE([python-bindings],
		[  --enable-python-bindings    Compile bindings for Python],
		[enable_python="$enableval"],[enable_python="no"])

AM_CONDITIONAL([ENABLE_PYTHON_BINDINGS], [test x$enable_python = xyes])

dnl **************************************************
dnl * libcargos                                      *
dnl **************************************************


AC_SUBST([LIB_CARGOS])
AC_SUBST([LIB_CARGOS_LIBDIR])
AC_SUBST([LIB_CARGOS_INCDIR])


enable_cargos="yes"

AC_ARG_ENABLE(cargos, [  --enable-cargos      enable support for libcargos],
			[enable_cargos="$enableval"],[enable_cargos="yes"])

AC_ARG_WITH(cargos-include,
			[  --with-cargos-include=DIR  libcargos include dir],
				      [cargos_inc="$withval"],[cargos_inc=no])
AC_ARG_WITH(cargos-lib,
			[  --with-cargos-lib=DIR  libcargos library dir],
			[cargos_lib="$withval"],[cargos_lib=no])


if test x$enable_cargos = "xyes" ; then
	OLD_CPPFLAGS=${CPPFLAGS};
	if test x$cargos_inc != "xno"; then
		CPPFLAGS="${CPPFLAGS} -I${cargos_inc}"
	fi
	
	AC_CHECK_HEADER(cargos-lib.h,[enable_cargos=yes],[enable_cargos=no])
	
	if test x$enable_cargos = "xyes" ; then
		OLD_LDFLAGS=${LDFLAGS}
		if test x$cargos_lib != "xno"; then
			LDFLAGS="${LDFLAGS}  -L${cargos_lib}"
		fi
	
		AC_CHECK_LIB([cargos],[cargos_lib_create],enable_cargos="yes", enable_cargos="no")
	
		if test x$enable_cargos = "xyes" ; then
			LIB_CARGOS="-lcargos"
			LIB_CARGOS_LIBDIR="-L$cargos_lib"
			LIB_CARGOS_INCDIR="-I$cargos_inc"
			AC_DEFINE([HAVE_LIBCARGOS], [1], [Define to 1 to compile with cargos support])
		fi
	fi
	CPPFLAGS=${OLD_CPPFLAGS}
	LDFLAGS=${OLD_LDFLAGS}
fi


AC_DEFUN([AC_DEFINE_DIR], [
  prefix_NONE=
  exec_prefix_NONE=
  test "x$prefix" = xNONE && prefix_NONE=yes && prefix=$ac_default_prefix
  test "x$exec_prefix" = xNONE && exec_prefix_NONE=yes && exec_prefix=$prefix
dnl In Autoconf 2.60, ${datadir} refers to ${datarootdir}, which in turn
dnl refers to ${prefix}.  Thus we have to use `eval' twice.
  eval ac_define_dir="\"[$]$2\""
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST($1, "$ac_define_dir")
  AC_DEFINE_UNQUOTED($1, "$ac_define_dir", [$3])
  test "$prefix_NONE" && prefix=NONE
  test "$exec_prefix_NONE" && exec_prefix=NONE
])

AC_DEFINE_DIR(PREFIX, prefix, [default working directory])
AC_DEFINE_DIR(LOCALESTATEDIR, localstatedir, [where to put logs etc])
AC_DEFINE_DIR(LIBDIR, libdir, [where to look for plugins])
AC_DEFINE_DIR(SYSCONFDIR, [sysconfdir], [System configuration dir])
AC_DEFINE_DIR(DATADIR, [datadir], [The directory for installing idiosyncratic read-only architecture-independent data.])
AC_DEFINE_DIR(DATAROOTDIR, [datarootdir], [The root of the directory tree for read-only architecture-independent data files.])

emu_data_dir=$datadir/emu

AC_SUBST(emu_data_dir)

CFLAGS="${CFLAGS} -Wstrict-prototypes"

AC_CONFIG_FILES([Makefile
		src/Makefile
		src/functions/Makefile
		include/Makefile
		include/emu/Makefile
		include/emu/environment/Makefile
		include/emu/environment/win32/Makefile
		include/emu/environment/linux/Makefile
		testsuite/Makefile
		doc/Makefile
		libemu.pc
		bindings/Makefile
		bindings/python/Makefile
		bindings/python/setup.py
		tools/Makefile
		tools/sctest/Makefile])

                 
AC_OUTPUT

echo
echo "*** libemu configuration ***"
echo ""
echo "debug                              : $enable_debug"
echo ""
echo "bindings"
echo " - python                          : $enable_python"
echo ""
echo "support"
echo " - cargos                          : $enable_cargos"

