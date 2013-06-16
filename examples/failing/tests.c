#include <utest.h>

#include <stdio.h>
#include <string.h>

UT_TEST(pointers) {
    int *a = (int *) 0xCAFE;
    ut_assert_null(a);
    ut_assert_not_null(strstr("foobar", "X"));
}

UT_TEST(integers) {
    int a = 10;
    ut_assert_equal_int(15, a);
    unsigned b = 10;
    ut_assert_equal_uint(0, b);
}

UT_TEST(crash) {
    int *a = NULL;
    *a = 5;
}

UT_TEST(strings) {
    const char *str = "foobarbaz";
    ut_assert_equal_string("fooquubaz", str);
    ut_assert_equal_string(NULL, str);
    str = NULL;
    ut_assert_equal_string("foobar", str);
}
