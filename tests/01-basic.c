#include <utest.h>

UT_TEST(basic) {
    ut_assert(1);
    int *p = NULL;
    ut_assert_null(p);
    p = (int *) 0xCAFE;
    ut_assert_not_null(p);
}

UT_TEST(equal_x) {
    int i = 1;
    ut_assert_equal_int(1, i);

    char c = 'a';
    ut_assert_equal_char('a', c);

    unsigned u = 2;
    ut_assert_equal_uint(2, u);

    const char str[] = "hello";
    ut_assert_equal_string("hello", str);
}
