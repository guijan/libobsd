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

# The keys inside the stdlib dictionary are the names of the functions.
# The value is an array. Here are the uses of the 3 members of the arrays:
# [0] = 'filename of the source file that implements the function'
# [1] = 'filename of the manual of the function'
# [2] = is the function needed? true/false
#
stdlib = {
  'arc4random': ['arc4random.c', 'arc4random.3'],
  'arc4random_uniform': ['arc4random_uniform.c', 'arc4random.3'],
  'strtonum': ['strtonum.c', 'arc4random.3'],
}

# set [2] in each array of stdlib
foreach func, info : stdlib
  stdlib += {func: stdlib[func] + [not cc.has_function(func)]}
endforeach

if not cc.has_function('getprogname')
  getprogname_backends = {
    '_pgmptr': ['getprogname__pgmptr.c', 'getprogname.3', true],
    '__progname': ['getprogname.c', 'getprogname.3', true],
  }
  getprogname_backend_tests = {
    '_pgmptr': '''#include <stdlib.h>
                  int main(void) {_pgmptr = "";}''',
    '__progname': '''extern const char *__progname;
                     int main(void) {__progname = "";}''',
  }
  getprogname_implemented = false
  foreach var, try : getprogname_backend_tests
    if cc.links(try, name : var)
     getprogname_implemented = true
     stdlib += {'getprogname': getprogname_backends[var]}
     break
    endif
  endforeach
  if not getprogname_implemented
    error('no way to implement getprogname()')
  endif
else
  stdlib += {'getprogname': ['', 'getprogname.3', false]}
endif

if not cc.has_function('arc4random_buf',
                       args : args,
                       prefix : '#include <stdlib.h>')
  arc4random_backends = {
    'getrandom': ['arc4random_buf_getrandom.c', '#include <sys/random.h>'],
    'rand_s': ['arc4random_buf_rand_s.c', '''#define _CRT_RAND_S
                                             #include <stdlib.h>'''],
  }
  arc4random_implemented = false
  foreach func, info : arc4random_backends
    if cc.has_function(func, args : args, prefix : info[1])
      stdlib += {'arc4random_buf': [info[0], 'arc4random.3', true]}
      arc4random_implemented = true
      break
    endif
  endforeach
  if not arc4random_implemented
    error('no way to implement arc4random')
  endif
else
  stdlib += {'arc4random_buf': ['', 'arc4random.3', false]}
endif

stdlib_h_data = configuration_data()
stdlib_needed = false
stdlib_funcs = []
stdlib_src = []
stdlib_man = []
stdlib_inc = []
stdlib_incdir = []

foreach func, info : stdlib
  stdlib_h_data.set(func, info[2].to_int())
  if info[2] # If func is needed
    stdlib_funcs += func # build up a database of functions in the library
    stdlib_src += files(info[0])
    stdlib_man += files(info[1])
    stdlib_needed = true
  endif
endforeach

if stdlib_needed
  stdlib_inc = configure_file(input : 'stdlib.h.in',
                              output : 'stdlib.h',
                              configuration : stdlib_h_data)
  stdlib_incdir = include_directories('.')
endif
