<!--
Copyright (c) 2022 Guilherme Janczak <guilherme.janczak@yandex.com>

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
-->

# Libobsd - Transparent OpenBSD compatibility library
Libobsd is much like [libbsd](https://libbsd.freedesktop.org/wiki/).

It is an attempt at writing a very similar library with more modern tools and
practices to make it easy to port, maintain, and add functions to the library.

## Build instructions
Libobsd depends on [Meson](https://mesonbuild.com/).

Install Meson, and follow the build instructions:
```console
foo@bar:~/libobsd$ meson setup build
foo@bar:~/libobsd$ meson compile -C build
```

To compile with Visual Studio, use Meson's Visual Studio backend.
```console
C:\Users\foo\libobsd> meson setup --backend=vs build
C:\Users\foo\libobsd> meson compile -C build
```

## Features
### Portability
Libobsd is highly portable. Linux (musl, glibc, dietlibc, Bionic), HaikuOS,
NetBSD, Mac, Windows (MinGW, Cygwin, Visual Studio), and many other platforms
are officially supported.
The intent is to support every single system supported by Meson.

### Libbsd compatibility mode
```console
foo@bar:~/libobsd$ meson configure -Dprovide_libbsd=true build
```
The compatibility mode installs a _libbsd-overlay.pc_ pkg-config definition.
It should make projects that find libbsd through
[pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) compile and
run against libobsd.

### Meson subproject
Libobsd is available in the
[WrapDB](https://mesonbuild.com/Wrapdb-projects.html).
As
[the Meson documentation](https://mesonbuild.com/Wrap-dependency-system-manual.html#provide-section)
describes, Meson will automatically download and compile the subproject if the
dependency is not found in the system.

## Usage
The [docs/USAGE.md](docs/USAGE.md) file contains complete instructions about
programming against libobsd and supporting both libbsd and libobsd. If the usage
instructions are lacking, open an issue.

## Contributing
The [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) file contains information about
the design of libobsd and pointers to the documentation of the various C
implementations the library has been ported to.
