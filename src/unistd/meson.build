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
unistd = {
  'closefrom': ['closefrom.c', 'closefrom.3'],
  'getopt': ['getopt.c', 'getopt.3'],
  'pledge': ['pledge.c', 'pledge.3'],
  'unveil': ['unveil.c', 'unveil.3'],
}

# set [2] in each array of unistd
foreach func, info : unistd
  unistd += {func: unistd[func] + [not cc.has_function(func,
                                                       dependencies: deps,
                                                       args: args)]}
endforeach

unistd_needed = false
unistd_sym = []
unistd_src = []
unistd_man = []
unistd_incdir = []
unistd_pragma = []

foreach func, info : unistd
  if not info[2]
    continue
  endif

  unistd_sym += func # build up a database of symbols in the library
  unistd_src += files(info[0])
  unistd_man += files(info[1])
  unistd_needed = true
endforeach

if cc.has_header('io.h')
  unistd_pragma += 'io.h'
  unistd_needed = true
endif

if unistd['closefrom'][2]
  closefrom_data = configuration_data()

  close_range = cc.has_function('close_range', args: args,
                                 dependencies: deps,
                                 prefix: '#include <unistd.h>')
  # Some libcs don't have a wrapper for the syscall, so we provide our own in
  # those cases.
  if not close_range and host_machine.system() == 'linux'
    cc.has_header('linux/close_range.h')
    unistd_src += files('closefrom_close_range_syscall.c')
    closefrom_data.set('CLOSEFROM_CLOSE_RANGE_WRAPPER', true)
    close_range = true
  endif
  if close_range
    unistd_src += files('closefrom_close_range.c')
    closefrom_data.set('CLOSEFROM_CLOSE_RANGE', true)
  else # No fallbacks if we have close_range.

    if import('fs').exists('/dev/fd/0')
      devfd_hdrs = '''#include <sys/types.h>
                      #include <dirent.h>
                      #include <unistd.h>'''
      # XXX: Might want to check if readdir() is thread-safe. Both readdir() and
      # readdir_r() are more or less broken APIs as far as the POSIX version
      # goes, but typical implementations are not.
      devfd_funcs = true
      foreach func : ['opendir', 'readdir', 'closedir', 'close']
        if not cc.has_function(func, args: args, dependencies: deps,
                               prefix: devfd_hdrs)
          devfd_funcs = false
          break
        endif
      endforeach
      if devfd_funcs
        closefrom_data.set('CLOSEFROM_DEVFD', true)
        unistd_src += files('closefrom_devfd.c')
      endif
    endif

    # We always have at the very least the closefrom() that uses
    # getdtablesize(), this is probably the worst backend but also the most
    # portable.
    closefrom_data.set('CLOSEFROM_GETDTABLESIZE', true)
    # if close() is in io.h, assume it's Microsoft's close(); Microsoft's
    # differs from POSIX by invoking an invalid parameter handler on error.
    if cc.has_function('close', args: args, dependencies: deps,
                       prefix: '#include <io.h>')
      unistd_src += files('closefrom_wingetdtablesize.c')
      if cc.has_function('_set_thread_local_invalid_parameter_handler',
                         args: args, dependencies: deps,
                         prefix: '#include <stdlib.h>')
        closefrom_data.set('CLOSEFROM_THREADED_PARAM_HANDLER', true)
      endif
    else
      unistd_src += files('closefrom_getdtablesize.c')
    endif
    if not cc.has_function('getdtablesize', args: args, dependencies: deps,
                           prefix: '#include <unistd.h>')
      closefrom_data.set('CLOSEFROM_GETDTABLESIZE_DECL', true)

      maxstdio = true
      foreach func : ['_setmaxstdio', '_getmaxstdio']
        if not cc.has_function(func, args: args, dependencies: deps,
                               prefix: '#include <stdio.h>')
          maxstdio = false
          break
        endif
      endforeach
      if maxstdio
        unistd_src += files('closefrom_getdtablesize__setmaxstdio.c')
      elif cc.has_function('getrlimit', args: args, dependencies: deps,
                           prefix: '#include <sys/resource.h>') \
           and cc.has_header_symbol('sys/resource.h', 'RLIMIT_NOFILE',
                                    args: args, dependencies: deps)
        unistd_src += files('closefrom_getdtablesize_getrlimit.c')
      elif cc.has_function('sysconf', args: args, dependencies: deps,
                           prefix: '#include unistd.h')
        unistd_src += files('closefrom_getdtablesize_sysconf.c')
      else
        error('no way to implement closefrom()')
      endif
    endif
  endif

  gen_inc += configure_file(input: 'closefrom.h.in',
                            output: 'closefrom.h',
                            configuration: closefrom_data)
endif

getopt_syms_needed = false
foreach sym : ['optarg', 'opterr', 'optind', 'optopt', 'optreset']
  if not cc.has_header_symbol('unistd.h', sym, args: args, dependencies: deps)
    getopt_syms_needed = true
    unistd_needed = true
    unistd_pragma += sym
  endif
endforeach

if unistd_needed
  if cc.has_header('unistd.h', args: args, dependencies: deps)
    unistd_pragma += 'unistd.h'
  endif

  unistd_gen = custom_target('generate_unistd.h',
                             input: 'unistd.h.in',
                             feed: true,
                             output: 'unistd.h',
                             capture: true,
                             command: [header_generator] + unistd_sym +
                                      unistd_pragma,
                             install: inst_h,
                             install_dir: h_inst_dir)
  if getopt_syms_needed and not unistd['getopt'][2]
    unistd_src += custom_target('generate_getopt_syms.c',
                                input: 'getopt_syms.c.in',
                                feed: true,
                                output: 'getopt_syms.c',
                                capture: true,
                                command: [header_generator] + unistd_sym +
                                         unistd_pragma,
                                depends: unistd_gen)
  endif

  gen_inc += unistd_gen
  unistd_incdir = include_directories('.')
endif
