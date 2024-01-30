# Copyright (c) 2022, 2024 Guilherme Janczak <guilherme.janczak@yandex.com>
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
  'arc4random_buf': ['', 'arc4random.3'],
  'arc4random_uniform': ['arc4random_uniform.c', 'arc4random.3'],
  'daemon': ['', 'daemon.3'],
  'freezero': ['freezero.c', 'freezero.3'],
  'getprogname': ['', 'getprogname.3'],
  'reallocarray': ['reallocarray.c', 'reallocarray.3'],
  'recallocarray': ['recallocarray.c', 'recallocarray.3'],
  'setproctitle': ['', 'setproctitle.3'],
  'strtonum': ['strtonum.c', 'strtonum.3'],
}

# set [2] in each array of stdlib
foreach func, info : stdlib
  func_found = cc.has_function(func, args: args, prefix: '#include <stdlib.h>',
                               dependencies: deps)
  stdlib += {func: stdlib[func] + [not func_found]}
endforeach

if stdlib['arc4random_buf'][2]
  bcrypt_dep = cc.find_library('bcrypt', required: false)
  bcrypt_pref = '''#include <windows.h>\n#include <bcrypt.h>'''
  impl = ''
  if bcrypt_dep.found() and cc.has_function('BCryptGenRandom',
                                           prefix: bcrypt_pref,
                                           dependencies: bcrypt_dep)
    deps += bcrypt_dep
    impl = 'arc4random_buf_BCryptGenRandom.c'
  elif host_machine.system() == 'emscripten'
    impl = 'arc4random_buf_emscripten.c'
  elif cc.has_function('getrandom', args: args,
                       prefix: '#include <sys/random.h>')
    impl = 'arc4random_buf_getrandom.c'
  endif
  if impl != ''
    stdlib += {'arc4random_buf': [impl, 'arc4random.3', true]}
  endif
endif

stdlib_pragma = [] # daemon() needs stdlib_pragma early
if stdlib['daemon'][2]
  daemon_functions = ['fork', '_exit', 'setsid', 'chdir', 'open', 'dup2']
  # openbsd has daemon() declared in stdlib.h
  # glibc and dietlibc have daemon() declared in unistd.h
  if cc.has_function('daemon', args: args, prefix: '#include <unistd.h>',
                     dependencies: deps)
    stdlib += {'daemon': ['', '', false]}
    stdlib_pragma += 'unistd_h'
  else
    has_daemon_funcs = true
    foreach func : daemon_functions
      if not cc.has_function(func, args: args, dependencies: deps,
                             prefix: '''#include <unistd.h>
                                        #include <fcntl.h>''')
        has_daemon_funcs = false
      endif
    endforeach
    if has_daemon_funcs
      stdlib += {'daemon': ['daemon.c', 'daemon.3', true]}
    elif cc.has_function('FreeConsole', args: args, dependencies: deps,
                         prefix: '#include <windows.h>')
      stdlib += {'daemon': ['daemon_win.c', 'daemon.3', true]}
    endif
  endif
endif

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
                                 name: '__progname',
                              args: args, dependencies: deps)
  if __progname_found
    stdlib += {'getprogname': getprogname_backends['__progname']}
  else
    foreach symbol, header : getprogname_symbols
      if cc.has_header_symbol(header, symbol, args: args, dependencies: deps)
        stdlib += {'getprogname': getprogname_backends[symbol]}
        break
      endif
    endforeach
  endif
endif

stdlib_sym = []

if stdlib['setproctitle'][2]
  if cc.has_function('SetConsoleTitle', args: args,
                     prefix: '#include <windows.h>', dependencies: deps)
    setproctitle_backend = 'setproctitle_SetConsoleTitle.c'
  elif host_machine.system() == 'emscripten'
    setproctitle_backend = 'setproctitle_emscripten.c'
  else
    # This is an ugly hack, but it's implemented in standard C and works on
    # multiple OSes, so it either works or does nothing meaningful.
    setproctitle_backend = 'setproctitle_argv.c'
    stdlib_sym += 'setproctitle_init'
  endif
  stdlib += {'setproctitle': [setproctitle_backend, 'setproctitle.3', true]}
endif

stdlib_needed = false
stdlib_src = []
stdlib_man = []
stdlib_incdir = []

if not cc.has_type('uint32_t', prefix: '#include <stdlib.h>')
  stdlib_pragma += ['uint32_t']
  stdlib_needed = true
endif

foreach func, info : stdlib
  if not info[2]
    continue
  endif
  if info[0] == ''
   error('no way to implement', func)
  endif
  stdlib_sym += func # build up a database of symbols in the library
  stdlib_src += files(info[0])
  stdlib_man += files(info[1])
  stdlib_needed = true
endforeach

if stdlib_needed
  stdlib_pragma += stdlib_sym
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
