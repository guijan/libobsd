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

project('libobsd', 'c', version: '1.0.0', license: 'ISC',
        default_options: ['c_std=gnu99', 'warning_level=2', 'b_lto=true'])

args = ['-D_GNU_SOURCE']
deps = []
funcs = []
gen_inc = [] # generated headers
incdir = []
src = []
man = []
cc = meson.get_compiler('c')

# Some systems need options specific to them.
if host_machine.system() == 'haiku'
  # Haiku has a BSD compatibility library that we can depend on.
  deps += cc.find_library('bsd')
  args += ['-D_BSD_SOURCE']
elif cc.has_header_symbol('features.h', '__dietlibc__')
  # dietlibc also implements some GNU and BSD extensions in a separate library.
  deps += cc.find_library('compat')
endif

inst_h = not meson.is_subproject()
h_inst_dir = get_option('includedir') / 'obsd'
# The global args are needed to get the underlying system to expose its
# functions to anyone who uses this library.
add_global_arguments(args, language: 'c')
subdir('src')

if meson.is_subproject()
  libobsd = static_library('obsd', src + gen_inc,
                           install: false,
                           include_directories: incdir,
                           vs_module_defs: def,
                           dependencies: deps,
                           version: meson.project_version())
else
  libobsd = library('obsd', src + gen_inc,
                    install: true,
                    include_directories: incdir,
                    vs_module_defs: def,
                    dependencies: deps,
                    version: meson.project_version())
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
               extra_cflags: args,
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
                                 compile_args: args)
subdir('test')
