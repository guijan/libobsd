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
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void arc4random_buf_except(void *, size_t, unsigned char);
static void arc4random_buf_with(void *, uint32_t, unsigned char);

int
main(void)
{
	char buf[64];
	char *p, *ret;
	unsigned char b;
	int i;

	/* Simple overread test. */
	p = "AAAABBBBAAAA";
	if ((ret = memrchr(p+4, 'A', 4)) != NULL)
		errx(1, "memrchr overread at index %zu", (size_t)(ret-p));

	p = "ABBBBBBBBBBBBBBB";
	if ((ret = memrchr(p, 'A', strlen(p))) != p) {
		fprintf(stderr, "%s: memrchr found the wrong character: ",
		    getprogname());
		if (ret == NULL) {
			fputs("'(null)'", stderr);
		} else {
			fprintf(stderr, "'%#02x' at index %zu", *ret,
			    (size_t)(ret-p));
		}
		fputc('\n', stderr);
		exit(1);

	}

	/* Randomized test - char shouldn't be found */
	for (i = 0; i < 100; i++) {
		arc4random_buf(&b, sizeof(b));
		arc4random_buf_except(buf, sizeof(buf), b);
		if ((ret = memrchr(buf, b, sizeof(buf))) != NULL) {
			errx(1,
			    "memrchr incorrectly found '%#02x' at index %zu",
			    *ret, (size_t)(ret-p));
		}
	}

	/* Randomized test - char should be found */
	for (i = 0; i < 100; i++) {
		arc4random_buf(&b, sizeof(b));
		arc4random_buf_with(buf, sizeof(buf), b);
		if (memrchr(buf, b, sizeof(buf)) == NULL) {
			errx(1, "memrchr failed to find '%#02x'", b);
		}
	}
}

/* arc4random_buf_except: fill _buf of length len with random bytes but never
 * have a byte that matches except.
 */
static void
arc4random_buf_except(void *_buf, size_t len, unsigned char except)
{
	unsigned char *buf = _buf;
	size_t i;
	unsigned char b;

	arc4random_buf(buf, len);
	for (i = 0; i < len; i++) {
		if (buf[i] == except) {
			do
				arc4random_buf(&b, sizeof(b));
			while (b == except);
			buf[i] = b;
		}
	}
}

/* arc4random_buf_with: fill buf with len random bytes, but make sure there is
 * exactly one instance of the byte with in the buffer.
 */
static void
arc4random_buf_with(void *_buf, uint32_t len, unsigned char with)
{
	unsigned char *buf = _buf;
	arc4random_buf_except(buf, len, with);

	buf[arc4random_uniform(len)] = with;
}
