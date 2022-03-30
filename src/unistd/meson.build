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

getopt_syms_needed = false
foreach sym : ['optarg', 'opterr', 'optind', 'optopt', 'optreset']
  if not cc.has_header_symbol('unistd.h', sym, args: args, dependencies: deps)
    getopt_syms_needed = true
    unistd_needed = true

    unistd_pragma += sym

    # XXX: This is a kludge. MSVC-style compilers warn if a variable is exported
    # in a .def file and with declspec simultaneously. Adding sym to unistd_sym
    # leads to it being put in the .def file later on, so we avoid this. All the
    # possible sym values will be exported with declspec.
    # Additionally, exporting variables like the contents of sym can't be done
    # through the .def file anyway.
    if cc.get_argument_syntax() != 'msvc'
      unistd_sym += sym
    endif
  endif
endforeach

if unistd_needed
  if cc.has_header('unistd.h', args: args, dependencies: deps)
    unistd_pragma += 'unistd.h'
  endif

  if getopt_syms_needed
    unistd_pragma += 'getopt_syms'
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
