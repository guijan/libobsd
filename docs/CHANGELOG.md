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

