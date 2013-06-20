---
title: Running the tests
prev: writing-tests.html
next: compiling-tests.html
---

Tests written using *libutest* are meant to be compiled into a separate
executable file. Therefore to be able to run it, there needs to be a `main`
function.

There is a default main provided which you can insert into your program with
`UT_DEFAULT_MAIN` (now deprecated). Starting from version 0.2, there is no need
to manually include `UT_DEFAULT_MAIN` anymore, as the default `main` function
is included in the library and is used unless your code defines your own entry
point function.

`UT_DEFAULT_MAIN`

:   Deprecated. This macro expands to definition of a `main()` function.

However, if you are not satisfied with it and want something more complex, you
can write your own entrypoint. In that case you will need `ut_run_all_tests`
function to run the tests.

`int ut_run_all_tests (UtVerbosityLevel verbosity)`

:   Run all registered tests from all suites. This function returns number of
    failed tests. The argument to this function specifies what output should be
    printed. There are a few options:

     * `UT_NORMAL` prints progress and error messages
     * `UT_QUIET` disables all output

    Not printing can be useful if the tests are run as part of a script or in
    other automated way. The result can still be determined based on exit
    value.
