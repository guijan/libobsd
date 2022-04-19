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

/* Based on many setproctitle.c implementations from all over the place.
 *
 * Please read the following links to understand how it works:
 * https://source.chromium.org/chromium/chromium/src/+/master:content/common/set_process_title_linux.cc
 * https://github.com/openssh/openssh-portable/blob/master/openbsd-compat/setproctitle.c
 * https://github.com/guillemj/libbsd/blob/main/src/setproctitle.c
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Pointer to the first byte of *argv[0].
 * If NULL, setproctitle_init() hasn't been called.
 */
static char *argv_start = NULL;
/*
 * Pointer to the original '\0' terminator if setproctitle_init() has been
 * called and setproctitle() hasn't.
 * Otherwise, `prev_terminator` is a pointer to where setproctitle() last put
 * the '\0' terminator.
 *
 * We need to keep track of this because if we shrink the title, we'll have to
 * memset() the space between the old '\0' terminator and the new '\0'
 * terminator to '\0' to prevent the kernel from printing leftover garbage.
 */
static char *prev_terminator = NULL;
/* The length of the original block of memory that contains all the argv[] and
 * environ[] strings.
 */
static size_t argv_env_len = 0;

enum {
	TITLE_MAX = 2048,
};

void
setproctitle_init(int _unused, char **argv, char **envp)
{
	/* Length of the contiguous block of memory starting at argv[0]. */
	size_t block_len;
	size_t i;
	char *p;
	/*
	 * Do the strings pointed to by argv[] and envp[] overlap within
	 * TITLE_MAX bytes?
	 */
	int overlap;

	/* This parameter is here for libbsd compatibility. */
	(void)_unused;

	block_len = 0;
	/* Try to find a contiguous block of memory of up to TITLE_MAX length */
	for (i = 0; argv[i] != NULL; i++) {
		if (block_len >= TITLE_MAX)
			break;
		if (argv[0] + block_len == argv[i])
			block_len += strlen(argv[i]) + 1;
	}
	overlap = argv[0] + block_len == envp[0];
	for (i = 0; envp[i] != NULL; i++) {
		if (block_len >= TITLE_MAX)
			break;
		if (argv[0] + block_len == envp[i])
			block_len += strlen(envp[i]) + 1;
	}

	if (block_len == 0) /* Is this even possible? */
		return;

	/*
	 * Copy all the strings in the contiguous block of memory to somewhere
	 * else.
	 * This malloc() is the only failure point in the function. If we return
	 * here, argv/envp are untouched and setproctitle() will be a noop.
	 */
	if ((p = malloc(block_len)) == NULL)
		return;
	memcpy(p, argv[0], block_len);
	/*
	 * We'll overwrite argv[0] soon, but this function still needs the
	 * original. Now is a good time to set argv_start.
	 */
	argv_start = argv[0];

	/*
	 * Update in argv and envp ONLY the strings that have been moved out of
	 * the way.
	 */
	for (i = 0; argv[i] != NULL; i++) {
		if ((size_t)(argv[i] - argv_start) >= block_len)
			break;
		/* argv[0] now contains the address returned by malloc(). */
		argv[i] = p + (argv[i] - argv_start);
	}
	if (overlap) {
		for (i = 0; envp[i] != NULL; i++) {
			if ((size_t)(envp[i] - argv_start) >= block_len)
				break;
			envp[i] = p + (envp[i] - argv_start);
		}
	}

	prev_terminator = argv_start + block_len - 1;
	/* block_len can actually be a little over TITLE_MAX because it stops
	 * at the end of the first string in argv/envp to cross TITLE_MAX.
	 * Make sure argv_env_len is always <= TITLE_MAX for tidyness.
	 */
	argv_env_len = block_len < TITLE_MAX ? block_len : TITLE_MAX;
}

void
setproctitle(const char *fmt, ...)
{
	va_list ap;
	char *buf;
	size_t buflen;
	size_t ret;
	int intret;
	int errnum;
	if (argv_start == NULL)
		return;
	buf = argv_start;
	buflen = argv_env_len;

#define WALK()								\
	do {								\
		if (ret >= buflen) {					\
			buf += buflen-1;				\
			/* done implicitly:  buf[buflen-1] = '\0'; */	\
			goto end;					\
		}							\
		buf += ret, buflen -= ret;				\
	} while (0)

	ret = strlcpy(buf, getprogname(), buflen);
	/* We'll truncate when ret >= buflen throughout this function. */
	WALK();
	/* fmt == NULL means "put only the program's name in the title". */
	if (fmt == NULL)
		goto end;

	ret = strlcpy(buf, ": ", buflen);
	WALK();

	va_start(ap, fmt);
	errnum = errno;
	intret = vsnprintf(buf, buflen, fmt, ap);
	errno = errnum;
	va_end(ap);
	if (intret < 0)
		goto end;
	ret = intret;
	WALK();

end:
	/*
	 * Set all the characters between the current terminator and the
	 * previous terminator to '\0' to prevent the kernel from reading
	 * leftover characters beyond the end of the new title.
	 */
	if (buf < prev_terminator)
		memset(buf, '\0', prev_terminator - buf);
	prev_terminator = buf;
}
