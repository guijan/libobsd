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

/*
 * This header is for internal use by libobsd. Do not use it in your own
 * program.
 */

/* This is an extremely ugly hack for doing #include_next on MSVC. The lack of
 * include guards is intentional.
 *
 * You should see including this header as a function call of sorts. You
 * pass arguments to this function by defining a variable just before calling
 * it, that variable is LIBOBSD_INC_NEXT. You can #define LIBOBSD_INC_NEXT again
 * afterwards.
 *
 * Here's how to use it:
 *
 * #define LIBOBSD_INC_NEXT stdio.h
 * #include <libobsd_inc_next.h>
 *
 * Note the lack of <> or "" around the header stdio.h. The sequence of 2
 * preprocessor macros above is equivalent to the following GCC extension:
 *
 * #include_next <stdio.h>
 */

#if defined(_MSC_VER)
	#if _MSC_VER >= 1900
		#define LIBOBSD_INC(x) <../ucrt/x>
	#else
		#define LIBOBSD_INC(x) <../include/x>
	#endif

	#include LIBOBSD_INC(LIBOBSD_INC_NEXT)
#else
	#define LIBOBSD_INC(x) <x>
	#include_next LIBOBSD_INC(LIBOBSD_INC_NEXT)
#endif

#undef LIBOBSD_INC
#undef LIBOBSD_INC_NEXT
