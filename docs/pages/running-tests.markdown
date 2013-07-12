---
title: Running the tests
prev: writing-tests.html
next: compiling-tests.html
---

Tests written using *libutest* are meant to be compiled into a separate
executable file. Therefore to be able to run it, there needs to be a `main`
function.

There is a default main provided which you can insert into your program with
`UT_DEFAULT_MAIN`. However, starting from version 0.2, there is no need
to manually include `UT_DEFAULT_MAIN` anymore, as the default `main` function
is included in the library and is used unless your code defines your own entry
point function (as weak symbol). If your system does not support `main` as a
weak symbol, you will have to resolve to including it yourself..

`UT_DEFAULT_MAIN`

:   <p>This macro expands to definition of a `main()` function.</p>

However, if you are not satisfied with it and want something more complex, you
can write your own entrypoint. In that case you will need `ut_run_all_tests`
function to run the tests.

`int ut_run_all_tests (UtFlags flags)`{.C}

:   Run all registered tests from all suites. This function returns number of
    failed tests. The argument allows you to customize the execution of tests.
    It is a bitset, which should be accessed only by provided constants.

     * `UT_QUIET` disables all output
     * `UT_NO_FORK` makes everything run in same thread
     * `UT_COLOR_NEVER` disables colorizing of the output
     * `UT_COLOR_AUTO` forces the output to be colorized
     * use `0` if you want no flag

    Not printing can be useful if the tests are run as part of a script or in
    other automated way. The result can still be determined based on exit
    value.

    Disabling forking is useful when you want to use a tool like [Valgrind],
    which gives you confusing output when many processes take place. However,
    disabling the forking will lead to segmentation fault should any single
    test crash.

    The default behaviour about colorizing output is to use colors if printing
    output to a TTY and skip colors otherwise. It makes no sense to specify
    both color controls at the same time.

[Valgrind]: http://www.valgrind.org/
