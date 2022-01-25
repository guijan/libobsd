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

/* How to test a 32 bit random number generator? Test that it sets every bit
 * eventually at the very least.
 * arc4random is implemented through arc4random_buf, so arc4random_buf should
 * be the one tested for the gritty details.
 */
int
main(void)
{
	uint32_t bits;
	int i;
#if !defined(UINT32_MAX) /* dietlibc doesn't declare UINT32_MAX. */
	uint32_t UINT32_MAX = 0;
	UINT32_MAX = ~UINT32_MAX;
#endif

	bits = i = 0;
	/*
	 * The condition reads: While bits ORed with the return of arc4random()
	 * doesn't have all bits set.
	 */
	while ((bits |= arc4random()) != UINT32_MAX) {
		if (++i > ITERATIONS) {
			errx(1, "%d iterations have passed and arc4random has"
			        " yet to set all bits in its\n"
			        "returned value. Set bits: %#x",
			        ITERATIONS, bits);
		}
	}
	return (0);
}
