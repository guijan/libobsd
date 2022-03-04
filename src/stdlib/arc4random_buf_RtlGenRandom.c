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

#include <stdlib.h>

/*
 * Microsoft documents RtlGenRandom here:
 * https://docs.microsoft.com/en-us/windows/win32/api/ntsecapi/nf-ntsecapi-rtlgenrandom
 *
 * MinGW has a declaration in ntsecapi.h
 * https://github.com/Alexpux/mingw-w64/blob/master/mingw-w64-headers/include/ntsecapi.h#L850
 *
 * Microsoft also has a declaration in ntsecapi.h
 * https://github.com/tpn/winsdk-10/blob/master/Include/10.0.14393.0/um/NTSecAPI.h#L4016-L4017
 * However, Microsoft's declaration is inside a macro conditional.
 * This page documents _WIN32_WINNT:
 * https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-170
 *
 * Annoyingly, ntsecapi.h doesn't declare the types it uses, so make sure to
 * include windows.h BEFORE including ntsecapi.h
 */
#define _WIN32_WINNT 0x0501 /* Oldest version with SystemFunction036, XP. */
#include <windows.h>
#include <ntsecapi.h>

void
arc4random_buf(void *_buf, size_t nbytes)
{
	unsigned char *buf = _buf;
	ULONG blk;

	while (nbytes > 0) {
		blk = nbytes;
		if (blk == 0)
			blk = ~blk;
		if (RtlGenRandom(buf, blk) == FALSE)
			abort();
		buf += blk;
		nbytes -= blk;
	}
}
