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
#include <string.h>

int
main(void)
{
	char csv[] = "comma,separated,values, ,\n,,";
	char *values[] = {"comma", "separated", "values", " ", "\n", "", ""};
	char *p, *next;
	size_t i;

	/* Test that it actually separates the string. */
	next = csv;
	for (i = 0; i < sizeof(values) / sizeof(*values); i++) {
		p = strsep(&next, ",");
		if (strcmp(p, values[i]) != 0)
			errx(1, "strcmp(\"%s\", \"%s\") != 0", p, values[i]);
	}

	/* "If *stringp is initially NULL, strsep() returns NULL." */
	p = NULL;
	if (strsep(&p, ",") != NULL)
		errx(1, "strsep(NULL, "") != NULL");

	return (0);
}
