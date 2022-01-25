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

# Keep this list in sync with queue.h by running printsyms.sed on queue.h every
# time queue.h is updated.
queue_macros = [
  'SLIST_HEAD',
  'SLIST_ENTRY',
  'SLIST_REMOVE',
  'LIST_HEAD',
  'LIST_HEAD_INITIALIZER',
  'LIST_ENTRY',
  'LIST_FOREACH',
  'LIST_INSERT_AFTER',
  'LIST_INSERT_HEAD',
  'LIST_REMOVE',
  'LIST_REPLACE',
  'SIMPLEQ_HEAD',
  'SIMPLEQ_HEAD_INITIALIZER',
  'SIMPLEQ_ENTRY',
  'SIMPLEQ_FOREACH',
  'SIMPLEQ_INSERT_HEAD',
  'SIMPLEQ_INSERT_TAIL',
  'SIMPLEQ_INSERT_AFTER',
  'SIMPLEQ_REMOVE_HEAD',
  'SIMPLEQ_REMOVE_AFTER',
  'SIMPLEQ_CONCAT',
  'XSIMPLEQ_HEAD',
  'XSIMPLEQ_ENTRY',
  'XSIMPLEQ_XOR',
  'XSIMPLEQ_FOREACH',
  'XSIMPLEQ_INSERT_HEAD',
  'XSIMPLEQ_INSERT_TAIL',
  'XSIMPLEQ_INSERT_AFTER',
  'XSIMPLEQ_REMOVE_HEAD',
  'XSIMPLEQ_REMOVE_AFTER',
  'TAILQ_HEAD',
  'TAILQ_HEAD_INITIALIZER',
  'TAILQ_ENTRY',
  'TAILQ_LAST',
  'TAILQ_PREV',
  'TAILQ_FOREACH',
  'TAILQ_FOREACH_REVERSE',
  'TAILQ_INSERT_HEAD',
  'TAILQ_INSERT_TAIL',
  'TAILQ_INSERT_AFTER',
  'TAILQ_REMOVE',
  'TAILQ_REPLACE',
  'TAILQ_CONCAT',
  'STAILQ_FOREACH',
  'STAILQ_REMOVE_HEAD',
  'STAILQ_REMOVE_AFTER',
]

queue_man = []

foreach macro : queue_macros
  if not cc.has_header_symbol('sys/queue.h', macro)
    # There's nothing to gain by slicing and dicing the header, just install the
    # whole thing if a single macro is missing.
    install_headers('queue.h', '_null.h', subdir: 'obsd' / 'sys')
    man += files('queue.3')
    include_directories('.')
    break
  endif
endforeach
