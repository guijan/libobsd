Changes in libobsd master:

- Further work on license compliance
- Replace BSD-4-Clause licensed sources
- Add daemon()
- Add setproctitle()
- Add CompCert build testing
- Use feature test macros and meson deps strictly
- Add NetBSD support
- Simplify reallocarray, recallocarray, reducing source/machine code size
- Add memrchr()
- Change the strategy used for symbol visibility to a simpler and better one
- Improve compatibility with numerous libcs
- Fix some functions which weren't including the headers they should
- Fix wrong handling of a NULL pointer in freezero()
- Provide uint32_t in stdlib.h on systems that don't
- Fix sys/queue.h not being available to Meson subprojects
- Add closefrom()
- Include io.h in unistd.h on Windows to expose io.h's POSIX functions
- Add C++ support
- Add timespeccmp(), timespecsub(), and other timeval/timespec macros from
  sys/time.h
- Ensure the license is placed in the build directory before the dependency is
  done.
- Explicitly set the minmum Meson version to 1.3.0
- Install readme and license
- Override libobsd and libbsd-overlay in Meson
- Don't force LTO
- Fix cross compilation when the host machine compiler has MSVC syntax and the
  buld machine compiler doesn't
