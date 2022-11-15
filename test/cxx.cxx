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
#include <unistd.h>

/* cxx: Test that the C++ support in the library works.
 * We'll be calling functions from each header, the linker will throw an error
 * if the declarations are not inside 'extern "C"'. Notice that there is the
 * complication that sometimes the functions are inside the libc and not inside
 * libobsd.
 */
int
main(void)
{
	char buf[16];
	char *p;

	/* string.h */
	explicit_bzero(buf, sizeof(buf));
	strlcpy(buf, "test string", sizeof(buf));
	strlcat(buf, "another test string", sizeof(buf));
	(void)memrchr(buf, '\0', sizeof(buf));
	p = buf;
	strsep(&p, "t");

	/* stdlib.h */
	freezero(reallocarray(NULL, 1, 1), 0);
	freezero(recallocarray(NULL, 0, 1, 1), 0);
	setproctitle("%s", "cxx test"); /* Don't need setproctitle_init. */
	strtonum("1", 0, 128, NULL);
	arc4random();
	(void)getprogname();

	/* unistd.h */
	closefrom(3);
	pledge(NULL, NULL);
	unveil(NULL, NULL);

	/* err.h */
	errno = 0;
	warn("called warn()");
	warnx("called warnx()");
	warnc(0, "called warnc()");
	errx(0, "called err()"); /* Make sure this is last. */
}
