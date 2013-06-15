---
title: Writing tests
next: running-tests.html
prev: defining-tests.html
---

In each test, you can use many test assertions.

`ut_assert(expr)`

:   Check that `expr` evaluates to truth (i.e. anything non-zero).


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

:   Check that both arguments are non-`NULL` and contain the same string. The
    character data is compared, not memory addresses.


## Custom assertions

You may find yourself in a situation where you need to write your own
assertion. There is support for doing that.

`ut_assert_func(expr, fmt, ...)`

:   Verify that `expr` evaluates to true. The error message is determined by
    the `fmt` argument, which is a `printf`-like formatting string.

    Please note that is is in fact a macro, that wraps a function and gives it
    current file name and line number to further enhance error message.

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
