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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test_config.h"

#if defined(HAVE_HANDLER)
static void null_handler(wchar_t const* const, wchar_t const* const,
    wchar_t const* const, unsigned int const, uintptr_t);
#endif
static int dup2_noerr(int, int);
static int closefrom_r(int);

int
main(void)
{
	int i;
#if defined(HAVE_HANDLER)
	/* close() will trigger the handler. */
	_set_invalid_parameter_handler(null_handler);
#endif

	/* Test that closefrom() works at all. */
	for (i = 10; i < 20; i++)
		dup2_noerr(1, i);
	closefrom_r(11);
	/* Test that a descriptor one less than the minimum wasn't closed. */
	if (close(10) == -1)
		err(1, "close(10)");
	/* Test that all descriptors were closed. */
	for (i = 11; i < 20; i++) {
		if (close(i) == 0)
			errx(1, "closefrom_r(11) didn't close fd %d", i);
	}

	/* Test that a negative value returns EBADF. */
	errno = 0;
	if (closefrom_r(-1) == -1 && errno != EBADF)
		errx(1, "closefrom_r(-1) didn't fail with EBADF");

	/* Test that having no fds to close returns EBADF as long as it returns
	 * error. It won't fail on some platforms.
	 */
	errno = 0;
	if (closefrom_r(10) == -1 && errno != EBADF) {
		errx(1, "closefrom_r(/* more than all open fds */) didn't fail"
		    " with EBADF");
	}
}

#if defined(HAVE_HANDLER)
/* null_handler: Windows invalid parameter handler that does nothing. */
static void
null_handler(wchar_t const* const unused0, wchar_t const* const unused1,
    wchar_t const* const unused2, unsigned int const unused3, uintptr_t unused4)
{
	(void)unused0;
	(void)unused1;
	(void)unused2;
	(void)unused3;
	(void)unused4;
}
#endif

static int
dup2_noerr(int oldd, int newd)
{
	int rval;
	int saved_errno;

	saved_errno = errno;
	while ((rval = dup2(oldd, newd)) == -1) {
		if (errno == EINTR)
			continue;
		err(1, "dup2");
	}
	errno = saved_errno;
	return rval;
}

/* closefrom_r: closefrom with restart
 *
 * Compatible with both Solaris and OpenBSD closefrom() APIs.
 */
static int
closefrom_r(int fd)
{
	int saved_errno;

	saved_errno = errno;
	for (;;) {
		errno = 0;
		closefrom(fd);

		if (errno == 0)
			break;
		else if (errno == EINTR)
			continue;
		else
			return -1;
	}
	errno = saved_errno;
	return 0;
}
