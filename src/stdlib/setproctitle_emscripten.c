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

#include <emscripten.h>

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
	TITLE_MAX = 2048,
};

/*
 * Have a look at setproctitle_argv.c. I copypasted most of the code here from
 * it.
 */
void
setproctitle(const char *fmt, ...)
{
	static char buf[TITLE_MAX];
	size_t ret;
	char *p;
	size_t plen;
	va_list ap;
	int errnum;
	int intret;
	p = buf;
	plen = sizeof(buf);

#define WALK()								\
	do {								\
		if (ret >= plen) {					\
			p += plen-1; 					\
			goto set;					\
		}							\
		p += ret, plen -= ret;					\
	} while (0)

	ret = strlcpy(p, getprogname(), plen);
	WALK();
	if (fmt == NULL)
		goto set;

	ret = strlcpy(p, ": ", plen);
	WALK();

	va_start(ap, fmt);
	errnum = errno;
	intret = vsnprintf(p, plen, fmt, ap);
	errno = errnum;
	va_end(ap);
	if (intret < 0)
		goto set;
	ret = intret;
	WALK();

set:
	EM_ASM({
		const process = require('process');
		const u8view = HEAPU8.subarray($0, $1);
		process.title = u8view;
	}, buf, p);
}
