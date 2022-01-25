# Contributing

## C Programming API reference
Some systems are not well documented, these directions help finding
documentation relevant to libobsd. The project's name links to the official web
view of the source tree with cloning instructions.

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
I can't find the Mac OS X manual pages.

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

### [musl](https://git.musl-libc.org/cgit/musl)
Browse the headers at
[include/](https://git.musl-libc.org/cgit/musl/tree/include).

### [uclibc-ng](https://gogs.waldemar-brodkorb.de/oss/uclibc-ng)
Browse the headers at
[include/](https://gogs.waldemar-brodkorb.de/oss/uclibc-ng/src/master/include).
