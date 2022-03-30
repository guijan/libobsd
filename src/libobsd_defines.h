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

#if !defined(LIBOBSD_DEFINES)
#define LIBOBSD_DEFINES

/* PUBLIC means this symbol is a library symbol and must be exported. */
#define PUBLIC

#if defined(_MSC_VER)
	#undef PUBLIC

	/*
	 * __declspec is necessary for variables on MSVC and only supported by
	 * MSVC and the Windows versions of Clang and GCC.
	 */
	#if defined(LIBOBSD_EXPORTING)
		#define PUBLIC __declspec(dllexport)
	#else
		#define PUBLIC __declspec(dllimport)
	#endif
#endif /* defined(_MSC_VER) */

#endif /* !defined(LIBOBSD_DEFINES) */
