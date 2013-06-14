#include <utest.h>
#include <stdlib.h>

static int *foo;
static int *bar;

UT_SUITE_SETUP(Foo) {
    foo = malloc(sizeof *foo);
    *foo = 0;
}

UT_SUITE_TEST(Bar, bar_is_not_null) {
    ut_assert_not_null(bar);
}

UT_SUITE_TEST(Foo, foo_is_zero) {
    ut_assert_equal_int(*foo, 0);
}

UT_SUITE_SETUP(Bar) {
    bar = malloc(sizeof *bar);
    *bar = 1;
}

UT_SUITE_TEST(Foo, foo_is_not_null) {
    ut_assert_not_null(foo);
}

UT_SUITE_TEARDOWN(Foo) {
    free(foo);
    foo = NULL;
}

UT_SUITE_TEST(Bar, bar_is_one) {
    ut_assert_equal_int(*bar, 1);
}

UT_SUITE_TEARDOWN(Bar) {
    free(bar);
    bar = NULL;
}
