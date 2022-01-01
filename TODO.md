Add an option to build anyway if certain functions which can't be implemented
portably like arc4random() haven't been ported to the current system, make it
off by default.

Create a subset of the library that is not so tied to unix.

Deduplicate repetitive Meson definitions in _src/*/meson.build_. It's not so
simple because of Meson's limitations.
