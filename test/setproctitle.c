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

#include <stdlib.h>
#include <string.h>

/* XXX: somehow figure out how to verify that the title was actually set.
 *
 * Most of the tests here are just hoping to catch a memory corruption.
 * My setproctitle() is just the kind of function to cause it.
 */
int
main(int argc, char *argv[])
{
	char buf[16384];

#if defined(SETPROCTITLE_INIT)
	extern char **environ;

	/*
	 * Does setting the title before calling setproctitle_init do anything
	 * funky? It should be a no-op.
	 */
	setproctitle("funky");
	setproctitle(NULL);

	setproctitle_init(argc, argv, environ);
#endif
	(void)argc;
	(void)argv;

	/* Do varargs work? */
	setproctitle("1[%s] 2[%s] 3[%s]", "test1", "test2", "test3");

	/* Try a big title. */
	memset(buf, '.', sizeof(buf));
	buf[sizeof(buf)-1] = '\0';
	setproctitle("%s", buf);

	/* Try the smallest possible custom title. */
	setproctitle("");

	setproctitle(NULL);
	setproctitle("success");
	setproctitle(NULL);

	/*
	 * Did I break argv somehow? Try to read/write it to see if it triggers
	 * memory protections.
	 */
	while (*argv != NULL) {
		strlcpy(buf, *argv, sizeof(buf));
		memset(*argv, '.', strlen(*argv));
		argv++;
	}

#if defined(SETPROCTITLE_INIT)
	/* Did I break environ somehow? */
	while (*environ != NULL) {
		strlcpy(buf, *environ, sizeof(buf));
		memset(*environ, '.', strlen(*environ));
		environ++;
	}
	/* setproctitle_init allocates memory, free() it so valgrind doesn't
	 * complain.
	 */
	free(*argv);
#endif
}
