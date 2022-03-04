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

# The keys inside the string dictionary are the names of the functions.
# The value is an array. Here are the uses of the 3 members of the arrays:
# [0] = 'filename of the source file that implements the function'
# [1] = 'filename of the manual of the function'
# [2] = is the function needed? true/false
#
unistd = {
  'pledge': ['pledge.c', 'pledge.3'],
  'unveil': ['unveil.c', 'unveil.3'],
}

# set [2] in each array of unistd
foreach func, info : unistd
  unistd += {func: unistd[func] + [not cc.has_function(func,
                                                       dependencies: deps)]}
endforeach

unistd_needed = false
unistd_func = []
unistd_src = []
unistd_man = []
unistd_incdir = []
unistd_pragma = []

foreach func, info : unistd
  if not info[2]
    continue
  endif
  unistd_func += func # build up a database of functions in the library
  unistd_src += files(info[0])
  unistd_man += files(info[1])
  unistd_needed = true
endforeach

if unistd_needed
  if cc.has_header('unistd.h')
    unistd_pragma += 'unistd_h'
  endif
  gen_inc += custom_target('generate_unistd_h',
                              input: 'unistd.h.in',
                              feed: true,
                              output: 'unistd.h',
                              capture: true,
                              command: [header_generator] + unistd_func +
                                       unistd_pragma,
                              install: inst_h,
                              install_dir: h_inst_dir)
  unistd_incdir = include_directories('.')
endif
