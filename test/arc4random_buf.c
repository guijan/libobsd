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

#include <err.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

enum {
	BUFLEN = 1024
};

int
main(void)
{
	const unsigned char zeroes[BUFLEN] = {0};
	unsigned char _buf[BUFLEN * 3] = {0};
	unsigned char *buflow = _buf;
	unsigned char *bufmid = buflow + BUFLEN;
	unsigned char *bufhigh = bufmid + BUFLEN;

	arc4random_buf(bufmid, BUFLEN);

	/* Careful, the first test is different. */
	if (memcmp(bufmid, zeroes, BUFLEN) == 0)
		errx(1, "memcmp(bufmid, zeroes, BUFLEN) == 0");

	if (memcmp(buflow, zeroes, BUFLEN) != 0)
		errx(1, "memcmp(buflow, zeroes, BUFLEN) != 0");
	if (memcmp(bufhigh, zeroes, BUFLEN) != 0)
		errx(1, "memcmp(bufhigh, zeroes, BUFLEN) != 0");

	/* XXX: Add randomness analysis and check for thread and fork safety. */
	return (0);
}
