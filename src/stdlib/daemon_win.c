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

#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include <direct.h>
#include <errno.h>
#include <stdlib.h>

static int dup2_r(int, int);

/* The Windows idea of a daemon does not match the Unix one at all. This is a
 * bad approximation.
 */
int
daemon(int nochdir, int noclose)
{
	DWORD lasterr;
	int rval = 0;
	int errnum;
	int fd;
	const char *homedrive;

	lasterr = GetLastError();
	if (FreeConsole() != 0 && GetLastError() == ERROR_INVALID_PARAMETER) {
		errnum = EPERM;
		rval = -1;
		goto end;
	}
	errnum = errno;

	if (!nochdir) {
		if ((homedrive = getenv("HOMEDRIVE")) == NULL) {
			/* Actually a valid path on Windows. It means "the root
			 * of the current drive". Windows has no concept of a
			 * root directory.
			 */
			homedrive = "/";
		}
		(void)chdir(homedrive);
	}

	if (!noclose) {
		while ((fd = open("NUL", O_RDWR)) == -1) {
			if (errno == EINTR)
				continue;
			goto end;
		}
		(void)dup2_r(fd, 0);
		(void)dup2_r(fd, 1);
		(void)dup2_r(fd, 2);
		if (fd > 2)
			(void)close(fd);
	}

end:
	errno = errnum;
	SetLastError(lasterr);
	return (rval);
}

static int
dup2_r(int oldfd, int newfd)
{
	int rval;
	while ((rval = dup2(oldfd, newfd)) == -1 && errno == EINTR)
		continue;
	return (rval);
}
