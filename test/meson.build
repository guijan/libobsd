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

tests = {
  'arc4random': 'arc4random.c',
  'arc4random_buf': 'arc4random_buf.c',
  'arc4random_uniform': 'arc4random_uniform.c',
  'err': 'err.c',
  'errc': 'errc.c',
  'errx': 'errx.c',
  'explicit_bzero': 'explicit_bzero.c',
  'freezero': 'freezero.c',
  'getopt': 'getopt.c',
  'getprogname': 'getprogname.c',
  'pledge': 'pledge.c',
  'reallocarray': 'reallocarray.c',
  'recallocarray': 'recallocarray.c',
  'strlcat': 'strlcat.c',
  'strlcpy': 'strlcpy.c',
  'strsep': 'strsep.c',
  'strtonum': 'strtonum.c',
  'unveil': 'unveil.c',
  'warn': 'warn.c',
  'warnc': 'warn.c',
  'warnx': 'warnx.c',
}

foreach testname, src : tests
  if syms.contains(testname)
    test(testname, executable(testname, src, dependencies: libobsd_dep),
         suite: 'libobsd')
  elif get_option('test_system')
    test(testname, executable(testname, src, dependencies: libobsd_dep),
         suite: 'system')
  endif
endforeach
