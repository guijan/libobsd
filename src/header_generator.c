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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long mygetline(char **, size_t *, FILE *);
static int myerr(int, int, const char *, ...);
static int strpref(const char *, const char *);
static int compar_str(const void *, const void *);

/* header_generator.c: generate header from tiny C-compatible DSL
 *
 * The DSL is very simple: If the line starts with '#pragma obsd ' (including
 * the space), we parse it. If the word after '#pragma obsd ' is among the
 * words we were passed in argv, we print the part of the line after the word.

 * If the line doesn't start with '#pragma obsd ', we print it.
 *
 * We also silently refuse to print more than 1 blank line in sequence.
 */
int
main(int argc, char *argv[])
{
	/* Was the previous line a blank line? */
	int wasblank;
	char *line;
	size_t linebufsz;
	long long linelen;
	char *p;
	const char pref[] = "#pragma obsd ";
	size_t preflen = sizeof(pref)-1;

	/* Move past argv[0]. */
	argv++;
	argc--;
	/*
	 * argv is ready for bsearch() and contains all the tokens to look for.
	 * argc contains its number of elements as usual.
	 */
	qsort(argv, argc, sizeof(*argv), compar_str);

	wasblank = linebufsz = 0;
	line = NULL;
	for (;;) {
		if ((linelen = mygetline(&line, &linebufsz, stdin)) == -1) {
			if (feof(stdin))
				exit(0);
			myerr(1, 1, "fgets");

		}
		if (linelen > 0 && line[linelen-1] == '\n')
			line[linelen-1] = '\0';


		p = line;
		if (strpref(line, pref)) {
			if ((p = strtok(line + preflen, " \t")) == NULL)
				continue;
			if (bsearch(&p, argv, argc, sizeof(*argv), compar_str)
			    == NULL)
				continue;
			if ((p = strtok(NULL, "")) == NULL)
				continue;
		}

		if (*p == '\0' && wasblank)
			continue;
		wasblank = *p == '\0';

		errno = 0;
		if (puts(p) == EOF && errno != 0)
			myerr(1, 1, "puts");
	}

	return (0);
}

/* mygetline: portable POSIX getline()
 * Windows doesn't have ssize_t so we use long long.
 */
static long long
mygetline(char **lineptr, size_t *n, FILE *stream)
{
	int saved_errno = errno;
	void *p;
	unsigned long long i, tmp;

	for (i = 0;; i++) {
		if (i >= *n) {
			tmp = i == 0 ? 1 : i * 2;
			if ((p = realloc(*lineptr, tmp)) == NULL)
				return -1;
			*n = tmp;
			*lineptr = p;
		}
		switch ((*lineptr)[i] = fgetc(stream)) {
		case EOF:
			if (ferror(stream)) {
				return -1;
			} else if (feof(stream)) {
				if (i == 0)
					return -1;
				/* If end of file and i != 0, fallthrough. */
			} else {
				break;
			}
			/* FALLTHROUGH */
		case '\n':
			ungetc('\0', stream);
			break;
		case '\0':
			errno = saved_errno;
			return i;
		}
	}
	/* NOTREACHED */
}

/* err: portable version of the BSD err() and errx() functions
 * If `printerr` is set, print the string associated with the current errno
 * value.
 */
static int
myerr(int eval, int printerr, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	if (printerr)
		fprintf(stderr, ": %s\n", strerror(errno));
	va_end(ap);
	exit(eval);
}

/* strpref: Does `str` start with `pref`? */
static int
strpref(const char *str, const char *pref)
{
	for (; *pref != '\0'; str++, pref++) {
		if (*pref != *str)
			return (0);
	}
	return (1);
}

/* compar_str: string `compar` function for qsort() */
static int
compar_str(const void *a, const void *b)
{
	return (strcmp(*(char **)a, *(char **)b));
}
