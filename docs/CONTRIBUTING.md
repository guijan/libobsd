# Contributing

## Sending patches
Follow [OpenBSD's KNF](https://man.openbsd.org/style) programming style when
writing C. Follow Meson's documentation when writing Meson.

Here's a checklist:
- [ ] Add your name and (optionally) a mail address to the copyright of every
  file you modify.
- [ ] Put the name of every function you added in [CHANGELOG.md](CHANGELOG.md).
  Create the file it if it doesn't exist.
- [ ] Run this: `meson setup -Dtest_system=true build && meson test -C build`.
  Did it fail? Don't break the build.

## Design
Headers are generated with a simple DSL by
[src/header_generator.c](src/header_generator.c). It's essentially our own
#pragma for the C language. The purpose of the DSL is to ship clean headers
without an #ifdef forest.

Every subdirectory under *src/* is named after a header. *stdlib/* for
*stdlib.h* and *err/* for *err.h* and so on. All the functions inside each
subdirectory belong to that header. Instead of piling up `#ifdef`s, I've chosen
to split each "backend" that implements a function into its own file.

Feature detection is used unless something is impossible to detect. For
instance, there is some manual checking for Haiku.

I don't intend to implement deprecated functions like rindex(), or silly
functions like getbsize(). I trust the underlying platform, if it is buggy, fix
the bug at the source instead of working around it.

## 1st class systems.
It's very easy to support all the systems in this category because we have
working CI for them.

### [Bionic](https://android.googlesource.com/platform/bionic/)
[Current libc symbols](https://android.googlesource.com/platform/bionic/+/master/libc/libc.map.txt).
[Status](https://android.googlesource.com/platform/bionic/+/HEAD/docs/status.md).

### [dietlibc](https://www.fefe.de/dietlibc/)
Download the latest stable and check out the _include/_ subdirectory.

### [Emscripten](https://github.com/emscripten-core/emscripten)
Look at the emscripten headers at
[system/include/](https://github.com/emscripten-core/emscripten/tree/main/system/include),
and musl headers at
[system/lib/libc/musl/include/](https://github.com/emscripten-core/emscripten/tree/main/system/lib/libc/musl/include).

### [FreeBSD](https://cgit.freebsd.org/src)
They have a nice [man.freebsd.org](https://www.freebsd.org/cgi/man.cgi) website
with all the man pages.

### [glibc](https://sourceware.org/git/?p=glibc.git)
[The wiki](https://www.gnu.org/software/libc/documentation.html) explains a
little, [Linux man-pages](https://www.kernel.org/doc/man-pages) is good but
unofficial, and
[the official documentation](https://www.gnu.org/software/libc/manual) is the
ultimate authority on it.

### [Haiku](https://git.haiku-os.org/haiku/tree/)
Browse the headers at [headers/](https://git.haiku-os.org/haiku/tree/headers).

### Mac OS X
"[These manual pages are a subset of the Mac OS X manual pages](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/index.html#//apple_ref/doc/uid/TP40007259)".
[keith/xcode-man-pages](https://keith.github.io/xcode-man-pages/).

### [Mingw-w64](https://github.com/mingw-w64/mingw-w64)
Mingw conveniently provides some useful functions on top of Microsoft's C
system API and runtimes, for instance neither ucrt nor msvcrt implement
`getopt()`, but Mingw provides it for you. Mingw's headers also require less
kludging than Windows' and the project provides export libraries Microsoft
doesn't like advapi32.def. Thus, Microsoft's documentation is not strictly
correct when used for understanding Mingw. For instance, Microsoft's
RtlGenRandom documentation tells us to use the runtime loader to access the
function, but Mingw's advapi32.def allows us to link against the library that
contains it at compile time.

Browse the C runtime headers at
[mingw-w64-headers/crt/](https://github.com/mingw-w64/mingw-w64/tree/master/mingw-w64-crt)
and Windows system headers at
[mingw-w64-headers/include/](https://github.com/mingw-w64/mingw-w64/tree/master/mingw-w64-crt).
Microsoft also has a
[UCRT alphabetical function reference](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/crt-alphabetical-function-reference?view=msvc-170).

### [MSYS2](https://github.com/msys2/msys2-runtime)
Browse the headers at
[newlib/libc/include](https://github.com/msys2/msys2-runtime/tree/msys2-3_3_4-release/newlib/libc/include).

### [musl](https://git.musl-libc.org/cgit/musl)
Browse the headers at
[include/](https://git.musl-libc.org/cgit/musl/tree/include).

## 2nd class systems
I was unable to get working CI for these systems, but libobsd intends to work
with them.

### [DragonflyBSD](https://gitweb.dragonflybsd.org/dragonfly.git/tree)
[On-Line Manual Pages](https://leaf.dragonflybsd.org/cgi/web-man).

### [Illumos](https://github.com/illumos/illumos-gate)
Browse the [man pages](https://illumos.org/man).

### [NetBSD](http://cvsweb.netbsd.org/bsdweb.cgi/?only_with_tag=MAIN)
See [man.netbsd.org](https://man.netbsd.org).

### [uclibc-ng](https://gogs.waldemar-brodkorb.de/oss/uclibc-ng)
Browse the headers at
[include/](https://gogs.waldemar-brodkorb.de/oss/uclibc-ng/src/master/include).
