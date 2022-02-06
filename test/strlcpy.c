/*	$OpenBSD: strlcpytest.c,v 1.5 2021/09/27 19:33:58 millert Exp $ */

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
	char *buf, *buf2, *cp, *ep;
	size_t len;

	buf = malloc(BUFSIZE);
	buf2 = malloc(BUFSIZE);
	if (buf == NULL || buf2 == NULL)
		err(1, "malloc");

	memset(buf, 'z', BUFSIZE);
	ep = buf + BUFSIZE;

	/* Test copying to a zero-length NULL buffer. */
	len = strlcpy(NULL, "abcd", 0);
	if (len != 4)
		errx(1, "strlcpy: failed zero-length buffer test (1a)");

	/* Test copying small string to a large buffer. */
	len = strlcpy(buf, "abcd", BUFSIZE);
	if (len != 4)
		errx(1, "strlcpy: failed large buffer test (2a)");

	/* Make sure we only wrote where expected. */
	if (memcmp(buf, "abcd", sizeof("abcd")) != 0)
		errx(1, "strlcpy: failed large buffer test (2b)");

	for (cp = buf + len + 1; cp < ep; cp++) {
		if (*cp != 'z')
			errx(1, "strlcpy: failed large buffer test (2c)");
	}

	/* Test copying large string to a small buffer. */
	memset(buf, 'z', BUFSIZE);
	memset(buf2, 'x', BUFSIZE - 1);
	buf2[BUFSIZE - 1] = '\0';
	len = strlcpy(buf, buf2, BUFSIZE / 2);
	if (len != BUFSIZE - 1)
		errx(1, "strlcpy: failed small buffer test (3a)");

	/* Make sure we only wrote where expected. */
	len = (BUFSIZE / 2) - 1;
	if (memcmp(buf, buf2, len) != 0 || buf[len] != '\0')
		errx(1, "strlcpy: failed small buffer test (3b)");

	for (cp = buf + len + 1; cp < ep; cp++) {
		if (*cp != 'z')
			errx(1, "strlcpy: failed small buffer test (3c)");
	}

	/* Test copying to a 1-byte buffer. */
	memset(buf, 'z', BUFSIZE);
	len = strlcpy(buf, "abcd", 1);
	if (len != 4)
		errx(1, "strlcpy: failed 1-byte buffer test (4a)");

	/* Make sure we only wrote where expected. */
	if (buf[0] != '\0')
		errx(1, "strlcpy: failed 1-byte buffer test (4b)");

	for (cp = buf + 1; cp < ep; cp++) {
		if (*cp != 'z')
			errx(1, "strlcpy: failed 1-byte buffer test (4c)");
	}

	return (0);
}
