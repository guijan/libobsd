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
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * MUL_NO_OVERFLOW * MUL_NO_OVERFLOW overflows.
 * (MUL_NO_OVERFLOW-1) * (MUL_NO_OVERFLOW-1) doesn't.
 */
#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))

#define ACNT(n) ((sizeof(n) / sizeof(*n)))

static void *not_memchr(const void *, unsigned char, size_t);

int
main(void)
{
	unsigned char *p;
	void *tmp;
	size_t i;
	struct num_pairs {
		size_t a;
		size_t b;
	};
	struct num_pairs allocs[] = {
		{0, 0}, {1024, 2}, {0, 1}, {2, 1024},
		{1, 1}, {4, 4096}, {1, 0}, {4096, 4},
		{arc4random_uniform(2), arc4random_uniform(2048)},
		{arc4random_uniform(4), arc4random_uniform(4096)},
		{arc4random_uniform(8), arc4random_uniform(8192)},
	};
	struct num_pairs overflows[] = {
		{MUL_NO_OVERFLOW, MUL_NO_OVERFLOW},
		{MUL_NO_OVERFLOW * 2, MUL_NO_OVERFLOW / 2},
		{MUL_NO_OVERFLOW / 2, MUL_NO_OVERFLOW * 2},
		{MUL_NO_OVERFLOW * 4, MUL_NO_OVERFLOW / 4}
	};
	size_t reallocs[] = {
		0, 1, 2, 1024, 555, 9999, 8192, 4096, arc4random_uniform(16386)
	};
	/* The corresponding `size` of every `nmemb` in reallocs[] */
	enum {size = 8};
	size_t oldalloc;

	/* Test a NULL first argument. */
	for (i = 0; i < ACNT(allocs); i++) {
		p = recallocarray(NULL, SIZE_MAX, allocs[i].a, allocs[i].b);
		if (p == NULL) {
			err(1, "(p = recallocarray(NULL, %zu, %zu, %zu))"
			    " == NULL", SIZE_MAX, allocs[i].a, allocs[i].b);
		}
		/*
		 * Use the memory to try to trigger a crash if there's something
		 * wrong.
		 */
		arc4random_buf(p, allocs[i].a * allocs[i].b);
		free(p);
	}

	/* Test overflow detection. */
	if ((tmp = malloc(1)) == NULL)
		err(1, "malloc");
	for (i = 0; i < ACNT(overflows); i++) {
		p = recallocarray(NULL, 0, overflows[i].a, overflows[i].b);
		if (p != NULL) {
			errx(1,
			    "(p = recallocarray(NULL, 0, %zu, %zu)) != NULL",
			    overflows[i].a, overflows[i].b);
		}
		(void)p;
		if (errno != ENOMEM) {
			err(1, "newnmemb * size: %zu * %zu == %zu\n"
			       "errno not set to ENOMEM after recallocarray"
			       " overflow, integer value: %d\n",
			       overflows[i].a, overflows[i].b,
			       overflows[i].a * overflows[i].b,
			       errno);
		}

		p = recallocarray(tmp, overflows[i].a, 0, overflows[i].b);
		if (p != NULL) {
			errx(1,
			    "(p = recallocarray(tmp, %zu, 0, %zu)) != NULL",
			    overflows[i].a, overflows[i].b);
		}
		(void)p;
		if (errno != EINVAL) {
			err(1, "oldnmemb * size: %zu * %zu == %zu\n"
			       "errno not set to EINVAL after recallocarray"
			       " overflow, integer value: %d\n",
			       overflows[i].a, overflows[i].b,
			       overflows[i].a * overflows[i].b,
			       errno);
		}
	}
	free(tmp);

	oldalloc = 8;
	if ((p = recallocarray(NULL, 0, oldalloc, size)) == NULL)
		err(1, "recallocarray");
	/* Test reallocating. */
	for (i = 0; i < ACNT(reallocs); i++) {
		size_t oldsize;
		size_t newsize;

		tmp = recallocarray(p, oldalloc, reallocs[i], size);
		if (tmp == NULL) {
			err(1,
			    "(tmp = recallocarray(%p, %zu, %zu, %d)) == NULL",
			    p, oldalloc, reallocs[i], size);
		}

		oldsize = oldalloc * size;
		newsize = reallocs[i] * size;
		p = tmp;
		if (newsize > oldsize
		    && not_memchr(p + oldsize, 0, newsize - oldsize) != NULL) {
			errx(1, "not_memchr(%p, 0, %zu) != NULL", p + oldsize,
			    newsize - oldsize);
		}

		arc4random_buf(p, newsize);
		oldalloc = reallocs[i];
	}
	free(p);

	return (0);
}

/* not_memchr: return a pointer to the first char that is NOT c */
static void *
not_memchr(const void *_buf, unsigned char c, size_t len)
{
	size_t i;
	const unsigned char *buf = _buf;

	for (i = 0; i < len; i++) {
		if (buf[i] != c)
			return ((void *)&buf[i]);
	}

	return (NULL);
}
