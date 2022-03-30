/*
 * Copyright (c) 2020 Ingo Schwarze <schwarze@openbsd.org>
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

/* Reimplementation of OpenBSD's /usr/src/regress/lib/libc/getopt/getopt.sh in
 * C.
 */

#include <err.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum {
	BUFSIZE = 256,
};

/* boolean flags for test_run */
enum {
	TEST_NORMAL = (1 << 0),
	TEST_PLUS = (1 << 1),
	TEST_MINUS = (1 << 2),
};

static int test_run(const char *, char *[], int, const char *);
static int test_backend(const char *, int, char *[], const char *);
static void strlcat_many(char *, size_t, ...);

int
main(void)
{
	size_t i;
	int ret;
	/*
	 * Must be at least one larger than the largest element count of struct
	 * tests' argv member or a stack overread happens.
	 */
	enum {ARGV_MAX = 8};
	struct tests {
		char *optstring; /* Optstring for getopt(). */
		/* argv vector for getopt(). test_run derives an argc for us. */
		char *argv[ARGV_MAX];
		/* Any combination of TEST_NORMAL, TEST_PLUS, and TEST_MINUS. */
		int tests;
		/* What test_run should compare the result to. */
		char *expected;
	} tests[] = {
		/* valid isolated options without arguments */
		{
			.optstring = "ax",
			.argv = {"-a", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(x)ARG(arg)",
		},
		{
			.optstring = "x-",
			.argv = {"-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(-)OPT(x)ARG(arg)",
		},

		/* invalid isolated options without arguments */
		{
			.optstring = "ax",
			.argv = {"-a", "-y", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)ERR(?y)ARG(arg)",
		},
		{
			.optstring = ":ax",
			.argv = {"-a", "-y", "arg"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ERR(?y)ARG(arg)",
		},
		{
			.optstring = "x",
			.argv = {"-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "ARG(-)ARG(-x)ARG(arg)",
		},
		{
			.optstring = "-x",
			.argv = {"-", "-x", "arg"},
			.tests = TEST_NORMAL,
			.expected = "NONE(-)OPT(x)NONE(arg)",
		},
		{
			.optstring = "a-",
			.argv = {"-a", "-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(-)ERR(?x)ARG(arg)",
		},
		{
			.optstring = ":a-",
			.argv = {"-a", "-", "-x", "arg"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)OPT(-)ERR(?x)ARG(arg)",
		},

		/* valid grouped options without arguments */
		{
			.optstring = "ax",
			.argv = {"-ax", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(x)ARG(arg)",
		},
		{
			.optstring = "ax-",
			.argv = {"-a-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(-)OPT(x)ARG(arg)",
		},
		{
			.optstring = "abx-",
			.argv = {"-a-b", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(-)OPT(b)OPT(x)ARG(arg)",
		},
		{
			.optstring = "ax-",
			.argv = {"--a", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(-)OPT(a)OPT(x)ARG(arg)",
		},

		/* invalid grouped options without arguments */
		{
			.optstring = "ax",
			.argv = {"-ay", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)ERR(?y)ARG(arg)",
		},
		{
			.optstring = ":ax",
			.argv = {"-ay", "arg"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ERR(?y)ARG(arg)",
		},
		{
			.optstring = "ax",
			.argv = {"-a-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)ERR(?-)OPT(x)ARG(arg)",
		},
		{
			.optstring = ":ax",
			.argv = {"-a-", "-x", "arg"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ERR(?-)OPT(x)ARG(arg)",
		},
		{
			.optstring = "abx",
			.argv = {"-a-b", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)ERR(?-)OPT(b)OPT(x)ARG(arg)",
		},
		{
			.optstring = ":abx",
			.argv = {"-a-b", "-x", "arg"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ERR(?-)OPT(b)OPT(x)ARG(arg)",
		},
		{
			.optstring = "ax",
			.argv = {"--a", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "ERR(?-)OPT(a)OPT(x)ARG(arg)",
		},
		{
			.optstring = ":ax",
			.argv = {"--a", "-x", "arg"},
			.tests = TEST_NORMAL,
			.expected = "ERR(?-)OPT(a)OPT(x)ARG(arg)",
		},

		/* non-option arguments terminating option processing */
		{
			.optstring = "ax",
			.argv = {"-a", "arg", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(a)ARG(arg)ARG(-x)",
		},
		{
			.optstring = "-ax",
			.argv = {"-a", "arg1", "-x", "arg2"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)NONE(arg1)OPT(x)NONE(arg2)",
		},
		{
			.optstring = "ax",
			.argv = {"-a", "--", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(a)ARG(-x)",
		},
		{
			.optstring = "-ax",
			.argv = {"-a", "--", "-x"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ARG(-x)",
		},
		{
			.optstring = "ax",
			.argv = {"-a", "-", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(a)ARG(-)ARG(-x)",
		},
		{
			.optstring = "-ax",
			.argv = {"-a", "-", "-x", "arg"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)NONE(-)OPT(x)NONE(arg)",
		},

		/* the ':' option never works */
		{
			.optstring = "::a",
			.argv = {"-:a", "arg"},
			.tests = TEST_NORMAL,
			.expected = "ERR(?:)OPT(a)ARG(arg)",
		},
		{
			.optstring = ":::a",
			.argv = {"-:", "arg", "-a"},
			.tests = TEST_NORMAL,
			.expected = "ERR(?:)ARG(arg)ARG(-a)",
		},

		/* isolated options with arguments */
		{
			.optstring = "o:",
			.argv = {"-o"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "ERR(?o)",
		},
		{
			.optstring = ":o:",
			.argv = {"-o"},
			.tests = TEST_NORMAL,
			.expected = "ERR(:o)",
		},
		{
			.optstring = "o-:",
			.argv = {"-"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "ERR(?-)",
		},
		{
			.optstring = ":-:",
			.argv = {"-"},
			.tests = TEST_NORMAL,
			.expected = "ERR(:-)",
		},
		{
			.optstring = "o:x",
			.argv = {"-o", "arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(oarg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "o:x",
			.argv = {"-oarg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(oarg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "o::x",
			.argv = {"-oarg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(oarg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "o::x",
			.argv = {"-o", "arg", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(o)ARG(arg)ARG(-x)",
		},
		{
			.optstring = "-o::x",
			.argv = {"-o", "arg1", "-x", "arg2"},
			.tests = TEST_NORMAL,
			.expected = "OPT(o)NONE(arg1)OPT(x)NONE(arg2)",
		},
		{
			.optstring = "o:x",
			.argv = {"-o", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(o-x)ARG(arg)",
		},
		{
			.optstring = "o:x",
			.argv = {"-o", "--", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(o--)OPT(x)ARG(arg)",
		},
		{
			.optstring = "x-:",
			.argv = {"-", "arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(-arg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "x-:",
			.argv = {"--arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(-arg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "x-::",
			.argv = {"--arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(-arg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "x-::",
			.argv = {"-", "arg", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(-)ARG(arg)ARG(-x)",
		},
		{
			.optstring = "--::x",
			.argv = {"-", "arg1", "-x", "arg2"},
			.tests = TEST_NORMAL,
			.expected = "OPT(-)NONE(arg1)OPT(x)NONE(arg2)",
		},
		{
			.optstring = "x-:",
			.argv = {"-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(--x)ARG(arg)",
		},
		{
			.optstring = "x-:",
			.argv = {"-", "--", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(---)OPT(x)ARG(arg)",
		},

		/* grouped options with arguments */
		{
			.optstring = "ao:",
			.argv = {"-ao"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)ERR(?o)",
		},
		{
			.optstring = ":ao:",
			.argv = {"-ao"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ERR(:o)",
		},
		{
			.optstring = "a-:",
			.argv = {"-a-"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)ERR(?-)",
		},
		{
			.optstring = ":a-:",
			.argv = {"-a-"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)ERR(:-)",
		},
		{
			.optstring = "ao:x",
			.argv = {"-ao", "arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(oarg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "ao:x",
			.argv = {"-aoarg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(oarg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "ao::x",
			.argv = {"-aoarg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(oarg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "ao::x",
			.argv = {"-ao", "arg", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(a)OPT(o)ARG(arg)ARG(-x)",
		},
		{
			.optstring = "-ao::x",
			.argv = {"-ao", "arg1", "-x", "arg2"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)OPT(o)NONE(arg1)OPT(x)NONE(arg2)",
		},
		{
			.optstring = "ao:x",
			.argv = {"-ao", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(o-x)ARG(arg)",
		},
		{
			.optstring = "ao:x",
			.argv = {"-ao", "--", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(o--)OPT(x)ARG(arg)",
		},
		{
			.optstring = "a-:x",
			.argv = {"-a-", "arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(-arg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "a-:x",
			.argv = {"-a-arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(-arg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "a-::x",
			.argv = {"-a-arg", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(-arg)OPT(x)ARG(arg)",
		},
		{
			.optstring = "a-::x",
			.argv = {"-a-", "arg", "-x"},
			.tests = TEST_NORMAL|TEST_PLUS,
			.expected = "OPT(a)OPT(-)ARG(arg)ARG(-x)",
		},
		{
			.optstring = "-a-::x",
			.argv = {"-a-", "arg1", "-x", "arg2"},
			.tests = TEST_NORMAL,
			.expected = "OPT(a)OPT(-)NONE(arg1)OPT(x)NONE(arg2)",
		},
		{
			.optstring = "a-:x",
			.argv = {"-a-", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(--x)ARG(arg)",
		},
		{
			.optstring = "a-:x",
			.argv = {"-a-", "--", "-x", "arg"},
			.tests = TEST_NORMAL|TEST_PLUS|TEST_MINUS,
			.expected = "OPT(a)OPT(---)OPT(x)ARG(arg)",
		},
	};
	char *argv[ARGV_MAX+1] = {"getopt"};

	for (i = 0; i < sizeof(tests)/sizeof(*tests); i++) {
		memcpy(argv+1, tests[i].argv, sizeof(tests[i].argv));
		ret = test_run(tests[i].optstring, argv,
		    tests[i].tests, tests[i].expected);
		if (ret == -1)
			break;
	}

	return (-ret);
}

/* test_run: test getopt()
 * Tests getopt() with the arguments `optstring`, `argv`, an `argc` derived
 * from `argv`, and performs the tests set in the boolean flags contained by
 * `tests`, then compares the results to `expected` and returns 0 if equal and
 * -1 if unequal.
 */
static int
test_run(const char *optstring, char *argv[], int tests, const char *expected)
{
	int argc;
	int rval;
	char buf[BUFSIZE];
	size_t i, j;
	argc = 0;
	while (argv[argc] != NULL)
		argc++;

	rval = 0;
	if (tests & TEST_NORMAL)
		rval |= test_backend(optstring, argc, argv, expected);

	/* Test that the result is the same when the optstring is prefixed with
	 * a "+" character
	 */
	if (tests & TEST_PLUS) {
		strlcpy(buf, "+", sizeof(buf));
		if (strlcat(buf, optstring, sizeof(buf)) > sizeof(buf))
				errx(1, "strlcat");
		rval |= test_backend(buf, argc, argv, expected);
	}

	/* Test that the result is the same when the optstring is prefixed with
	 * a "-" character, except that it result changes from ARG() to NONE().
	 */
	if (tests & TEST_MINUS) {
		for (i = j = 0; optstring[i] != '\0'; i++) {
			if (strncmp(optstring+i, "ARG", 3) == 0) {
				if (j+4 >= sizeof(buf)) {
					rval |= -1;
					puts("extended optstring too large to"
					    "fit in buf");
					break;
				}
				if (strlcat(buf, "NONE", sizeof(buf))
				    >= sizeof(buf)) {
					errx(1, "strlcat");
				}
				j += 4;
			} else {
				buf[j++] = optstring[i];
			}
		}
		buf[j] = '\0';
		test_backend(buf, argc, argv, expected);
	}

	if (tests &= ~(TEST_NORMAL|TEST_PLUS|TEST_MINUS))
		errx(1, "bogus tests: %x", tests);

	return (rval);
}


/* Process command line options and arguments according to the
 * optstring. Write:
 * OPT(c) for an option "-c" without an option argument
 * OPT(carg) for an option "-c" with an option argument "arg"
 * ARG(arg) for a non-option argument "arg"
 * NONE(arg) for a non-option argument "arg" processed with OPTS =~ ^-
 * ERR(?c) for an invalid option "-c", or one lacking an argument
 * ERR(:c) for an option "-c" lacking an argument while OPTS =~ ^:
 *
 * Then compare the result with "expected" and return -1 if they don't match.
 */
static int
test_backend(const char *optstring, int argc, char *argv[], const char *expected)
{
	int ch;
	char res[BUFSIZE];
	char tmpch[2], tmpopt[2];
	int ret;
	*res = '\0';
	/* tmpch and tmpopt exist so their [0] element can be set, allowing us
	 * to pass a single character string to strlcat_many().
	 */
	tmpch[1] = tmpopt[1] = '\0';

	opterr = 0;
	while ((ch = getopt(argc, argv, optstring)) != -1) {
		switch (ch) {
		case '\1':
			strlcat_many(res, sizeof(res), "NONE(", optarg, ")",
			    (char *)NULL);
			break;
		case ':':
		case '?':
			*tmpch = ch;
			*tmpopt = optopt;
			strlcat_many(res, sizeof(res), "ERR(", tmpch, tmpopt,
			    ")", (char *)NULL);
			break;
		default:
			*tmpch = ch;
			strlcat_many(res, sizeof(res), "OPT(", tmpch,
			    optarg == NULL ? "" : optarg, ")", (char *)NULL);
			break;
		}
	}
	while (optind < argc) {
		strlcat_many(res, sizeof(res), "ARG(", argv[optind++], ")",
		    (char *)NULL);
	}
	optreset = 1;
	optind = 0;

	ret = strcmp(res, expected) == 0 ? 0 : -1;
	if (ret == -1)
		printf("%s != %s\n", res, expected);
	return (ret);
}

/* strlcat_many: variadic strlcat
 * concatenates variadic arguments into `dst` and error exits when the total
 * length is over `dstlen`
 */
static void
strlcat_many(char *dst, size_t dstlen, ...)
{
	va_list ap;
	char *p;

	va_start(ap, dstlen);
	while ((p = va_arg(ap, char *)) != NULL) {
		if (strlcat(dst, p, dstlen) >= dstlen)
			errx(1, "strlcat");
	}
	va_end(ap);
}
