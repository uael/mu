# mu
μ c cross-platform multi purpose library

[![Build Status](https://travis-ci.org/uael/mu.svg?branch=master)](https://travis-ci.org/uael/mu)
[![Build status](https://ci.appveyor.com/api/projects/status/dbi3vnyn5eyk2w59/branch/master?svg=true)](https://ci.appveyor.com/project/uael/mu/branch/master)


**IN PROGRESS**

## Build & Tests

```bash
$ git clone --recursive https://github.com/uael/mu.git 
```

### xmake
```bash
$ xmake
checking for the architecture ... x86_64
configure
{
    arch = "x86_64"
,   ld = "g++"
,   mode = "release"
,   host = "linux"
,   ar = "ar"
,   kind = "static"
,   buildir = "build"
,   plat = "linux"
,   cc = "gcc"
,   cxx = "gcc"
}
[00%]: [20%]: [40%]: [60%]: compiling.release src/buffer.c
compiling.release src/ds.c
compiling.release src/vector.c
compiling.release src/list.c
[80%]: compiling.release src/deque.c
[100%]: archiving.release libu.a
build ok!

$ xmake check
[25%]: archiving.release libu.a
[25%]: compiling.release vendor/gtest/googletest/src/gtest-all.cc
[50%]: archiving.release libgtest.a
[50%]: compiling.release vendor/gtest/googletest/src/gtest_main.cc
[75%]: archiving.release libgtest_main.a
[75%]: compiling.release test/vector.cc
[100%]: linking.release test
Running main() from gtest_main.cc
[==========] Running 4 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 4 tests from VectorTest
[ RUN      ] VectorTest.grow
[       OK ] VectorTest.grow (0 ms)
[ RUN      ] VectorTest.reserve
[       OK ] VectorTest.reserve (1 ms)
[ RUN      ] VectorTest.resize
[       OK ] VectorTest.resize (0 ms)
[ RUN      ] VectorTest.push
[       OK ] VectorTest.push (0 ms)
[----------] 4 tests from VectorTest (1 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test case ran. (1 ms total)
[  PASSED  ] 4 tests.

$ xmake install
[100%]: archiving.release libu.a
installing to /usr/local ...
error: installation failed, may permission denied!
note: try continue to install with administrator permission again?
please input: y (y/n)
y
[sudo] password for uael:           
install ok!
```
