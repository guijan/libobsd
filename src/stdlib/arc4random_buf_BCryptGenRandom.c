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
#include <bcrypt.h>

#include <string.h>

void
arc4random_buf(void *_buf, size_t nbytes)
{
	unsigned char *buf = _buf;
	ULONG blk;
	const ULONG rng = BCRYPT_USE_SYSTEM_PREFERRED_RNG;

	while (nbytes > 0) {
		/* Explicit cast to avoid MSVC warning. */
		blk = (ULONG)nbytes;
		if (blk == 0)
			blk = ~blk;

		if (!BCRYPT_SUCCESS(BCryptGenRandom(NULL, buf, blk, rng)))
			abort();
		buf += blk;
		nbytes -= blk;
	}
}
