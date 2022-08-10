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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "closefrom.h"

/* getdtablesize: Windows getdtablesize()
 * Will invoke the invalid parameter handler.
 */
int
getdtablesize(void)
{
	static int maxfd;

	/* https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setmaxstdio?view=msvc-170
	 * The takeaway from that documentation is that _setmaxstdio() sets the
	 * maximum amount of FILE that can be fopen()ed. The documentation
	 * explains that the upper limit of _setmaxstdio() is the maximum amount
	 * of file descriptors.
	 */
	if (maxfd == 0) {
		int old_max;
		int base;
		int lim;
		int n;
		int highest;

		base = highest = old_max = _getmaxstdio();
		/* Binary search the maximum. This is a rip off of the OpenBSD
		 * /usr/src/lib/libc/stdlib/bsearch.c
		 */
		for (lim = INT_MAX; lim > 0; lim >>= 1) {
			n = base + (lim >> 1);
			if (_setmaxstdio(n) != -1) {
				base = n + 1;
				lim--;
				if (n > highest)
					highest = n;
			}
		}
		/* Subtle breakage: another thread could have set the limit
		 * while we called _setmaxstdio in a loop. We probably overwrote
		 * that limit in the loop and we're overwriting it here again.
		 *
		 * Maybe the limits should be hardcoded.
		 */
		_setmaxstdio(old_max);
		maxfd = highest;
	}

	return maxfd;
}
