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

header_generator = executable('header_generator', 'header_generator.c',
                              c_args: args, native: true)

subdir('err')
subdir('stdlib')
subdir('string')
subdir('unistd')

# queue.h is a special case; it only provides a header. It also must not be
# added to `inc` because it needs to be installed to a subdir, it's handled
# separately.
subdir('queue')

syms += err_sym + stdlib_sym + string_sym + unistd_sym
incdir += [err_incdir, stdlib_incdir, string_incdir, unistd_incdir]
src += err_src + stdlib_src + string_src + unistd_src
man += err_man + stdlib_man + string_man + queue_man + unistd_man

incdir += include_directories('.')
gen_inc += files('libobsd_inc_next.h')
gen_inc += files('libobsd_defines.h')
if inst_h
  install_headers('libobsd_inc_next.h', 'libobsd_defines.h', subdir: h_inst_dir)
endif

def_generator = executable('def_generator', 'def_generator.c', native: true,
                           c_args: args)
def = custom_target('def_generator',
                    command: [def_generator] + syms,
                    capture: true,
                    output: 'libobsd.def')
