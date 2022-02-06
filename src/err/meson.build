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
  'errc': ['errc.c', 'errc.3'],
  'errx': ['errx.c', 'err.3'],
  'warn': ['warn.c', 'err.3'],
  'warnc': ['warnc.c', 'errc.3'],
  'warnx': ['warnx.c', 'err.3'],
}

verr = {
  'verr': ['verr.c', 'err.3'],
  'verrc': ['verrc.c', 'errc.3'],
  'verrx': ['verrx.c', 'err.3'],
  'vwarn': ['vwarn.c', 'err.3'],
  'vwarnc': ['vwarnc.c', 'errc.3'],
  'vwarnx': ['vwarnx.c', 'err.3'],
}

err += verr

# set [2] in each array of err
foreach func, info : err
  err += {func: err[func] + [not cc.has_function(func, dependencies: deps)]}
endforeach

# We might need to include stdarg.h if one of the err functions that take
# variadic arguments are needed.
stdarg_needed = false

err_func = []
err_src = []
err_man = []
err_incdir = []
err_pragma = []
err_needed = false

foreach func, info : err
  if not info[2]
    continue
  endif

  if func in verr
    stdarg_needed = true
  endif
  err_func += func # build up a database of functions in the library
  err_src += files(info[0])
  err_man += files(info[1])
  err_needed = true
endforeach

if err_needed
  err_pragma += err_func
  if cc.has_header('err.h')
    err_pragma += 'err_h'
  endif
  if stdarg_needed
    err_pragma += 'verr_func'
  endif
  gen_inc += custom_target('generate_err_h',
                           input: 'err.h.in',
                           feed: true,
                           output: 'err.h',
                           capture: true,
                           command: [header_generator] + err_pragma,
                           install: inst_h,
                           install_dir: h_inst_dir)
  err_incdir = include_directories('.')
endif
