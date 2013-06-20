---
title: Writing tests
next: running-tests.html
prev: defining-tests.html
---

In each test, you can use many test assertions.

`ut_assert(expr)`

:   <p>Check that `expr` evaluates to truth (i.e. anything non-zero).</p>


### Pointers

`ut_assert_null(ptr)`

:   Check that `ptr` is `NULL` pointer.

`ut_assert_not_null(ptr)`

:   Check that `ptr` points somewhere to memory and is not `NULL`.


### Basic values

`ut_assert_equal_int(expected, actual)`

:   Check equality of two integers. The first argument should be the expected
    value. While swapping the arguments makes no difference in whether or not
    the assertion succeeds, the order is important for error message.

`ut_assert_equal_uint(expected, actual)`

:   Check equality of two unsigned integers.

`ut_assert_equal_string(expected, actual)`

:   Check that both arguments are either `NULL` or contain the same string.
    The character data is compared, not memory addresses.


## Custom assertions

You may find yourself in a situation where you need to write your own
assertion. There is support for doing that.

`ut_assert_func(expr, fmt, ...)`

:   Verify that `expr` evaluates to true. The error message is determined by
    the `fmt` argument, which is a `printf`-like formatting string.

    Please note that is is in fact a macro, that wraps a function and gives it
    current file name and line number to further enhance error message.


If your assertion can not be easily delegated to `ut_assert_func`, you can use
following macros to build your assertion from scratch. Please note that each
assertion should call exactly one of `ut_pass` and `ut_fail`.

`ut_pass()`

:   Marks the assertion as passed. This is again a macro that wraps a function
    and passes it additional arguments.

`ut_fail(int line)`

:   Mark the assertion as failed. This macro also prints a header with
    information about which test this assertion is in as well as file and line
    number. File name is determined by the current test, line number must be
    supplied in an argument.

`ut_message(msg, ...)`

:   This macro is a helper for failed assertions. It enables you to print
    detailed information about why the assertion failed. Since there is an
    empty line after the message, you should only call this once.


### Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.C}
struct pair {
    int x;
    char *str;
};

/* Verify that the struct contains the same number
 * as string and as a real integer. */
void my_assert(struct pair *p)
{
    int str_value = atoi(p->str);

    ut_assert_func(p->x == str_value,
        "Struct data is not consistent\n"
        "{ x=%d; str=\"%s\" }",
        p->x, p->str);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
