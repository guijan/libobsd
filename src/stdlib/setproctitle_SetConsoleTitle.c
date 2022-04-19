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

#include <windows.h>

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

enum {
	TITLE_MAX = 2048,
};

/* setproctitle: set the closest thing to the process title on Windows
 *
 * Check setproctitle_argv.c for the reference implementation, most of the code
 * here is copypasted from it.
 */
void
setproctitle(const char *fmt, ...)
{
	char buf[TITLE_MAX];
	size_t ret;
	char *p;
	size_t plen;
	int errnum;
	va_list ap;
	DWORD lasterr;
	p = buf;
	plen = sizeof(buf);

#define WALK()								\
	do {								\
		if (ret >= plen)					\
			goto set;					\
		p += ret, plen -= ret;					\
	} while (0)

	ret = strlcpy(p, getprogname(), plen);
	if (fmt == NULL)
		goto set;
	WALK();

	ret = strlcpy(p, ": ", plen);
	WALK();

	errnum = errno;
	va_start(ap, fmt);
	vsnprintf(p, plen, fmt, ap);
	va_end(ap);
	errno = errnum;

set:
	lasterr = GetLastError();
	SetConsoleTitle(buf);
	SetLastError(lasterr);
}
