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

subdir('err')
subdir('stdlib')
subdir('string')

funcs += err_funcs + stdlib_funcs + string_funcs
inc += [err_inc, stdlib_inc, string_inc]
incdir += [err_incdir, stdlib_incdir, string_incdir]
src += err_src + stdlib_src + string_src
man += err_man + stdlib_man + string_man

def_generator = executable('def_generator', 'def_generator.c')

def = custom_target('def_generator',
                     command: [def_generator] + funcs,
                     build_by_default: false,
                     capture: true,
                     output: 'libbsd.def')
