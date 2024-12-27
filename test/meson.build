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

configure_file(output: 'test_config.h', input: 'test_config.h.in',
  configuration: {
    'HAVE_HANDLER': cc.has_function('_set_invalid_parameter_handler',
                                    prefix: '#include <stdlib.h>')
  }
)

tests = [
  'arc4random', 'arc4random_buf', 'arc4random_uniform', 'closefrom', 'daemon',
  'err', 'errc', 'errx', 'explicit_bzero', 'memrchr', 'freezero', 'getopt',
  'getprogname', 'pledge', 'reallocarray', 'recallocarray', 'setproctitle',
  'strlcat', 'strlcpy', 'strsep', 'strtonum', 'unveil', 'warn',
  'warnc', 'warnx',
]

if cc.has_type('struct timeval', prefix: '#include <sys/time.h>') and \
   cc.has_type('struct timespec', prefix: '#include <time.h>')
  tests += 'time-macros'
endif

foreach testname : tests
  suite = []
  if syms.contains(testname)
    suite += 'libobsd'
  elif get_option('test_system')
    suite += 'system'
  endif

  if suite.length() != 0
    exe = executable(testname, testname + '.c', dependencies: libobsd_dep,
                     include_directories: include_directories('.'))
    test(testname, exe, suite: suite)
  endif
endforeach

if cc.get_id() == 'ccomp'
  cxx_support = false
endif
if cxx_support
  test('cxx', executable('cxx', 'cxx.cxx', dependencies: libobsd_dep),
       suite: 'libobsd')
endif
