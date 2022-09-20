Add unix-y functions/macros such as `timespecsub` but somehow feature detect
wether they should be included or not.

Deduplicate repetitive Meson sources in _src/*/meson.build_. It's not so simple
because of Meson's limitations.

Some functions are hacky, so even if their implementation is portable, we should
find and make use of platform features that fulfill the same purpose to pass the
blame and avoid future bugs. Check the first and the last explicit_bzero()
and setproctitle() choices to see why.

Compile scrot with libobsd in CI for testing.
Compile a Meson program that uses libobsd as a subproject and makes use of
sys/queue.h in CI for testing. This last one should probably be libtirpc with
the wrapdb meson.build.
