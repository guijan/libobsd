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

static int err(int, const char *, ...);
static int strpref(const char *, const char *);
static int compar_str(const void *, const void *);

/* header_generator.c: generate header from tiny C-compatible DSL
 *
 * The DSL is very simple: If the line starts with '#pragma libobsd ' (including
 * the space), we parse it. If the word after '#pragma libobsd ' is among the
 * words we were passed in argv, we print the part of the line after the word.

 * If the line doesn't start with '#pragma libobsd ', we print it.
 *
 * We also silently refuse to print more than 1 blank line in sequence.
 */
int
main(int argc, char *argv[])
{
	/* No line in my source code is over 80 chars, but we need 2 more for
	 * DOS newlines, and 1 more for the '\0' terminator.
	 */
	char line[80+2+1];
	char *p;
	const char pref[] = "#pragma libobsd ";
	size_t preflen = sizeof(pref)-1;
	int wasblank;

	/* Move past argv[0]. */
	argv++;
	argc--;
	/*
	 * argv is ready for bsearch() and contains all the tokens to look for.
	 * argc contains its number of elements as usual.
	 */
	qsort(argv, argc, sizeof(*argv), compar_str);

	/* Was the previous line a blank line? */
	wasblank = 0;
	for (;;) {
		if (fgets(line, sizeof(line), stdin) == NULL) {
			if (ferror(stdin))
				err(1, "fgets");
			break;
		}

		line[strcspn(line, "\r\n")] = '\0';

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
			err(1, "puts");
	}

	return (0);
}

/* err: err() for a program that can't count on the system's err() */
static int
err(int eval, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
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
