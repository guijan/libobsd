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

/* This header is internal to libobsd, do not use it in your program. */

#if !defined(H_OBSD_DEFS)
#define H_OBSD_DEFS

#if defined(OBSD_EXPORTING)
	#if defined(_MSC_VER)
		/*
	 	 * __declspec is necessary for variables on MSVC. The compiler
		 * also supports passing a .def file with a list of symbols to
		 * export, but I've found that Windows requires exported
		 * variables to be alinged ot 8 bytes and the .def file doesn't
		 * guarantee it.
	 	 */
		#define OBSD_PUB __declspec(dllexport)
	#else
		#define OBSD_PUB __attribute__ ((visibility ("default")))
	#endif
#elif defined(OBSD_IMPORTING)
	#define OBSD_PUB __declspec(dllimport)
#else
	#define OBSD_PUB
#endif

#if defined(__cplusplus)
	#define	OBSD_BEGIN_EXTERN_C	extern "C" {
	#define	OBSD_END_EXTERN_C	}
#else
	#define OBSD_BEGIN_EXTERN_C
	#define	OBSD_END_EXTERN_C
#endif

#endif /* !defined(H_OBSD_DEFS) */
