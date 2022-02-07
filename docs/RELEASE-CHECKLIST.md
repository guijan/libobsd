- [ ] decide on the new semantic version number, represented by `$VERSION`
- [ ] set `revision` in [libobsd.wrap](libobsd.wrap) to `$VERSION`
- [ ] set `version` in [meson.build](meson.build)'s project() call to `$VERSION`
- [ ] git mv docs/CHANGELOG.md docs/CHANGELOG-`$VERSION`.md
- [ ] git commit -m 'prepare for `$VERSION`'
- [ ] git checkout master && git merge `$BRANCH`
- [ ] git tag -m 'libobsd `$VERSION`' `v$VERSION`

Remember: a version is only preceded by a 'v' in the tag name and nowhere else.
