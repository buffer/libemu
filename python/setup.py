#!/usr/bin/python

from distutils.core import setup, Extension

libemu = Extension('libemu',
                    sources = ['libemu_module.c'],
                    include_dirs = ['../include'],
                    library_dirs = ['/opt/libemu/lib/libemu'],
                    libraries = ['emu'],
                    )

setup (name = 'libemu',
       version = '0.1.0',
       description = 'Python interface to the libemu x86 emualtor.',
       author = 'Georg Wicherski',
       author_email = 'gw@mwcollect.org',
       url = 'http://libemu.mwcollect.org/',
       ext_modules = [libemu])
