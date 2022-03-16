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
  'arc4random_buf': ['', 'arc4random.3'],
  'freezero': ['freezero.c', 'freezero.3'],
  'getprogname': ['', 'getprogname.3'],
  'reallocarray': ['reallocarray.c', 'reallocarray.3'],
  'recallocarray': ['recallocarray.c', 'recallocarray.3'],
  'strtonum': ['strtonum.c', 'strtonum.3'],
}

# These are the functions that need the uint32_t type.
stdlib_uint32 = {
  'arc4random': ['arc4random.c', 'arc4random.3'],
  'arc4random_uniform': ['arc4random_uniform.c', 'arc4random.3'],
}

stdlib += stdlib_uint32

# set [2] in each array of stdlib
foreach func, info : stdlib
  func_found = cc.has_function(func, args: args, prefix: '#include <stdlib.h>',
                               dependencies: deps)
  stdlib += {func: stdlib[func] + [not func_found]}
endforeach

if stdlib['getprogname'][2]
  short_name = 'program_invocation_short_name'
  getprogname_backends = {
    '_pgmptr': ['getprogname__pgmptr.c', 'getprogname.3', true],
    '__progname': ['getprogname___progname.c', 'getprogname.3', true],
    short_name: ['getprogname_' + short_name + '.c', 'getprogname.3', true],
  }
  getprogname_symbols = {
    '_pgmptr': 'stdlib.h',
    short_name: 'errno.h'
  }
  __progname_found = cc.links('''extern const char *__progname;
                                 int main(void) {__progname = "";}''',
                                 name: '__progname')
  if __progname_found
    stdlib += {'getprogname': getprogname_backends['__progname']}
  else
    foreach symbol, header : getprogname_symbols
      if cc.has_header_symbol(header, symbol, args: args)
        stdlib += {'getprogname': getprogname_backends[symbol]}
        break
      endif
    endforeach
  endif
endif

if stdlib['arc4random_buf'][2]
  arc4random_backends = {
    'getrandom': ['arc4random_buf_getrandom.c', '#include <sys/random.h>'],
    'BCryptGenRandom': ['arc4random_buf_BCryptGenRandom.c',
                        '''#include <windows.h>
                        #include <bcrypt.h>'''],

  }

  if host_machine.system() == 'emscripten'
    stdlib += {'arc4random_buf': ['arc4random_buf_emscripten.c',
                                  'arc4random.3', true]}
  elif host_machine.system() == 'haiku'
    stdlib += {'arc4random_buf': ['arc4random_buf_dev_urandom.c',
                                  'arc4random.3', true]}
  endif

  bcrypt_dep = cc.find_library('bcrypt', required: false)
  foreach func, info : arc4random_backends
    if cc.has_function(func, args: args, prefix: info[1],
                       dependencies: bcrypt_dep)
      stdlib += {'arc4random_buf': [info[0], 'arc4random.3', true]}
      if func == 'BCryptGenRandom'
        deps += bcrypt_dep
      endif
      break
    endif
  endforeach
endif

stdlib_needed = false
stdlib_func = []
stdlib_src = []
stdlib_man = []
stdlib_incdir = []
stdlib_pragma = []

foreach func, info : stdlib
  if not info[2]
    continue
  endif
  if info[0] == ''
   error('no way to implement', func)
  endif
  if func in stdlib_uint32
    stdlib_pragma = ['stdlib_uint32']
  endif
  stdlib_func += func # build up a database of functions in the library
  stdlib_src += files(info[0])
  stdlib_man += files(info[1])
  stdlib_needed = true
endforeach

if stdlib_needed
  stdlib_pragma += stdlib_func
  gen_inc += custom_target('generate_stdlib_h',
                           input: 'stdlib.h.in',
                           feed: true,
                           output: 'stdlib.h',
                           capture: true,
                           command: [header_generator] + stdlib_pragma,
                           install: inst_h,
                           install_dir: h_inst_dir)
  stdlib_incdir = include_directories('.')
endif
