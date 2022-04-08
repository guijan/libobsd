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

- [ ] decide on the new semantic version number, represented by `$VERSION`
- [ ] set `revision` in [libobsd.wrap](libobsd.wrap) to `$VERSION`
- [ ] set `version` in [meson.build](meson.build)'s project() call to `$VERSION`
- [ ] Make sure docs/CHANGELOG.md ends with a blank line and has "changes in
      libobsd `$VERSION`" written at the top
- [ ] git mv docs/CHANGELOG.md docs/CHANGELOG-`$VERSION`.md
- [ ] git commit -m 'prepare for `$VERSION`'
- [ ] git checkout master && git merge `$BRANCH`
- [ ] git tag -m 'libobsd `$VERSION`' `v$VERSION`

Remember: a version is only preceded by a 'v' in the tag name and nowhere else.
