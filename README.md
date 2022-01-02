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

## Libobsd - Transparent OpenBSD compatibility library
Libobsd is much like [libbsd](https://libbsd.freedesktop.org/wiki/).

It is an attempt to write a very similar library with more modern tools and
practices to make it easy to port, maintain, and add functions to the library.

## Build instructions
Libobsd depends on [Meson](https://mesonbuild.com/).

Install Meson, and follow the build instructions:
```console
foo@bar:~/libobsd$ meson setup build
foo@bar:~/libobsd$ meson compile -C build
```

## Features
### Libbsd compatibility mode:
```console
foo@bar:~/libobsd$ meson configure -Dprovide_libbsd=true build
```
The compatibility mode installs a _libbsd-overlay.pc_ pkg-config definition
which is a symlink to _libobsd.pc_. It should make projects that find libbsd
through
[pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) work if
libobsd implements the used functions.

### Meson subproject
There's a wrap file at [libobsd.wrap](libobsd.wrap) that provides 'libobsd' and
'libbsd-overlay' dependencies.
As
[the Meson documentation](https://mesonbuild.com/Wrap-dependency-system-manual.html#provide-section)
describes, Meson will automatically fallback to the subproject if the dependency
is not found in the system.
```meson
libbsd_dep = dependency('libbsd-overlay') # libbsd compat
libbsd_dep = dependency('libobsd')        # use libobsd exclusively
```
When used as a subproject, libobsd is linked statically and installs nothing.
