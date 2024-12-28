# Copyright (c) 2022-2023 Guilherme Janczak <guilherme.janczak@yandex.com>
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
                              native: true)

subdir('err')
subdir('queue')
subdir('stdlib')
subdir('string')
subdir('time')
subdir('unistd')

syms += err_sym + queue_sym + stdlib_sym + string_sym + time_sym + unistd_sym
incdir += [err_incdir, queue_incdir, stdlib_incdir, string_incdir, time_incdir,
  unistd_incdir]
src += err_src + stdlib_src + string_src + unistd_src
man += err_man + queue_man + stdlib_man + string_man + time_man + unistd_man

incdir += include_directories('.')
if inst_h
  install_headers('obsd_inc_next.h', 'obsd_defs.h', subdir: 'obsd')
endif
