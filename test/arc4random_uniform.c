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

enum {
	ITERATIONS = 10000
};

int
main(void)
{
	int i;
	uint32_t halfmax = ~(uint32_t)0 / 2;

	for (i = 0; i < 100; i++) {
		if (arc4random_uniform(0) != 0)
			errx(1, "arc4random_uniform(0) != 0");
		if (arc4random_uniform(1) != 0)
			errx(1, "arc4random_uniform(1) != 0");
		if (arc4random_uniform(halfmax) >= halfmax)
			errx(1, "arc4random_uniform(%1$u) >= %1$u", halfmax);
		/* XX: Test the uniform spread of the return value. */
	}
}
