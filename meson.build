# Copyright (c) 2022 Guilherme Janczak <guilherme.janczak@yandex.com>
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
        default_options: ['c_std=gnu99', 'warning_level=2', 'b_lto=true'])

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
elif cc.has_header_symbol('features.h', '__NEWLIB__') \
     or cc.has_header_symbol('features.h', '__BIONIC__') \
     or cc.has_header_symbol('features.h', '__GLIBC__')
  # Some libcs (e.g. uClibc-ng) might set __GLIBC__ in features.h to try to
  # imitate glibc. Make sure we test for __GLIBC__ last for this reason. If the
  # library exposes everything when _GNU_SOURCE is set, then it really does
  # behave like glibc, so put it here if it doesn't require anything else.
  args += ['-D_GNU_SOURCE']
elif cc.has_header_symbol('features.h', '_GNU_SOURCE',
                          prefix: '''#define _ALL_SOURCE
                          #undef _GNU_SOURCE
                          ''')
  # musl doesn't want to be detected, so it doesn't provide its own #define.
  # However, it appears to have an unique behavior that is easy to detect:
  # musl's features.h defines _GNU_SOURCE if _ALL_SOURCE is defined. _ALL_SOURCE
  # appears to be an original musl extension.
  args += ['-D_GNU_SOURCE', '-D_BSD_SOURCE']
endif

if cc.get_argument_syntax() == 'msvc'
  # This library and its users shouldn't be warned for using standard POSIX
  # names. That would warn over OpenBSD behavior.
  args += '-D_CRT_NONSTDC_NO_WARNINGS'
endif

# user_args are compiler args that users of this library NEED to use.
user_args = args

args += '-DOBSD_EXPORTING'
ms_windows = host_machine.system() == 'windows' \
             or host_machine.system() == 'cygwin'
if ms_windows
  user_args += '-DOBSD_IMPORTING'
endif
if cc.get_argument_syntax() == 'msvc'
  args += '-D_CRT_SECURE_NO_WARNINGS'
endif
# Spectre mitigations are off by default under MSVC for some ungodly reason.
if cc.get_id() == 'msvc'
  args += '/Qspectre'
endif

inst_h = not meson.is_subproject()
h_inst_dir = get_option('includedir') / 'obsd'
# We do have global args, but I've chosen to pass them manually where they're
# needed, so add_global_arguments isn't used.
subdir('src')

if meson.is_subproject()
  libobsd = static_library('obsd', src + gen_inc,
                           c_args: args,
                           install: false,
                           include_directories: incdir,
                           dependencies: deps,
                           gnu_symbol_visibility: 'hidden')
  if host_machine.system() == 'windows' or host_machine.system() == 'cygwin'
    custom_target('license',
                  command: ['unix2dos'],
                  input: 'LICENSE.txt',
                  feed: true,
                  output: 'LICENSE_libobsd.txt',
                  capture: true,
                  build_by_default: true)
  else
    configure_file(input: 'LICENSE.txt',
                   output: 'LICENSE_libobsd.txt',
                   copy: true)
  endif
else
  libobsd = library('obsd', src + gen_inc,
                    c_args: args,
                    install: true,
                    include_directories: incdir,
                    dependencies: deps,
                    gnu_symbol_visibility: 'hidden')

  install_man(man)
  pkg = import('pkgconfig')
  pkg.generate(libobsd,
               description: 'Transparent OpenBSD compatibility library',
               filebase: 'libobsd',
               libraries: libobsd,
               name: 'libobsd',
               subdirs: 'obsd',
               # Kitchen sink approach: OpenBSD makes all symbols visible by
               # default. We need to provide declarations for the OpenBSD
               # functions the system provides, the easiest way to do this is to
               # just tell the system to show all of its functions too.
               extra_cflags: user_args,
               url: 'https://github.com/guijan/libobsd')
  if get_option('provide_libbsd')
    # https://mesonbuild.com/Pkgconfig-module.html
    # "install_dir the directory to install to, defaults to the value of option
    # libdir followed by /pkgconfig"
    install_symlink('libbsd-overlay.pc',
                    install_dir: get_option('libdir') / 'pkgconfig',
                    pointing_to: 'libobsd.pc')
  endif
endif

libobsd_dep = declare_dependency(link_with: libobsd,
                                 sources: gen_inc,
                                 dependencies: deps,
                                 include_directories: incdir,
                                 compile_args: user_args)
subdir('test')
