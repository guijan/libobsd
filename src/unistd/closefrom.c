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

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "closefrom.h"

/* The API of a closefrom() backend is as follows:
 *
 * The fd parameter is the minimum file descriptor to close, all fds more than
 * or equal the fd are closed.
 *
 * Return -1 on error. If the backend sets errno to EBADF, no fds needed to be
 * closed. If errno is set to EINTR, we were interrupted, return.
 *
 * Return a positive integer on partial success. This integer is the next fd
 * that needs to be closed.
 *
 * Return 0 on success.
 *
 * Backends aren't responsible for saving and restoring errno.
 */
typedef int (*closefrom_backend)(int fd);

/* closefrom: OpenBSD closefrom() for many systems
 *
 * Slightly different from Solaris closefrom(), consult man pages.
 */
int
closefrom(int fd)
{
	static closefrom_backend backend[] = {
		/* Keep this array sorted from best to worst. */
#if defined(CLOSEFROM_CLOSE_RANGE)
		closefrom_close_range,
#endif
#if defined(CLOSEFROM_DEVFD)
		closefrom_devfd,
#endif
#if defined(CLOSEFROM_GETDTABLESIZE)
		closefrom_getdtablesize,
#endif
	};
	int ret;
	size_t i;
	int saved_errno;

	/* OpenBSD puns closefrom()'s argument from int to unsigned int when
	 * making the syscall. The result is that any negative fd number becomes
	 * higher than all possible fds, so negative fds return EBADF. The man
	 * page doesn't mention closefrom() fails with EBADF on invalid fd
	 * numbers.
	 */
	if (fd < 0) {
		errno = EBADF;
		return -1;
	}

	saved_errno = errno;
	for (i = 0;; i++) { /* Try all the backends in sequence. */
		if (i >= (sizeof(backend) / sizeof(backend[0]))) {
			/*
			 * closefrom_getdtablesize() always succeeds. If we get
			 * here, that's a bug.
			 */
			abort();
		}

		errno = 0;
		ret = backend[i](fd);
		if (ret == -1) {
			if (errno == EBADF || errno == EINTR)
				goto end; /* Nothing to close or interrupted. */
		} else if (ret == 0) {
			break; /* Success. */
		} else {
			fd = ret; /* Partial success. */
		}
	}
	errno = saved_errno;

end:
	return ret;
}
