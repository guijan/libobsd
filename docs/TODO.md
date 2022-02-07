Add unix-y functions/macros such as `timespecsub` but somehow feature detect
wether they should be included or not.

Deduplicate repetitive Meson sources in _src/*/meson.build_. It's not so simple
because of Meson's limitations.
