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
string = {
  'explicit_bzero': ['', 'explicit_bzero.3'],
  'strlcat': ['strlcat.c', 'strlcpy.3'],
  'strlcpy': ['strlcpy.c', 'strlcpy.3'],
  'strsep': ['strsep.c', 'strsep.3'],
}

# set [2] in each array of string
foreach func, info : string
  string += {func: string[func] + [not cc.has_function(func,
                                                       dependencies: deps)]}
endforeach

if string['explicit_bzero'][2]
  if cc.has_function('SecureZeroMemory', args: args,
                     prefix: '#include <windows.h>')
    string += {'explicit_bzero': ['explicit_bzero_SecureZeroMemory.c',
                      string['explicit_bzero'][1], string['explicit_bzero'][2]]}
  else
    string += {'explicit_bzero': ['explicit_bzero.c',
                      string['explicit_bzero'][1], string['explicit_bzero'][2]]}
  endif
endif

string_needed = false
string_func = []
string_src = []
string_man = []
string_incdir = []

foreach func, info : string
  if not info[2]
    continue
  endif
    string_func += func # build up a database of functions in the library
    string_src += files(info[0])
    string_man += files(info[1])
    string_needed = true
endforeach

if string_needed
  gen_inc += custom_target('generate_string_h',
                              input: 'string.h.in',
                              feed: true,
                              output: 'string.h',
                              capture: true,
                              command: [header_generator] + string_func,
                              install: inst_h,
                              install_dir: h_inst_dir)
  string_incdir = include_directories('.')
endif
