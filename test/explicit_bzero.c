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

#include <string.h>
#include <err.h>

enum {
	BUFLEN = 1024
};

/* Check that explicit_bzero() doesn't overrun at the very least.
 *
 * XXX: Check that it's not optimized out.
 */
int
main(void)
{
	unsigned char _buf[BUFLEN * 3];
	unsigned char *buflow = _buf;
	unsigned char *bufmid = buflow + BUFLEN;
	unsigned char *bufhigh = bufmid + BUFLEN;
	unsigned char compare[BUFLEN];
	memset(_buf, 0xFF, sizeof(_buf));

	explicit_bzero(bufmid, BUFLEN);


	memset(compare, 0xFF, sizeof(compare));
	if (memcmp(buflow, compare, BUFLEN) != 0)
		err(1, "memcmp(buflow, compare, buflen) != 0");
	if (memcmp(bufhigh, compare, BUFLEN) != 0)
		err(1, "memcmp(bufhigh, compare, buflen) != 0");

	memset(compare, 0, sizeof(compare));
	if (memcmp(bufmid, compare, BUFLEN) != 0)
		err(1, "memcmp(bufmid, compare, buflen) != 0");

	return (0);
}
