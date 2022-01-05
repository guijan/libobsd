tests = {
  'getprogname': 'getprogname.c'
}

foreach testname, src : tests
  if get_option('test_all') or funcs.contains(testname)
    test(testname, executable(testname, src, dependencies: libobsd_dep))
  endif
endforeach
