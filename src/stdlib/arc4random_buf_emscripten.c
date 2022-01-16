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

#include <stddef.h>

#include <emscripten.h>

/* References:
 * https://livebook.manning.com/book/webassembly-in-action/c-emscripten-macros/v-7/54
 * https://developer.mozilla.org/en-US/docs/Web/API/Crypto/getRandomValues
 * https://developer.mozilla.org/en-US/docs/Web/JavaScript/Typed_arrays
 * https://nodejs.org/api/webcrypto.html#class-crypto
 */
void
arc4random_buf(void *_buf, size_t nbytes)
{
	size_t blk;
	/*
	 * The Mozilla documentation says getRandomValues() can throw an
	 * exception if "The requested length exceeds 65,536 bytes."
	 */
	enum {quota = 65536};
	unsigned char *buf = _buf;

	while (nbytes > 0) {
		blk = nbytes <= quota ? nbytes : quota;
		EM_ASM({
			const crypto = require('crypto').webcrypto;
			const u8view = HEAPU8.subarray($0, $0 + $1);
			crypto.getRandomValues(u8view);
		}, buf, blk);
		nbytes -= blk;
		buf += blk;
	}
}
