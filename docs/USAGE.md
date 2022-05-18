# Basic usage
## Usage
Follow
[scrot](https://github.com/resurrecting-open-source-projects/scrot/blob/ed96aa4ae3d69c186b3dda88543cc541acaf83c2/configure.ac#L33-L43)
when using libobsd with Autotools.

Follow
[dictpw](https://github.com/guijan/dictpw/blob/fb56c002109276369ca6c02c2f735ee83bedbbd2/meson.build#L32-L45)
when using libobsd with Meson. Install the subproject from the WrapDB to allow
falling back to compiling the library.

(find a good CMake example to add here)

Always feature test the presence of the functions you use. Always use
pkg-config when depending on libobsd, not using pkg-config is forbidden.

If you require strict OpenBSD behavior, ask pkg-config for _libobsd_. If you're
programming against broadly BSD behavior, ask pkg-config for _libbsd-overlay_.
Maintain source compatibility with libbsd by using libbsd's _libbsd-overlay_
pkg-config definition, do not access libbsd in any other way.

Program against the documentation, not against behavior inferred from the source
code.

Following these principles ensures beautiful, short, maintainable, easy to
understand, and portable programs with no unnecessary dependencies.

## Meson subproject
When used as a subproject, libobsd is linked statically and installs nothing. It
also places a copy of the license at
**build/subprojects/libobsd/LICENSE_libobsd.txt**. Please distribute it to
comply with terms of the license.
The Windows subproject depends on `unix2dos` from
[dos2unix](https://waterlan.home.xs4all.nl/dos2unix.html) during the compilation
process.

# Advanced usage
## ABI/API
Refraining from including a function that is provided by the libc present at the
time of compilation isn't an ABI break, even if a previous version of libobsd
included the function.

Making the ABI compatible with libbsd is not a goal.

ABI stability isn't a goal. The project's versioning follows SemVer, however.

Changing behavior to correctly match OpenBSD is never an API break. Make your
libobsd programs compatible with OpenBSD always.

It is not an API break to change the behavior of the source code if it still
fits the documentation.

## Caveats
At the moment, libobsd merely provides functions that the current system
doesn't. These system functions sometimes differ in behavior. For instance,
arc4random_uniform(0) on NetBSD will terminate the program with a SIGFPE, and
the musl strlcat() doesn't return the length of the string it tried to create if
the size of the destination buffer is smaller than the length of the
string in it.

Currently, libobsd recommends a programming style which targets all the
differing implementations of POSIX and OpenBSD functions, not OpenBSD itself.
The intention is to add a workaround for a differing implementation every time
a need for one is found, and only after the implementation refuses to match
OpenBSD.
