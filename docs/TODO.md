Add unix-y functions/macros such as `timespecsub` but somehow feature detect
wether they should be included or not.

Deduplicate repetitive Meson sources in _src/*/meson.build_. It's not so simple
because of Meson's limitations.

Some functions are hacky, so even if their implementation is portable, we should
find and make use of platform features that fulfill the same purpose to pass the
blame and avoid future bugs.
Check the preferred and the last explicit_bzero() and setproctitle()
choices to see why.
