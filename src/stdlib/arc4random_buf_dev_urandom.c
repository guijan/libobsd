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
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/* Haiku doesn't have a proper programming API for generating random numbers, so
 * it's impossible to provide the semantics of arc4random_buf(). We'll
 * approximate them with /dev/urandom.
 */
void
arc4random_buf(void *_buf, size_t nbytes)
{
	int errnum;
	int fd;
	unsigned char *buf = _buf;
	ssize_t nr;
	size_t unr;
	errnum = errno;

	while ((fd = open("/dev/urandom", O_RDONLY|O_CLOEXEC)) == -1) {
		if (errno == EINTR)
			continue;
		abort();
	}

	while (nbytes > 0) {
		nr = read(fd, buf, nbytes);
		if (nr == -1) {
			if (errno == EINTR)
				continue;
			abort();
		} else if (nr == 0) {
			abort(); /* /dev/urandom should never EOF. */
		}

		unr = nr;
		buf += unr;
		nbytes -= unr;
	}

	close(fd);
	errno = errnum;
}
