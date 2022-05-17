/*	$OpenBSD: recallocarray.c,v 1.1 2017/03/06 18:44:21 otto Exp $	*/
/*
 * Copyright (c) 2008, 2017 Otto Moerbeek <otto@drijf.net>
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

/* OPENBSD ORIGINAL: lib/libc/stdlib/recallocarray.c */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *
recallocarray(void *ptr, size_t oldnmemb, size_t newnmemb, size_t size)
{
	size_t oldsize, newsize, newalloc;
	void *newptr;

	if (size == 0) /* Try again, but without a division by 0. */
		return recallocarray(ptr, 0, 0, 1);

	newsize = size * newnmemb;
	if (newsize / size != newnmemb) {
		errno = ENOMEM;
		return NULL;
	}
	newalloc = newsize > 0 ? newsize : 1;

	if (ptr == NULL)
		return calloc(newalloc, 1);

	oldsize = size * oldnmemb;
	if (oldsize / size != oldnmemb) {
		errno = EINVAL;
		return NULL;
	}

	/*
	 * Don't bother too much if we're shrinking just a bit,
	 * we do not shrink for series of small steps, oh well.
	 */
	if (newsize <= oldsize) {
		size_t d = oldsize - newsize;

		if (d < oldsize / 2 && d < BUFSIZ) {
			explicit_bzero((char *)ptr + newsize, d);
			return ptr;
		}
	}

	newptr = malloc(newalloc);
	if (newptr == NULL)
		return NULL;

	if (newsize > oldsize) {
		memcpy(newptr, ptr, oldsize);
		explicit_bzero((char *)newptr + oldsize, newsize - oldsize);
	} else {
		memcpy(newptr, ptr, newsize);
	}

	freezero(ptr, oldsize);

	return newptr;
}
