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

project('libobsd', 'c', version: '1.1.1',
        license: 'ISC AND BSD-2-Clause AND BSD-3-Clause',
        default_options: ['c_std=gnu99', 'warning_level=2', 'b_lto=true'])

args = ['-D_GNU_SOURCE']
deps = []
syms = []
gen_inc = [] # generated headers
incdir = []
src = []
man = []
cc = meson.get_compiler('c')

# Some systems implement some OpenBSD functions, but need to link in a library
# or set a macro to expose them.
if host_machine.system() == 'haiku'
  deps += cc.find_library('bsd')
  args += ['-D_BSD_SOURCE']
elif host_machine.system() == 'netbsd'
  args += ['-D_OPENBSD_SOURCE']
elif cc.has_header_symbol('features.h', '__dietlibc__')
  deps += cc.find_library('compat')
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

inst_h = not meson.is_subproject()
h_inst_dir = get_option('includedir') / 'obsd'
# We do have global args, but I've chosen to pass them manually where they're
# needed, so add_global_arguments isn't used.
add_project_arguments(args, language: 'c')
subdir('src')

if meson.is_subproject()
  libobsd = static_library('obsd', src + gen_inc,
                           install: false,
                           include_directories: incdir,
                           dependencies: deps,
                           c_args: '-DLIBOBSD_EXPORTING')
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
                    install: true,
                    include_directories: incdir,
                    dependencies: deps,
                    vs_module_defs: def,
                    version: meson.project_version(),
                    c_args: '-DLIBOBSD_EXPORTING')
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
