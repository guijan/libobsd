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
#include <stdlib.h>
#include <string.h>

/*
 * Copied from src/stdlib/reallocarray.c.
 * MUL_NO_OVERFLOW * MUL_NO_OVERFLOW overflows.
 * (MUL_NO_OVERFLOW-1) * (MUL_NO_OVERFLOW-1) doesn't.
 */
#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))

#define ACNT(n) ((sizeof(n) / sizeof(*n)))

int
main(void)
{
	void *p, *tmp;
	size_t i;
	struct num_pairs {
		size_t a;
		size_t b;
	};
	struct num_pairs nums[] = {
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

	/* Test a NULL first argument. */
	for (i = 0; i < ACNT(nums); i++) {
		if ((p = reallocarray(NULL, nums[i].a, nums[i].b)) == NULL) {
			err(1, "(p = reallocarray(NULL, %zu, %zu)) == NULL",
			    nums[i].a, nums[i].b);
		}
		arc4random_buf(p, nums[i].a * nums[i].b);
		free(p);
	}

	/* Test overflow detection. */
	for (i = 0; i < ACNT(overflows); i++) {
		errno = 0;
		if ((p = reallocarray(NULL, overflows[i].a, overflows[i].b))
		    != NULL) {
			errx(1, "(p = reallocarray(NULL, %zu, %zu)) != NULL",
				overflows[i].a, overflows[i].b);
		}
		(void)p;
		if (errno != ENOMEM) {
			err(1, "errno not set to ENOMEM after reallocarray"
			       " overflow: %d", errno);
		}
	}

	if ((p = reallocarray(NULL, 8, 8)) == NULL)
		err(1, "reallocarray");
	/* Test reallocating. */
	for (i = 0; i < ACNT(nums); i++) {
		if ((tmp = reallocarray(p, nums[i].a, nums[i].b)) == NULL) {
			err(1, "(tmp = reallocarray(%p, %zu, %zu)) == NULL",
			    p, nums[i].a, nums[i].b);
		}
		p = tmp;
		arc4random_buf(p, nums[i].a * nums[i].b);
	}
	free(p);

	return (0);
}
