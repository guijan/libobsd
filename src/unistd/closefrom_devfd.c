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

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "closefrom.h"

int
closefrom_devfd(int fd)
{
	DIR *dirp;
	int closed; /* Have I close()d something? */
	int rval = -1;


	if ((dirp = opendir("/dev/fd")) == NULL)
		return -1;

	closed = 0;
	for (;;) {
		struct dirent *dp;
		int fdnum;
		const char *errstr;

		errno = 0;
		if ((dp = readdir(dirp)) == NULL) {
			if (errno) {
				/* XXX: does readdir() error in the real world?
				 *
				 * readdir() doesn't guarantee any ordering, so
				 * return error rather than partial success.
				 */
				goto end;
			}
			break;
		}
		fdnum = strtonum(dp->d_name, fd, INT_MAX, &errstr);
		if (!errstr && close(fdnum) == 0)
			closed = 1;
	}
	if (!closed) {
		errno = EBADF;
		goto end;
	}

	rval = 0;
end:
	closedir(dirp);
	return rval;
}
