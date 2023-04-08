# Copyright (c) 2023 Guilherme Janczak <guilherme.janczak@yandex.com>
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

time_macros = [
  'TIMESPEC_TO_TIMEVAL',
  'TIMEVAL_TO_TIMESPEC',
  'timeradd',
  'timerclear',
  'timercmp',
  'timerisset',
  'timerisvalid',
  'timersub',
  'timespecadd',
  'timespecclear',
  'timespeccmp',
  'timespecisset',
  'timespecisvalid',
  'timespecsub',
]

time_needed = false
foreach macro : time_macros
  if not cc.has_header_symbol('sys/time.h', macro, args: args,
                              dependencies: deps)
    time_needed = true
    break
  endif
endforeach

time_incdir = []
time_man = []
time_sym = []
if time_needed
  time_pragma = []
  if cc.has_header('sys/time.h')
    time_pragma += 'sys_time.h'
  endif

  time_incdir += include_directories('.')
  time_man += files('timeradd.3')
  time_sym += 'time-macros'

  # Due to Meson weirdness, custom_target() can't output to a directory, so we
  # step into the directory first.
  subdir('sys')
endif
