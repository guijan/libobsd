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

#if defined(OBSD_EXPORTING) /* Not enabled for Windows static builds. */
	#if defined(_WIN32) || defined(__CYGWIN__)
		#define OBSD_PUB __declspec(dllexport)
	#else
		#define OBSD_PUB __attribute__((visibility ("default")))
	#endif
#elif defined(OBSD_IMPORTING) /* Only enabled for Windows shared builds. */
	#define OBSD_PUB __declspec(dllimport)
#else
	/* The Windows __declspec(export) and __declspec(import) directives must
	 * not be used when statically linking. Additionally, non-Windows
	 * platforms don't require a special directive for importing. We don't
	 * use a .def file because Windows requires exported variables to be
	 * aligned to 8 bytes and the .def file doesn't guarantee it.
	 */
	#define OBSD_PUB
#endif

#if defined(__cplusplus)
	#define	OBSD_BEGIN_EXTERN_C	extern "C" {
	#define	OBSD_END_EXTERN_C	}
#else
	#define OBSD_BEGIN_EXTERN_C
	#define	OBSD_END_EXTERN_C
#endif

#if defined(__has_attribute)
	#define OBSD_HAS_GNUC_ATTR(attr) __has_attribute(attr)
#else
	#define OBSD_HAS_GNUC_ATTR(attr) 0
#endif

/* As the standard and preferred version explains, OBSD_DEAD specifies that a
 * function does not return.
 */
#if __STDC_VERSION__ >= 201112L
	#define OBSD_DEAD _Noreturn
#elif OBSD_HAS_GNUC_ATTR(noreturn)
	#define OBSD_DEAD __attribute__((noreturn))
#elif defined(_MSC_VER)
	#define OBSD_DEAD __declspec(noreturn)
#else
	#define OBSD_DEAD
#endif

/* OBSD_NONNULL is placed inside the brackets of an array function parameter to
 * say that the pointer cannot be NULL.
 *
 * XXX: Add a version for void pointers, which can't be made into arrays.
 */
#if __STDC_VERSION__ >= 199901L
	#define OBSD_NONNULL static 1
#elif OBSD_HAS_GNUC_ATTR(nonnull)
	#define OBSD_NONNULL __attribute__((nonnull))
#else
	#define OBSD_NONNULL
#endif

#endif /* !defined(H_OBSD_DEFS) */
