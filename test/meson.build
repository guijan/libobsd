tests = {
  'arc4random': 'arc4random.c',
  'arc4random_buf': 'arc4random_buf.c',
  'arc4random_uniform': 'arc4random_uniform.c',
  'err': 'err.c',
  'errc': 'errc.c',
  'errx': 'errx.c',
  'getprogname': 'getprogname.c',
  'reallocarray': 'reallocarray.c',
  'strlcat': 'strlcat.c',
  'strlcpy': 'strlcpy.c',
  'strtonum': 'strtonum.c',
}

foreach testname, src : tests
  if funcs.contains(testname)
    test(testname, executable(testname, src, dependencies: libobsd_dep),
                              suite: 'libobsd')
  elif get_option('test_system')
    test(testname, executable(testname, src, dependencies: libobsd_dep),
                              suite: 'system')
  endif
endforeach
