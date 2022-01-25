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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void *
recallocarray(void *_ptr, size_t oldnmemb, size_t newnmemb, size_t size)
{
	unsigned char *ptr = _ptr;
	size_t oldsize, newsize;
	unsigned char *newptr;

	if (__builtin_mul_overflow(newnmemb, size, &newsize)) {
		errno = ENOMEM;
		return NULL;
	}

	if (newsize == 0)
		newsize = 1;
	if (ptr == NULL)
		return calloc(newsize, 1);

	if (__builtin_mul_overflow(oldnmemb, size, &oldsize)) {
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
			explicit_bzero(ptr + newsize, d);
			return ptr;
		}
	}

	if ((newptr = malloc(newsize)) == NULL)
		return NULL;

	if (newsize > oldsize) {
		memcpy(newptr, ptr, oldsize);
		explicit_bzero(newptr + oldsize, newsize - oldsize);
	} else {
		memcpy(newptr, ptr, newsize);
	}

	explicit_bzero(ptr, oldsize);
	free(ptr);

	return newptr;
}
