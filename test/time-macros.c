/*
 * Copyright (c) 2023 Guilherme Janczak <guilherme.janczak@yandex.com>
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

#include <sys/time.h>

#include <err.h>
#include <time.h>

int
main(void)
{
	struct timespec a, b;

	a.tv_sec = 1;
	a.tv_nsec = 0;
	if (!timespecisset(&a))
		errx(1, "timespeccisset() says tv_sec of 1 is not set");
	timespecclear(&a);
	if (a.tv_sec != 0 || a.tv_nsec != 0)
		errx(1,"timespecclear() failed to clear timespec");
	if (timespecisset(&a))
		errx(1, "timespecisset() says zeroed timespec is set");
	if (!timespecisvalid(&a))
		errx(1, "timespecisvalid() says zeroed timespec is not valid");
	a.tv_nsec = 1000000000-1;
	if (!timespecisvalid(&a)) {
		errx(1, "timespecisvalid() says tv_nsec of %ld isn't valid",
		    a.tv_nsec);
	}
	a.tv_nsec = 1000000000;
	if (timespecisvalid(&a)) {
		errx(1, "timespecisvalid() says tv_nsec of %ld is valid",
		    a.tv_nsec);
	}
	a.tv_nsec = -1;
	if (timespecisvalid(&a))
		errx(1, "timespecisvalid() says tv_nsec of -1 is valid");
	a.tv_sec = a.tv_nsec = b.tv_sec = b.tv_nsec = 1;
	if (timespeccmp(&a, &b, !=))
		errx(1, "timespeccmp() says {-1,-1} != {-1,-1}");
}
