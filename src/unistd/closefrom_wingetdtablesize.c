/*
 * Copyright (c) 2022 Guilherme Janczak <guilherme.janczak@yandex.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <io.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

#include "closefrom.h"

/* UCRT has a thread local invalid parameter handler which makes this code
 * closer to thread-safe, so we try to use it. Otherwise, use the global one.
 */
#if defined(CLOSEFROM_THREADED_PARAM_HANDLER)
#  define SET_HANDLER _set_thread_local_invalid_parameter_handler
#else
#  define SET_HANDLER _set_invalid_parameter_handler
#endif

static void null_handler(wchar_t const* const, wchar_t const* const,
    wchar_t const* const, unsigned int const, uintptr_t);

int
closefrom_getdtablesize(int fd)
{
	_invalid_parameter_handler handler;
	int rval;
	int i;
	int fdlim;

	handler = SET_HANDLER(null_handler);
	/* Make sure getdtablesize() is called after the invalid parameter
	 * handler (a Windows feature) is set because the Windows
	 * getdtablesize() invokes it.
	 */
	fdlim = getdtablesize();
	rval = -1;
	for (i = fd; i < fdlim; i++) {
		if (close(i) == 0)
			rval = 0;
	}
	SET_HANDLER(handler);

	return rval;
}

/* null_handler: Windows invalid parameter handler that does nothing. */
static void
null_handler(wchar_t const* const unused0, wchar_t const* const unused1,
    wchar_t const* const unused2, unsigned int const unused3, uintptr_t unused4)
{
	(void)unused0;
	(void)unused1;
	(void)unused2;
	(void)unused3;
	(void)unused4;
}
