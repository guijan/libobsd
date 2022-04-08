Changes in libobsd 1.1.0:

- Add full Visual Studio support
- Add cross compilation support
- Add getopt()
- Include args and deps in compilation tests to prevent duplicating system
  symbols
- Fix most compiler warnings
- Add sanitizers to CI
- Put all the licenses of all the source files in LICENSE.txt to make it easier
  to comply with them
- Put a copy of the license in the build directory with the right newlines for
  the platform if we're a subproject
- Properly fill Meson's `license:` kwarg in `project()` call
- Add Windows AArch8 support.
- Remove the wrap file from the tree, let the WrapDB handle it

