# libswid

[![Build Status](https://travis-ci.org/OpenSCAP/libswid.svg?branch=master)](https://travis-ci.org/OpenSCAP/libswid)
[![codecov](https://codecov.io/gh/OpenSCAP/libswid/branch/master/graph/badge.svg)](https://codecov.io/gh/OpenSCAP/libswid)
[![Sonar Cloud](https://sonarcloud.io/api/project_badges/measure?project=openscap-libswid&metric=alert_status)](https://sonarcloud.io/dashboard?id=openscap-libswid)
[![Documentation Status](https://readthedocs.org/projects/libswid/badge/?version=latest)](http://libswid.readthedocs.io/en/latest/?badge=latest)

Library for interfacing with SWID tags

## Build

You will need:
* C++ toolchain.
* Xerces XML parsing library.
* TinyXML XML parsing library.
* `lcov` for coverage reports.

```
$ mkdir -p build && cd build && rm -rf *
$ cmake .. && make && make test
$ make lcov && xdg-open lcov/html/all_targets/index.html
```

If you have downloaded the [SWID validator](https://csrc.nist.gov/Projects/Software-Identification-SWID/resources), extract it and pass the path to the validator `.jar` to Cmake like this:

```
cmake -DSWID_VALIDATOR=/path/to/validator/swid/swidval-0.5.0.jar ..
```

Then, the test suite will use it in some of the tests.
