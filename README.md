# libswid
Library for interfacing with SWID tags

## Build

You will need:
* C++ toolchain.
* Xerces XML parsing library.
* `lcov` for coverage reports.

```
$ mkdir -p build && cd build && rm -rf *
$ cmake .. && make && make test
$ make lcov && xdg-open lcov/html/all_targets/index.html
```
