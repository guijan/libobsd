/*	$OpenBSD: strlcattest.c,v 1.5 2021/09/27 19:33:58 millert Exp $ */

/*
 * Copyright (c) 2014 Todd C. Miller <millert@openbsd.org>
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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum {
	BUFSIZE = 1024
};

int
main(void)
{
	char buf[BUFSIZE], *cp, *ep;
	size_t len;

	memset(buf, 'z', BUFSIZE);
	ep = buf + BUFSIZE;

	/* Test appending to an unterminated string. */
	len = strlcat(buf, "abcd", BUFSIZE);
	if (len != 4 + BUFSIZE)
		errx(1, "strlcat: failed unterminated buffer test (1a)");

	/* Make sure we only wrote where expected. */
	for (cp = buf; cp < ep; cp++) {
		if (*cp != 'z')
			errx(1, "strlcat: failed unterminated buffer test (1b)");
	}

	/* Test appending to a full string. */
	ep[-1] = '\0';
	len = strlcat(buf, "abcd", BUFSIZE);
	if (len != 4 + BUFSIZE - 1)
		errx(1, "strlcat: failed full buffer test (2a)");

	/* Make sure we only wrote where expected. */
	for (cp = buf; cp < ep - 1; cp++) {
		if (*cp != 'z')
			errx(1, "strlcat: failed full buffer test (2b)");
	}

	/* Test appending to an empty string. */
	ep[-1] = 'z';
	buf[0] = '\0';
	len = strlcat(buf, "abcd", BUFSIZE);
	if (len != 4)
		err(1, "strlcat: failed empty buffer test (3a)");

	/* Make sure we only wrote where expected. */
	if (memcmp(buf, "abcd", sizeof("abcd")) != 0)
		err(1, "strlcat: failed empty buffer test (3b)");

	for (cp = buf + len + 1; cp < ep; cp++) {
		if (*cp != 'z')
			err(1, "strlcat: failed empty buffer test (3c)");
	}

	/* Test appending to a NUL-terminated string. */
	memcpy(buf, "abcd", sizeof("abcd"));
	len = strlcat(buf, "efgh", BUFSIZE);
	if (len != 8)
		err(1, "strlcat: failed empty buffer test (4a)");

	/* Make sure we only wrote where expected. */
	if (memcmp(buf, "abcdefgh", sizeof("abcdefgh")) != 0)
		err(1, "strlcat: failed empty buffer test (4b)");

	for (cp = buf + len + 1; cp < ep; cp++) {
		if (*cp != 'z')
			err(1, "strlcat: failed empty buffer test (4c)");
	}

	return (0);
}
