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

project('libobsd', 'c', version : '0.0.0', license : 'ISC',
        default_options : ['c_std=gnu99', 'warning_level=2', 'b_lto=true'])

args = '-D_GNU_SOURCE'
inc = []
incdir = []
src = []
man = []

add_project_arguments(args, language : 'c')
cc = meson.get_compiler('c')
subdir('src')

if meson.is_subproject()
  libobsd = static_library('obsd', src, install : false,
                           include_directories : incdir)
else
  libobsd = library('obsd', src, install : true,
                    include_directories : incdir)
  install_man(man)
  install_headers(inc, subdir : 'obsd')
  pkg = import('pkgconfig')
  pkg.generate(libobsd,
               description : 'Transparent OpenBSD compatibility library',
               filebase : 'libobsd',
               libraries : libobsd,
               name : 'libobsd',
               subdirs : 'obsd',
               url : 'https://github.com/guijan/libobsd')
  if get_option('provide_libbsd')
    # https://mesonbuild.com/Pkgconfig-module.html
    # "install_dir the directory to install to, defaults to the value of option
    # libdir followed by /pkgconfig"
    install_symlink('libbsd-overlay.pc',
                    install_dir : get_option('libdir') + '/pkgconfig',
                    pointing_to : 'libobsd.pc')
  endif
endif

libobsd_dep = declare_dependency(link_with : libobsd,
                                 include_directories : incdir)
