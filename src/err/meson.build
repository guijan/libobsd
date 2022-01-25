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

# The keys inside the err dictionary are the names of the functions.
# The value is an array. Here are the uses of the 3 members of the arrays:
# [0] = 'filename of the source file that implements the function'
# [1] = 'filename of the manual of the function'
# [2] = is the function needed? true/false
#
err = {
  'err': ['err.c', 'err.3'],
  'errc': ['errc.c', 'err.3'],
  'errx': ['errx.c', 'err.3'],
  'verr': ['verr.c', 'err.3'],
  'verrc': ['verrc.c', 'err.3'],
  'verrx': ['verrx.c', 'err.3'],
  'vwarn': ['vwarn.c', 'err.3'],
  'vwarnc': ['vwarnc.c', 'err.3'],
  'vwarnx': ['vwarnx.c', 'err.3'],
  'warn': ['warn.c', 'err.3'],
  'warnc': ['warnc.c', 'err.3'],
  'warnx': ['warnx.c', 'err.3'],
}

# set [2] in each array of err
foreach func, info : err
  err += {func: err[func] + [not cc.has_function(func, dependencies: deps)]}
endforeach

err_h_data = configuration_data()
err_needed = false
err_funcs = []
err_src = []
err_man = []
err_inc = []
err_incdir = []

foreach func, info : err
  err_h_data.set(func, info[2].to_int())
  if info[2] # If func is needed
    err_funcs += func # build up a database of functions in the library
    err_src += files(info[0])
    err_man += files(info[1])
    err_needed = true
  endif
endforeach

err_h_data.set('err_h', cc.has_header('err.h').to_int())

if err_needed
  err_inc = configure_file(input: 'err.h.in',
                           output: 'err.h',
                           configuration: err_h_data)
  err_incdir = include_directories('.')
endif
