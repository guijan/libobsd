# Copyright (c) 2022, 2024 Guilherme Janczak <guilherme.janczak@yandex.com>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

project('libobsd', 'c', 'cpp', version: '1.1.1',
        license: 'ISC AND BSD-2-Clause AND BSD-3-Clause',
        default_options: ['c_std=gnu99,c99', 'warning_level=2'],
        meson_version: '>=1.3.0')

args = []
deps = []
syms = []
gen_inc = [] # generated headers
incdir = []
src = []
man = []
cc = meson.get_compiler('c')

# Whether our environment supports C++. Not the case with e.g. CompCert,
# dietlibc.
cxx_support = true
# Some systems implement some OpenBSD functions, but need to link in a library
# or set a macro to expose them.
#
# Be very careful here and be aware that what one libc does is very different
# from another even when macro names match.
if host_machine.system() == 'haiku'
  args += ['-D_GNU_SOURCE']
  deps += cc.find_library('bsd')
elif host_machine.system() == 'netbsd'
  args += ['-D_OPENBSD_SOURCE']
elif cc.has_header_symbol('features.h', '__dietlibc__')
  args += ['-D_GNU_SOURCE', '-D_BSD_SOURCE']
  deps += cc.find_library('compat')
  cxx_support = false
elif cc.has_header_symbol('features.h', '__NEWLIB__') or \
     cc.has_header_symbol('features.h', '__BIONIC__') or \
     cc.has_header_symbol('features.h', '_GNU_SOURCE',
                          prefix:
                          '''#define _ALL_SOURCE\n#undef _GNU_SOURCE''') or \
     cc.has_header_symbol('features.h', '__GLIBC__')
  # Some libcs (e.g. uClibc-ng) might set __GLIBC__ in features.h to try to
  # imitate glibc. Make sure we test for __GLIBC__ last for this reason. If the
  # library exposes everything when _GNU_SOURCE is set, then it really does
  # behave like glibc, so put it here if it doesn't require anything else.
  args += ['-D_GNU_SOURCE']
endif

if cc.get_argument_syntax() == 'msvc'
  # This library and its users shouldn't be warned for using standard POSIX
  # names. That would warn over OpenBSD behavior.
  args += '-D_CRT_NONSTDC_NO_WARNINGS'
endif

# user_args are compiler args that users of this library NEED to use.
user_args = args

if cc.get_argument_syntax() == 'msvc'
  args += '-D_CRT_SECURE_NO_WARNINGS'
endif
# Spectre mitigations are off by default under MSVC for some ungodly reason.
if cc.get_id() == 'msvc'
  args += '/Qspectre'
endif

subproj = meson.is_subproject()

inst_h = not subproj
h_inst_dir = get_option('includedir') / 'obsd'
subdir('src')

windows_host = ['windows', 'cygwin'].contains(host_machine.system())
src += gen_inc
link = []
if windows_host
  if get_option('default_library') == 'both'
    error('building both libraries is not supported under Windows')
  elif get_option('default_library') == 'shared'
    args += '-DOBSD_EXPORTING'
    user_args += '-DOBSD_IMPORTING'
  endif
else
  args += '-DOBSD_EXPORTING'
  if subproj
    link += 'default_library=static'
  endif
endif
libobsd = library('obsd', src,
                  c_args: args,
                  install: not subproj,
                  include_directories: incdir,
                  # To prevent Meson from making the compiler confuse sys/time.h
                  # with time.h
                  implicit_include_directories: false,
                  dependencies: deps,
                  gnu_symbol_visibility: 'hidden',
                  override_options: link)

libobsd_dep = declare_dependency(link_with: libobsd,
                                 dependencies: deps,
                                 include_directories: incdir,
                                 sources: gen_inc,
                                 compile_args: user_args)
meson.override_dependency('libbsd-overlay', libobsd_dep)
meson.override_dependency('libobsd', libobsd_dep)

subdir('test')

### Installation. Headers and the library are handled elsewhere.

libobsd_license = 'LICENSE.txt'
libobsd_readme = 'README.md'
docdir = get_option('docdir')
if docdir == ''
  docdir = get_option('datadir') / 'doc' \
           / meson.project_name() + '-' + meson.project_version()
endif
if windows_host
  unix2dos = find_program('unix2dos')
  custom_target('license',
                command: [unix2dos],
                input: libobsd_license,
                feed: true,
                capture: true,
                output: subproj ? 'LICENSE_libobsd.txt' : libobsd_license,
                build_by_default: true,
                install: not subproj,
                install_dir: docdir)
  custom_target('readme',
                command: [unix2dos],
                input: libobsd_readme,
                feed: true,
                capture: true,
                output: libobsd_readme,
                build_by_default: true,
                install: not subproj,
                install_dir: docdir)
elif not subproj
    install_data(libobsd_license, libobsd_readme,
                 install_dir: docdir,
                 install_tag: 'doc')
endif

if not subproj
  install_man(man)

  pkg = import('pkgconfig')
  pkg.generate(libobsd,
               name: 'libobsd',
               description: 'Transparent OpenBSD compatibility library',
               filebase: 'libobsd',
               url: 'https://github.com/guijan/libobsd',
               subdirs: 'obsd',
               # Kitchen sink approach: OpenBSD makes all symbols visible by
               # default. We need to provide declarations for the OpenBSD
               # functions the system provides, the easiest way to do this is to
               # just tell the system to show all of its functions too.
               extra_cflags: user_args)
  if get_option('provide_libbsd')
    # https://mesonbuild.com/Pkgconfig-module.html
    # "install_dir the directory to install to, defaults to the value of option
    # libdir followed by /pkgconfig"
    install_symlink('libbsd-overlay.pc',
                    install_dir: get_option('libdir') / 'pkgconfig',
                    pointing_to: 'libobsd.pc')
  endif
endif
