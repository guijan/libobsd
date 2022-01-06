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
#include <limits.h>
#include <stdlib.h>

enum {
	ITERATIONS = 10000
};

/* How to test a 32 bit number random number generator? Test that it sets each
 * byte eventually at the very least.
 * arc4random is implemented through arc4random_buf, so arc4random_buf should
 * be the one tested for the gritty details.
 */
int
main(void)
{
	int bits;
	uint32_t ret;
	int i;
	size_t j;


	bits = i = 0;
	while (bits != 0xF) { /* While the lower 4 bits aren't all set. */
		ret = arc4random();
		if (++i > ITERATIONS) {
			errx(1, "%d iterations have passed and arc4random has"
			        " yet to set all 4 bytes in its\n"
			        "returned value. bitarray where 1 bit is 1\n"
				"byte: %#x",
			        ITERATIONS, bits);
		}
		for (j = 0; j < sizeof(ret); j++) {
			if (ret & 0xFF)
				bits |= 1 << j;
			ret >>= CHAR_BIT;
		}
	}
	return (0);
}
