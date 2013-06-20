---
title: Compiling the tests
prev: running-tests.html
---

The `libutest` framework is designed for the tests to be compiled into a
standalone executable. The installed library needs to be linked to the
executable.

There are `pkg-config` data provided for ease of use.

Given the tests are in `tests.c` file, to compile you need to execute following
command.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
$ gcc -o test-exe tests.c `pkg-config --cflags --libs libutest`
$ # Now you can run the tests
$ ./test-exe
....

11 assertions succeeded, 0 assertions failed
4 tests ran, 0 failed, 0 crashed
Time elapsed: 0.001 sec
$ 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you used the provided default main, the behavior of the resulting executable
can be tweaked using command line arguments. To print help about them, run the
executable with `--help` (or `-h` for short).

## Command line options

`-h`, `--help`
:   prints help about the options
`-V`, `--version`
:   prints version information
